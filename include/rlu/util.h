#ifndef RLU_UTIL_H
#define RLU_UTIL_H

#include <assert.h>
#include <stddef.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#ifndef NDEBUG
	#include <stdio.h>
	#define msg_assert(Expr, ...) do { \
		if (!!(Expr)) {} \
		else { \
			fprintf(stderr, "Error: " __VA_ARGS__); \
			fprintf(stderr, "\n"); \
			assert(Expr); \
		} \
	} while (0)
#else
	#define msg_assert(...) ((void)0)
#endif // NDEBUG

/* definitely not thread safe lol */
void pool_init(size_t kilobytes);
void pool_cleanup(void);
void pool_usage(void);
void *poolloc(size_t size);
void poolfree(void *p);

/**
 * Use allocate and dealloc for dynamic
 * memory allocation. If pool isn't
 * initialized, it just used malloc and
 * free. But if pool is initialized, then
 * poolloc and poolfree is used instad.
 * On pool cleanup, malloc and free are
 * retied to these pointers.
 */
extern void *(*allocate)(size_t size);
extern void (*dealloc)(void *p);

#define RAND_RANGE(min, max) (rand() % (max - min) + min)
int rand_range(int min, int max);
float rand_uniform(void);

#define streq(s1, s2) (strcmp((s1), (s2)) == 0)

char *file_read(const char *fname, size_t *size);
int file_write(const char *fname, const char *str);
int file_append(const char *fname, const char *str);

int arg_check(int argc, char **argv, const char *arg);
char *arg_get(int argc, char **argv, const char *arg);

float fast_sqrtf(float number);
float distance(float x0, float y0, float x1, float y1);

#endif // RLU_UTIL_H
