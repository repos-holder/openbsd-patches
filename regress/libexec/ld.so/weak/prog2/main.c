
/*
 * Public domain. 2002, Federico Schwindt <fgsch@openbsd.org>.
 */

#include <err.h>
#include "defs.h"

int
main(int argc, char **argv)
{
	if (weak_func() != WEAK_REF)
		errx(1, "error calling weak_func");

	if (func() != WEAK_REF)
		errx(1, "error calling weak reference reference");

	return (0);
}
