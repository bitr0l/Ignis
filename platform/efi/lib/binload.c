/*
 * Kernel loader.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <binload.h>
#include <efilib.h>
#include <string.h>

#define PROTOCOL_FLAT L"flat"

status_t
binload_delegate (
    __in EFI_FILE_HANDLE boot_volume,
    __in struct config_menu_entry *entry,
    __out EFI_PHYSICAL_ADDRESS *kbase,
    __out bool *require_high_map
    )
{
    struct file_descriptor kernel_desc;
    status_t status;

    if (entry == NULL) {
        return STATUS_INVALID_PARAM;
    }

    *require_high_map = false;

    /*
     * Open the selected kernel.
     */

    status = open_file_path(
        boot_volume,
        entry->kernel,
        &kernel_desc
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(
            DOTS L"fatal: failed to locate '%s' on the boot volume\r\n",
            entry->kernel
        );
        return status;
    }

    if (memcmp(entry->protocol, PROTOCOL_FLAT, sizeof(PROTOCOL_FLAT)) == 0) {
        status = binload_flat(&kernel_desc, kbase);
        if (!SYS_SUCCESS(status))
            return status;

        *require_high_map = true;
    } else {
        cons_printf(DOTS L"fatal: Invalid protocol '%s'\r\n", entry->protocol);
        return STATUS_FAILURE;
    }

    return STATUS_SUCCESS;
}
