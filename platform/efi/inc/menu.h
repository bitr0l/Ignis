/*
 * Boot menu interface.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _MENU_H
#define _MENU_H 1

#include <sys/types.h>
#include <sys/comdef.h>
#include <config.h>

/*
 * Start the simple boot menu.
 *
 * @boot_tab: Boot table
 * @entry:    Selected menu entry
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns error code on failure.
 */
status_t
simple_menu_start (
    __in const struct config_boot_table *boot_tab,
    __out struct config_menu_entry *entry
    );

#endif  /* _MENU_H */
