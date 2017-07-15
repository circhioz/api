# SimpleFS - a simple filesystem

[![Build Status](https://travis-ci.org/circhioz/api.svg?branch=development)](https://travis-ci.org/circhioz/api)

This is my proposed solution for the final assignment of the API
(Algoritmi e Principi dell'Informatica) course.

## Brief
The aim of the project is to implement a simple hierarchical file system
stored uniquely in volatile memory. The program must be implemented in
`C99`, using only the standard library (`libc`) and the base runtime.

The program that implements the file system will receive a journal of
the actions from stdin and will print the result on stdout.

The project assignment PDF (in Italian) can be found [here][1].

## How to run

    ./build.sh
    ./build/src/project

## License

This project is distributed under the terms of the Apache License v2.0.
See file [`LICENSE`][2] for further reference.

 [1]: https://github.com/circhioz/api/blob/master/doc/SimpleFS.pdf
 [2]: https://github.com/circhioz/api/blob/master/LICENSE