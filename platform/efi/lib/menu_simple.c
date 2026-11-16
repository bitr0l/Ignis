/*
 * Boot menu interface.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <bootlib.h>
#include <menu.h>
#include <stdbool.h>

/*
 * Checks if the given character is numeric
 *
 * @c: Character to check
 *
 * Return value:
 *      Returns true if character is numeric.
 *      Returns false if character is not numeric.
 */
static inline bool
is_numeric (
    __in char c
    )
{
    return (c >= '0' && c <= '9');
}

status_t
simple_menu_start (
    __in const struct config_boot_table *boot_tab,
    __out struct config_menu_entry *entry
    )
{
    EFI_STATUS efi_status;
    EFI_INPUT_KEY key;
    const struct config_menu_entry *menu_entry;
    size_t i;
    uint8_t sel;
    char keyc;

    if (boot_tab == NULL) {
        return STATUS_INVALID_PARAM;
    }

    cons_printf(L"Press a number to boot an entry\r\n");
    cons_printf(L"------------------------------------\r\n");
    for (i = 0; i < boot_tab->nr_entries; ++i) {
        menu_entry = &boot_tab->entries[i];

        cons_printf(
            L"%d ~ %s (%s)\r\n",
            i,
            menu_entry->name,
            menu_entry->kernel
        );
    }
    cons_printf(L"------------------------------------\r\n");

    /*
     * Handle the boot menu.
     */

    for (;;) {
        for (;;) {
            efi_status = g_st->ConIn->ReadKeyStroke(g_st->ConIn, &key);
            if (efi_status == EFI_SUCCESS) {
                keyc = (char)key.UnicodeChar;
                if (is_numeric(keyc))
                    break;
            }
        }

        sel = keyc - '0';
        if (sel >= boot_tab->nr_entries) {
            continue;
        }

        break;
    }

    menu_entry = &boot_tab->entries[sel];
    cons_printf(DOTS L"Handing off to %s...\r\n", menu_entry->name);
    *entry = *menu_entry;
    return STATUS_SUCCESS;
}
