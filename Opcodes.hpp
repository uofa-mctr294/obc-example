#pragma once

enum Payloads {
    PAYLOAD_UNUSED = 0,
    PAYLOAD_OBC,
    PAYLOAD_LEOP,
    PAYLOAD_EPS,
    PAYLOAD_ADCS,
    PAYLOAD_LAST
};

enum OBCOpcodes {
    OBC_RESTART = 1,
    OBC_STOP,
};

enum EPSOpcodes {
    EPS_RESET = 1,
    EPS_ENABLE,
    EPS_DISABLE,
};

enum LEOPOpcodes {
    LEOP_DEPLOY_ALL = 1,
    LEOP_DEPLOY_UHF,
    LEOP_DEPLOY_SOLAR,
};
