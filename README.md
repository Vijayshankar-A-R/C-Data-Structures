# C Data Structures

A collection of generic data structures implemented in C. Elements are stored as byte copies, so each container receives an element size during initialization. Optional callbacks can release resources owned by stored elements.

## Contents

- Dynamic array: `list_t`
- Singly linked list: `linklist_t`
- Doubly linked list: `double_ll_t`
- Stack: `stack_t` (requires `linklist.h`)
- Queue: `queue_t` (requires `double_ll.h`)
- Max heap: `hp_t` (requires `math.h`)
- Binary search tree / AVL tree: `bst_t`
- Hash table: `hash_t` (requires `linklist.h`)

Public headers are in `headers/`, implementations are in `src/`, tests are in `tests/`, and compiled test executables are stored in `bin/`.

## Requirements

- GCC or Clang
- Standard C library
- Math library when building the heap (`-lm`)

## Include Paths

Source and test files include headers by name, for example:

```c
#include "bst.h"
```

Add the `headers/` directory to the compiler include path with `-Iheaders`.

## Build and Run Tests

Build an individual test from the repository root:

```sh
gcc -Wall -Wextra -Iheaders src/bst.c tests/bst_test.c -o bin/bst_test
bin/bst_test
```

For the stress test, compile only the implementations it uses. The array-backed stack and linked-list stack both export the same `stack_*` symbols, so they should not be linked together:

```sh
gcc -Wall -Wextra -Iheaders \
  src/queue.c src/heap.c src/hash.c src/double_ll.c src/linklist.c \
  tests/dsa_stress_test.c -lm \
  -o bin/dsa_stress_test
bin/dsa_stress_test
```

Other tests follow the same pattern: include the relevant source files, add `-Iheaders`, and write the executable to `bin/`.

## Using the Library

Include the header for the structure you need and link its implementation file. For example:

```c
#include "stack.h"

stack_t stack;
int value = 42;

stack_init(&stack, sizeof(value), NULL);
stack_push(&stack, &value);
stack_free(&stack);
```

Initialization functions generally return nonzero on success and zero on failure. Check the individual headers for the complete API and return-value behavior.

## Memory Management

The library copies each element into container storage. Pass a `free_ele` callback when an element contains dynamically allocated resources that must be released. The callback is used when elements are removed or the container is freed.

For comparison-based structures (`hp_t` and `bst_t`), provide a comparator using the usual `qsort` convention:

- less than zero when the first value is smaller
- zero when values are equal
- greater than zero when the first value is larger

## Repository Layout

```text
headers/   Public header files
src/       Implementations
tests/     Test programs
bin/       Compiled test executables
```

## AI Notice

I have used AI for writing the tests, README and for debugging cases which were particularly tricky.
