# SimpleFS - a simple filesystem
This is my proposed solution for the final assignment of the API (Alrgoritmi e Principi dell'Informatica) course.

## Brief
The aim of the project is to implement a simple hierarchical file system stored uniquely in volatile memory.
A hierarchical file system stores resources in a tree structure and univocally identifies every resource
by a path from the root.

Resources in a file system can be either *files* or *directories*. They both have a
name, represented as a character string. The former is only inserted as tree leaf, the latter both as leaf
and intermediate node. The tree root is conventionally a directory, the root directory.

Only files can contain data, represented as a byte sequence, while directories don't have associated data.
Every tree node can contain metadata, but for this project, only directories contain them. Directory metadata
is represented by the name of its descendants.

The program that implements the file system will receive a journal of the actions from stdin and will print
the result on stdout

The program must be implemented in `C99`, using only the standard library (`libc`) and the base runtime.
