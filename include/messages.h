#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum MessageType{
    MESSAGE_INCOMING,
    MESSAGE_OUTGOING,
    MESAGE_SYSTEM
};

struct Message{
    u_int64_t   id;
    char    *text;
    time_t  timestamp;
    enum MessageType type;
};

struct MessageList {
    Message *items;
    size_t  count;
    size_t  capacity;
};