; NameAsmRun -- Runtime and main() for nameasm
;
; Notes:
;   Must be assembled separately from name.asm due to name.asm using 'option oldstructs' for a.b references
;   EDI is reserved for return addresses and error reporting
;   Built with MASM32 SDK from masm32.com
;   For debugging tutorial with Microsoft C++, see kipivine.com
;
;Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
;$Id: //main/2005/thid/src/nameasm/NameAsmRun.asm#3 $$Change: 2478 $
;$Date: 2018/09/14 $$Author: bbarber $

.xlist
    ; masm32.com/...
    include \masm32\include\masm32rt.inc
.list
.lall
    include NameAsmRun.inc

    ; from C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\include\time.h
    CLOCKS_PER_SEC equ 1000  
    ; Invoke clock() from /c/masm32/lib/msvcrt.lib
    extrn clock:near 

    PUBLIC fn@err_notKnown, fn@err_unknownAX, fn@err_unknownLabel
    PUBLIC fn@breakpoint, fn@clockMsec, fn@error, fn@incAX, fn@newAX, fn@initialize
    PUBLIC fn@print, fn@printComma, fn@printCommaSP, fn@printNL, fn@printSP, fn@print_fields, fn@print_name

    extrn   th_first_assignable:byte  ; FIXUP how to import a 16 bit constant?  Cannot use EXTERNDEF with EQU.  ':abs" produced LNK1190 invalid fixup found, type ...1

.code 
align 16 ;segment alignment is 16

;;main -- main() for nameasm.exe
;
;   start -- entry point from END
;       'start' is immediately after the end of name.asm
;       NameAsmRun is followed by masm32 code from /masm32/m32lib/masm32.lib
;       See src/nameasm/name.lst and src/nameasm/NameAsmRun.lst
;
;   main
;       initializes nameasm 
;       invokes fn@main in ThAsmRun
;       repeats th_runs_remaining times

    fn@main PROTO 

start: 
  call main
  exit

main proc
  mov   ax,th_runs
  mov   th_runs_remaining,ax
  call  fn@initialize
@@:
  invoke fn@main
  call  fn@clearAssigned
  dec   th_runs_remaining
  jnz   @B
  ret

main endp

;==== Alphabetic by function label ===

;; invoke breakpoint (int 3)
; 
;   assumes that a debugger is available
;
;group: thasm.-runtime
; call:  writeAsmCode

fn@breakpoint:
        ;; 'int 3' exits non-zero if not in debugger
    ret

;; clear and check assigned names
;
;   th_next_assignable
;       th_previous_clock_msec
;returns:
;       reports elapsed time and updates th_previous_clock_msec
;       check for overwritten th_noname_n
;       clears name memory up to th_next_assignable
;
;group: thasm.-runtime
; call: ThAsmMain::main

fn@clearAssigned:
  print "Elapsed "
  call  fn@clockMsec ;>eax
  mov   ecx,eax
  sub   ecx,th_previous_clock_msec
  mov   th_previous_clock_msec,eax
  print ustr$(ecx)
  print " msec",10
  xor   ecx,ecx                                         ;;check th_noname_n fields for non-zero values
@@:                             ;ecx= value
  mov   ax,th_first_name[ecx]
  and   ax,ax
  jnz   aa@overwrite
  inc   ecx
  inc   ecx
  cmp   ecx,th_nameSize
  jbe   @B
  jmp   @F
aa@overwrite:
  push  eax
  push  ecx
  print "Error: th_noname field "
  pop   ecx
  push  ecx
  print str$(ecx)
  print " overwritten with value "
  pop   ecx
  pop   eax
  push  ecx
  print str$(eax),10
  pop   ecx
  jmp   @B
@@:
aa@clearAssignedBytes:
  print "fn@clearAssigned clears "
  xor   ecx,ecx                                                                 ;;Count= th_nameSize*th_next_assignable/2
  mov   cx,th_next_assignable
  shr   cx,1
  mov   eax,th_nameSize
  mul   ecx ;>eax
  push  eax
  shr   eax,10
  print ustr$(eax)
  print "KB of assigned name memory "
  pop   eax
  invoke memfill,th_first_name2,eax,0
  mov   eax,offset th_first_assignable                  ;;th_next_assignable <- th_first_assignable
  mov   th_next_assignable,ax
  call  fn@clockMsec ;>eax                                              ;;update previousClockMsec and log elapsed time
  mov   ecx,eax
  sub   ecx,th_previous_clock_msec                              
  mov   th_previous_clock_msec,eax
  print ustr$(ecx)
  print " msec",10
  ret

;; Return processor clock as msec since process start
;
;returns:
;       EAX= msec since process start
;
;group: thasm.-runtime
; call: fn@initialize

fn@clockMsec:
  call  clock ;>eax
  mov   ecx,CLOCKS_PER_SEC/1000
  div   ecx
  ret

;; Report error ...
;
;       EDI= most recent return address (@F)
;returns:
;               Never returns, 'exit 1'
;
;group: thasm.-runtime
; call: ThAsmRun::fn@initialize, ThAsmCode::writeAsmInstruction, etc.

fn@err_assignable:
  push  edi
  print "th_ERROR out-of-memory.  Cannot assign a new value after 0x"
  pop   edi
  print uhex$(edi),10
  exit 1

fn@err_goto_ax:
  push  edi
  push  eax
  print "th_ERROR unknown label '"
  pop   eax ; name
  call  fn@print_name
  print "' after 0x"
  pop   edi ; previous return
  print uhex$(edi),10
  exit  1

fn@err_inc_assignable:
  push  edi
  print "th_ERROR out-of-memory.  Cannot assign a th_INC value after 0x"
  pop   edi
  print uhex$(edi),10
  exit 1

fn@err_inc_dx:
  push  edi
  push  edx
  print "th_ERROR out-of-bounds th_INC value 0x"
  pop   edx ; name
  print uhex$(edx),10
  print "' after 0x"
  pop   edi ; previous return
  print uhex$(edi),10
  exit  1

fn@err_initialize:  ; AX=th_names.end(), DX= th_first_assignable
  push  edx
  push  eax
  print "th_ERROR initialize th_names.end "
  pop   eax
  print str$(ax)
  print " != th_first_assignable "
  pop   edx
  print str$(dx),10
  exit  1

fn@err_notKnown:
  push  edi
  print "th_ERROR not-known value for known name at or after 0x"
  pop   edi ; previous return
  print uhex$(edi),10
  call  fn@clearAssigned ; checks for overwrite
  exit 1

fn@err_oddPrint:
  push  edi
  print "th_ERROR oddPrint after 0x"
  pop   edi ; previous return
  print uhex$(edi),10
  exit  1

;;report unknown name in AX
fn@err_unknownAX:
  push  edi
  push  eax
  print "th_ERROR '"
  pop   eax
  call  fn@print_name
  print "' at or after 0x"
  pop   edi ; previous return
  print uhex$(edi),10
  call  fn@clearAssigned ; checks for overwrite
  exit 1

;;Report unknown Label in BX
;       Only used for deprecated "a=b ->name.field"
fn@err_unknownLabel:
  push  edi
  push  ebx
  print "th_ERROR unknown label '"
  pop   eax
  call  fn@print_name
  print "' contents of field at or after 0x"
  pop   edi ; previous return
  print uhex$(edi),10
  call  fn@clearAssigned ; checks for overwrite
  exit 1

;; report error
;
;group: thasm.-runtime
; call:  writeAsmCode

fn@error:
  print "th_ERROR error reported at "
  pop   edi ; return location
  push  edi
  print uhex$(edi),10
  exit 1

;; Update AX to the next assignable value 
;
;       AX= an assignable value
;       th_next_assignable= next assignable value, if needed
;       th_INC_n= array of next assignable values or th_new(0) if none
;       EDI= return address (@F)
;returns:
;       AX= the next assignable value
;       EAX,EDX not preserved
;       Other registers preserved
;               if AX' is a new assignable value
;                       AX' == th_next_assignable
;           th_INC_n[AX]= AX'
;           th_next_assignable += 2
;
;group: thasm.-runtime
; call: ThAsmCode::writeAsmInstruction

fn@incAX:
  xor   edx,edx ; clear edx
  mov   dx,ax
  xor   eax,eax ; clear eax
  cmp   edx,th_maxValue
  ja    fn@err_inc_dx
  mov   ax,th_INC_n[edx]
  and   eax,eax
  jz    @F      ; fn_new
  ret
@@:
  mov   ax,th_next_assignable
  cmp   eax,th_maxValue
  ja    fn@err_inc_assignable
  add   th_next_assignable,th_valueInc
  mov   th_INC_n[edx],ax
  ret

;; initialize ThAsmMain
; 
;returns:
;   th_name_table is DWORD pointers into th_names
;      Free with GlobalFree or free
;   verifies sizeof(th_name_table)==th_first_assignable
;
;group: thasm.-runtime
; call:  writeAsmCode

fn@initialize:
  print "fn@main at 0x"
  mov   edi,offset fn@main_base
  print uhex$(edi)
  print " at "
  call  fn@clockMsec ;>eax
  print ustr$(eax)
  print " msec",10
  invoke ltok,th_names,ADDR th_name_table
  add   ax,ax
  mov   edx,offset th_first_assignable
  cmp   ax,dx
  jne   fn@err_initialize
  invoke memfill,th_assigned_lbl2,th_assigned_lbl_size,0
  mov   th_last_assigned_n,0 ; check allocation of name memory -- last word filled by memfill,th_first_name2
  invoke memfill,th_first_name2,th_nameCount*th_nameSize,0
  print "fn@initialize cleared "
  print ustr$(th_nameCount*th_nameSize/_K)
  print "KB of name memory "
  call  fn@clockMsec ;>eax
  mov   th_previous_clock_msec,eax
  print ustr$(eax)
  print " msec",10
  ret

;; Set AX to a new assignable value 
;
;       th_next_assignable= next assignable value
;       EDI= return address (@F)
;returns:
;       AX= a new value, EAX is otherwise preserved
;       Other registers preserved
;       th_next_assignable += 2
;
;group: thasm.-runtime
; call: ThAsmCode::writeAsmInstruction

fn@newAX:
  mov   ax,th_next_assignable
  cmp   ax,th_maxValue
  ja    fn@err_assignable
  add   th_next_assignable,th_valueInc
  ret

;; print Value
;
;       AX= Value
;returns:
;       EDI preserved
;
;group: thasm.-runtime
; call: wrtAsmInstruction.PrintA
; same: fn@printComma

fn@print:
  push  edi
  call  fn@print_name
  pop   edi
  ret

;; print comma
;
;returns:
;       EDI preserved
;
;group: thasm.-runtime
; call: wrtAsmInstruction.PrintComma
; same: fn@print

fn@printComma:
  push  edi
  print ","
  pop   edi
  ret

;; print comma space
;
;       AX= Value
;returns:
;       EDI preserved
;
;group: thasm.-runtime
; call: wrtAsmInstruction.PrintCommaSpace
; same: fn@print

fn@printCommaSP:
  push  edi
  print ", "
  pop   edi
  ret

;; print new line
;
;returns:
;       EDI preserved
;
;group: thasm.-runtime
; call: wrtAsmInstruction.PrintNewline
; same: fn@print

fn@printNL:
  push  edi
  print chr$(10) ;\n
  pop   edi
  ret

;; print space
;
;returns:
;       EDI preserved
;
;group: thasm.-runtime
; call: wrtAsmInstruction.PrintSpace
; same: fn@print

fn@printSP:
  push  edi
  print " "
  pop   edi
  ret

;;print Name
;
;   AX= Name
;returns:
;   EDI preserved
;
;group: thasm.-runtime
; call: fn@print...

fn@print_name:
  push  edi
  test  ax,0001h
  jnz   fn@err_oddPrint
  mov   edx,offset th_first_assignable
  cmp   ax,dx
  jb    @F
  push  eax
  print "n_"
  pop   eax
  and   eax,0000ffffh
  print str$(eax)
  pop   edi
  ret
@@:
  mov   esi,th_name_table
  xor   ebx,ebx
  mov   bx,ax
  add   ebx,ebx  ;2x since ax is a word offset, th_name_table is dwords
  print [esi+ebx]
  pop   edi
  ret

;; print fields of Value
;
;       AX= Value
;returns:
;       EDI preserved
;
;group: thasm.-runtime
; call: wrtAsmInstruction.DebugA

fn@print_fields:
  push  edi
  test  ax,0001h
  jnz   fn@err_oddPrint
  mov   edx,offset th_first_assignable
  cmp   ax,dx
  jb    @F
  push  eax
  print "n"
  pop   eax
  push  eax
  print str$(ax),'-',62,10 ;->
  pop   eax
  jmp   aa@fields
@@:
  mov   esi,th_name_table
  xor   ebx,ebx
  mov   bx,ax
  add   ebx,ebx  ;2x since ax is words while th_name_table is dwords
  push  eax
  print [esi+ebx],'-',62,10,"  " ;->
  pop   eax
aa@fields:
  xor   esi,esi
  mov   si,ax
  shl   esi,th_valueBits-1 ;convert value to name
  add   esi,offset th_first_name
  xor   ebx,ebx
@@:
  mov   ax,[esi+ebx]
  and   ax,ax
  jnz   @F
aa@nextField:
  cmp   bx,th_maxValue
  jae   aa@exitFields
  add   ebx,th_valueInc
  jmp   @B
@@:
  push  ebx
  push  esi
  push  eax
  mov   ax,bx
  call  fn@print_name
  print chr$('-',62,10),"    " ;->
  pop   eax
  call  fn@print_name
  print chr$(10) ;\n
  pop   esi
  pop   ebx
  jmp   aa@nextField
aa@exitFields:
  pop   edi
  ret

END start 

        