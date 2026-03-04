#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include <new>

#include "Opcodes.hpp"
#include "Message.hpp"

void help() {
    printf("\teps: send time to the EPS\n");
    printf("\tleop [1|2|3]: perform LEOP action\n");
    printf("\tobc: get OBC status\n");
}

int main(int argc, char *argv[]) {
    int sockfd;
    int obcport;
    struct sockaddr_in servaddr = {0};

    if (argc == 1) {
        printf("%s: missing OBC port\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%d", &obcport) != 1) {
        fprintf(stderr, "%s: invalid port: %s\n", argv[0], argv[1]);
    }
    
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 2;
    }
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(obcport);
 
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        perror("connect");
        return 3;
    }

    bool go = true;
    while(go) {
        char input[128];
        char cmd[CMD_MAX_LEN];
        char payload[32];
        int op;

        fprintf(stdout, "cmd> ");
        fflush(stdout);

        int len = read(STDIN_FILENO, input, sizeof(input));
        if (len <= 0) {
            if (len == -1) {
                perror("cmd read");
                return 4;
            }
            return 0;
        }

        CmdMsg *msg = new(cmd) CmdMsg();
        msg->opcode = 0;

        switch(input[0]) {
        case 'e': {
            char tstr[32];
            time_t t;
            time(&t);
            ctime_r(&t, tstr);
            msg->len = strlen(tstr) + 1;
            memcpy(msg->data, tstr, msg->len);

            msg->dest = PAYLOAD_EPS;
        }
        break;
        case 'l':
            if (sscanf(input, "%s %d", payload, &op) != 2) {
                fprintf(stderr, "Couldn't parse opcode: %s\n", input);
                continue;
            }

            msg->dest = PAYLOAD_LEOP;
            msg->len = 0;
            msg->opcode = op;
            break;
        case 'o':
            msg->dest = PAYLOAD_OBC;
            msg->len = 0;
            break;
        case 'q':
            go = false;
            continue;
        default:
            help();
            continue;
        }

        /* Send the command to the OBC */
        int rc = write(sockfd, msg, msg->size());
        if (rc <= 0) {
            if (len == -1) {
                perror("cmd send");
                return 5;
            }
            return 0;
        }

        /* Get the synchronous response */
        len = read(sockfd, cmd, sizeof(cmd));
        if (len <= 0) {
            if (len == -1) {
                perror("reply");
                return 6;
            }
            return 0;
        }

        ReplyMsg *reply = new(cmd) ReplyMsg();
        printf("Response status %d (len %d)", reply->status, reply->len);
        switch(reply->len) {
        case 1:
            printf(": %d", reply->data[0]);
            break;
        default:
            reply->data[reply->len] = 0;
            printf(": %s", reply->data);
            break;
        }
        printf("\n");
    }

    close(sockfd);
    return 0;
}
