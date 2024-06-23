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
            case STRING:
                size_t keyLen = strlen(temp->key);
                size_t strLen = strlen(temp->value.string);
                size_t total = keyLen + strLen + 8;
                jsonString = (char *)realloc(jsonString, sizeof(*jsonString) + total);
                char *holder = (char *)malloc(sizeof(*jsonString));
                holder = strncpy(holder, jsonString, sizeof(jsonString));
                if (temp->next == NULL) {
                    snprintf(jsonString, total+2, "%s\"%s\":\"%s\"\n}", holder, temp->key, temp->value.string);
                }else {
                    snprintf(jsonString, total+2, "%s\"%s\":\"%s\",\n", holder,temp->key, temp->value.string);
                }
            break;
            case NUMBER:break;
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