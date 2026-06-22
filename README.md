# C Generic Queue — Doubly Linked List

A generic queue implementation in C built on a doubly linked list. Supports any data type via `void *` and function pointers for comparison and printing.

## Features

- Enqueue and dequeue from the front in O(1)
- Dequeue from the back in O(1) (doubly linked)
- Peek at the front without removing
- Sort using bubble sort
- Works with any data type via `void *`
- Clean destroy function to free all memory

## Build & Run

```bash
gcc -Wall -o queue queue.c && ./queue
```

## API

### Initialization

```c
void init_queue(Queue *q, int (*compare)(void *, void *), void (*print)(void *));
```

Initialize a queue with a comparator and print function for your data type.

```c
Queue q;
init_queue(&q, compare_ints, print_int);
```

### Enqueue

```c
void enqueue(Queue *q, void *data);
```

Add an item to the back of the queue. O(1).

```c
int *val = malloc(sizeof(int));
*val = 42;
enqueue(&q, val);
```

### Dequeue (front)

```c
void *dequeue(Queue *q);
```

Remove and return the item at the front. O(1). Caller is responsible for freeing the returned data.

```c
int *val = dequeue(&q);
printf("%d\n", *val);
free(val);
```

### Dequeue (back)

```c
void *dequeue_back(Queue *q);
```

Remove and return the item at the back. O(1). Caller is responsible for freeing the returned data.

### Peek

```c
void *peek(Queue *q);
```

Return the front item without removing it. Returns `NULL` if the queue is empty.

### Length

```c
int length(Queue *q);
```

Return the number of items in the queue. O(1).

### Sort

```c
void sort_queue(Queue *q);
```

Sort the queue in ascending order using the comparator provided at init. O(n²) bubble sort.

### Print

```c
void print_queue(Queue *q);
```

Print all items from front to back using the print function provided at init.

### Destroy

```c
void destroy_queue(Queue *q, void (*free_data)(void *));
```

Free all nodes and their data, then reset the queue to an empty state.

```c
destroy_queue(&q, free);  // for simple types like int, float
```

## Supported Types

Built-in helpers are provided for `int` and `float`. Pass them at init:

```c
// Int queue
init_queue(&q, compare_ints, print_int);

// Float queue
init_queue(&q, compare_floats, print_float);
```

## Adding a New Type

Write a comparator and print function for your type, then pass them to `init_queue`:

```c
int compare_doubles(void *a, void *b) {
    if (*(double *)a < *(double *)b) return -1;
    if (*(double *)a > *(double *)b) return  1;
    return 0;
}

void print_double(void *data) {
    printf("%f", *(double *)data);
}

Queue q;
init_queue(&q, compare_doubles, print_double);
```

## Memory Model

Each enqueue requires two allocations — one for the node, one for the data:

```
malloc(sizeof(Node))   — managed by the queue internally
malloc(sizeof(int))    — managed by the caller
```

Always free data returned by `dequeue` and `dequeue_back`. Use `destroy_queue` to clean up remaining items in bulk.

## Example

```c
Queue q;
init_queue(&q, compare_ints, print_int);

int *a = malloc(sizeof(int)); *a = 30;
int *b = malloc(sizeof(int)); *b = 10;
int *c = malloc(sizeof(int)); *c = 20;

enqueue(&q, a);
enqueue(&q, b);
enqueue(&q, c);
print_queue(&q);   // Queue: 30 → 10 → 20 → NULL

sort_queue(&q);
print_queue(&q);   // Queue: 10 → 20 → 30 → NULL

int *val = dequeue(&q);
printf("Dequeued: %d\n", *val);  // Dequeued: 10
free(val);

destroy_queue(&q, free);
```

## Concepts Covered

- Singly and doubly linked lists
- `void *` for generic programming
- Function pointers for type-agnostic behavior
- Heap allocation and ownership
- Dangling pointer prevention
- O(1) enqueue and dequeue via head and tail tracking
