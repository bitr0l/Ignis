/*
 * Initial program load.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/cdef.h>
#include <bootlib.h>
#include <binload.h>
#include <menu.h>

void
ipl_main (
    void
    )
{
    EFI_PHYSICAL_ADDRESS kbase;
    EFI_FILE_HANDLE boot_volume;
    EFI_STATUS efi_status;
    struct config_boot_table boot_tab;
    struct config_menu_entry boot_entry;
    struct memory_map mem_map;
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
    if (!SYS_SUCCESS(status)) {
        return;
    }

    /*
     * Enter the boot menu.
     */

    status = simple_menu_start(&boot_tab, &boot_entry);
    if (!SYS_SUCCESS(status)) {
        return;
    }

    /*
     * Load the kernel.
     */

    status = binload_delegate(boot_volume, &boot_entry, &kbase);
    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: failed to load kernel\r\n");
        return;
    }

    /*
     * Obtain the platform memory map.
     */

    status = get_memory_map(&mem_map);
    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: failed to obtain memory map\r\n");
        return;
    }

    /*
     * Exit boot services.
     */
    efi_status = g_bs->ExitBootServices(
        g_image,
        mem_map.key
    );

    if (efi_status != EFI_SUCCESS) {
        cons_printf(DOTS L"fatal: failed to exit boot services\r\n");
        return;
    }

    if (!SYS_SUCCESS(status)) {
        return;
    }

    ((void(*)(void))kbase)();
    __builtin_unreachable();
}
