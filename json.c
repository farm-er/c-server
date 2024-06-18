#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pair {
    char * key;
    char * value;
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


char *DecodeJSON (const char *jsonString) {
    // excluding any leading junks
    while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') {
        jsonString ++;
    }
    if (*jsonString != '{') {
        fprintf(stderr, "Invalid json format");
        return NULL;
    }
    jsonString++;
    while (*jsonString != '\0' && *jsonString != '}') {
        while (*jsonString == '\n' || *jsonString == '\t' || *jsonString == '\r' || *jsonString == ' ') {
            jsonString ++;
        }
        // extracting the key
        if (*jsonString != '\"'){
            fprintf(stderr, "Invalid json format");
            return NULL;
        }
        jsonString++;
        const char *keyStart = jsonString;
        while (*jsonString != '\"' && *jsonString != '\0') {jsonString++;}
        if (*jsonString == '\0') {
            fprintf(stderr, "Invalid json format");
            return NULL;
        }
        size_t lenKey = jsonString - keyStart;
        char *key = (char *)malloc(lenKey);
        // now we have the key we need to parse the next
        key = strncpy(key, keyStart, lenKey);
        return key;
    }
    return NULL;
}