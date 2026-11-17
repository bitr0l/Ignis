/*
 * Initial program load.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/cdef.h>
#include <pexpert/mmu.h>
#include <pexpert/prim.h>
#include <protocol/ignis.h>
#include <protocol/protocol.h>
#include <bootlib.h>
#include <binload.h>
#include <menu.h>

/* Globals */
static void *proto_handle;

/* Kernel entry function */
typedef void(__attribute__((sysv_abi)) *kentry_t)(void *);

/*
 * Hang the processor in case we can't use boot services and
 * something goes wrong.
 *
 * @reason:  Reason for hang
 *
 * TODO: Log to serial
 */
static void
hang (
    __in const char *reason
    )
{
    (void)reason;

    for (;;) {
        PX_INT_DISABLE();
        PX_CORE_HALT();
    }
}

/*
 * Allocate a protocol handle.
 *
 * @protocol:  Protocol to allocate for
 *
 * Return value:
 *      Returns STATUS_SUCCESS if successful.
 *      Returns error code on failure.
 */
static status_t
allocate_proto_handle (
    __in protocol_t protocol
    )
{
    EFI_STATUS efi_status;
    uint64_t size;

    switch (protocol) {
    case PROTOCOL_IGNIS:
        size = sizeof(struct ignis_protocol);
        break;
    default:
        return STATUS_INVALID_PARAM;
    }

    /*
     * Allocate a protocol structure.
     */

    efi_status = g_bs->AllocatePool(
        EfiLoaderData,
        size,
        (void **)&proto_handle
    );

    if (efi_status != EFI_SUCCESS) {
        cons_printf(DOTS L"fatal: failed to allocate ignis protocol structure\r\n");
        return efi_status_conv(efi_status);
    }

    return STATUS_SUCCESS;
}

void
ipl_main (
    void
    )
{
    EFI_PHYSICAL_ADDRESS kbase;
    EFI_FILE_HANDLE boot_volume;
    EFI_STATUS efi_status;
    struct config_boot_table boot_tab;
    struct config_menu_entry boot_entry;
    struct memory_map mem_map;
    bool need_high_map;
    uint64_t kentry;
    kentry_t handoff;
    status_t status;
    protocol_t protocol;

    /*
     * Log loader information.
     */

    cons_printf(DOTS L"Begin %s IPL/0\r\n", LOADER_NAME);
    cons_printf(DOTS L"Loader revision: %s\r\n", LOADER_REVISION);

    /*
     * Disable the platform watchdog timer as a good
     * practice...
     */

    status = set_watchdog_timer(
        0,
        0,
        0,
        NULL
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"warning: Could not disable watchdog, continuing...\r\n");
    }

    /*
     * Obtain the boot volume.
     */

    status = get_boot_volume(&boot_volume);
    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: Unable to obtain boot volume\r\n");
        return;
    }

    /*
     * Configure the loader through ignis.cfg
     *
     * XXX: This call is expected to produce logs thus no error messages
     *      are needed on this failure path.
     */

    status = loader_configure(boot_volume, &boot_tab);
    if (!SYS_SUCCESS(status)) {
        return;
    }

    /*
     * Enter the boot menu.
     */

    status = simple_menu_start(&boot_tab, &boot_entry);
    if (!SYS_SUCCESS(status)) {
        return;
    }

    /*
     * Load the kernel.
     */

    status = binload_delegate(
        boot_volume,
        &boot_entry,
        &kbase,
        &need_high_map,
        &protocol
    );

    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: failed to load kernel\r\n");
        return;
    }

    /*
     * Allocate a protocol handle.
     */

    status = allocate_proto_handle(protocol);
    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: failed allocate protocol handle\r\n");
        return;
    }

    /*
     * Obtain the platform memory map.
     */

    status = get_memory_map(&mem_map);
    if (!SYS_SUCCESS(status)) {
        cons_printf(DOTS L"fatal: failed to obtain memory map\r\n");
        return;
    }

    status = protocol_fill(protocol, proto_handle);
    if (!SYS_SUCCESS(status)) {
        return;
    }

    /*
     * Exit boot services.
     */

    efi_status = g_bs->ExitBootServices(
        g_image,
        mem_map.key
    );

    if (efi_status != EFI_SUCCESS) {
        cons_printf(DOTS L"fatal: failed to exit boot services\r\n");
        return;
    }

    kentry = (uint64_t)kbase;

    /*
     * Jump to kernel entry.
     */

    if (need_high_map) {
        status = px_mmu_init((uint64_t)kbase, &kentry);
        if (!SYS_SUCCESS(status)) {
            hang("px_mmu_init failure");
        }
    }

    handoff = (kentry_t)kentry;
    handoff(proto_handle);
    __builtin_unreachable();
}
