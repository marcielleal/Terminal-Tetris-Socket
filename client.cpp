using namespace std;
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define	PORT_MIN	1024
#define	PORT_MAX	65535

int main( int ac, char **av ){
	int	s;		// file descriptor for our socket
	char	buf[ 512 ];	// data buffer
	char	*hostname;	// name of remote host
	struct	hostent	*hostent; // to get IP address of remote host
	struct	sockaddr_in server; // address of remote socket
	int	port;		// port number of remote socket
	int	length;

	//
	// Make sure that hostname and port arguments were both given
	//
	if( ac < 3 ){
		cerr << "Usage: " << av[ 0 ] << " hostname port" << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Grab the host name
	//
	hostname = av[ 1 ];

	//
	// Get the port number, and make sure that it is legitimate
	//
	port = atoi( av[ 2 ] );
	if( port < PORT_MIN || port > PORT_MAX ){
		cerr << av[ 2 ] << ": invalid port number" << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Create a datagram (UDP) socket.  The protocol argument is left 0 to
	// allow the system to choose an appropriate protocol.
	//
	s = socket( AF_INET, SOCK_DGRAM, 0 );
	if( s < 0 ){
		cerr << "socket: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Look up the IP address of the host.  "Hostent" is a pointer
	// to a structure in which the address has been stored.
	//
	hostent = gethostbyname( hostname );
	if( hostent == NULL ){
		cerr << "gethostbyname: error code " << h_errno << endl;
		exit( EXIT_FAILURE );
	}

	//
	// Fill in the "server" structure with the address of the socket
	// that the server is listening to.  First, the entire structure
	// must be zeroed.  Then that the host address and port number
	// are converted to network byte order and stored.
	//
	bzero( &server, sizeof( server ) );
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	server.sin_addr.s_addr = *(u_long *)hostent->h_addr;
	//
	// Now read lines from the keyboard, and send each line to the
	// server as a separate datagram.
	//
	std::cout<<"Target server is "<<inet_ntoa(server.sin_addr)<<" "<<port<<std::endl;
	std::cout << "? ";
	std::cin.getline( buf, sizeof( buf ) );
	while( strlen(buf)!=0 ){
		int	len;	// message length

		//
		// Get the length of the string (plus one for the trailing
		// null byte), and send it to the server.
		//
		len = strlen( buf ) + 1;
		if( sendto( s, buf, len, 0, (struct sockaddr *)&server, sizeof( server ) ) < 0 ){
			cerr << "sendto: " << strerror( errno ) << std::endl;
			exit( EXIT_FAILURE );
		}

		//
		// Read the reply from the server, and print it.
		//
		//len = recvfrom( s, (char*)&length, sizeof(length), 0, 0, 0 );

		//std::cout << "The server echoed: "<< length  << std::endl;
		    
		//
		// Read another line.
		//
		std::cout << "? ";
		std::cin.getline( buf, sizeof( buf ) );
	}

	//
	// We're all done.  Close the socket and quit.
	//
	close( s );

	return EXIT_SUCCESS;
}