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
#ifndef FILE_H

#define FILE_H


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

char *EncodeJsonObject (Pair *firstPair);
Pair *DecodeJSON (const char **JSONString, Pair *start, Pair *end);




#endif