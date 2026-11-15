/*
 * Boot library and interfaces.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _BOOTLIB_H
#define _BOOTLIB_H 1

#include <sys/types.h>
#include <sys/comdef.h>
#include <wchar.h>

/* Constant strings */
#define LOADER_REVISION L"v0.0.1"
#define LOADER_NAME     L"Ignis"
#define DOTS            L"........ "

/*
 * Write a formatted wide-character string to the console
 *
 * @fmt:    Format string
 * @<...>:  Variadic arguments
 */
void
cons_printf (
    __in const wchar_t *fmt,
    ...
    );

/*
 * Initial program load entry point.
 */
void
ipl_main (
    void
    );

#endif  /* !_BOOTLIB_H */
