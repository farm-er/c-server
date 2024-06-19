

#ifndef FILE_H

#define FILE_H


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

char *EncodeString (const char* key,const char* value);
Pair *DecodeJSON (const char *jsonString, Pair *start, Pair *end);




#endif