#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <time.h>

enum MessageType{
    MESSAGE_INCOMING,
    MESSAGE_OUTGOING,
    MESAGE_SYSTEM,
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

int main(){

    struct Message msg = {
        .id = 0,
        .text = NULL,
        .timestamp = 0,
        .type = MSG_NONE
    };
    
    struct MessageList msgs = {
        .items = &msg,
        .count = 0,
        .capacity = 0,
    };

    cJSON *ans = cJSON_CreateObject();
    if(ans == NULL){
        fprintf(stderr, "Error with cJSON init");
        return -1;
    }
    //creating object inside JSON object

    //Initializing Message
    //cJSON*str1 = cJSON_CreateString("Message");
    cJSON *msgArr = cJSON_CreateArray();
    cJSON *msgJ = cJSON_CreateObject();
    cJSON *id= cJSON_CreateNumber(1);
    cJSON *text = cJSON_CreateString("hello");
    cJSON *timestamp = cJSON_CreateNumber(983753);
    cJSON *type = cJSON_CreateString("MESSAGE_INCOMING");


    //fill Message
    
    cJSON_AddItemToObject(msgJ, "id", id);
    cJSON_AddItemToObject(msgJ, "text", text);
    cJSON_AddItemToObject(msgJ, "timestamp", timestamp);
    cJSON_AddItemToObject(msgJ, "type", type);
    

    //init Message List
    cJSON *count = cJSON_CreateNumber(87);
    cJSON *capacity = cJSON_CreateNumber(144);    
    cJSON_AddItemToArray(msgArr, msgJ);
    cJSON_AddItemToObject(ans, "messages", msgArr);
    cJSON_AddItemToObject(ans, "count", count);
    cJSON_AddItemToObject(ans, "capacity", capacity);


    char *stri = cJSON_Print(ans);

    printf("%s", stri);

    FILE *fp;

    fp = fopen("msg.json", "w+");
    fprintf(fp, stri);
        
    return 0;
}