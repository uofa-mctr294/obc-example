#pragma once

#include <vector>

#include "Opcodes.hpp"

class PayloadHandler;

class OBC {
public:
    OBC() { payloads.reserve(PAYLOAD_LAST); }

    int init(int cmdport, int fileport);

    void handleCmds();
    void handleFiles();

private:
    int cmdsock;
    int filesock;

    std::vector<PayloadHandler*> payloads;

    int cmdDispatch(int fd, void *msg);
};

    
