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
    EFI_FILE_HANDLE boot_volume;
    struct config_menu_entry *menu_entry;
    struct config_boot_table boot_tab;
    size_t index;
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

    /*
     * Obtain the boot volume.
     */

    status = get_boot_volume(&boot_volume);
    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: Unable to obtain boot volume\r\n");
        return;
    }

    /*
     * Configure the loader through ignis.cfg
     *
     * XXX: This call is expected to produce logs thus no error messages
     *      are needed on this failure path.
     */

    status = loader_configure(boot_volume, &boot_tab);
    if (status != STATUS_SUCCESS) {
        return;
    }

    for (index = 0; index < boot_tab.nr_entries; ++index) {
        menu_entry = &boot_tab.entries[index];

        cons_printf(
            DOTS L"MENU ENTRY(%s) : [proto '%s', kernel '%s']\r\n",
            menu_entry->name,
            menu_entry->protocol,
            menu_entry->kernel
        );
    }
}
