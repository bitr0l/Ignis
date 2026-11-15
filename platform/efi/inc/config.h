/*
 * Bootloader configuration.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _CONFIG_H
#define _CONFIG_H 1

#include <sys/types.h>
#include <sys/status.h>
#include <sys/comdef.h>

/* Maximum length of menu entry name */
#define CONFIG_MENU_ENTRY_LEN 32

/*
 * Represents valid protocols.
 *
 * @CONFIG_PROTOCOL_NONE:   No associated protocol (INVALID)
 * @CONFIG_PROTOCOL_FLAT:   Flat binary protocol @ 1 MiB
 */
typedef enum {
    CONFIG_PROTOCOL_NONE,
    CONFIG_PROTOCOL_FLAT
} config_protocol_t;

/*
 * Represents a menu entry.
 *
 * @name:       Name of menu entry
 * @protocol:   Protocol to be used for entry
 */
struct config_menu_entry {
    char name[CONFIG_MENU_ENTRY_LEN];
    config_protocol_t protocol;
};

/*
 * Represents a boot table.
 *
 * @entries:        List of boot entries
 * @nr_entries:     Number of entries present
 */
struct config_boot_table {
    struct config_menu_entry *entries;
    size_t nr_entries;
};

/*
 * Parse the given config file.
 *
 * @config_buf:         Buffer containing config file contents
 * @config_buf_size:    Size of configuration buffer
 * @boot_tab:           Boot table
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns an error code on failure.
 */
status_t
config_parse (
    __in const char *config_buf,
    __in size_t config_buf_size,
    __out struct config_boot_table *boot_tab
    );

#endif /* !CONFIG_H */
