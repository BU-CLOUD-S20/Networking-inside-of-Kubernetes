#include <iostream>
#include <thread>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"

using namespace std;

// --------------------------------------------------------------------------
// The thread function. This is run in a separate thread for each socket.
// Ownership of the socket object is transferred to the thread, so when this
// function exits, the socket is automatically closed.

void run_echo(sockpp::tcp_socket sock)
{
	ssize_t n;
	char buf[512];

	while ((n = sock.read(buf, sizeof(buf))) > 0)
		sock.write_n(buf, n);

	cout << "Connection closed from " << sock.peer_address() << endl;
}

// --------------------------------------------------------------------------
// The main thread runs the TCP port acceptor. Each time a connection is
// made, a new thread is spawned to handle it, leaving this main thread to
// immediately wait for the next connection.

int main(int argc, char* argv[])
{
	cout << "Sample TCP echo server for 'sockpp' "
		<< sockpp::SOCKPP_VERSION << '\n' << endl;

	in_port_t port = (argc > 1) ? atoi(argv[1]) : 8002;

	sockpp::socket_initializer sockInit;

	sockpp::tcp_acceptor acc(port);

	if (!acc) {
		cerr << "Error creating the acceptor: " << acc.last_error_str() << endl;
		return 1;
	}
    //cout << "Acceptor bound to address: " << acc.address() << endl;
	cout << "Awaiting connections on port " << port << "..." << endl;

	while (true) {
		sockpp::inet_address peer;

		// Accept a new client connection
		sockpp::tcp_socket sock = acc.accept(&peer);
		cout << "Received a connection request from " << peer << endl;

		if (!sock) {
			cerr << "Error accepting incoming connection: " 
				<< acc.last_error_str() << endl;
		}
		else {
			// Create a thread and transfer the new stream to it.
			thread thr(run_echo, std::move(sock));
			thr.detach();
		}
	}

	return 0;
}
