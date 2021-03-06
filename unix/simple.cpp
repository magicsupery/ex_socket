#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

using namespace std;

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_addr, client_addr;
	socklen_t len;	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9876);
	if(inet_aton("127.0.0.1", &server_addr.sin_addr) == 0)
	{
		return -1;
	}
	//如果不调用bind直接listen，那accpet的时候会直接返回errno 22，表示bad accept fd
	bind(listenfd, (sockaddr*)&server_addr, sizeof(server_addr));
	listen(listenfd, 1024);
	for(;;)
	{
		int connfd = accept(listenfd, (sockaddr*)&client_addr, &len);
		if(connfd <= 0)
		{
			cout << "accept failed with errno " << errno << endl;
			return -1;
		}

		cout << "get connection from " << inet_ntoa(client_addr.sin_addr) << ":" << client_addr.sin_port << endl;
		close(connfd);
	}
	
}
