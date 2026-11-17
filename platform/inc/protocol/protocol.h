/*
 * General protocol definitions.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _PROTOCOL_PROTOCOL_H
#define _PROTOCOL_PROTOCOL_H 1

#include <bootlib.h>

/*
 * Valid boot protocols
 */
typedef enum {
    PROTOCOL_IGNIS
} protocol_t;

/*
 * Allocate and fill in a protocol structure
 *
 * @protocol:    Protocol structure to allocate
 * @handle:      Handle of constructed protocol
 *
 * Return value:
 *      Returns STATUS_SUCCESS if successful.
 *      Returns error code on failure.
 */
status_t
protocol_fill (
    __in protocol_t protocol,
    __out void **handle
    );

#endif  /* !_PROTOCOL_PROTOCOL_H */
