/*
 * EFI firmware handoff point.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/comdef.h>
#include <efilib.h>
#include <bootlib.h>

EFIAPI
EFI_STATUS
efi_main (
    __in EFI_HANDLE image_handle,
    __in EFI_SYSTEM_TABLE *systab
    )
{
    const uint8_t MAX_ABORT_INDEX = 8;
    status_t status;
    uint8_t index;

    /*
     * Initialize the global firmware interface.
     */

    status = efi_firmware_init(image_handle, systab);
    if (!SYS_SUCCESS(status)) {
        return EFI_INVALID_PARAMETER;
    }

    /*
     * Reset the output console with no extended verification.
     * Doesn't matter if it fails as it isn't too important...
     */

    g_cons->Reset(
        g_cons,
        FALSE
    );

    /*
     * Enter initial program load phase and abort if it returns.
     */

    ipl_main();
    cons_printf(DOTS L"IPL ABORTED ****\r\n");

    /*
     * Stall before aborting.
     */

    for (index = 0; index < MAX_ABORT_INDEX; ++index) {
        cons_printf(DOTS L"Aborting in %d\r", MAX_ABORT_INDEX - index);
        g_bs->Stall(500000);
    }

    cons_printf(L"\r\n");
    return EFI_ABORTED;
}
