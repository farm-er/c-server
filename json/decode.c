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

#define TRUE 1

#define null 0
#define STRING 1
#define NUMBER 2
#define ARRAY 3
#define OBJECT 4
#define True 5
#define False 6



typedef struct Value {
    int Type;
    char *string;
    struct Pair *object;
    struct Array *array;
}Value;

typedef struct Pair {
    char * key;
    struct Value value;
    struct Pair *next;
}Pair;

typedef struct Array {
    Value *value;
    struct Array *next;
}Array;

Pair *DecodeJSON (const char **JSONString, Pair *start, Pair *end);

Array *parseArray (const char **JSONString, Array *start, Array *end) {
    const char *jsonString = *JSONString;
    Array *newNode = (Array *)malloc(sizeof(Array));// add it after to the array
    newNode->value = (Value *)malloc(sizeof(Value));
    while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
    switch (*jsonString){
        case '\"':
            jsonString++;
            const char *stringStart = jsonString;
            while (*jsonString != '\"' && *jsonString != '\0') jsonString++;
            if (*jsonString == '\0') {
                fprintf(stderr, "parseArray: Invalid json format: early EOF");
                return NULL;
            }
            size_t len = jsonString - stringStart;
            newNode->value->string  = (char *)malloc(len+1);
            newNode->value->Type = STRING;// type of the value
            newNode->value->string = strncpy(newNode->value->string, stringStart, len); // value
            jsonString++;// pass "
            while (*jsonString == ' ') jsonString ++;// pass white spaces
            break;
        case '{':
        // parse another JSON object
            newNode->value->Type = OBJECT;
            newNode->value->object = DecodeJSON(&jsonString, NULL, NULL);
            break;
        case '[':
            // parse the array
            // [] can have any type
            jsonString++;// pass the [
            newNode->value->Type = ARRAY;
            newNode->value->array = parseArray(&jsonString, NULL, NULL);// when finishing the jsonString will be after ] so we can check for , or } after removing junks
            break;
        case 'n':
            // this is a null value
            char *empty = "null";
            if (strncmp(jsonString, empty, 4) != 0) {
                fprintf(stderr, "parseArray: Invalid json format expected null\n");
                return NULL;
            }
            newNode->value->Type = null;
            newNode->value->string = empty;
            jsonString = jsonString +4;// pass "null"
            break;
        case 't':
            char *trueValue = "true";
            if (strncmp(jsonString, empty, 5) != 0) {
                fprintf(stderr, "parseArray: Invalid json format expected null\n");
                return NULL;
            }
            newNode->value->Type = True;
            newNode->value->string = trueValue;
            jsonString = jsonString +4;// pass "true"
            break;
        case 'f':
            char *falseValue = "false";
            if (strncmp(jsonString, falseValue, 6) != 0) {
                fprintf(stderr, "parseArray: Invalid json format expected null\n");
                return NULL;
            }
            newNode->value->Type = False;
            newNode->value->string = falseValue;
            jsonString = jsonString +5;// pass "false"
            break;
        default:
            if ('0'<=(*jsonString) && (*jsonString)<='9') {
                // parse  NUMBER
                const char *numberStart = jsonString;
                while ((*jsonString >= '0' && *jsonString <= '9') || *jsonString == '.') jsonString++;
                size_t len = jsonString - numberStart;
                newNode->value->Type = NUMBER;
                newNode->value->string = (char *)malloc(len);
                newNode->value->string = strncpy(newNode->value->string, numberStart, len);
            }else {
                fprintf(stderr, "parseArray: Invalid json format expected ( {, [, null, \", (0, 1, 2 ... ,9)) but found: %c\n", *jsonString);
                return NULL;  
            }
            break;
        }
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
        newNode->next = NULL;
        *JSONString = jsonString+1;
        if (*jsonString == ']'){// end of array
            jsonString++;// pass ]
            if (start == NULL) {
                return newNode;
            }else {
                end->next = newNode;
                return start;
            }
        }else if (*jsonString == ',') {// still elements to parse in the array
            jsonString++;// pass ,
            if (start == NULL){
                return parseArray(JSONString, newNode, newNode);
            }else {        
                end->next = newNode;
                return parseArray(JSONString, start, newNode);
            }
        }
        fprintf(stderr, "parseArray: Invalid json format expected ( ], ,) but found: %c\n", *jsonString);
        return NULL;
}

Pair *DecodeJSON (const char **JSONString, Pair *start, Pair *end) {
    const char *jsonString = *JSONString;
    Pair *newPair = (Pair *)malloc(sizeof(Pair));
    if (start == NULL) {
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;// excluding any leading junks
        if (*jsonString != '{') {
            fprintf(stderr, "DecodeJSON: Invalid json format expected { and found %c\n", *jsonString);
            return NULL;
        }
        jsonString++;
    }
    while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
        // the key is always a string so we parse it and take the string
        if (*jsonString != '\"'){
            fprintf(stderr, "DecodeJSON: Invalid json format found: %c\n", *jsonString);
            return NULL;
        }
        jsonString++;// pass the "
        const char *keyStart = jsonString;
        while (*jsonString != '\"' && *jsonString != '\0') jsonString++;
        if (*jsonString == '\0') {
            fprintf(stderr, "DecodeJSON: Invalid json format: early EOF\n");
            return NULL;
        }
        size_t lenKey = jsonString - keyStart;
        newPair->key = (char *)malloc(lenKey+1);
        newPair->key = strncpy(newPair->key, keyStart, lenKey);// got the key here
        jsonString++;// pass the "
        // between key and value
        if (*jsonString != ':'){
            fprintf(stderr, "DecodeJSON: Invalid json format: expecting ':' found %c\n", *jsonString);
            return NULL;
        }
        // pass :
        jsonString++;
        while (*jsonString == ' ') jsonString ++;   
        switch (*jsonString){
            case '\"':
                jsonString++;
                const char *stringStart = jsonString;
                while (*jsonString != '\"' && *jsonString != '\0') jsonString++;
                if (*jsonString == '\0') {
                    fprintf(stderr, "DecodeJSON: Invalid json format: early EOF");
                    return NULL;
                }
                size_t len = jsonString - stringStart;
                newPair->value.string  = (char *)malloc(len+1);
                newPair->value.Type = STRING;// type of the value
                newPair->value.string = strncpy(newPair->value.string, stringStart, len); // value
                jsonString++;// pass "
                while (*jsonString == ' ') jsonString ++;// pass white spaces
                break;
            case '{':
                // parse another JSON object
                printf("started\n");
                newPair->value.Type = OBJECT;
                newPair->value.object = DecodeJSON(&jsonString, NULL, NULL);
                printf("finished");
                break;
            case '[':
                // parse the array
                // [] can have any type
                jsonString++;// pass the [
                newPair->value.Type = ARRAY;
                newPair->value.array = parseArray(&jsonString, NULL, NULL);// when finishing the jsonString will be after ] so we can check for , or } after removing junks
                break;
            case 'n':// this is a null value
                char *empty = "null";
                if (strncmp(jsonString, empty, 4) != 0) {
                    fprintf(stderr, "DecodeJSON: Invalid json format expected null\n");
                    return NULL;
                }
                newPair->value.Type = null;
                newPair->value.string = empty;
                jsonString = jsonString +4;// pass "null"
                break;
            case 't':
            char *trueValue = "true";
                if (strncmp(jsonString, trueValue, 4) != 0) {
                    fprintf(stderr, "DecodeJSON: Invalid json format expected true\n");
                    return NULL;
                }
                newPair->value.Type = True;
                newPair->value.string = trueValue;
                jsonString = jsonString +4;// pass "true"
                break;
            case 'f':
                char *falseValue = "false";
                if (strncmp(jsonString, falseValue, 5) != 0) {
                    fprintf(stderr, "DecodeJSON: Invalid json format expected false\n");
                    return NULL;
                }
                newPair->value.Type = False;
                newPair->value.string = falseValue;
                jsonString = jsonString +5;// pass "false"
                break;
            default:
                if ('0'<=(*jsonString) && (*jsonString)<='9') {// check if it's a number or an unwanted character
                    // parse  NUMBER
                    const char *numberStart = jsonString;
                    while ((*jsonString >= '0' && *jsonString <= '9') || *jsonString == '.') jsonString++;
                    size_t len = jsonString - numberStart;
                    newPair->value.Type = NUMBER;
                    newPair->value.string = (char *)malloc(len);
                    newPair->value.string = strncpy(newPair->value.string, numberStart, len);
                }else {
                    fprintf(stderr, "DecodeJSON: Invalid json format expected ( {, [, null, \", (0, 1, 2 ... ,9)) but found: %c\n", *jsonString);
                    return NULL;  
                }
                break;
        }
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
        newPair->next = NULL;// marking the end of the object
        *JSONString = jsonString+1;
        if (*jsonString == '}'){
            jsonString++;// pass }
            if (start == NULL) {
                return newPair;
            }else {
                end->next = newPair;
                return start;
            }
        }else if (*jsonString == ',') {
            jsonString++;// pass ,
            if (start == NULL) {
                return DecodeJSON(JSONString, newPair, newPair);
            }else {
                end->next = newPair;
                return DecodeJSON(JSONString, start, newPair);
            }
        }
        fprintf(stderr, "DecodeJSON: Invalid json format expected ( }, ,) but found: %c\n", *jsonString);
        return NULL;
}