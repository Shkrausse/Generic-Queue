#include <stdio.h>
#include <stdlib.h>

// ── Structs ──────────────────────────────────────────────
typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int length;
    int  (*compare)(void *, void *);
    void (*print)(void *);
} Queue;

// ── Init ─────────────────────────────────────────────────
void init_queue(Queue *q, int (*compare)(void *, void *), void (*print)(void *)) {
    q->head    = NULL;
    q->tail    = NULL;
    q->length  = 0;
    q->compare = compare;
    q->print   = print;
}

// ── Int helpers ──────────────────────────────────────────
int compare_ints(void *a, void *b) {
    return *(int *)a - *(int *)b;
}

void print_int(void *data) {
    printf("%d", *(int *)data);
}

// ── Float helpers ────────────────────────────────────────
int compare_floats(void *a, void *b) {
    if (*(float *)a < *(float *)b) return -1;
    if (*(float *)a > *(float *)b) return  1;
    return 0;
}

void print_float(void *data) {
    printf("%f", *(float *)data);
}

// ── Enqueue ──────────────────────────────────────────────
void enqueue(Queue *q, void *data) {
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (q->tail == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        new_node->prev = q->tail;
        q->tail->next  = new_node;
        q->tail        = new_node;
    }
    q->length++;
}

// ── Dequeue (front) ──────────────────────────────────────
void *dequeue(Queue *q) {
    if (q->head == NULL) {
        fprintf(stderr, "Dequeue from empty queue\n");
        exit(1);
    }
    Node *old_head = q->head;
    void *data     = old_head->data;

    q->head = old_head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    } else {
        q->head->prev = NULL;
    }

    free(old_head);
    q->length--;
    return data;
}

// ── Dequeue (back) ───────────────────────────────────────
void *dequeue_back(Queue *q) {
    if (q->tail == NULL) {
        fprintf(stderr, "Dequeue from empty queue\n");
        exit(1);
    }
    Node *old_tail = q->tail;
    void *data     = old_tail->data;

    q->tail = old_tail->prev;
    if (q->tail == NULL) {
        q->head = NULL;
    } else {
        q->tail->next = NULL;
    }

    free(old_tail);
    q->length--;
    return data;
}

// ── Peek ─────────────────────────────────────────────────
void *peek(Queue *q) {
    if (q->head == NULL) {
        return NULL;
    }
    return q->head->data;
}

// ── Length ───────────────────────────────────────────────
int length(Queue *q) {
    return q->length;
}

// ── Print ────────────────────────────────────────────────
void print_queue(Queue *q) {
    Node *current = q->head;
    printf("Queue: ");
    while (current != NULL) {
        q->print(current->data);
        printf(" → ");
        current = current->next;
    }
    printf("NULL\n");
}

// ── Sort ─────────────────────────────────────────────────
void sort_queue(Queue *q) {
    if (q->head == NULL || q->head->next == NULL) {
        return;
    }
    int swapped;
    do {
        swapped = 0;
        Node *current = q->head;
        while (current->next != NULL) {
            if (q->compare(current->data, current->next->data) > 0) {
                void *temp           = current->data;
                current->data        = current->next->data;
                current->next->data  = temp;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}

// ── Destroy ──────────────────────────────────────────────
void destroy_queue(Queue *q, void (*free_data)(void *)) {
    while (q->head != NULL) {
        void *x = dequeue(q);
        free_data(x);
    }
    q->head   = NULL;
    q->tail   = NULL;
    q->length = 0;
}

// ── Main ─────────────────────────────────────────────────
int main(void) {
    Queue q_int;
    init_queue(&q_int, compare_ints, print_int);

    int *a = malloc(sizeof(int)); *a = 30;
    int *b = malloc(sizeof(int)); *b = 10;
    int *c = malloc(sizeof(int)); *c = 20;

    enqueue(&q_int, a);
    enqueue(&q_int, b);
    enqueue(&q_int, c);
    print_queue(&q_int);
    sort_queue(&q_int);
    print_queue(&q_int);

    int *peeked = peek(&q_int);
    printf("Peek: %d\n", *peeked);

    int *front = dequeue(&q_int);
    printf("Dequeued front: %d\n", *front);
    free(front);

    int *back = dequeue_back(&q_int);
    printf("Dequeued back: %d\n", *back);
    free(back);

    print_queue(&q_int);
    destroy_queue(&q_int, free);

    // ── Float queue ──
    Queue q_float;
    init_queue(&q_float, compare_floats, print_float);

    float *x = malloc(sizeof(float)); *x = 3.14f;
    float *y = malloc(sizeof(float)); *y = 1.59f;
    float *z = malloc(sizeof(float)); *z = 2.71f;

    enqueue(&q_float, x);
    enqueue(&q_float, y);
    enqueue(&q_float, z);
    print_queue(&q_float);
    sort_queue(&q_float);
    print_queue(&q_float);

    float *fval = dequeue_back(&q_float);
    printf("Dequeued back: %f\n", *fval);
    free(fval);

    print_queue(&q_float);
    destroy_queue(&q_float, free);

    return 0;
}