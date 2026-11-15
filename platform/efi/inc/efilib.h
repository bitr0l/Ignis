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

#endif  /* !_EFILIB_H */
