; written by 'namec --assemble'
.486
.model flat, stdcall
        include NameAsmRun.inc
        extrn  fn@breakpoint:near, fn@err_goto_ax:near, fn@err_notKnown:near, fn@error:near, fn@initialize:near, fn@print:near, fn@printComma:near, fn@print_fields:near, fn@print_name:near
        option oldstructs   ; allow name.field
        option noscoped     ; otherwise OpLabel is not viewable by .data segment. Alternative is label:: instead of label:
        option casemap:none ; case-sensitive
        include name_data.asm
.code
fn@main_base: ; th@initialize reports base address
fn@main proc
i@0000:                                         ;; Three Turing machines->
Three_Turing_machines@thid8714:
i@0003:                                         ;; select Machine->
select_Machine@thid8714:
  mov   thid8714_n.Turing_machine,th_3_state_busy_beaver ;; Turing machine= 3-state busy beaver
i@0011:                                         ;; define Turing machines->
define_Turing_machines@thid8714:
  mov   incrementer_n.firstCell,c001            ;; incrementer.firstCell= c001
  mov   Cells_n.c001,th_1                       ;; Cells.c001= 1
  mov   Right_n.c001,c002                       ;; Right.c001= c002
  mov   Cells_n.c002,th_1                       ;; Cells.c002= 1
  mov   Left_n.c002,c001                        ;; Left.c002= c001
  mov   Right_n.c002,c003                       ;; Right.c002= c003
  mov   Cells_n.c003,th_1                       ;; Cells.c003= 1
  mov   Left_n.c003,c002                        ;; Left.c003= c002
  mov   incrementer_n.firstState,go             ;; incrementer.firstState= go
  mov   incrementer_n.Blank,B                   ;; incrementer.Blank= B
  mov   th_1_n.go,r001                          ;; 1.go= r001
  mov   Write_n.r001,th_1                       ;; Write.r001= 1
  mov   Move_n.r001,Right                       ;; Move.r001= Right
  mov   NextState_n.r001,go                     ;; NextState.r001= go
  mov   B_n.go,r002                             ;; B.go= r002
  mov   Write_n.r002,th_1                       ;; Write.r002= 1
  mov   Move_n.r002,Stay                        ;; Move.r002= Stay
  mov   NextState_n.r002,STOP                   ;; NextState.r002= STOP
  mov   th_3_state_busy_beaver_n.firstCell,c010 ;; 3-state busy beaver.firstCell= c010
  mov   Cells_n.c010,th_0                       ;; Cells.c010= 0
  mov   th_3_state_busy_beaver_n.firstState,a   ;; 3-state busy beaver.firstState= a
  mov   th_3_state_busy_beaver_n.Blank,th_0     ;; 3-state busy beaver.Blank= 0
  mov   th_0_n.a,r010                           ;; 0.a= r010
  mov   Write_n.r010,th_1                       ;; Write.r010= 1
  mov   Move_n.r010,Right                       ;; Move.r010= Right
  mov   NextState_n.r010,b                      ;; NextState.r010= b
  mov   th_1_n.a,r011                           ;; 1.a= r011
  mov   Write_n.r011,th_1                       ;; Write.r011= 1
  mov   Move_n.r011,Left                        ;; Move.r011= Left
  mov   NextState_n.r011,c_                     ;; NextState.r011= c
  mov   th_0_n.b,r012                           ;; 0.b= r012
  mov   Write_n.r012,th_1                       ;; Write.r012= 1
  mov   Move_n.r012,Left                        ;; Move.r012= Left
  mov   NextState_n.r012,a                      ;; NextState.r012= a
  mov   th_1_n.b,r013                           ;; 1.b= r013
  mov   Write_n.r013,th_1                       ;; Write.r013= 1
  mov   Move_n.r013,Right                       ;; Move.r013= Right
  mov   NextState_n.r013,b                      ;; NextState.r013= b
  mov   th_0_n.c_,r014                          ;; 0.c= r014
  mov   Write_n.r014,th_1                       ;; Write.r014= 1
  mov   Move_n.r014,Left                        ;; Move.r014= Left
  mov   NextState_n.r014,b                      ;; NextState.r014= b
  mov   th_1_n.c_,r015                          ;; 1.c= r015
  mov   Write_n.r015,th_1                       ;; Write.r015= 1
  mov   Move_n.r015,Stay                        ;; Move.r015= Stay
  mov   NextState_n.r015,STOP                   ;; NextState.r015= STOP
  mov   th_5_state_busy_beaver_n.firstCell,c020 ;; 5-state busy beaver.firstCell= c020
  mov   Cells_n.c020,th_0                       ;; Cells.c020= 0
  mov   th_5_state_busy_beaver_n.firstState,A   ;; 5-state busy beaver.firstState= A
  mov   th_5_state_busy_beaver_n.Blank,th_0     ;; 5-state busy beaver.Blank= 0
  mov   th_0_n.A,r020                           ;; 0.A= r020
  mov   Write_n.r020,th_1                       ;; Write.r020= 1
  mov   Move_n.r020,Right                       ;; Move.r020= Right
  mov   NextState_n.r020,B                      ;; NextState.r020= B
  mov   th_1_n.A,r021                           ;; 1.A= r021
  mov   Write_n.r021,th_1                       ;; Write.r021= 1
  mov   Move_n.r021,Left                        ;; Move.r021= Left
  mov   NextState_n.r021,C_                     ;; NextState.r021= C
  mov   th_0_n.B,r022                           ;; 0.B= r022
  mov   Write_n.r022,th_1                       ;; Write.r022= 1
  mov   Move_n.r022,Right                       ;; Move.r022= Right
  mov   NextState_n.r022,C_                     ;; NextState.r022= C
  mov   th_1_n.B,r023                           ;; 1.B= r023
  mov   Write_n.r023,th_1                       ;; Write.r023= 1
  mov   Move_n.r023,Right                       ;; Move.r023= Right
  mov   NextState_n.r023,B                      ;; NextState.r023= B
  mov   th_0_n.C_,r024                          ;; 0.C= r024
  mov   Write_n.r024,th_1                       ;; Write.r024= 1
  mov   Move_n.r024,Right                       ;; Move.r024= Right
  mov   NextState_n.r024,D                      ;; NextState.r024= D
  mov   th_1_n.C_,r025                          ;; 1.C= r025
  mov   Write_n.r025,th_0                       ;; Write.r025= 0
  mov   Move_n.r025,Left                        ;; Move.r025= Left
  mov   NextState_n.r025,E                      ;; NextState.r025= E
  mov   th_0_n.D,r026                           ;; 0.D= r026
  mov   Write_n.r026,th_1                       ;; Write.r026= 1
  mov   Move_n.r026,Left                        ;; Move.r026= Left
  mov   NextState_n.r026,A                      ;; NextState.r026= A
  mov   th_1_n.D,r027                           ;; 1.D= r027
  mov   Write_n.r027,th_1                       ;; Write.r027= 1
  mov   Move_n.r027,Left                        ;; Move.r027= Left
  mov   NextState_n.r027,D                      ;; NextState.r027= D
  mov   th_0_n.E,r028                           ;; 0.E= r028
  mov   Write_n.r028,th_1                       ;; Write.r028= 1
  mov   Move_n.r028,Stay                        ;; Move.r028= Stay
  mov   NextState_n.r028,STOP                   ;; NextState.r028= STOP
  mov   th_1_n.E,r029                           ;; 1.E= r029
  mov   Write_n.r029,th_0                       ;; Write.r029= 0
  mov   Move_n.r029,Left                        ;; Move.r029= Left
  mov   NextState_n.r029,A                      ;; NextState.r029= A
i@0464:                                         ;; execute Turing machine->
execute_Turing_machine@thid8714:
  mov   ax,thid8714_n.Turing_machine            ;; Machine= Turing machine
  mov   thid8714_n.Machine,ax
  mov   ax,thid8714_n.Machine                   ;; Machine=
  call  fn@print
  call  fn@printNL
  xor   ecx,ecx                                 ;; Cell= Machine[firstCell]
  mov   cx,thid8714_n.Machine
  shl   ecx,th_valueBits-1 ;convert value to name
  mov   ax,th_first_name[ecx].firstCell
  mov   thid8714_n.Cell,ax
  xor   ecx,ecx                                 ;; State= Machine[firstState]
  mov   cx,thid8714_n.Machine
  shl   ecx,th_valueBits-1 ;convert value to name
  mov   ax,th_first_name[ecx].firstState
  mov   thid8714_n.State,ax
  xor   ecx,ecx                                 ;; Blank= Machine[Blank]
  mov   cx,thid8714_n.Machine
  shl   ecx,th_valueBits-1 ;convert value to name
  mov   ax,th_first_name[ecx].Blank
  mov   thid8714_n.Blank,ax
i@0497:                                         ;; while State is not STOP->
while_State_is_not_STOP@thid8714:
i@0500:                                         ;; execute Rule for Cell at State->
execute_Rule_for_Cell_at_State@thid8714:
  cmp   thid8714_n.State,STOP                   ;; State == STOP ->exit execute Rule
  je    exit_execute_Rule@thid8714
  xor   edx,edx                                 ;; Value= Cells[Cell]
  mov   dx,thid8714_n.Cell
  mov   ax,Cells_n[edx]
  mov   thid8714_n.Value,ax
  xor   ecx,ecx                                 ;; Rule= Value[State]
  mov   cx,thid8714_n.Value
  shl   ecx,th_valueBits-1 ;convert value to name
  xor   edx,edx
  mov   dx,thid8714_n.State
  mov   ax,th_first_name[ecx+edx]
  mov   thid8714_n.Rule,ax
  xor   ebx,ebx                                 ;; ->Move[Rule]
  xor   edx,edx
  mov   dx,thid8714_n.Rule
  mov   bx,Move_n[edx]
  and   ebx,ebx ; always check
  mov   edi,@F
  jz    fn@err_notKnown
@@:
  mov   eax,thid8714_lbl[ebx*2]
  and   eax,eax ; fall through on unknown label
  jz    @F
  jmp   eax
@@:
i@0529:                                         ;; unknown Rule->
unknown_Rule@thid8714:
  mov   ax,thid8714_n.State                     ;; State=,
  call  fn@print
  call  fn@printCommaSP
  mov   ax,thid8714_n.Cell                      ;; Cell=,
  call  fn@print
  call  fn@printCommaSP
  mov   ax,thid8714_n.Value                     ;; Value=,
  call  fn@print
  call  fn@printCommaSP
  mov   ax,Error____unknown_Rule_or_missing_Move_for_State__Cell__Value ;; Error -- unknown Rule or missing Move for State, Cell, Value=
  call  fn@print
  call  fn@printNL
  call  fn@error ; never returns                ;; !>
i@0549:                                         ;; Left->
Left@thid8714:
  xor   edx,edx                                 ;; Cells[Cell]= Write[Rule]
  mov   dx,thid8714_n.Rule
  mov   ax,Write_n[edx]
  xor   ebx,ebx
  mov   bx,thid8714_n.Cell
  mov   Cells_n[ebx],ax
  xor   edx,edx                                 ;; State= NextState[Rule]
  mov   dx,thid8714_n.Rule
  mov   ax,NextState_n[edx]
  mov   thid8714_n.State,ax
  xor   ebx,ebx                                 ;; Left[Cell] == th_NEW ->new left Cell
  mov   bx,thid8714_n.Cell
  cmp   Left_n[ebx],th_new
  je    new_left_Cell@thid8714
  xor   edx,edx                                 ;; Cell= Left[Cell]
  mov   dx,thid8714_n.Cell
  mov   ax,Left_n[edx]
  mov   thid8714_n.Cell,ax
  jmp   execute_Rule_for_Cell_at_State@thid8714 ;; ->execute Rule for Cell at State
i@0587:                                         ;; new left Cell->
new_left_Cell@thid8714:
  call  fn@newAX ;>ax                           ;; NewCell= th_NEW
  mov   thid8714_n.NewCell,ax
  mov   ax,thid8714_n.Blank                     ;; Cells[NewCell]= Blank
  xor   ebx,ebx
  mov   bx,thid8714_n.NewCell
  mov   Cells_n[ebx],ax
  mov   ax,thid8714_n.Cell                      ;; Right[NewCell]= Cell
  xor   ebx,ebx
  mov   bx,thid8714_n.NewCell
  mov   Right_n[ebx],ax
  mov   ax,thid8714_n.NewCell                   ;; Left[Cell]= NewCell
  xor   ebx,ebx
  mov   bx,thid8714_n.Cell
  mov   Left_n[ebx],ax
  mov   ax,thid8714_n.NewCell                   ;; Cell= NewCell
  mov   thid8714_n.Cell,ax
  mov   ax,thid8714_n.Cell                      ;; Machine[firstCell]= Cell
  xor   ebx,ebx
  mov   bx,thid8714_n.Machine
  shl   ebx,th_valueBits-1 ;convert value to name
  mov   th_first_name[ebx].firstCell,ax
  jmp   execute_Rule_for_Cell_at_State@thid8714 ;; ->execute Rule for Cell at State
i@0631:                                         ;; Stay->
Stay@thid8714:
  xor   edx,edx                                 ;; Cells[Cell]= Write[Rule]
  mov   dx,thid8714_n.Rule
  mov   ax,Write_n[edx]
  xor   ebx,ebx
  mov   bx,thid8714_n.Cell
  mov   Cells_n[ebx],ax
  xor   edx,edx                                 ;; State= NextState[Rule]
  mov   dx,thid8714_n.Rule
  mov   ax,NextState_n[edx]
  mov   thid8714_n.State,ax
  jmp   execute_Rule_for_Cell_at_State@thid8714 ;; ->execute Rule for Cell at State
i@0653:                                         ;; Right->
Right@thid8714:
  xor   edx,edx                                 ;; Cells[Cell]= Write[Rule]
  mov   dx,thid8714_n.Rule
  mov   ax,Write_n[edx]
  xor   ebx,ebx
  mov   bx,thid8714_n.Cell
  mov   Cells_n[ebx],ax
  xor   edx,edx                                 ;; State= NextState[Rule]
  mov   dx,thid8714_n.Rule
  mov   ax,NextState_n[edx]
  mov   thid8714_n.State,ax
  xor   ebx,ebx                                 ;; Right[Cell] == th_NEW ->new right Cell
  mov   bx,thid8714_n.Cell
  cmp   Right_n[ebx],th_new
  je    new_right_Cell@thid8714
  xor   edx,edx                                 ;; Cell= Right[Cell]
  mov   dx,thid8714_n.Cell
  mov   ax,Right_n[edx]
  mov   thid8714_n.Cell,ax
  jmp   execute_Rule_for_Cell_at_State@thid8714 ;; ->execute Rule for Cell at State
i@0691:                                         ;; new right Cell->
new_right_Cell@thid8714:
  call  fn@newAX ;>ax                           ;; NewCell= th_NEW
  mov   thid8714_n.NewCell,ax
  mov   ax,thid8714_n.Blank                     ;; Cells[NewCell]= Blank
  xor   ebx,ebx
  mov   bx,thid8714_n.NewCell
  mov   Cells_n[ebx],ax
  mov   ax,thid8714_n.Cell                      ;; Left[NewCell]= Cell
  xor   ebx,ebx
  mov   bx,thid8714_n.NewCell
  mov   Left_n[ebx],ax
  mov   ax,thid8714_n.NewCell                   ;; Right[Cell]= NewCell
  xor   ebx,ebx
  mov   bx,thid8714_n.Cell
  mov   Right_n[ebx],ax
  mov   ax,thid8714_n.NewCell                   ;; Cell= NewCell
  mov   thid8714_n.Cell,ax
  jmp   execute_Rule_for_Cell_at_State@thid8714 ;; ->execute Rule for Cell at State
i@0728:                                         ;; exit execute Rule->
exit_execute_Rule@thid8714:
i@0731:                                         ;; print Turing tape->
print_Turing_tape@thid8714:
  mov   ax,Skip_print                           ;; Skip print == true ->exit print tape
  cmp   ax,true
  je    exit_print_tape@thid8714
  xor   ecx,ecx                                 ;; Cell= Machine[firstCell]
  mov   cx,thid8714_n.Machine
  shl   ecx,th_valueBits-1 ;convert value to name
  mov   ax,th_first_name[ecx].firstCell
  mov   thid8714_n.Cell,ax
i@0748:                                         ;; print value for each Cell->
print_value_for_each_Cell@thid8714:
  xor   edx,edx                                 ;; Cells[Cell]=
  mov   dx,thid8714_n.Cell
  mov   ax,Cells_n[edx]
  call  fn@print
  call  fn@printNL
  xor   ebx,ebx                                 ;; Right[Cell] == th_NEW ->exit print tape
  mov   bx,thid8714_n.Cell
  cmp   Right_n[ebx],th_new
  je    exit_print_tape@thid8714
  xor   edx,edx                                 ;; Cell= Right[Cell]
  mov   dx,thid8714_n.Cell
  mov   ax,Right_n[edx]
  mov   thid8714_n.Cell,ax
  jmp   print_value_for_each_Cell@thid8714      ;; ->print value for each Cell
i@0776:                                         ;; exit print tape->
exit_print_tape@thid8714:
  mov   ax,STOP                                 ;; STOP=
  call  fn@print
  call  fn@printNL
  ret                                           ;; ->
fn@main endp
END
