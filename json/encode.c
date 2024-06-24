#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"



// the format we follow
//{\n
//  \"key\": \"value\",\n
//  ....
//  ....
//  \"key\": \"value\"\n
//}
char *EncodeString (Pair *firstPair) {
    Pair *temp = firstPair;
    char *res;
    char *jsonString = (char *)malloc(3);
    jsonString = strncpy(jsonString, "{\n", 3);
    while (temp != NULL) {
        switch (temp->value.Type) {
            size_t total;
            size_t keyLen;
            size_t strLen;
            char *holder;
            case STRING:
                keyLen = strlen(temp->key);
                strLen = strlen(temp->value.string);
                total = keyLen + strLen + 8;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);
                holder = (char *)malloc(strlen(jsonString));
                holder = strncpy(holder, jsonString, strlen(jsonString));
                if (temp->next == NULL) {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\":\"%s\"\n}", holder, temp->key, temp->value.string);
                }else {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\":\"%s\",\n", holder,temp->key, temp->value.string);
                }
            break;
            case NUMBER:
                keyLen = strlen(temp->key);
                strLen = strlen(temp->value.number);
                total = keyLen + strLen + 8;
                jsonString = (char *)realloc(jsonString, sizeof(*jsonString) + total);
                holder = (char *)malloc(sizeof(*jsonString));
                holder = strncpy(holder, jsonString, sizeof(*jsonString));
                if (temp->next == NULL) {
                    snprintf(jsonString, total+2, "%s\"%s\":\"%s\"\n}", holder, temp->key, temp->value.number);
                }else {
                    snprintf(jsonString, total+2, "%s\"%s\":\"%s\",\n", holder,temp->key, temp->value.number);
                }
            break;
            case OBJECT:break;
            case ARRAY:break;
            case True:break;
            case False:break;
            case null:break;
            default:break;
        }
        temp = temp->next;
    }
    return jsonString;
}