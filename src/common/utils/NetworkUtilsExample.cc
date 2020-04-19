#include "../base/Base.h"
#include <string>

int main(int argc, char *argv[]) {
	NetworkUtils *network = new NetworkUtils();
	std::string hostName;
	std::string Ip;
	
	bool ret = network->GetHostInfo(hostName, Ip);
	if (true == ret) {
		std::cout << "hostname: " << hostName << std::endl;
		std::cout << "Ip: " << Ip << std::endl;
	}
	return 0;
}