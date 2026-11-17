/*
 * Ignis boot protocol.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _PROTOCOL_IGNIS_H
#define _PROTOCOL_IGNIS_H 1

#include <sys/types.h>

/*
 * The IGNIS boot protocol structure.
 *
 * @direct_map:  Direct map base address
 */
struct ignis_protocol {
    uint64_t direct_map;
};

#endif  /* !_PROTOCOL_IGNIS */
