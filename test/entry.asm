;;
;; Test kernel payload.
;; Copyright (c) 2026, Ian Moffett, et al.
;; Provided under the BSD-3 clause.
;;

[org 0x100000]

    section .text
_start:
    cli
    hlt
    jmp _start
