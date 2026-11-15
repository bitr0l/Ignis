/*
 * Bootloader configuration.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <bootlib.h>

status_t
loader_configure (
    __in EFI_FILE_HANDLE boot_volume
    )
{
    struct file_descriptor config;
    status_t status;

    if (boot_volume == NULL) {
        cons_printf(DOTS L"bug: Boot volume cannot be NULL\r\n");
        cons_printf(DOTS L"bug: In loader_configure()\r\n");
        return STATUS_INVALID_PARAM;
    }

    /*
     * Locate the configuration file.
     */

    status = open_file_path(
        boot_volume,
        L"ignis.cfg",
        &config
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: Unable to locate 'ignis.cfg'\r\n");
        cons_printf(DOTS L"fatal: Ensure that 'ignis.cfg' is on the boot volume\r\n");
        return status;
    }

    return STATUS_SUCCESS;
}
