# pai

Proof of concept interpreter based on Flex and Bison (for a language somewhat similar to Python).

## Building

### Dependencies

* libfmt
* Clang version >= 11.0 (cannot be compiled with GCC because of some syntax limitations regarding non-trivial types in anonymous aggregates)
* flex
* bison

To build, run:

```
$ sh compile.sh
```

### Running

Test the interpreter on the [provided examples](./tests), such as:

```
$ ./pai tests/3.pai
```
