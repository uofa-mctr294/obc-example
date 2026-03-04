#include <unistd.h>
#include <sys/socket.h>

#include "OBC.hpp"
#include "Message.hpp"
#include "PayloadHandler.hpp"

int OBC::init(int cmdPort, int filePort) {
    int i;
    for (i=0; i<PAYLOAD_LAST; i++) {
        if (payloads[i]) {
            delete payloads[i];
            payloads[i] = 0;
        }
    }

    payloads[PAYLOAD_OBC] = new OBCHandler();
    payloads[PAYLOAD_LEOP] = new LEOPHandler();
    payloads[PAYLOAD_EPS] = new EPSHandler();
    payloads[PAYLOAD_ADCS] = new ADCSHandler();

    for (i=0; i<PAYLOAD_LAST; i++) {
        /* We don't really care if init fails,
         * because we still want to keep
         * monitoring the payload to see if its
         * health changes.
         */
        if (payloads[i]) payloads[i]->init();
    }

    if (cmdsock > 0) close(cmdsock);
    cmdsock = initReceiver(cmdPort);
    if (filesock > 0) close(filesock);
    filesock = initReceiver(filePort);
    if (cmdsock<=0 || filesock<=0) {
        fprintf(stderr, "hmm, we are in autonomous mode?\n");
        return 1;
    }

    return 0;
}

void OBC::handleCmds() {
    struct sockaddr client;
    socklen_t clientlen = sizeof(client);
    char msgbuf[CMD_MAX_LEN];

    while(1) {
        int connfd = accept(cmdsock, &client, &clientlen);
        if (connfd < 0) {
            fprintf(stderr, "accept error: %s\n", strerror(errno));
            break;
        }

        while (1) {
            int len = read(connfd, msgbuf, sizeof(msgbuf));
            if (len > 0) {
                // This would probably be a good place for try/catch
                int rc = cmdDispatch(connfd, msgbuf);
                switch(rc) {
                case -OBC_RESTART:
                    fprintf(stderr, "OBC restart command received\n");
                    break;
                case -OBC_STOP:
                    fprintf(stderr, "OBC stop command received\n");
                    close(cmdsock);
                    return;
                default:
                    break;
                }
            }
            else if(len == 0) {
                fprintf(stderr, "connection closed\n");
                break;
            }
            else {
                fprintf(stderr, "read error: %s\n", strerror(errno));
                break;
            }
        }
    }

	// After chatting close the socket
	close(cmdsock);
}

int OBC::cmdDispatch(int fd, void *msgbuf) {
    CmdMsg *msg = new(msgbuf) CmdMsg();
    ReplyMsg *reply = new(msgbuf) ReplyMsg();
    int rc = 0;

    if (msg->dest < 0 || msg->dest >= PAYLOAD_LAST
        || !payloads[msg->dest]) {
        fprintf(stderr, "bad dest: %d\n", msg->dest);
        reply->status = MSG_BAD_DEST;
        reply->len = 0;
    }
    else {
        rc = payloads[msg->dest]->cmdHandler(msg);
    }

    int wlen = write(fd, reply, reply->size());
    if (wlen != reply->size()) {
        if (wlen == -1) {
            fprintf(stderr, "write error: %s\n", strerror(errno));
        }
        else {
            fprintf(stderr, "short write: wanted %d, got %d \n",
                    reply->size(), wlen);
        }
        // Not sure if failed reply should override payload return status
        rc = MSG_FAILED_REPLY;
    }

    return rc;
}
