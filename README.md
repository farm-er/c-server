# Project FastC


## Table of Contents
- [Project FastC](#project-fastc)
  - [Table of Contents](#table-of-contents)
  - [introduction](#introduction)
  - [installation](#installation)
  - [usage](#usage)
    - [json](#json)
  - [contributing](#contributing)
  - [goals](#goals)
  - [license](#license)




## introduction

This program started as a way to discover how servers work to building a framwork for JSON apis that can be used to simplify setting up a server in C. Inspired by FastAPI's aproach for simplicity we hope we can create a user friendly interface and show people that C  can be a web or backend developpement language.


## installation

Project not finished

## usage

- [Json manipulation](#json)

### json
- In this library we use a custom local json Object.  
  Main structure:
```
    Pair(key + value + next) ---> ... ---> Pair(key + value + next) ---> NULL
```
- Where each pair represents a row in a json object "key": "value", this makes the object dynamic and easy to use by going from node to node until NULL.  
- The key is just a string and always a string.
Now every pair has a value and a value can be any supported json type.
```
    Value |--> type
          |
          |--> string
          |
          |--> object (nested json object) 
          |
          |--> array
```
By doing this we store the type and the value (the actual value from json object).
## contributing

## goals

-- JSON decoder and encoder
-- ROUTING
-- REQUEST and RESPONSE types
-- FUNCTION that starts a server on the choosen port
-- Sync requests handling

## license

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](./LICENSE) file for details



