/*
 * EFI firmware handoff point.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/comdef.h>
#include <efi/efi.h>

EFIAPI
EFI_STATUS
efi_main (
    __in EFI_HANDLE image_handle,
    __in EFI_SYSTEM_TABLE *systab
    )
{
    /* Abort if this point is reached */
    return EFI_ABORTED;
}
