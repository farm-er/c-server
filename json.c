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
    int number;
    struct Pair *object;
    struct Value *array;
}Value;

typedef struct Pair {
    char * key;
    struct Value value;
    struct Pair *next;
}Pair;

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


Pair *DecodeJSON (const char *jsonString, Pair *start, Pair *end) {
    Pair *newPair = (Pair *)malloc(sizeof(Pair));
    // excluding any leading junks
    if (start == NULL) {
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
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
        // pass the "
        jsonString++;
        const char *keyStart = jsonString;
        while (*jsonString != '\"' && *jsonString != '\0') jsonString++;
        if (*jsonString == '\0') {
            fprintf(stderr, "Invalid json format: early EOF\n");
            return NULL;
        }
        size_t lenKey = jsonString - keyStart;
        newPair->key = (char *)malloc(lenKey);
        // got the key here
        newPair->key = strncpy(newPair->key, keyStart, lenKey);
        // pass the "
        jsonString++;
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
            while (*jsonString != '\"' && *jsonString != '\0') {jsonString++;}
            if (*jsonString == '\0') {
                fprintf(stderr, "Invalid json format: early EOF");
                return NULL;
            }
            size_t lenKey = jsonString - stringStart;
            newPair->value.string  = (char *)malloc(lenKey);
            newPair->value.Type = STRING;// type of the value
            newPair->value.string = strncpy(newPair->value.string, stringStart, lenKey); // value
            jsonString++;// pass "
            while (*jsonString == ' ') jsonString ++;// pass white spaces
            break;
        case '{':
            break;
        case '[':
            break;
        case 'n':
            break;
        default:
            // check if it's a number or an unwanted character
            fprintf(stderr, "Invalid json format expected ( {, [, null, \") but found: %c\n", *jsonString);
            return NULL;
            break;
        }
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') jsonString ++;
        newPair->next = NULL;
        if (*jsonString == '}'){
            jsonString++;
            if (start == NULL) {
                return newPair;
            }else {
                end->next = newPair;
                return start;
            }
        }else if (*jsonString == ',') {
            // clean string start
            jsonString++;
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