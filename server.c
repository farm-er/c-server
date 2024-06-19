#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"





int main (int argc, char* argv) {

    // char *Key = "name";
    // char *value = "oussama";

    // printf("Json string is: %s\n", EncodeString(Key, value));

    char *check = "{\"key1\":\"value1\",\t\n \"key2\":\"value2\",\t\n \"key3\":\"value3\"\t\n}";

    Pair *content = DecodeJSON(check, NULL, NULL);
    Pair *temp = content;
    int i = 0;
    while (temp != NULL){
        printf("\n%d)- key: %s,value: %s\n", i,temp->key, temp->value.string);
        temp = temp->next;i++;
    }
    

    

    return 0;
}

