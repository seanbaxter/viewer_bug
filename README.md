## How to run it:
```
$ clang++ loader.cxx -lGL -lgl3w -lglfw -o separate
$ ./separate
glSpecialized worked!

$ clang++ loader.cxx -lGL -lgl3w -lglfw -D LINK_SPV -o linked
$ ./linked
Segmentation fault (core dumped)
```