#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Message.hpp"

int initReceiver(int port) {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        return -errno;
	}

	memset(&servaddr, 0, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        return -errno;
	}

	if ((listen(sockfd, 5)) != 0) {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        return -errno;
	}

    return sockfd;
}
