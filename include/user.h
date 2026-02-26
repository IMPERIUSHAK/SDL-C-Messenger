#include "messages.h"
#include <stdint.h>

struct User{
    uint64_t id;
    char    *name;
    struct MessageList list;
};