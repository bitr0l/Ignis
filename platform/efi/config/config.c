/*
 * Bootloader configuration.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <bootlib.h>
#include <efilib.h>
#include <config.h>

status_t
loader_configure (
    __in EFI_FILE_HANDLE boot_volume,
    __out struct config_boot_table *boot_tab
    )
{
    char *config_buf;
    struct file_descriptor config;
    size_t config_size;
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

    /*
     * Allocate memory to read the config file into.
     */

    status = g_bs->AllocatePool(
        EfiLoaderData,
        config.size_bytes,
        (void **)&config_buf
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: Unable to allocate config buffer\r\n");
        return status;
    }

    /*
     * Read the config file.
     */

    config_size = config.size_bytes;
    status = read_file(
        &config,
        &config_size,
        config_buf
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: Unable to read config file\r\n");
        g_bs->FreePool(config_buf);
        return status;
    }

    /*
     * Parse the config
     */

    status = config_parse(config_buf, config_size, boot_tab);
    if (status != STATUS_SUCCESS) {
        g_bs->FreePool(config_buf);
        return status;
    }

    g_bs->FreePool(config_buf);
    return STATUS_SUCCESS;
}
