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
    status_t status;

    /*
     * Log loader information.
     */

    cons_printf(DOTS L"Begin %s IPL/0\r\n", LOADER_NAME);
    cons_printf(DOTS L"Loader revision: %s\r\n", LOADER_REVISION);

    /*
     * Disable the platform watchdog timer as a good
     * practice...
     */

    status = set_watchdog_timer(
        0,
        0,
        0,
        NULL
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"warning: Could not disable watchdog, continuing...\r\n");
    }
}
