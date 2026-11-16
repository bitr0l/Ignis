/*
 * Kernel loader.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _BINLOAD_H_
#define _BINLOAD_H_ 1

#include <sys/types.h>
#include <sys/comdef.h>
#include <efilib.h>
#include <bootlib.h>
#include <config.h>

/*
 * Delegate a binary to the proper loader.
 *
 * @boot_volume:    Boot volume kernel lives on
 * @entry:          Boot entry to load
 * @base:           Resulting base address
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns error code on failure.
 */
status_t
binload_delegate (
    __in EFI_FILE_HANDLE boot_volume,
    __in struct config_menu_entry *entry,
    __out EFI_PHYSICAL_ADDRESS *kbase
    );

/*
 * Load a flat kernel binary.
 *
 * @kernel:  Kernel file descriptor
 * @base:    Resulting image base is written here
 *
 * Return value:
 *     Returns STATUS_SUCCESS if successful.
 *     Returns error code on failure.
 */
status_t
binload_flat (
    __in struct file_descriptor *kernel,
    __out EFI_PHYSICAL_ADDRESS *base
    );

#endif  /* !_BINLOAD_H */
