//------------------------------------------------------------------------------------------------
// File: NetworkTCP.cpp
// Project: LG Exec Ed Program
// Versions:
// 1.0 April 2017 - initial version
// Provides the ability to send and recvive TCP byte streams for both Window and linux platforms
//------------------------------------------------------------------------------------------------
#include <iostream>
#include <new>
#include <stdio.h>
#include <string.h>
#include "NetworkTCP.h"
#include "ProtocolManager.h"

//-----------------------------------------------------------------
// OpenTCPListenPort - Creates a Listen TCP port to accept
// connection requests
//-----------------------------------------------------------------
TTcpListenPort *OpenTcpListenPort(short localport)
{
	TTcpListenPort *TcpListenPort;
	struct sockaddr_in myaddr;

	TcpListenPort= new (std::nothrow) TTcpListenPort;  

	if (TcpListenPort==NULL)
	{
		fprintf(stderr, "TUdpPort memory allocation failed\n");
		return(NULL);
	}
	TcpListenPort->ListenFd=BAD_SOCKET_FD;
#if  defined(_WIN32) || defined(_WIN64)
	WSADATA wsaData;
	int     iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		delete TcpListenPort;
		printf("WSAStartup failed: %d\n", iResult);
		return(NULL);
	}
#endif
	// create a socket
	if ((TcpListenPort->ListenFd= socket(AF_INET, SOCK_STREAM, 0)) == BAD_SOCKET_FD)
	{
		CloseTcpListenPort(&TcpListenPort);
		perror("socket failed");
		return(NULL);  
	}
	int option = 1; 

	if(setsockopt(TcpListenPort->ListenFd,SOL_SOCKET,SO_REUSEADDR,(char*)&option,sizeof(option)) < 0)
	{
		CloseTcpListenPort(&TcpListenPort);
		perror("setsockopt failed");
		return(NULL);
	}

	// bind it to all local addresses and pick any port number
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(localport);

	if (bind(TcpListenPort->ListenFd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		CloseTcpListenPort(&TcpListenPort);
		perror("bind failed");
		return(NULL); 
	}


	if (listen(TcpListenPort->ListenFd,5)< 0)
	{
		CloseTcpListenPort(&TcpListenPort);
		perror("bind failed");
		return(NULL);	  
	}
	return(TcpListenPort);
}
//-----------------------------------------------------------------
// END OpenTCPListenPort
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// CloseTcpListenPort - Closes the specified TCP listen port
//-----------------------------------------------------------------
void CloseTcpListenPort(TTcpListenPort **TcpListenPort)
{
	if ((*TcpListenPort)==NULL) return;
	if ((*TcpListenPort)->ListenFd!=BAD_SOCKET_FD)  
	{
		CLOSE_SOCKET((*TcpListenPort)->ListenFd);
		(*TcpListenPort)->ListenFd=BAD_SOCKET_FD;
	}
	delete (*TcpListenPort);
	(*TcpListenPort)=NULL;
#if  defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}
//-----------------------------------------------------------------
// END CloseTcpListenPort
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// AcceptTcpConnection -Accepts a TCP Connection request from a 
// Listening port
//-----------------------------------------------------------------
TTcpConnectedPort *AcceptTcpConnectionTLS(TTcpListenPort *TcpListenPort, 
		struct sockaddr_in *cli_addr,socklen_t *clilen)
{
	TTcpConnectedPort *TcpConnectedPort;
	int ret = 0;

	TcpConnectedPort= new (std::nothrow) TTcpConnectedPort;  

	if (TcpConnectedPort==NULL)
	{
		fprintf(stderr, "TUdpPort memory allocation failed\n");
		return(NULL);
	}
	
	TcpConnectedPort->ConnectedFd= accept(TcpListenPort->ListenFd,
			(struct sockaddr *) cli_addr,clilen);

	if (TcpConnectedPort->ConnectedFd== BAD_SOCKET_FD) 
	{
		perror("ERROR on accept");
		delete TcpConnectedPort;
		return NULL;
	}

	int bufsize = 200 * 1024;
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, 
				SO_RCVBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, 
				SO_SNDBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}

	/* declare wolfSSL objects */
	TcpConnectedPort->ctx = NULL;
	
	/* Create and initialize WOLFSSL_CTX */
	if ((TcpConnectedPort->ctx = wolfSSL_CTX_new(wolfTLSv1_3_server_method())) == NULL) {
		perror("ERROR: failed to create WOLFSSL_CTX");
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Load server certificates into WOLFSSL_CTX */
	if (wolfSSL_CTX_use_certificate_file(TcpConnectedPort->ctx, CERT_FILE, WOLFSSL_FILETYPE_PEM)
			!= WOLFSSL_SUCCESS) {
		fprintf(stderr, "ERROR: failed to load %s, please check the file.\n",
				CERT_FILE);
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Load server key into WOLFSSL_CTX */
	if (wolfSSL_CTX_use_PrivateKey_file(TcpConnectedPort->ctx, KEY_FILE, WOLFSSL_FILETYPE_PEM)
			!= WOLFSSL_SUCCESS) {
		fprintf(stderr, "ERROR: failed to load %s, please check the file.\n",
				KEY_FILE);
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}


	/* Create a WOLFSSL object */
	if ((TcpConnectedPort->ssl = wolfSSL_new(TcpConnectedPort->ctx)) == NULL) {
		perror("ERROR: failed to create WOLFSSL object");
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Attach wolfSSL to the socket */
	wolfSSL_set_fd(TcpConnectedPort->ssl, TcpConnectedPort->ConnectedFd);

	/* Establish TLS connection */
	if ((ret = wolfSSL_accept(TcpConnectedPort->ssl)) != WOLFSSL_SUCCESS) {
		fprintf(stderr, "wolfSSL_accept error = %d\n",
				wolfSSL_get_error(TcpConnectedPort->ssl, ret));
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	return TcpConnectedPort;
}
TTcpConnectedPort *AcceptTcpConnection(TTcpListenPort *TcpListenPort, 
		struct sockaddr_in *cli_addr,socklen_t *clilen)
{
	TTcpConnectedPort *TcpConnectedPort;

	TcpConnectedPort= new (std::nothrow) TTcpConnectedPort;  

	if (TcpConnectedPort==NULL)
	{
		fprintf(stderr, "TUdpPort memory allocation failed\n");
		return(NULL);
	}
	TcpConnectedPort->ConnectedFd= accept(TcpListenPort->ListenFd,
			(struct sockaddr *) cli_addr,clilen);

	if (TcpConnectedPort->ConnectedFd== BAD_SOCKET_FD) 
	{
		perror("ERROR on accept");
		delete TcpConnectedPort;
		return NULL;
	}

	int bufsize = 200 * 1024;
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, 
				SO_RCVBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, 
				SO_SNDBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}


	return TcpConnectedPort;
}
//-----------------------------------------------------------------
// END AcceptTcpConnection
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// OpenTCPConnection - Creates a TCP Connection to a TCP port
// accepting connection requests
//-----------------------------------------------------------------
TTcpConnectedPort *OpenTcpConnectionTLS(const char *remotehostname, const char * remoteportno)
{
	TTcpConnectedPort *TcpConnectedPort;
	struct sockaddr_in myaddr;
	int                s;
	struct addrinfo   hints;
	struct addrinfo   *result = NULL;

	TcpConnectedPort= new (std::nothrow) TTcpConnectedPort;  

	if (TcpConnectedPort==NULL)
	{
		fprintf(stderr, "TUdpPort memory allocation failed\n");
		return(NULL);
	}
	TcpConnectedPort->ConnectedFd=BAD_SOCKET_FD;
#if  defined(_WIN32) || defined(_WIN64)
	WSADATA wsaData;
	int     iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		delete TcpConnectedPort;
		printf("WSAStartup failed: %d\n", iResult);
		return(NULL);
	}
#endif
	// create a socket
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	s = getaddrinfo(remotehostname, remoteportno, &hints, &result);
	if (s != 0) 
	{
		delete TcpConnectedPort;
		fprintf(stderr, "getaddrinfo: Failed\n");
		return(NULL);
	}
	if ( result==NULL)
	{
		delete TcpConnectedPort;
		fprintf(stderr, "getaddrinfo: Failed\n");
		return(NULL);
	}
	if ((TcpConnectedPort->ConnectedFd= socket(AF_INET, SOCK_STREAM, 0)) == BAD_SOCKET_FD)
	{
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		freeaddrinfo(result);
		perror("socket failed");
		return(NULL);  
	}

	int bufsize = 200 * 1024;
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET,
				SO_SNDBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, 
				SO_RCVBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}

	timeval tv;
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, SO_RCVTIMEO,(char*)&tv, sizeof(timeval)) == -1)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		perror("setsockopt SO_RCVTIMEO failed");
		return(NULL);
	}	

	if (connect(TcpConnectedPort->ConnectedFd,result->ai_addr,result->ai_addrlen) < 0) 
	{
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		freeaddrinfo(result);
		perror("connect failed");
		return(NULL);
	}

	freeaddrinfo(result);	 
	
	/*---------------------------------*/
	/* Start of security */
	/*---------------------------------*/
	/* Create and initialize WOLFSSL_CTX */
	if ((TcpConnectedPort->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method())) == NULL) {
		fprintf(stderr, "ERROR: failed to create WOLFSSL_CTX\n");
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Load client certificates into WOLFSSL_CTX */
	if (wolfSSL_CTX_load_verify_locations(TcpConnectedPort->ctx, CHAIN_CERT_FILE, NULL)
			!= WOLFSSL_SUCCESS) {
		fprintf(stderr, "ERROR: failed to load %s, please check the file.\n",
				CERT_FILE);
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Create a WOLFSSL object */
	if ((TcpConnectedPort->ssl = wolfSSL_new(TcpConnectedPort->ctx)) == NULL) {
		fprintf(stderr, "ERROR: failed to create WOLFSSL object\n");
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Attach wolfSSL to the socket */
	if (wolfSSL_set_fd(TcpConnectedPort->ssl, TcpConnectedPort->ConnectedFd) != WOLFSSL_SUCCESS) {
		fprintf(stderr, "ERROR: Failed to set the file descriptor\n");
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}

	/* Connect to wolfSSL on the server side */
	if (wolfSSL_connect(TcpConnectedPort->ssl) != WOLFSSL_SUCCESS) {
		fprintf(stderr, "ERROR: failed to connect to wolfSSL\n");
		CloseTcpConnectedPortTLS(&TcpConnectedPort);
		return(NULL);
	}


	return(TcpConnectedPort);
}
TTcpConnectedPort *OpenTcpConnection(const char *remotehostname, const char * remoteportno)
{
	TTcpConnectedPort *TcpConnectedPort;
	struct sockaddr_in myaddr;
	int                s;
	struct addrinfo   hints;
	struct addrinfo   *result = NULL;

	TcpConnectedPort= new (std::nothrow) TTcpConnectedPort;  

	if (TcpConnectedPort==NULL)
	{
		fprintf(stderr, "TUdpPort memory allocation failed\n");
		return(NULL);
	}
	TcpConnectedPort->ConnectedFd=BAD_SOCKET_FD;
#if  defined(_WIN32) || defined(_WIN64)
	WSADATA wsaData;
	int     iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		delete TcpConnectedPort;
		printf("WSAStartup failed: %d\n", iResult);
		return(NULL);
	}
#endif
	// create a socket
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	s = getaddrinfo(remotehostname, remoteportno, &hints, &result);
	if (s != 0) 
	{
		delete TcpConnectedPort;
		fprintf(stderr, "getaddrinfo: Failed\n");
		return(NULL);
	}
	if ( result==NULL)
	{
		delete TcpConnectedPort;
		fprintf(stderr, "getaddrinfo: Failed\n");
		return(NULL);
	}
	if ((TcpConnectedPort->ConnectedFd= socket(AF_INET, SOCK_STREAM, 0)) == BAD_SOCKET_FD)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		freeaddrinfo(result);
		perror("socket failed");
		return(NULL);  
	}

	int bufsize = 200 * 1024;
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET,
				SO_SNDBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, 
				SO_RCVBUF, (char *)&bufsize, sizeof(bufsize)) == -1)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		perror("setsockopt SO_SNDBUF failed");
		return(NULL);
	}

	timeval tv;
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	if (setsockopt(TcpConnectedPort->ConnectedFd, SOL_SOCKET, SO_RCVTIMEO,(char*)&tv, sizeof(timeval)) == -1)
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		perror("setsockopt SO_RCVTIMEO failed");
		return(NULL);
	}	

	if (connect(TcpConnectedPort->ConnectedFd,result->ai_addr,result->ai_addrlen) < 0) 
	{
		CloseTcpConnectedPort(&TcpConnectedPort);
		freeaddrinfo(result);
		perror("connect failed");
		return(NULL);
	}
	freeaddrinfo(result);	 
	return(TcpConnectedPort);
}
//-----------------------------------------------------------------
// END OpenTcpConnection
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// CloseTcpConnectedPort - Closes the specified TCP connected port
//-----------------------------------------------------------------
void CloseTcpConnectedPortTLS(TTcpConnectedPort **TcpConnectedPort)
{
	if ((*TcpConnectedPort)==NULL) return;

	if ((*TcpConnectedPort)->ConnectedFd!=BAD_SOCKET_FD)  
	{
		CLOSE_SOCKET((*TcpConnectedPort)->ConnectedFd);
		(*TcpConnectedPort)->ConnectedFd=BAD_SOCKET_FD;
	}
	
	if((*TcpConnectedPort)->ssl) {
		wolfSSL_free((*TcpConnectedPort)->ssl);
	}
	if((*TcpConnectedPort)->ctx) {
		wolfSSL_CTX_free((*TcpConnectedPort)->ctx);
	}
	delete (*TcpConnectedPort);
	(*TcpConnectedPort)=NULL;
#if  defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}
void CloseTcpConnectedPort(TTcpConnectedPort **TcpConnectedPort)
{
	if ((*TcpConnectedPort)==NULL) return;
	if ((*TcpConnectedPort)->ConnectedFd!=BAD_SOCKET_FD)  
	{
		CLOSE_SOCKET((*TcpConnectedPort)->ConnectedFd);
		(*TcpConnectedPort)->ConnectedFd=BAD_SOCKET_FD;
	}
	delete (*TcpConnectedPort);
	(*TcpConnectedPort)=NULL;
#if  defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}
//-----------------------------------------------------------------
// END CloseTcpListenPort
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// ReadDataTcp - Reads the specified amount TCP data 
//-----------------------------------------------------------------

void print_pkt_header(const unsigned char* buff,int size) {

	for (int i=0; i<size; i++) {
		printf("%d[%d][%c] ",i, buff[i], (buff[i]<32 ? ' ' : buff[i])  );		
	}
	printf("\n");

}


ssize_t ReadDataTcpTLS(TTcpConnectedPort *TcpConnectedPort,unsigned char *data, size_t length)
{
	ssize_t bytes;
	ssize_t my_packet_size=-1;
	ssize_t accumulated=0;

	for (size_t i = 0; i < length; i += bytes)
	{
		if ((bytes = wolfSSL_recv(TcpConnectedPort->ssl, (char *)(data+i), length  - i,0)) == -1) 
		{
			return (-1);
		}
		accumulated+=bytes;
		if (i==0) {
			MyPacket *p=(MyPacket*)data;
			printf("max packet length=%zu received=%zu packet_length=%d timestamp=%u msgtype=%d\n", 
						length, bytes, p->hdr.size , p->hdr.timestamp, p->hdr.msgtype);
			if (p->hdr.head[0]=='S' && p->hdr.head[1]=='B' && p->hdr.head[2]=='1' && p->hdr.head[3]=='T') {
				my_packet_size=p->hdr.size;
			}
			// print_pkt_header(data,60);
		}
		if (my_packet_size==-1) {
			MyPacket *p=(MyPacket*)(data+i);
			if (p->hdr.head[0]=='S' && p->hdr.head[1]=='B' && p->hdr.head[2]=='1' && p->hdr.head[3]=='T') {
				my_packet_size=p->hdr.size;
				printf("Found Header\n");
				memcpy(data,data+i,bytes);
				i=0;
				accumulated=bytes;
			}
		}
		printf("accumulated packets=%zu   my_packet_size=%zd\n",accumulated, my_packet_size );
		if (my_packet_size>0 && accumulated>=my_packet_size)
			return accumulated;
	}
	return(length);
}


ssize_t ReadDataTcp(TTcpConnectedPort *TcpConnectedPort,unsigned char *data, size_t length)
{
	ssize_t bytes;
	ssize_t my_packet_size=-1;
	ssize_t accumulated=0;

	for (size_t i = 0; i < length; i += bytes)
	{
		if ((bytes = recv(TcpConnectedPort->ConnectedFd, (char *)(data+i), length  - i,0)) == -1) 
		{
			return (-1);
		}
		accumulated+=bytes;
		if (i==0) {
			MyPacket *p=(MyPacket*)data;
			printf("max packet length=%zu received=%zu packet_length=%d timestamp=%u msgtype=%d\n", 
						length, bytes, p->hdr.size , p->hdr.timestamp, p->hdr.msgtype);
			if (p->hdr.head[0]=='S' && p->hdr.head[1]=='B' && p->hdr.head[2]=='1' && p->hdr.head[3]=='T') {
				my_packet_size=p->hdr.size;
			}
			// print_pkt_header(data,60);
		}
		if (my_packet_size==-1) {
			MyPacket *p=(MyPacket*)(data+i);
			if (p->hdr.head[0]=='S' && p->hdr.head[1]=='B' && p->hdr.head[2]=='1' && p->hdr.head[3]=='T') {
				my_packet_size=p->hdr.size;
				printf("Found Header\n");
				memcpy(data,data+i,bytes);
				i=0;
				accumulated=bytes;
			}
		}
		printf("accumulated packets=%zu   my_packet_size=%zd\n",accumulated, my_packet_size );
		if (my_packet_size>0 && accumulated>=my_packet_size)
			return accumulated;
	}
	return(length);
}
//-----------------------------------------------------------------
// END ReadDataTcp
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// WriteDataTcp - Writes the specified amount TCP data 
//-----------------------------------------------------------------
ssize_t WriteDataTcpTLS(TTcpConnectedPort *TcpConnectedPort,unsigned char *data, size_t length)
{
	ssize_t total_bytes_written = 0;
	ssize_t bytes_written;
	while (total_bytes_written != length)
	{
		bytes_written = wolfSSL_send(TcpConnectedPort->ssl,
				(char *)(data+total_bytes_written),
				length - total_bytes_written,0);
		if (bytes_written == -1)
		{
			return(-1);
		}
		total_bytes_written += bytes_written;
	}
	return(total_bytes_written);
}
ssize_t WriteDataTcp(TTcpConnectedPort *TcpConnectedPort,unsigned char *data, size_t length)
{
	ssize_t total_bytes_written = 0;
	ssize_t bytes_written;
	while (total_bytes_written != length)
	{
		bytes_written = send(TcpConnectedPort->ConnectedFd,
				(char *)(data+total_bytes_written),
				length - total_bytes_written,0);
		if (bytes_written == -1)
		{
			return(-1);
		}
		total_bytes_written += bytes_written;
	}
	return(total_bytes_written);
}
//-----------------------------------------------------------------
// END WriteDataTcp
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// END of File
//-----------------------------------------------------------------


