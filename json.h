

#ifndef FILE_H

#define FILE_H


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

char *EncodeString (const char* key,const char* value);
Pair *DecodeJSON (const char *jsonString, Pair *start, Pair *end);
Array *parseArray (char **jsonString, Array *start, Array *end);




#endif