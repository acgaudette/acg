#ifndef INTERN_H
#define INTERN_H

#include <stddef.h>

typedef const char *istr;

void init_interns();
istr intern(const char*, size_t);
istr intern_substr(const char*, const char*);
istr intern_str(const char*); // Null-terminated

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define INTERN_DEF_CAP 1024

typedef struct {
    istr data;
    size_t len;
} intern_entry;

// const char* -> istr
typedef struct {
	intern_entry *data;
	size_t cap;
	size_t len;
} intern_table;

static intern_table interns;

static size_t hash(const char *str, size_t len)
{
	size_t h = 5381;
	size_t i = 0;

	while (i < len)
		h += (h << 5) + str[i++];

	return h;
}

intern_entry table_get_ins(intern_table*, const char*, size_t);

void table_resize(intern_table *table)
{
	size_t sz = table->cap * 2;

	intern_table swap = {
		calloc(sz, sizeof(intern_entry)),
		sz,
		0,
	};

	if (table->data) {
		for (size_t i = 0; i < table->cap; ++i) {
			intern_entry entry = table->data[i];
			if (!entry.data) continue;
			table_get_ins(&swap, entry.data, entry.len);
		}

		free(table->data);
	}

	*table = swap;
}

intern_table new_intern_table(size_t desired_len)
{
	assert(desired_len);

	intern_table table;
	table.data = NULL;
	table.cap = desired_len;

	table_resize(&table);
	return table;
}

intern_entry table_get_ins(intern_table *table, const char *str, size_t len)
{
	if ((table->len + 1) * 2 >= table->cap)
		table_resize(table);
	assert((table->len + 1) * 2 < table->cap);

	size_t i = hash(str, len);
	i %= table->cap;

	intern_entry entry;

	while (1) {
		entry = table->data[i];

		if (!entry.data) break;
		else if (len == entry.len && !strncmp(str, entry.data, len))
			return entry;

		i = (i + 1) % table->cap;
	}

	/* Make new intern */

	// Allocate string
	char *data = (char*)malloc(len + 1);

	if (!data) {
		perror("Memory error");
		exit(1);
	}

	memcpy(data, str, len);
	data[len] = 0; // Null terminator

	entry.data = data;
	entry.len = len;

	table->data[i] = entry;
	table->len++;

	return entry;
}

void init_interns()
{
	interns = new_intern_table(INTERN_DEF_CAP);
}

istr intern(const char *str, size_t len)
{
    assert(interns.data);
    return table_get_ins(&interns, str, len).data;
}

istr intern_substr(const char *beg, const char *end)
{
    size_t len = end - beg;
    return intern(beg, len);
}

istr intern_str(const char *str)
{
    size_t len = strlen(str);
    return intern(str, len);
}

#endif
