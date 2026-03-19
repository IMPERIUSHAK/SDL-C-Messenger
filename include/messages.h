#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <pthread.h>
#include <unistd.h>

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

struct JsonQueue {
    struct Message msgs[64];
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

bool init_messages(struct MessageList* list);

bool message_from_json(cJSON *json, struct Message* msg);
bool messages_from_json(const char *json_str, struct MessageList* list);
bool update_json(struct Message *obj);

void queue_push(struct JsonQueue *q, struct Message *msg);
void* json_worker(void* arg);
void messages_free(struct MessageList* list);
char* read_file_to_string(const char *filename);
