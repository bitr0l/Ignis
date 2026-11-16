/*
 * Kernel flat binary loader.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/cdef.h>
#include <binload.h>

status_t
binload_flat (
    __in struct file_descriptor *kernel,
    __out EFI_PHYSICAL_ADDRESS *base
    )
{
    EFI_PHYSICAL_ADDRESS kernel_image;
    EFI_STATUS efi_status;
    status_t status;
    uint64_t kimage_size;

    if (kernel == NULL || base == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /*
     * Load the kernel, it expects to sit at 1 MiB.
     */

    kimage_size = ALIGN_UP(kernel->size_bytes, EFI_PAGE_SIZE);
    kernel_image = (EFI_PHYSICAL_ADDRESS)0x100000;

    efi_status = g_bs->AllocatePages(
        AllocateAddress,
        EfiLoaderData,
        kimage_size / EFI_PAGE_SIZE,
        &kernel_image
    );

    if (efi_status != EFI_SUCCESS) {
        cons_printf(DOTS L"fatal: failed to reserve region at 1 MiB\r\n");
        return efi_status_conv(efi_status);
    }

    /*
     * Fill the kernel image.
     */

    status = read_file(
        kernel,
        &kimage_size,
        (void *)kernel_image
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: failed to read kernel binary\r\n");
        return status;
    }

    *base = kernel_image;
    return STATUS_SUCCESS;
}
