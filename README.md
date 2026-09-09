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

The project uses stb-style single-header implementations. Each header contains its public API and the implementation behind a matching `*_IMPLEMENTATION` define, so the typical pattern is:

```c
#define BST_IMPLEMENTATION
#include "bst.h"
```

The headers live at the repository root, and test programs are in `tests/` with compiled executables stored in `bin/`.

## Requirements

- GCC or Clang
- Standard C library
- Math library when building the heap (`-lm`)

## Include Paths

Tests and user code include the project headers by name. From the project root, add the repo root to the include path:

```c
#include "stack.h"
```

Compile with `-I.` so GCC can resolve the root-level headers.
If running outside the project repository, compile with `-Idirectory_name`.

## Build and Run Tests

Build an individual test from the repository root:

```sh
gcc -Wall -Wextra -I. tests/bst_test.c -o bin/bst_test
bin/bst_test
```

The test source already defines the matching implementation macro before including the header:

```c
#define BST_IMPLEMENTATION
#include "bst.h"
```

For the stress test, compile the test file directly with the relevant headers included.:

```sh
gcc -Wall -Wextra -I. tests/dsa_stress_test.c -lm -o bin/dsa_stress_test
bin/dsa_stress_test
```

Other tests follow the same pattern: include the root-level header, define the matching `_IMPLEMENTATION` macro, add `-I.`, and write the executable to `bin/`.

## Using the Library

Define the implementation macro for the data structure you want to use and include the header. For example:

```c
#define STACK_IMPLEMENTATION
#include "stack.h"

stack_t stack;
int value = 42;

stack_init(&stack, sizeof(value), NULL);
stack_push(&stack, &value);
stack_free(&stack);
```

This is the project’s stb-style pattern: the implementation is compiled when the matching macro is set before the header is included. Initialization functions generally return nonzero on success and zero on failure. Check the individual headers for the complete API and return-value behavior.

## Memory Management

The library copies each element into container storage. Pass a `free_ele` callback when an element contains dynamically allocated resources that must be released. The callback is used when elements are removed or the container is freed.

For comparison-based structures (`hp_t` and `bst_t`), provide a comparator using the usual `qsort` convention:

- less than zero when the first value is smaller
- zero when values are equal
- greater than zero when the first value is larger

## Repository Layout

```text
*.h        stb-style single-header implementations
tests/     Test programs
bin/      Compiled test executables
```

## AI Notice

I have used AI for writing the tests, README and for debugging cases which were particularly tricky.
