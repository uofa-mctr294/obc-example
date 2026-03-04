#pragma once

#include <stdint.h>

#define CMD_MAX_LEN 128

enum ReplyStatus {
    MSG_OK = 0,
    MSG_BAD_DEST,
    MSG_BAD_OPCODE,
    MSG_FAILED_REPLY,
    MSG_LAST
};


class Message {
public:
    uint8_t len; // payload size only
    uint8_t dest;

    Message() {}

    int size() { return sizeof(Message) + len; }
};

class CmdMsg : public Message {
public:
    uint8_t opcode;
    uint8_t data[0];

    CmdMsg() {}

    int size() { return sizeof(CmdMsg) + len; }
};

class ReplyMsg : public Message {
public:
    uint8_t status;
    uint8_t data[0];

    ReplyMsg() {}

    int size() { return sizeof(ReplyMsg) + len; }
};

class XferMsg : public Message {
public:
    uint8_t seqno;
    uint8_t data[0];

    XferMsg() {}
};

class TelemetryMsg : public Message {
public:
    uint8_t payload;
    uint8_t paylen;
    uint8_t data[0];

    TelemetryMsg() {}
};

int initReceiver(int port);
