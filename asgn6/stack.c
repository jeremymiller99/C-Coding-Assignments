#include "stack.h"
#include <stdlib.h>

struct stack {
    	uint32_t capacity;
    	uint32_t top;
    	uint32_t *items;
};

Stack *stack_create(uint32_t capacity) {
    	Stack *s = malloc(sizeof(Stack));
    	if (s == NULL) {
        	return NULL;
    	}
    	s->capacity = capacity;
    	s->top = 0;
    	s->items = malloc(sizeof(uint32_t) * capacity);
    	if (s->items == NULL) {
        	free(s);
        	return NULL;
    	}
    	return s;
}

void stack_free(Stack **sp) {
    	if (sp != NULL && *sp != NULL) {
        	free((*sp)->items);
        	free(*sp);
        	*sp = NULL;
    	}
}

bool stack_push(Stack *s, uint32_t val) {
    	if (s == NULL || stack_full(s)) {
        	return false;
    	}
    	s->items[s->top++] = val;
    	return true;
}

bool stack_pop(Stack *s, uint32_t *val) {
    	if (s == NULL || stack_empty(s)) {
        	return false;
    	}
    	*val = s->items[--s->top];
    	return true;
}

bool stack_peek(const Stack *s, uint32_t *val) {
    	if (s == NULL || stack_empty(s)) {
		return false;
    	}
    	*val = s->items[s->top - 1];
    	return true;
}

bool stack_empty(const Stack *s) {
    	return s == NULL || s->top == 0;
}

bool stack_full(const Stack *s) {
    	return s != NULL && s->top == s->capacity;
}

uint32_t stack_size(const Stack *s) {
    	return s != NULL ? s->top : 0;
}

void stack_copy(Stack *dst, const Stack *src) {
    	if (dst == NULL || src == NULL) {
        	return;
    	}
    	if (dst->capacity < src->top) {
        	return;
    	}
    	dst->top = src->top;
    	for (uint32_t i = 0; i < src->top; i++) {
        	dst->items[i] = src->items[i];
    	}
}

void stack_print(const Stack *s, FILE *f, char *vals[]) {
    	if (s == NULL || f == NULL) {
        	return;
    	}
    	for (uint32_t i = 0; i < s->top; i++) {
        	fprintf(f, "%s\n", vals[s->items[i]]);
    	}
}

