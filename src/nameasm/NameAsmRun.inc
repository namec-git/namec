;;ThAsmRun.inc -- include file for ThAsmRun.asm
;
;Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
;$Id: $$Change: 2398 $
;$Date: 2018/05/15 $$Author: bbarber $

; Defined by ThAsmRun.asm
        EXTERNDEF  fn@decAX:near, fn@incAX:near, fn@newAX:near, fn@breakpoint:near, fn@clearAssigned:near, fn@err_goto_ax:near, fn@err_notKnown:near, fn@err_unknownAX:near, fn@err_unknownLabel:near, fn@error:near, fn@initialize:near, fn@print:near, fn@printComma:near, fn@printCommaSP:near, fn@printNL:near, fn@printSP:near, fn@print_fields:near, fn@print_name:near
        EXTERNDEF  fn@main_base:near

        ; Defined by dunpData.asm
        EXTERNDEF  th_first_name2:dword
        EXTERNDEF  th_first_name:word
        EXTERNDEF  th_DEC_n:word
        EXTERNDEF  th_INC_n:word
        EXTERNDEF  th_last_assigned_n:word
        EXTERNDEF  th_next_assignable:word
        EXTERNDEF  th_previous_clock_msec:dword
        EXTERNDEF  th_assigned_lbl2:dword
        EXTERNDEF  th_assigned_lbl_size:dword
        EXTERNDEF  th_names:dword
        EXTERNDEF  th_name_table:dword
        EXTERNDEF  th_runs:word
        EXTERNDEF  th_runs_remaining:word              ; Set th_runs_remaining for timing

th_valueBits       equ 15      ;values are n bits and even, at most 16.  For 16-bit code, max is 8.  
                                                       ; Worked around post-MASM-6.11 performance bug by limiting DUP(...) to known names.  
                                                       ; Otherwise at most 11 bits can be allocated. If a DFA, then 15 bits OK if comment-out allocation of assigned names (th_assigned_n)
_K                 equ 1024
th_valueInc        equ 2       ;increment for next_assignable.  All names are positive, even 32-bit integers
th_maxValue        equ (1 shl th_valueBits)-th_valueInc  ;maximum value
th_nameCount       equ (1 shl th_valueBits)/th_valueInc
th_nameSize        equ th_maxValue+th_valueInc  ;bytes allocated for each name (n_...)
th_new             equ 0       ;represents all unknown values, same as ThCode::NoName (0)
th_jzSize          equ 4       ;size of 'jz label' instruction for runtime returns from conditional calls


