/*  
    Copyright (C) 2024  Oussama MAJDOULI

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"


char *EncodeArray(Array *firstValue) {
    Array *temp = firstValue;
    if (temp == NULL) {
        return "[]";
    }
    size_t total;
    char *JsonString = (char *)malloc(3);
    strncpy(JsonString, "[\n", 2);
    while (temp != NULL) {
        switch (temp->value->Type)
        {
        case STRING:
            total = strlen(temp->value->string) + 6;
            JsonString = (char *)realloc(JsonString, strlen(JsonString) + total);
            strncat(JsonString, "\"", 2);
            printf("jsonString: %ld\n", strlen(JsonString));
            printf("%s\n", JsonString);
            printf("total: %ld\n", total);
            strncat(JsonString, temp->value->string, total-6);
            printf("jsonString: %ld\n", strlen(JsonString));
            printf("%s\n", JsonString);
            if (temp->next == NULL){
                strncat(JsonString, "\"\n]", 4);
            }else {
                strncat(JsonString, "\",\n", 4);
            }
            break;
        case NUMBER:
            total = strlen(temp->value->string) + 3;
            JsonString = (char *)realloc(JsonString, strlen(JsonString) + total);
            strncat(JsonString, temp->value->string, total - 3);
            if (temp->next == NULL){
                strncat(JsonString, "\n]", 3);
            }else {
                strncat(JsonString, ",\n", 3);
            }
            break;
        case OBJECT:
                char *nestedObj =  EncodeJsonObject(temp->value->object);
                size_t strLen = strlen(nestedObj);
                total = strLen + 3;
                JsonString = (char *)realloc(JsonString, strlen(JsonString) + total);
                strncat(JsonString, nestedObj, strLen);
                if (temp->next == NULL) {
                    strncat(JsonString, "\n]", 3);
                }else {
                    strncat(JsonString, ",\n", 3);  
                }
            break;
        case ARRAY:
            break;
        case True:
            break;
        case False:
            break;
        case null:
            break;
        default:
            break;
        }
        temp = temp->next;
    }

}

// the format we follow
//{\n
//  \"key\": \"value\",\n
//  ....
//  ....
//  \"key\": \"value\"\n
//}
char *EncodeJsonObject (Pair *firstPair) {
    if (firstPair == NULL) {
        fprintf(stderr, "EncodeJsonObject: empty json object\n");
        return "";
    }
    Pair *temp = firstPair;
    char *jsonString = (char *)malloc(3);
    strncpy(jsonString, "{\n", 2);
    size_t total;
    size_t keyLen;
    size_t strLen;
    char *holder;
    while (temp != NULL) {
        switch (temp->value.Type) { 
            case STRING:
            keyLen = strlen(temp->key); // key
                strLen = strlen(temp->value.string); // value
                total = keyLen + strLen + 8; // key and value and space for " ":" " + \n} or ,\n and null terminator
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen+1);
                strncat(jsonString, "\": \"", 5);
                strncat(jsonString, temp->value.string, strLen+1);
                strncat(jsonString, "\"", 2);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n}", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
                break;
            case NUMBER:
                keyLen = strlen(temp->key); // key
                strLen = strlen(temp->value.string); // value
                total = keyLen + strLen + 8; // key and value and space for " ":" " + \n} or ,\n and null terminator
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen);
                strncat(jsonString, "\": ", 4);
                strncat(jsonString, temp->value.string, strLen+1);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n}", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
            break;
            case OBJECT:
                keyLen = strlen(temp->key);
                char *nestedObj =  EncodeJsonObject(temp->value.object);
                strLen = strlen(nestedObj);
                total = keyLen + strLen + 7;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen);
                strncat(jsonString, "\": ", 4);
                strncat(jsonString, nestedObj, strLen+1);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n}", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
            break;
            case ARRAY:

                keyLen = strlen(temp->key);
                const char *arrayString = EncodeArray(temp->value.array);
                strLen = strlen(arrayString);
                total = keyLen + strLen + 7;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen);
                strncat(jsonString, "\": ", 4);
                strncat(jsonString, arrayString, strLen+1);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n]", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
                /* ARRAY Encoding function: seperate function for encoding arrays */
            break;
            case True:
                keyLen = strlen(temp->key);
                total = keyLen + 11;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen);
                strncat(jsonString, "\": true", 8);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n}", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
            break;
            case False:
                keyLen = strlen(temp->key);
                total = keyLen + 12;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen);
                strncat(jsonString, "\": false", 9);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n}", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
            break;
            case null:
                 keyLen = strlen(temp->key);
                total = keyLen + 10;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);// reaclloc space for the string and the new element to add
                strncat(jsonString, "\"", 2);
                strncat(jsonString, temp->key, keyLen);
                strncat(jsonString, "\": null", 8);
                if (temp->next == NULL) {
                    strncat(jsonString, "\n}", 3);
                }else {
                    strncat(jsonString, ",\n", 3);  
                }
            break;
            default:break;
        }
        temp = temp->next;
    }
    return jsonString;
}