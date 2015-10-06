/*
 * Public Domain 2003 Dale Rahn
 *
 */
#include <stdio.h>
#include <dlfcn.h>

int
main()
{
	void *handle;
	int ret = 0;

	handle = dlopen("libac.so.0.0", RTLD_LAZY);
	if (handle != NULL) {
		printf("found libaa, dependancy of libac, not expected\n");
		ret = 1;
	}

	return ret;
}
