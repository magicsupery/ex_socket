#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

using namespace std;

void echo_str(int connfd)
{
	
	char buff[1024];	
	size_t n;
	do{
		n = read(connfd, buff, 1024);
		write(connfd, buff, n);

	}while(n > 0 or errno == EINTR);

	

}
int main()
{
	pid_t pid;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_addr, client_addr;
	socklen_t len;	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9876);
	if(inet_aton("127.0.0.1", &server_addr.sin_addr) == 0)
	{
		return -1;
	}
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
		if((pid = fork()) == 0)
		{
			//child, close operation is related with reference count
			close(listenfd);

			cout << "start connfd " << connfd << endl;
			cout << "get connection from " << inet_ntoa(client_addr.sin_addr) << ":" << client_addr.sin_port << endl;

			echo_str(connfd);

			cout << "close connfd " << connfd << endl;
			close(connfd);
			exit(0);
		}
		else
		{
			// parent
			close(connfd);
		}
	}

}
