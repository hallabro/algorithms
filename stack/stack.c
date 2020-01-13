#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define STACK_SIZE 10

typedef struct _stack {
  uint8_t content[STACK_SIZE];
  uint8_t top;
} stack;

void init_stack(stack* s) {
  s->top = 0;
}

bool stack_is_empty(const stack* s) {
  return s->top <= 0;
}

void stack_push(stack *s, uint8_t data) {
  s->content[s->top++] = data;
}

uint8_t stack_pop(stack *s) {
  if (stack_is_empty(s)) {
    printf("nothing to pop, stack is empty");
    return 0;
  }

  return s->content[--s->top];
}

int main(void) {
  stack s;
  init_stack(&s);

  stack_push(&s, 1);
  stack_push(&s, 2);
  stack_push(&s, 3);

  if (stack_is_empty(&s)) {
    printf("stack empty");
  } else {
    uint8_t data = stack_pop(&s);
    printf("%u", data);
    data = stack_pop(&s);
    printf("%u", data);
    data = stack_pop(&s);
    printf("%u", data);
  }

  return 0;
}
