#include "messages.h"
#include <cjson/cJSON.h>
#include <string.h>
#include  <stdbool.h>

bool init_messages(struct MessageList* list){

    char *json_str = read_file_to_string("msg.json");
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




//!!! Test case below |
//                    |  
/*
int main(void) {
#include <stdbool.h>
p
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