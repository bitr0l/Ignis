/*
 * AMD64 MMU interface.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/units.h>
#include <machine/mmu.h>
#include <pexpert/mmu.h>
#include <string.h>

/* Unit multiple helpers */
#define GIB_N(N) (UNIT_GIB * (N))
#define MIB_N(N) (UNIT_MIB * (N))

/* Page tables */
ALIGN(4096) uint64_t pml4[512];
ALIGN(4096) uint64_t pdpt_low[512];
ALIGN(4096) uint64_t pdpt_high[512];
ALIGN(4096) uint64_t pd_high0[512];
ALIGN(4096) uint64_t pd_high1[512];

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

status_t
px_mmu_init (
    __in uint64_t kimage_base,
    __out uint64_t *kimage_entry
    )
{
    uint64_t kmap_base;
    size_t i;

    if (kimage_entry == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* Zero initialize page tables */
    memset(pml4, 0, sizeof(pml4));
    memset(pdpt_low, 0, sizeof(pdpt_low));
    memset(pdpt_high, 0, sizeof(pdpt_high));
    memset(pd_high0, 0, sizeof(pd_high0));
    memset(pd_high1, 0, sizeof(pd_high1));

    /* Point PML4 to low PDPT */
    pml4[0] = (uint64_t)pdpt_low;
    pml4[0] |= PTE_P | PTE_RW;

    /* Set up direct map */
    pml4[256] = (uint64_t)pdpt_low;
    pml4[256] |= PTE_P | PTE_RW;

    /* Point PML4 to high PDPT */
    pml4[511] = (uint64_t)pdpt_high;
    pml4[511] |= PTE_P | PTE_RW;

    /* Setup low 4 GiB identity mapping */
    pdpt_low[0] = GIB_N(0) | PTE_P | PTE_RW | PTE_PS;
    pdpt_low[1] = GIB_N(1) | PTE_P | PTE_RW | PTE_PS;
    pdpt_low[2] = GIB_N(2) | PTE_P | PTE_RW | PTE_PS;
    pdpt_low[3] = GIB_N(3) | PTE_P | PTE_RW | PTE_PS;

    /* Setup high kernel mappings */
    kmap_base = ALIGN_DOWN(kimage_base, UNIT_MIB * 2);
    pdpt_high[510] = (uint64_t)pd_high0 | PTE_P | PTE_RW;
    pdpt_high[511] = (uint64_t)pd_high1 | PTE_P | PTE_RW;

    /* Populate PD high entries */
    for (i = 0; i < 512; ++i) {
        pd_high0[i] = (kmap_base + (i * MIB_N(2)));
        pd_high0[i] |= PTE_P | PTE_RW | PTE_PS;
    }
    for (i = 0; i < 512; ++i) {
        pd_high1[i] = MIB_N(512) + (kmap_base + (i * MIB_N(2)));
        pd_high1[i] |= PTE_P | PTE_RW | PTE_PS;
    }

    /* Switch address space to our own */
    mmu_write_cr3((uint64_t)pml4);
    *kimage_entry = KERNEL_VBASE;
    return STATUS_SUCCESS;
}
