/*
 * AMD64 MMU interface.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <machine/mmu.h>

uint64_t
mmu_read_cr3 (
    void
    )
{
    uint64_t cr3;

    ASMV(
        "mov %%cr3, %0"
        : "=r" (cr3)
        :
        : "memory"
    );

    return cr3;
}

void
mmu_write_cr3 (
    uint64_t value
    )
{
    ASMV(
        "mov %0, %%cr3"
        :
        : "r" (value)
        : "memory"
    );
}
