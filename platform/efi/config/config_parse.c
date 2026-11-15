/*
 * Bootloader configuration parser.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/cdef.h>
#include <config.h>
#include <configvar.h>
#include <bootlib.h>
#include <string.h>

/*
 * Symbolic token.
 */
#define SYMTOK(TOK) \
    L"[" TOK L"]"

/*
 * Literal token.
 */
#define LITTOK(TOK) \
    L"'" TOK L"'"

/*
 * Safe `toktab' indexer.
 */
#define TT_STRING(TYPE)             \
    ((TYPE) >= NELEM(toktab))       \
        ? L"[BAD]"                  \
        : toktab[(TYPE)]

/*
 * Token variant wrapper of TT_STRING
 */
#define TOKEN_STRING(TOK_P) \
    TT_STRING((TOK_P)->kind)

/*
 * Token to string translation table.
 */
static const wchar_t *toktab[] = {
    [CONFIG_TT_NONE]        = SYMTOK(L"NONE"),
    [CONFIG_TT_IDENT]       = SYMTOK(L"IDENT"),
    [CONFIG_TT_STRING]      = SYMTOK(L"STRING"),
    [CONFIG_TT_COLON]       = LITTOK(L":"),
    [CONFIG_TT_LBRACE]      = LITTOK(L"{"),
    [CONFIG_TT_RBRACE]      = LITTOK(L"}")
};

/*
 * Assert that the next token is of a specific kind
 *
 * @lexer:  Lexer state machine
 * @token:  Current token pointer
 * @next:   Next token to assert
 */
static status_t
assert_next (
    __in struct config_lexer_state *lexer,
    __out struct config_token *token,
    __in config_token_t next
    )
{
    status_t status;

    if (lexer == NULL) {
        return STATUS_INVALID_PARAM;
    }

    status = config_lexer_scan(lexer, token);
    if (status != STATUS_SUCCESS) {
        cons_printf(DOTS L"fatal: expected %s\r\n", TT_STRING(next));
        return status;
    }

    if (token->kind != next) {
        cons_printf(
            DOTS L"fatal: expected %s, got %s instead\r\n",
            TT_STRING(next),
            TOKEN_STRING(token)
        );

        return STATUS_FAILURE;
    }

    return STATUS_SUCCESS;
}

/*
 * Parse a menu entry.
 *
 * @lexer:      Lexer state machine
 * @token:      Token to parse
 * @boot_tab:   Boot table
 *
 * Return value:
 *      Returns STATUS_SUCCESS if successful.
 *      Returns an error code on failure.
 */
static status_t
parse_menu_entry (
    __in struct config_lexer_state *lexer,
    __inout struct config_token *token,
    __out struct config_boot_table *boot_tab
    )
{
    struct config_menu_entry *entry;
    size_t name_len;
    status_t status;

    if (lexer == NULL || token == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (boot_tab == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (boot_tab->nr_entries >= CONFIG_MENU_MAX_ENTRIES) {
        cons_printf(DOTS L"fatal: Maximum entry count exceeded\r\n");
        return STATUS_FAILURE;
    }

    entry = &boot_tab->entries[boot_tab->nr_entries++];
    name_len = wcslen(token->s);

    if (name_len >= CONFIG_MENU_ENTRY_LEN - 1) {
        cons_printf(
            DOTS L"fatal: Menu entry name '%s' exceeds maximum length\r\n",
            token->s
        );

        return STATUS_FAILURE;
    }

    /* Copy the menu entry name */
    wmemcpy(entry->name, token->s, name_len);

    /* Expect '{' */
    status = assert_next(lexer, token, CONFIG_TT_LBRACE);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    /* TODO: Handle key values */
    for (;;) {
        /* Expect KEY */
        status = assert_next(lexer, token, CONFIG_TT_IDENT);
        if (status != STATUS_SUCCESS) {
            cons_printf(
                DOTS L"fatal: near entry '%s'\r\n",
                entry->name
            );
            return status;
        }

        cons_printf(DOTS L"Got key %s in entry %s\r\n", token->s, entry->name);

        /* Expect ':' */
        status = assert_next(lexer, token, CONFIG_TT_COLON);
        if (!SYS_SUCCESS(status)) {
            cons_printf(
                DOTS L"fatal: near entry '%s'\r\n",
                entry->name
            );
            return status;
        }

        /* Expect STRING */
        status = assert_next(lexer, token, CONFIG_TT_STRING);
        if (!SYS_SUCCESS(status)) {
            cons_printf(
                DOTS L"fatal: near entry '%s'\r\n",
                entry->name
            );
            return status;
        }

        cons_printf(DOTS L"Got value %s in entry %s\r\n", token->s, entry->name);

        /* Peek to see if there is a '}' */
        status = config_lexer_scan(lexer, token);
        if (!SYS_SUCCESS(status)) {
            cons_printf(
                DOTS L"fatal: Got unexpected EOF in entry %s\r\n",
                entry->name
            );
            return status;
        }

        /* End of this boot entry? */
        if (token->kind != CONFIG_TT_RBRACE) {
            config_lexer_tputback(lexer, token);
        } else {
            break;
        }
    }

    return STATUS_SUCCESS;
}

/*
 * Parse an incoming token.
 *
 * @lexer:      Lexer state machine
 * @token:      Token to parse
 * @boot_tab:   Boot table
 *
 * Return value:
 *      Returns STATUS_SUCCESS if successful.
 *      Returns an error code on failure.
 */
static status_t
parse_token (
    __in struct config_lexer_state *lexer,
    __inout struct config_token *token,
    __out struct config_boot_table *boot_tab
    )
{
    if (lexer == NULL || token == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (boot_tab == NULL) {
        return STATUS_INVALID_PARAM;
    }

    switch (token->kind) {
    case CONFIG_TT_IDENT:
        return parse_menu_entry(lexer, token, boot_tab);
    default:
        cons_printf(
            DOTS L"fatal: Got unexpected token '%s'\r\n",
            TOKEN_STRING(token)
        );

        return STATUS_FAILURE;
    }
}

status_t
config_parse (
    __in const char *config_buf,
    __in size_t config_buf_size,
    __out struct config_boot_table *boot_tab
    )
{
    struct config_lexer_state lexer;
    struct config_token token;
    status_t status;

    if (config_buf == NULL) {
        return STATUS_INVALID_PARAM;
    }

    memset(boot_tab, 0, sizeof(*boot_tab));

    /*
     * Initialize the lexer.
     */

    status = config_lexer_init(
        config_buf,
        config_buf_size,
        &lexer
    );

    if (!SYS_SUCCESS(status)) {
        return status;
    }

    for (;;) {
        status = config_lexer_scan(&lexer, &token);

        /* Handle end of file or error */
        if (status == STATUS_END_OF_FILE)
            break;
        if (!SYS_SUCCESS(status))
            return status;

        #if defined(_IGNIS_DEBUG)
        cons_printf(DOTS L"Got config token %s\r\n", TOKEN_STRING(&token));
        if (token.kind == CONFIG_TT_IDENT || token.kind == CONFIG_TT_STRING) {
            cons_printf(DOTS L"(%s)\r\n", token.s);
        }
        #endif /* _IGNIS_DEBUG */

        status = parse_token(&lexer, &token, boot_tab);
        if (!SYS_SUCCESS(status)) {
            return status;
        }
    }

    return STATUS_SUCCESS;
}
