#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
	cout << "Sample TCP echo client for 'sockpp' "
		<< sockpp::SOCKPP_VERSION << '\n' << endl;

	string host = (argc > 1) ? argv[1] : "localhost";
	in_port_t port = (argc > 2) ? atoi(argv[2]) : 8002;

	sockpp::socket_initializer sockInit;

	// Implicitly creates an inet_address from {host,port}
	// and then tries the connection.

	sockpp::tcp_connector conn({host, port});
	if (!conn) {
		cerr << "Error connecting to server at "
			<< sockpp::inet_address(host, port)
			<< "\n\t" << conn.last_error_str() << endl;
		return 1;
	}

	cout << "Created a connection from " << conn.address() << endl;

    // Set a timeout for the responses
    if (!conn.read_timeout(seconds(5))) {
        cerr << "Error setting timeout on TCP stream: "
                << conn.last_error_str() << endl;
    }

	string s, sret;
	while (getline(cin, s) && !s.empty()) {
		if (conn.write(s) != ssize_t(s.length())) {
			cerr << "Error writing to the TCP stream: "
				<< conn.last_error_str() << endl;
			break;
		}

		sret.resize(s.length());
		ssize_t n = conn.read_n(&sret[0], s.length());

		if (n != ssize_t(s.length())) {
			cerr << "Error reading from TCP stream: "
				<< conn.last_error_str() << endl;
			break;
		}

		cout << sret << endl;
	}

	return (!conn) ? 1 : 0;
}