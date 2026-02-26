#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
enum MessageType{
    MESSAGE_INCOMING,
    MESSAGE_OUTGOING,
    MSG_NONE
};

struct Message{
    __uint32_t   id;
    char    *text;
    time_t  timestamp;
    enum MessageType type;
};

struct MessageList {
    struct Message *items;
    size_t  count;
    size_t  capacity;
};

bool init_messages(struct MessageList* list);

bool message_from_json(cJSON *json, struct Message* msg);
bool messages_from_json(const char *json_str, struct MessageList* list);
void messages_free(struct MessageList* list);
char* read_file_to_string(const char *filename);
