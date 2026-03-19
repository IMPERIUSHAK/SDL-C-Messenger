#include "messages.h"
#include <cjson/cJSON.h>
#include <string.h>
#include  <stdbool.h>


#define MSGS "msg.json" 


bool init_messages(struct MessageList* list){

    char *json_str = read_file_to_string(MSGS);
    if(!json_str){
        return true;
    }
    
    if(!messages_from_json(json_str, list)){
        return true;
    }

    free(json_str);
    return false;
}

enum MessageType message_type_from_string(const char *type) {
    if (strcmp(type, "MESSAGE_INCOMING") == 0)
        return MESSAGE_INCOMING;
    if (strcmp(type, "MESSAGE_OUTGOING") == 0)
        return MESSAGE_OUTGOING;
    if (strcmp(type, "MESSAGE_SYSTEM") == 0)
        return MSG_NONE;

    return MSG_NONE; 
}

bool message_from_json(cJSON *json, struct Message* msg) {
    
    cJSON *id = cJSON_GetObjectItem(json, "id");
    cJSON *text = cJSON_GetObjectItem(json, "text");
    cJSON *timestamp = cJSON_GetObjectItem(json, "timestamp");
    cJSON *type = cJSON_GetObjectItem(json, "type");

    if (!cJSON_IsNumber(id) ||
        !cJSON_IsString(text) ||
        !cJSON_IsNumber(timestamp) ||
        !cJSON_IsString(type)) {
        return false;
    }

    msg->id = id->valueint;
    msg->text = strdup(text->valuestring); 
    msg->timestamp = (time_t)timestamp->valuedouble;
    msg->type = message_type_from_string(type->valuestring);

    return true;
}

bool messages_from_json(const char *json_str, struct MessageList* list) {
    cJSON *root = cJSON_Parse(json_str);
    if (!root)
        return false;

    cJSON *messages = cJSON_GetObjectItem(root, "messages");
    if (!cJSON_IsArray(messages)) {
        cJSON_Delete(root);
        return false;
    }

    size_t count = cJSON_GetArraySize(messages);

    list->items = calloc(count, sizeof(struct Message));
    list->count = count;
    list->capacity = count;

    for (size_t i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(messages, i);
        if (!message_from_json(item, &list->items[i])) {
            cJSON_Delete(root);
            return false;
        }
    }

    cJSON_Delete(root);
    return true;
}

void messages_free(struct MessageList* list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->items[i].text);
    }
    free(list->items);

    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

char *read_file_to_string(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size <= 0) {
        fclose(fp);
        return NULL;
    }

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    size_t read = fread(buffer, 1, size, fp);
    buffer[read] = '\0';

    fclose(fp);
    return buffer;
}

int update_id(cJSON *str){
    int len = cJSON_GetArraySize(str);
    if (len == 0) return 1;
    
    cJSON *last_item = cJSON_GetArrayItem(str, len - 1);
    cJSON *last_id = cJSON_GetObjectItemCaseSensitive(last_item, "id");
    
    if (!cJSON_IsNumber(last_id)) {
        printf("Error: 'id' is not a number\n");
        return 1;
    }
    
    return (last_id->valueint) + 1;
}



bool update_json(struct Message *obj){
    char *str = read_file_to_string(MSGS);
    cJSON *parse_json = cJSON_Parse(str);

    cJSON *msgArr = cJSON_GetObjectItemCaseSensitive(parse_json, "messages");
    int id = update_id(msgArr);


    if (!cJSON_IsArray(msgArr)) {
        fprintf(stderr, "Error: 'messages' is not an array\n");
        return true;
    }

    cJSON *newObj = cJSON_CreateObject();
    if (!newObj){
        fprintf(stderr, "error with new json obj\n");
    }

    cJSON_AddNumberToObject(newObj, "id", id);
    cJSON_AddStringToObject(newObj, "text", obj->text);
    cJSON_AddNumberToObject(newObj, "timestamp", obj->timestamp);
    
    switch (obj->type)
    {
    case MESSAGE_INCOMING:
        cJSON_AddStringToObject(newObj, "type", "MESSAGE_INCOMING");
        break;
    case MESSAGE_OUTGOING:
        cJSON_AddStringToObject(newObj, "type", "MESSAGE_OUTGOING");
        break;
    default:
        break;
    }
    

    cJSON_AddItemToArray(msgArr, newObj);
    

    cJSON *count = cJSON_GetObjectItemCaseSensitive(parse_json, "count");
    if (!cJSON_IsNumber(count)) {
        fprintf(stderr, "Error with json input variable\n");
        return true;
    }
    
    cJSON_SetNumberValue(count, count->valueint + 1);
    
    cJSON *capacity = cJSON_GetObjectItemCaseSensitive(parse_json, "capacity");
    if (!cJSON_IsNumber(capacity)) {
        fprintf(stderr, "Error with updating json variable\n");
    }

    int new_cap = cJSON_GetArraySize(msgArr);
    cJSON_SetNumberValue(capacity, new_cap);

    char *new_json = cJSON_Print(parse_json);
    FILE *fp = fopen(MSGS, "w");
    if (fp == NULL){
        fprintf(stderr, "Error: Could not open file for writing\n");
        return true;
    }
    fprintf(fp, "%s", new_json);
    
    
    fclose(fp);
    free(new_json);
    cJSON_Delete(parse_json);
    
    return false;
}


void queue_push(struct JsonQueue *q, struct Message *msg)
{
    
    struct Message *dst = &q->msgs[q->count];

    dst->id = msg->id;
    dst->timestamp = msg->timestamp;
    dst->type = msg->type;

    dst->text = strdup(msg->text);

    q->count++;

}

void *json_worker(void *arg)
{
    struct JsonQueue *q = arg;

    while (1) {

        pthread_mutex_lock(&q->mutex);

        if (q->count > 0) {

            struct Message msg = q->msgs[0];

            for (int i = 1; i < q->count; i++)
                q->msgs[i-1] = q->msgs[i];

            q->count--;

            pthread_mutex_unlock(&q->mutex);

            update_json(&msg);
            pthread_cond_signal(&q->cond);
            free(msg.text);
        }
        else {
            pthread_mutex_unlock(&q->mutex);
        }
    }

    return NULL;
}
//     |    !!!Test case    |
//     |                    |  
/*


int main(void) {


    struct MessageList msgs = {0};

    char *json_str = read_file_to_string("msg.json");
    
    if (!json_str) {
        fprintf(stderr, "Failed to read file\n");
        return 1;
    }

    if (!messages_from_json(json_str, &msgs)) {
        fprintf(stderr, "Failed to parse MESSAGES\n");
        free(json_str);
        return 1;
    }

    free(json_str);

    for (size_t i = 0; i < msgs.count; i++) {
        printf("[%zu] %s\n", i, msgs.items[i].text);
    }

    messages_free(&msgs);
    return 0;
}
*/
