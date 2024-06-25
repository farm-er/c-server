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
#include "json/json.h"  



int main (int argc, char* argv) {
    // char *check = "{\"key1\":\"value1\",\t\n \"key2\":\"value2\",\t\n \"key3\": 12.3,\t\n \"key3\": null\t\n}";
    const char *jsonString = 
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
    const char *check = "{\"The Shawshank Redemption\": {\"key_1\": \"value_1\", \"key_2\": \"value_2\", \"key_2\": \"value_2\"}, \"key_2\": \"value_2\"}";
    Pair *content = DecodeJSON(&check, NULL, NULL);
    
    char *encodedJson = EncodeJsonObject(content);

    printf("%s", encodedJson);

    return 0;
}

