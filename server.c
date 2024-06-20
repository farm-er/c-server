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



int main (int argc, char* argv) {
    // char *check = "{\"key1\":\"value1\",\t\n \"key2\":\"value2\",\t\n \"key3\": 12.3,\t\n \"key3\": null\t\n}";
    const char jsonString[] = 
        "{\n"
        "  \"name\": \"John Doe\",\n"
        "  \"age\": 30,\n"
        "  \"isStudent\": 0,\n"  // 0 for false (assuming integer representation for boolean)
        "  \"grades\": [85, 92, 78, 88],\n"
        "  \"street\": \"123 Main St\",\n"
        "  \"city\": \"Anytown\",\n"
        "  \"zipcode\": \"12345\",\n"
        "  \"phoneNumbers\": [\"123-456-7890\", \"987-654-3210\"],\n"
        "  \"email\": null,\n"
        "  \"isActive\": 1,\n"  // 1 for true (assuming integer representation for boolean)
        "  \"movies\": [\"The Shawshank Redemption\", \"The Godfather\"],\n"
        "  \"colors\": [\"blue\", \"green\", \"red\"]\n"
        "}";
    const char *check = "{\"The Shawshank Redemption\": [{\"key\": \"value\"}, {\"key\": \"value\"}, {\"key\": \"value\"}]}";
    Pair *content = DecodeJSON(&check, NULL, NULL);
    Pair *temp = content;
    int i = 1;
    while (temp != NULL){
        switch (temp->value.Type){
            case STRING:printf("\n%d)- key: %s,value: %s\n", i,temp->key, temp->value.string);
                break;
            case NUMBER:printf("\n%d)- key: %s,value: %s\n", i,temp->key, temp->value.number);
                break;  
            case null:printf("\n%d)- key: %s,value: %s\n", i, temp->key, "null");   
                break;
            case ARRAY:
                printf("\n%d)- key: %s\n", i, temp->key);
                Array *temp1 = temp->value.array;\
                printf("values:[");
                int n= 1;
                while (temp1 != NULL){
                    switch (temp1->value->Type){
                        case STRING:printf("\n  %d- %s\n", n,temp1->value->string);
                        break;
                        case NUMBER:printf("\n  %d) %s\n", n,temp1->value->number);
                        break;  
                        case null:printf("\n  %d) %s\n", n, "null");   
                        break;
                        case OBJECT:printf("Object: {%s: %s}", temp1->value->object->key, temp1->value->object->value.string);
                        default:break;
                    }
                    temp1 = temp1->next;n++;
                }
                printf("]\n");
                break;
            case OBJECT:
                printf("\n%d)- key: %s\n", i, temp->key);
                Pair *temp2 = content->value.object;
                int l=0;
                while (temp2 != NULL) {
                    switch (temp2->value.Type){
                        case STRING:printf("\n%d)- key: %s,value: %s\n", l,temp2->key, temp2->value.string);
                            break;
                        case NUMBER:printf("\n%d)- key: %s,value: %s\n", l,temp2->key, temp2->value.number);
                            break;  
                        case null:printf("\n%d)- key: %s,value: %s\n", l, temp2->key, "null");   
                            break;
                        case ARRAY:
                            printf("\n%d)- key: %s\n", l, temp2->key);
                            Array *temp1 = temp->value.array;\
                            printf("values:[");
                            int n= 1;
                            while (temp1 != NULL){
                                switch (temp1->value->Type){
                                    case STRING:printf("\n  %d- %s\n", n,temp1->value->string);
                                    break;
                                    case NUMBER:printf("\n  %d) %s\n", n,temp1->value->number);
                                    break;  
                                    case null:printf("\n  %d) %s\n", n, "null");   
                                    break;
                                    default:break;
                                }
                                temp1 = temp1->next;n++;
                            }
                            printf("]\n");
                            break;
                        case OBJECT:
                            printf("\n%d)- key: %s\n", l, temp2->key);
                            break;
                        default:break;
                    }
                    temp2 = temp2->next;
                }
                break;
            default:break;
        }
        temp = temp->next;i++;
    }
    return 0;
}

