# Project FastC


## Table of Contents
- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Goals](#goals)




## introduction

This program started as a way to discover how servers work to building a framwork for JSON apis that can be used to simplify setting up a server in C. Inspired by FastAPI's aproach for simplicity we hope we can create a user friendly interface and show people that C  can be a web or backend developpement language.


## installation

Project not finished

## usage

- [Json manipulation](#json)

### json
``` 
    In this library we introduce our local json Object used to pass json object accross the program and It's structure is:
```
Main structure:
```
    Pair(key + value + next) ---> ... ---> Pair(key + value + next) ---> NULL
    This make the object dynamic and easy to use by going from node to node until NULL
```

## contributing

## goals

-- JSON decoder and encoder
-- ROUTING
-- REQUEST and RESPONSE types
-- FUNCTION that starts a server on the choosen port
-- Sync requests handling

## license

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](./LICENSE) file for details



