#include <new>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "PayloadHandler.hpp"
#include "Opcodes.hpp"

int OBCHandler::cmdHandler(CmdMsg *msg) {
    int rc = -msg->opcode;
    printf("OBC handler got op %d\n", msg->opcode);

    ReplyMsg *reply = new(msg) ReplyMsg();
    reply->len = strlen("status: OK");
    reply->status = MSG_OK;
    memcpy(reply->data, "status: OK", reply->len);
    return rc;
}

int EPSHandler::cmdHandler(CmdMsg *msg) {
    printf("EPS handler got op 0x%x, len %d\n", msg->opcode, msg->len);
    msg->data[msg->len] = 0;
    printf("time is: %s\n", msg->data);

    // Return the battery % charged.
    ReplyMsg *reply = new(msg) ReplyMsg();
    reply->len = 1;
    reply->data[0] = 50;

    reply->status = MSG_OK;
    return 0;
}

int LEOPHandler::cmdHandler(CmdMsg *msg) {
    ReplyMsg *reply = new(msg) ReplyMsg();
    int op = msg->opcode;
    switch(op) {
    case LEOP_DEPLOY_ALL:
        printf("LEOP deploy all\n");
        break;
    case LEOP_DEPLOY_UHF:
        printf("LEOP deploy UHF\n");
        break;
    case LEOP_DEPLOY_SOLAR:
        printf("LEOP deploy solar\n");
        break;
    default:
        break;
    }
    reply->status = MSG_OK;
    reply->len = 1;
    reply->data[0] = op;
    return 0;
}

