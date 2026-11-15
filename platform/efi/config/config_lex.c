/*
 * Bootloader configuration lexer.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#include <configvar.h>
#include <bootlib.h>
#include <stdbool.h>

/* Character transformers */
#define TO_LOWER(C) ((C) | 0x20)

/*
 * Checks if a character is a whitespace.
 *
 * @c: Character to check
 *
 * Return value:
 *      Returns true if the given character is a whitespace.
 *      Returns false if the given character is not a whitespace.
 */
static inline bool
lexer_is_ws (
    __in char c
    )
{
    switch (c) {
    case '\n':
    case '\t':
    case '\r':
    case ' ':
        return true;
    }

    return false;
}

/*
 * Checks if a character is alphabetical
 *
 * @c: Character to check
 *
 * Return value:
 *      Returns true if the given character is a letter.
 *      Returns false if the given character is not a letter.
 */
static inline bool
lexer_is_alpha (
    __in char c
    )
{
    char lc = TO_LOWER(c);

    return lc >= 'a' && lc <= 'z';
}

/*
 * Consume a single byte from within the config
 *
 * @state:   Lexer state
 * @skip_ws: If true, skip whitespace characters
 *
 * Return value:
 *      Returns the consumed character on success.
 *      Returns '\0' on EOF.
 */
static char
lexer_consume_byte (
    __in struct config_lexer_state *state,
    __in bool skip_ws
    )
{
    char c;

    if (state == NULL) {
        return '\0';
    }

    while (state->buf_index < state->buf_max_size) {
        c = state->config_buf[state->buf_index++];
        if (skip_ws && lexer_is_ws(c)) {
            continue;
        }

        return c;
    }

    return '\0';
}

/*
 * Scan an identifier
 *
 * @state:  Lexer state
 * @lc:     Last character scanned
 * @token:  Resulting token
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns error code on failure.
 */
static status_t
lexer_scan_ident (
    __in struct config_lexer_state *state,
    __in char lc,
    __out struct config_token *token
    )
{
    char c;
    size_t sidx = 0;

    if (state == NULL || token == NULL) {
        return STATUS_INVALID_PARAM;
    }

    token->s[sidx++] = lc;
    for (;;) {
        /* Consume and test */
        c = lexer_consume_byte(state, false);
        if (!lexer_is_alpha(c)) {
            break;
        }

        /* Don't overflow */
        if (sidx >= CONFIG_MAX_IDENT_LEN - 1) {
            cons_printf(DOTS L"fatal: Identifier exceeds maximum length\r\n");
            return STATUS_FAILURE;
        }

        token->s[sidx++] = c;
    }

    token->kind = CONFIG_TT_IDENT;
    token->s[sidx] = '\0';
    return STATUS_SUCCESS;
}

/*
 * Scan a string
 *
 * @state:  Lexer state
 * @token:  Resulting token
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns error code on failure.
 */
static status_t
lexer_scan_string (
    __in struct config_lexer_state *state,
    __out struct config_token *token
    )
{
    char c;
    size_t sidx = 0;

    if (state == NULL || token == NULL) {
        return STATUS_INVALID_PARAM;
    }

    for (;;) {
        c = lexer_consume_byte(state, false);

        if (c == '\0') {
            cons_printf(DOTS L"fatal: Unclosed '\"' in string\r\n");
            return STATUS_FAILURE;
        }

        if (c == '"') {
            break;
        }

        if (sidx >= CONFIG_MAX_IDENT_LEN - 1) {
            cons_printf(DOTS L"fatal: String exceeds maximum length\r\n");
            return STATUS_FAILURE;
        }

        token->s[sidx++] = c;
    }

    token->s[sidx] = '\0';
    token->kind = CONFIG_TT_STRING;
    return STATUS_SUCCESS;
}

status_t
config_lexer_init (
    __in const char *config_buf,
    __in size_t config_buf_size,
    __out struct config_lexer_state *lexer
    )
{
    if (config_buf == NULL || lexer == NULL) {
        return STATUS_INVALID_PARAM;
    }

    lexer->config_buf = config_buf;
    lexer->buf_index = 0;
    lexer->buf_max_size = config_buf_size;
    return STATUS_SUCCESS;
}

status_t
config_lexer_scan (
    __in struct config_lexer_state *state,
    __out struct config_token *token
    )
{
    char c;

    if (state == NULL || token == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if ((c = lexer_consume_byte(state, true)) == '\0') {
        return STATUS_END_OF_FILE;
    }

    switch (c) {
    case ':':
        token->kind = CONFIG_TT_COLON;
        token->c = c;
        return STATUS_SUCCESS;
    case '{':
        token->kind = CONFIG_TT_LBRACE;
        token->c = c;
        return STATUS_SUCCESS;
    case '}':
        token->kind = CONFIG_TT_RBRACE;
        token->c = c;
        return STATUS_SUCCESS;
    case '"':
        return lexer_scan_string(state, token);
    default:
        /* Is it an identifier? */
        if (lexer_is_alpha(c)) {
            return lexer_scan_ident(state, c, token);
        }

        break;
    }

    cons_printf(
        DOTS L"fatal: Got unexpected token '%c' in config\r\n",
        c
    );

    return STATUS_FAILURE;
}
