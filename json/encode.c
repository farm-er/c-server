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
    char *res;
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
            case ARRAY:break;
            case True:
                keyLen = strlen(temp->key);
                total = keyLen + 10;
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
                total = keyLen + 11;
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