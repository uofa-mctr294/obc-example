#include "Message.hpp"

class PayloadHandler {
public:
    PayloadHandler() {}

    virtual int init() { return 0; }

    virtual int cmdHandler(CmdMsg *msg) { return 0; }
    virtual int getTelemetry(TelemetryMsg *msg) { return 0; }
    virtual int download(const char *filename) { return 0; }
    virtual int upload(const char *data, int len) { return 0; }
};

class OBCHandler : public PayloadHandler {
public:
    OBCHandler() {}

    virtual int cmdHandler(CmdMsg *msg);
};

class EPSHandler : public PayloadHandler {
public:
    EPSHandler() {}

    virtual int cmdHandler(CmdMsg *msg);
};

class ADCSHandler : public PayloadHandler {
public:
    ADCSHandler() {}
};

class LEOPHandler : public PayloadHandler {
public:
    LEOPHandler() {}

    virtual int cmdHandler(CmdMsg *msg);
};

    

    
