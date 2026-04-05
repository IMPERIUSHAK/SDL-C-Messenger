//TEST CASE
//READ/UPDATE json by using cjson library



#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <time.h>
#include <string.h>
static char buffer[16];

char* get_timestamp(){
   time_t rawtime;
   struct tm * timeinfo;
   
   time (&rawtime);
   timeinfo = localtime (&rawtime);
   
   strftime (buffer,16,"%s",timeinfo);
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
    
    return last_id->valueint + 1;
}

struct Message{
    int id;
    char *text;
    time_t timestamp;
};

void update_json(struct Message obj, cJSON *str){

    cJSON *msgArr = cJSON_GetObjectItemCaseSensitive(str, "messages");
    if (!cJSON_IsArray(msgArr)) {
        printf("Error: 'messages' is not an array\n");
        return;
    }

    cJSON *newObj = cJSON_CreateObject();
    cJSON_AddNumberToObject(newObj, "id", obj.id);
    cJSON_AddStringToObject(newObj, "text", obj.text);
    cJSON_AddNumberToObject(newObj, "timestamp", obj.timestamp);
    

    cJSON_AddItemToArray(msgArr, newObj);
    

    cJSON *count = cJSON_GetObjectItemCaseSensitive(str, "count");
    if (cJSON_IsNumber(count)) {
        cJSON_SetNumberValue(count, count->valueint + 1);
    }
    
    cJSON *capacity = cJSON_GetObjectItemCaseSensitive(str, "capacity");
    if (cJSON_IsNumber(capacity)) {
        int new_cap = cJSON_GetArraySize(msgArr);
        cJSON_SetNumberValue(capacity, new_cap);
    }
}

int main(){
    struct Message newmsg = {0};

    FILE *file = fopen("msg1.json", "r");
    if (file == NULL){
        printf("Error: Could not open file msg1.json\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = (char*)malloc(file_size + 1);
    if (file_content == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    size_t bytes_read = fread(file_content, 1, file_size, file);
    file_content[bytes_read] = '\0';
    fclose(file);

    cJSON *parse_json = cJSON_Parse(file_content);
    free(file_content);
    
    if (parse_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error parsing JSON: %s\n", error_ptr);
        }
        return 1;
    }
    
    cJSON *msgs = cJSON_GetObjectItemCaseSensitive(parse_json, "messages");
    if (!cJSON_IsArray(msgs)) {
        printf("Error: 'messages' not found or not an array\n");
        cJSON_Delete(parse_json);
        return 1;
    }

    newmsg.id = update_id(msgs);
    newmsg.text = "NEW ONE!!";
    newmsg.timestamp = time(NULL);
    
    update_json(newmsg, parse_json);

    char *ans = cJSON_Print(parse_json);
    
    FILE *fp = fopen("msg1.json", "w");
    if (fp != NULL) {
        fprintf(fp, "%s", ans);
        fclose(fp);
    } else {
        printf("Error: Could not open file for writing\n");
    }
    
    free(ans);
    cJSON_Delete(parse_json);
    
    return 0;
}