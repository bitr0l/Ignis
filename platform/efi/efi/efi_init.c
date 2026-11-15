/*
 * EFI firmware handoff point.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/comdef.h>
#include <efilib.h>

EFIAPI
EFI_STATUS
efi_main (
    __in EFI_HANDLE image_handle,
    __in EFI_SYSTEM_TABLE *systab
    )
{
    status_t status;

    /*
     * Initialize the global firmware interface.
     */

    status = efi_firmware_init(image_handle, systab);
    if (!SYS_SUCCESS(status)) {
        return EFI_INVALID_PARAMETER;
    }

    /* Abort if this point is reached */
    return EFI_ABORTED;
}
