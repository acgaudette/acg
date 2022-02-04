#ifndef SYS_H
#define SYS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

__attribute__((noreturn))
static void panic()
{
	printf("Exit failure\n");
	fflush(stdout);
	abort();
}

__attribute__((noreturn))
static void panic_msg(const char *msg)
{
	fflush(stdout);
	fprintf(stderr, "panic! %s\n", msg);
	panic();
}

__attribute__((noreturn))
static void panic_perr(const char *msg)
{
#ifndef _WIN32
	char *buf = strerror(errno);
#else
	char buf[80];
	strerror_s(buf, 80, errno);
#endif
	fprintf(stderr, "panic! %s: %s\n", msg, buf);
	panic();
}

#endif
