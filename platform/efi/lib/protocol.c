/*
 * General protocol definitions.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <protocol/protocol.h>
#include <protocol/ignis.h>
#include <pexpert/mmu.h>
#include <efilib.h>

/*
 * Fill in the IGNIS protocol
 *
 * @handle:  Handle of constructed protocol
 *
 * Return value:
 *      Returns STATUS_SUCCESS if successful.
 *      Returns error code on failure.
 */
static status_t
ignis_fill (
    __out void **handle,
    __in const struct protocol_params *proto_params
    )
{
    EFI_STATUS efi_status;
    struct ignis_protocol *proto_handle;

    if (handle == NULL) {
        return STATUS_INVALID_PARAM;
    }

    proto_handle = (struct ignis_protocol *)handle;
    proto_handle->direct_map = DIRECT_MAP_VBASE;
    proto_handle->mem_map = proto_params->mem_map;
    return STATUS_SUCCESS;
}

status_t
protocol_fill (
    __in protocol_t protocol,
    __in const struct protocol_params *proto_params,
    __out void **handle
    )
{
    if (handle == NULL) {
        return STATUS_INVALID_PARAM;
    }

    switch (protocol) {
    case PROTOCOL_IGNIS:
        ignis_fill(handle, proto_params);
        break;
    default:
        return STATUS_INVALID_PARAM;
    }

    return STATUS_SUCCESS;
}
