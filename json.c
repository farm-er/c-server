#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1

#define null 0
#define STRING 1
#define NUMBER 2
#define ARRAY 3
#define OBJECT 4



typedef struct Value {
    int Type;
    char *string;
    char *number;
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
                fprintf(stderr, "Invalid json format: early EOF");
                return NULL;
            }
            size_t len = jsonString - stringStart;
            newNode->value->string  = (char *)malloc(len * sizeof(char));
            newNode->value->Type = STRING;// type of the value
            newNode->value->string = strncpy(newNode->value->string, stringStart, len); // value
            jsonString++;// pass "
            while (*jsonString == ' ') jsonString ++;// pass white spaces
            break;
        case '{':
        // parse another JSON object
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
                fprintf(stderr, "Invalid json format expected null\n");
                return NULL;
            }
            newNode->value->Type = null;
            jsonString = jsonString +4;// pass "null"
            break;
        default:
            if ('0'<=(*jsonString) && (*jsonString)<='9') {
                // parse  NUMBER
                const char *numberStart = jsonString;
                while ((*jsonString >= '0' && *jsonString <= '9') || *jsonString == '.') jsonString++;
                size_t len = jsonString - numberStart;
                newNode->value->Type = NUMBER;
                newNode->value->number = (char *)malloc(len);
                newNode->value->number = strncpy(newNode->value->number, numberStart, len);
            }else {
                fprintf(stderr, "Invalid json format expected ( {, [, null, \", (0, 1, 2 ... ,9)) but found: %c\n", *jsonString);
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
        fprintf(stderr, "Invalid json format expected ( ], ,) but found: %c\n", *jsonString);
        return NULL;
}

Pair *DecodeJSON (const char *jsonString, Pair *start, Pair *end) {
    Pair *newPair = (Pair *)malloc(sizeof(Pair));
    if (start == NULL) {
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;// excluding any leading junks
        if (*jsonString != '{') {
            fprintf(stderr, "Invalid json format expected { and found %c\n", *jsonString);
            return NULL;
        }
        jsonString++;
    }
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
        // the key is always a string so we parse it and take the string
        if (*jsonString != '\"'){
            fprintf(stderr, "Invalid json format found: %c\n", *jsonString);
            return NULL;
        }
        jsonString++;// pass the "
        const char *keyStart = jsonString;
        while (*jsonString != '\"' && *jsonString != '\0') jsonString++;
        if (*jsonString == '\0') {
            fprintf(stderr, "Invalid json format: early EOF\n");
            return NULL;
        }
        size_t lenKey = jsonString - keyStart;
        newPair->key = (char *)malloc(lenKey);
        newPair->key = strncpy(newPair->key, keyStart, lenKey);// got the key here
        jsonString++;// pass the "
        // between key and value
        if (*jsonString != ':'){
            fprintf(stderr, "Invalid json format: expecting ':' found %c\n", *jsonString);
            return NULL;
        }
        // pass :
        jsonString++;
        while (*jsonString == ' ') jsonString ++;        
        switch (*jsonString)
        {
        case '\"':
            jsonString++;
            const char *stringStart = jsonString;
            while (*jsonString != '\"' && *jsonString != '\0') jsonString++;
            if (*jsonString == '\0') {
                fprintf(stderr, "Invalid json format: early EOF");
                return NULL;
            }
            size_t len = jsonString - stringStart;
            newPair->value.string  = (char *)malloc(len);
            newPair->value.Type = STRING;// type of the value
            newPair->value.string = strncpy(newPair->value.string, stringStart, len); // value
            jsonString++;// pass "
            while (*jsonString == ' ') jsonString ++;// pass white spaces
            break;
        case '{':
        // parse another JSON object
            break;
        case '[':
            // parse the array
            // [] can have any type
            jsonString++;// pass the [
            newPair->value.Type = ARRAY;
            newPair->value.array = parseArray(&jsonString, NULL, NULL);// when finishing the jsonString will be after ] so we can check for , or } after removing junks
            break;
        case 'n':
            // this is a null value
            char *empty = "null";
            if (strncmp(jsonString, empty, 4) != 0) {
                fprintf(stderr, "Invalid json format expected null\n");
                return NULL;
            }
            newPair->value.Type = null;
            jsonString = jsonString +4;// pass "null"
            break;
        default:
            // check if it's a number or an unwanted character
            if ('0'<=(*jsonString) && (*jsonString)<='9') {
                // parse  NUMBER
                const char *numberStart = jsonString;
                while ((*jsonString >= '0' && *jsonString <= '9') || *jsonString == '.') jsonString++;
                size_t len = jsonString - numberStart;
                newPair->value.Type = NUMBER;
                newPair->value.number = (char *)malloc(len);
                newPair->value.number = strncpy(newPair->value.number, numberStart, len);
            }else {
                fprintf(stderr, "Invalid json format expected ( {, [, null, \", (0, 1, 2 ... ,9)) but found: %c\n", *jsonString);
                return NULL;  
            }
            break;
        }
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
        newPair->next = NULL;
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
                return DecodeJSON(jsonString, newPair, newPair);
            }else {
                end->next = newPair;
                return DecodeJSON(jsonString, start, newPair);
            }
        }
        fprintf(stderr, "Invalid json format expected ( }, ,) but found: %c\n", *jsonString);
        return NULL;
}


char *EncodeString (const char* key,const char* value) {
    size_t lenKey = strlen(key);
    size_t lenValue = strlen(value);
    size_t total = lenKey + lenValue + 6;
    char *JSONString = (char *)malloc(total * sizeof(char));
    if (JSONString == NULL) {
        fprintf(stderr, "memory allocation failed");
        return NULL;
    }

    int res = snprintf(JSONString, total, "\"%s\":\"%s\"", key, value);

    if (res < 0) {
        fprintf(stderr, "Error occured when creating JSON string: snprintf");
        return NULL;
    }

    return JSONString;
}

