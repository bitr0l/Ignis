/*
 * Boot library and interfaces.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _BOOTLIB_H
#define _BOOTLIB_H 1

#include <sys/types.h>
#include <sys/comdef.h>

/*
 * Initial program load entry point.
 */
void
ipl_main (
    void
    );

#endif  /* !_BOOTLIB_H */
