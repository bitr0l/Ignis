/*
 * EFI library and interfaces.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _EFILIB_H
#define _EFILIB_H 1

#include <sys/status.h>
#include <sys/comdef.h>
#include <sys/types.h>
#include <efi/efi.h>

/* EFI global state  */
extern EFI_HANDLE g_image;
extern EFI_SYSTEM_TABLE *g_st;
extern EFI_BOOT_SERVICES *g_bs;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *g_cons;

/*
 * Convert an EFI status code to our own internal status codes.
 *
 * @status: EFI status code to convert
 *
 * Return value:
 *      Returns an Ignis specific status code.
 */
status_t
efi_status_conv (
    __in EFI_STATUS status
    );

/*
 * Initialize firmware global state.
 *
 * @image_handle:  Image handle passed by firmware
 * @systab:        System table passed by firmware
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns an error code failure.
 */
status_t
efi_firmware_init (
    __in EFI_HANDLE image_handle,
    __in EFI_SYSTEM_TABLE *systab
    );

#endif  /* !_EFILIB_H */
