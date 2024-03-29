#include <rlu/rlu.h>

void *(*allocate)(size_t size) = malloc;
void (*dealloc)(void *p) = free;

static size_t pool_index = 0;
static size_t pool_size = 0;
static void *pool_begin = NULL;

void pool_init(size_t kilobytes)
{
	assert(pool_begin == NULL);
	pool_begin = malloc(kilobytes * 1024);
	assert(pool_begin);
	pool_size = kilobytes * 1024;
	memset(pool_begin, 0, pool_size);
	allocate = poolloc;
	dealloc = poolfree;
}

void pool_cleanup(void)
{
	assert(pool_begin);
	(void)memset(pool_begin, 0, pool_size);
	free(pool_begin);
	pool_index = 0;
	pool_size = 0;
	pool_begin = NULL;
	allocate = malloc;
	dealloc = free;
}

void pool_usage(void)
{
	(void)printf("%.3f KB / %.3f KB\n", pool_index / 1000.0f, pool_size / 1000.0f);
}

void *poolloc(size_t size)
{
	void *addr;
	msg_assert(pool_index + size < pool_size, "Not enough memory: %zu / %zu bytes", pool_index + size, pool_size);
	addr = (void *)&((unsigned char *)pool_begin)[pool_index];
	pool_index += size;

	return addr;
}

void poolfree(void *p)
{
	(void)p;
	// It's fine, just drain the pool.
	return;
}

int rand_range(int min, int max)
{
	return rand() % (max - min) + min;
}

float rand_uniform(void)
{
	return (float)rand() / (float)RAND_MAX;
}

char *file_read(const char *fname, size_t *size)
{
	if (!fname) {
		return NULL;
	}

	char *buf;
	size_t bytes;
	long length;

	FILE *f = fopen(fname, "rb");
	assert(f);

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = malloc(length + 1);
	assert(buf);
	
	bytes = fread(buf, 1, length, f);
	buf[bytes] = '\0';
	if (size) {
		*size = bytes;
	}

	fclose(f);
	return buf;
}

int file_write(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "wb");
	if (!f) {
		return 0;
	}

	fputs(str, f);
	fclose(f);
	return 1;
}

int file_append(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "ab");
	if (!f) {
		return 0;
	}

	fputs(str, f);
	fclose(f);
	return 1;
}

int arg_check(int argc, char **argv, const char *arg)
{
	int i;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], arg) == 0) {
			return 1;
		}
	}
	return 0;
}

char *arg_get(int argc, char **argv, const char *arg)
{
	int i;
	for (i = 0; i < argc; i++) {
		if ((strcmp(argv[i], arg) == 0) && (i + 1 < argc)) {
			return argv[i + 1];
		}
	}
	return NULL;
}

float fast_sqrtf(float number)
{
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv = { number }; /* member 'f' set to value of 'number'. */
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= (threehalfs - (x2 * conv.f * conv.f));
	return 1.0f / conv.f;
}


float distance(float x0, float y0, float x1, float y1)
{
	return fast_sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}
