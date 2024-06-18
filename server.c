#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"





int main (int argc, char* argv) {

    char *Key = "name";
    char *value = "oussama";

    printf("Json string is: %s\n", EncodeString(Key, value));

    char *check = "{\"check\"}";

    printf("Json string is: %s\n", DecodeJSON(check));

    return 0;
}

