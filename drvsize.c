#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "erl_driver.h"

static ErlDrvData
start(ErlDrvPort port, char* buf)
{
    set_port_control_flags(port, PORT_CONTROL_FLAG_BINARY);
    return (ErlDrvData) port;
}

static int
control(ErlDrvData data, unsigned int command,
            char *ibuf, int ilen,
            char **rbuf, int rlen)
{
    uint32_t val = htonl(ilen);
    if(sizeof(ilen) > rlen) {
        *rbuf = (char*) driver_alloc_binary(sizeof(val));
        if(*rbuf == NULL) return -1;
    }
    memcpy(*rbuf, &val, sizeof(val));
    return sizeof(val);
}

ErlDrvEntry drvsize_entry =
{
    NULL,
    start,
    NULL,
    NULL,
    NULL,
    NULL,
    "drvsize",
    NULL,
    NULL,
    control,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ERL_DRV_EXTENDED_MARKER,
    ERL_DRV_EXTENDED_MAJOR_VERSION,
    ERL_DRV_EXTENDED_MINOR_VERSION,
    ERL_DRV_FLAG_USE_PORT_LOCKING,
    NULL,
    NULL,
};

DRIVER_INIT(drvsize)
{
    return &drvsize_entry;
}
