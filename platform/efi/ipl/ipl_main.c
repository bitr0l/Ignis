/*
 * Initial program load.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <bootlib.h>

void
ipl_main (
    void
    )
{
    /* Loader information */
    cons_printf(DOTS L"Begin %s IPL/0\r\n", LOADER_NAME);
    cons_printf(DOTS L"Loader revision: %s\r\n", LOADER_REVISION);
}
