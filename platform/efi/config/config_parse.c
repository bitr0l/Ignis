/*
 * Bootloader configuration parser.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <sys/cdef.h>
#include <config.h>
#include <configvar.h>
#include <bootlib.h>

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
    [CONFIG_TT_COLON]       = LITTOK(L":"),
    [CONFIG_TT_LBRACE]      = LITTOK(L"{"),
    [CONFIG_TT_RBRACE]      = LITTOK(L"}")
};

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
        if (status != STATUS_SUCCESS)
            return status;

        cons_printf(DOTS L"Got config token %s\r\n", TOKEN_STRING(&token));
    }

    return STATUS_SUCCESS;
}
