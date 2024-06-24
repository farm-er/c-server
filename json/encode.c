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
char *EncodeString (Pair *firstPair) {
    Pair *temp = firstPair;
    char *res;
    char *jsonString = (char *)malloc(3);
    jsonString = strncpy(jsonString, "{\n", 3);
    size_t total;
    size_t keyLen;
    size_t strLen;
    char *holder;
    while (temp != NULL) {
        switch (temp->value.Type) { 
            case STRING:
                keyLen = strlen(temp->key);
                strLen = strlen(temp->value.string);
                total = keyLen + strLen + 8;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);
                holder = (char *)malloc(strlen(jsonString));
                holder = strncpy(holder, jsonString, strlen(jsonString)+1);
                if (temp->next == NULL) {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\":\"%s\"\n}", holder, temp->key, temp->value.string);
                }else {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\":\"%s\",\n", holder,temp->key, temp->value.string);
                }
            break;
            case NUMBER:
                keyLen = strlen(temp->key);
                strLen = strlen(temp->value.string);
                total = keyLen + strLen + 8;
                jsonString = (char *)realloc(jsonString, strlen(jsonString) + total);
                holder = (char *)malloc(strlen(jsonString));
                holder = strncpy(holder, jsonString, strlen(jsonString)+1);
                if (temp->next == NULL) {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\":\"%s\"\n}", holder, temp->key, temp->value.string);
                }else {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\":\"%s\",\n", holder,temp->key, temp->value.string);
                }
            break;
            case OBJECT:break;
            case ARRAY:break;
            case True:
                keyLen = strlen(temp->key);
                total = keyLen + 10;
                printf("total: %lu\n", total);
                printf("%lu\n", strlen(jsonString));
                printf("jsonString: %s\n", jsonString);
                if (temp->next == NULL) {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\": true\n}", holder, temp->key);
                }else {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\": true,\n", holder,temp->key);
                }
                printf("%lu\n", strlen(jsonString));
            break;
            case False:
                keyLen = strlen(temp->key);
                total = keyLen + 11;
                printf("total: %lu\n", total);
                printf("%lu\n", strlen(jsonString));
                printf("jsonString: %s\n", jsonString);
                if (temp->next == NULL) {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\": false\n}", holder, temp->key);
                }else {
                    snprintf(jsonString, strlen(jsonString) + total + 2, "%s\"%s\": false,\n", holder,temp->key);
                }
                printf("%lu\n", strlen(jsonString));
            break;
            case null:break;
            default:break;
        }
        temp = temp->next;
    }
    return jsonString;
}