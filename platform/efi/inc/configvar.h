/*
 * Internal config definitions.
 * Copyright (c) 2026, Ian Moffett, et al.
 * Provided under the BSD-3 clause.
 */

#ifndef _CONFIGVAR_H
#define _CONFIGVAR_H 1

#include <sys/types.h>
#include <sys/status.h>
#include <sys/comdef.h>

/* Maximum length of identifiers */
#define CONFIG_MAX_IDENT_LEN 32

/*
 * Represents valid lexical tokens.
 *
 * @CONFIG_TT_NONE:     [NONE]
 * @CONFIG_TT_IDENT:    [IDENTIFIER]
 * @CONFIG_TT_STRING:   [STRING]
 * @CONFIG_TT_COLON:    ':'
 * @CONFIG_TT_LBRACE:   '{'
 * @CONFIG_TT_RBRACE:   '}'
 */
typedef enum {
    CONFIG_TT_NONE,
    CONFIG_TT_IDENT,
    CONFIG_TT_STRING,
    CONFIG_TT_COLON,
    CONFIG_TT_LBRACE,
    CONFIG_TT_RBRACE
} config_token_t;

/*
 * Represents a lexical token.
 *
 * @kind:  Token kind
 * @c:     Single character variant
 * @s:     Null terminated string variant
 */
struct config_token {
    config_token_t kind;
    union {
        char c;
        wchar_t s[CONFIG_MAX_IDENT_LEN];
    };
};

/*
 * Represents the state of the config lexer.
 *
 * @config_buf:     Buffer containing config file contents
 * @buf_max_size:   Maximum size of buffer
 * @buf_index:      Index into @config_buf
 */
struct config_lexer_state {
    const char *config_buf;
    size_t buf_max_size;
    size_t buf_index;
};

/*
 * Initialize the config lexer.
 *
 * @config_buf:         Configuration file contents
 * @config_buf_size:    Size of configuration buffer
 * @lexer:              Lexer state machine
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns error code on failure.
 */
status_t
config_lexer_init (
    __in const char *config_buf,
    __in size_t config_buf_size,
    __out struct config_lexer_state *lexer
    );

/*
 * Scan a single token from within the config
 *
 * @state:  Lexer state
 * @token:  Token result
 *
 * Return value:
 *      Returns STATUS_SUCCESS on success.
 *      Returns error code on failure.
 */
status_t
config_lexer_scan (
    __in struct config_lexer_state *state,
    __out struct config_token *token
    );

#endif /* !_CONFIGVAR_H */
