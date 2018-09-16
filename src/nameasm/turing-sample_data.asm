.data
        public th_first_assignable

th_DEC  EQU 2
th_INC  EQU 4
th_LIT  EQU 6
th_NAME  EQU 8
th_NEW  EQU 10
th_VAR  EQU 12
thid8714  EQU 14
Three_Turing_machines  EQU 16
select_Machine  EQU 18
Turing_machine  EQU 20
th_3_state_busy_beaver  EQU 22
define_Turing_machines  EQU 24
incrementer  EQU 26
firstCell  EQU 28
c001  EQU 30
Cells  EQU 32
th_1  EQU 34
Right  EQU 36
c002  EQU 38
Left  EQU 40
c003  EQU 42
firstState  EQU 44
go  EQU 46
Blank  EQU 48
B  EQU 50
r001  EQU 52
Write  EQU 54
Move  EQU 56
NextState  EQU 58
r002  EQU 60
Stay  EQU 62
STOP  EQU 64
c010  EQU 66
th_0  EQU 68
a  EQU 70
r010  EQU 72
b  EQU 74
r011  EQU 76
c_  EQU 78
r012  EQU 80
r013  EQU 82
r014  EQU 84
r015  EQU 86
th_5_state_busy_beaver  EQU 88
c020  EQU 90
A  EQU 92
r020  EQU 94
r021  EQU 96
C_  EQU 98
r022  EQU 100
r023  EQU 102
r024  EQU 104
D  EQU 106
r025  EQU 108
E  EQU 110
r026  EQU 112
r027  EQU 114
r028  EQU 116
r029  EQU 118
execute_Turing_machine  EQU 120
Machine  EQU 122
Cell  EQU 124
State  EQU 126
while_State_is_not_STOP  EQU 128
execute_Rule_for_Cell_at_State  EQU 130
exit_execute_Rule  EQU 132
Value  EQU 134
Rule  EQU 136
unknown_Rule  EQU 138
Error____unknown_Rule_or_missing_Move_for_State__Cell__Value  EQU 140
new_left_Cell  EQU 142
NewCell  EQU 144
new_right_Cell  EQU 146
print_Turing_tape  EQU 148
exit_print_tape  EQU 150
Skip_print  EQU 152
true  EQU 154
print_value_for_each_Cell  EQU 156

th_first_assignable EQU 158

.data?
align 16 ;segment alignment is 16
th_first_name  LABEL word
  ORG th_first_name+th_nameSize/th_valueInc*2
th_DEC_n  LABEL word
    ORG th_DEC_n+th_nameSize/th_valueInc*2
th_INC_n  LABEL word
    ORG th_INC_n+th_nameSize/th_valueInc*2
th_LIT_n  LABEL word
    ORG th_LIT_n+th_nameSize/th_valueInc*2
th_NAME_n  LABEL word
    ORG th_NAME_n+th_nameSize/th_valueInc*2
th_NEW_n  LABEL word
    ORG th_NEW_n+th_nameSize/th_valueInc*2
th_VAR_n  LABEL word
    ORG th_VAR_n+th_nameSize/th_valueInc*2
thid8714_n  LABEL word
    ORG thid8714_n+th_nameSize/th_valueInc*2
Three_Turing_machines_n  LABEL word
    ORG Three_Turing_machines_n+th_nameSize/th_valueInc*2
select_Machine_n  LABEL word
    ORG select_Machine_n+th_nameSize/th_valueInc*2
Turing_machine_n  LABEL word
    ORG Turing_machine_n+th_nameSize/th_valueInc*2
th_3_state_busy_beaver_n  LABEL word
    ORG th_3_state_busy_beaver_n+th_nameSize/th_valueInc*2
define_Turing_machines_n  LABEL word
    ORG define_Turing_machines_n+th_nameSize/th_valueInc*2
incrementer_n  LABEL word
    ORG incrementer_n+th_nameSize/th_valueInc*2
firstCell_n  LABEL word
    ORG firstCell_n+th_nameSize/th_valueInc*2
c001_n  LABEL word
    ORG c001_n+th_nameSize/th_valueInc*2
Cells_n  LABEL word
    ORG Cells_n+th_nameSize/th_valueInc*2
th_1_n  LABEL word
    ORG th_1_n+th_nameSize/th_valueInc*2
Right_n  LABEL word
    ORG Right_n+th_nameSize/th_valueInc*2
c002_n  LABEL word
    ORG c002_n+th_nameSize/th_valueInc*2
Left_n  LABEL word
    ORG Left_n+th_nameSize/th_valueInc*2
c003_n  LABEL word
    ORG c003_n+th_nameSize/th_valueInc*2
firstState_n  LABEL word
    ORG firstState_n+th_nameSize/th_valueInc*2
go_n  LABEL word
    ORG go_n+th_nameSize/th_valueInc*2
Blank_n  LABEL word
    ORG Blank_n+th_nameSize/th_valueInc*2
B_n  LABEL word
    ORG B_n+th_nameSize/th_valueInc*2
r001_n  LABEL word
    ORG r001_n+th_nameSize/th_valueInc*2
Write_n  LABEL word
    ORG Write_n+th_nameSize/th_valueInc*2
Move_n  LABEL word
    ORG Move_n+th_nameSize/th_valueInc*2
NextState_n  LABEL word
    ORG NextState_n+th_nameSize/th_valueInc*2
r002_n  LABEL word
    ORG r002_n+th_nameSize/th_valueInc*2
Stay_n  LABEL word
    ORG Stay_n+th_nameSize/th_valueInc*2
STOP_n  LABEL word
    ORG STOP_n+th_nameSize/th_valueInc*2
c010_n  LABEL word
    ORG c010_n+th_nameSize/th_valueInc*2
th_0_n  LABEL word
    ORG th_0_n+th_nameSize/th_valueInc*2
a_n  LABEL word
    ORG a_n+th_nameSize/th_valueInc*2
r010_n  LABEL word
    ORG r010_n+th_nameSize/th_valueInc*2
b_n  LABEL word
    ORG b_n+th_nameSize/th_valueInc*2
r011_n  LABEL word
    ORG r011_n+th_nameSize/th_valueInc*2
c__n  LABEL word
    ORG c__n+th_nameSize/th_valueInc*2
r012_n  LABEL word
    ORG r012_n+th_nameSize/th_valueInc*2
r013_n  LABEL word
    ORG r013_n+th_nameSize/th_valueInc*2
r014_n  LABEL word
    ORG r014_n+th_nameSize/th_valueInc*2
r015_n  LABEL word
    ORG r015_n+th_nameSize/th_valueInc*2
th_5_state_busy_beaver_n  LABEL word
    ORG th_5_state_busy_beaver_n+th_nameSize/th_valueInc*2
c020_n  LABEL word
    ORG c020_n+th_nameSize/th_valueInc*2
A_n  LABEL word
    ORG A_n+th_nameSize/th_valueInc*2
r020_n  LABEL word
    ORG r020_n+th_nameSize/th_valueInc*2
r021_n  LABEL word
    ORG r021_n+th_nameSize/th_valueInc*2
C__n  LABEL word
    ORG C__n+th_nameSize/th_valueInc*2
r022_n  LABEL word
    ORG r022_n+th_nameSize/th_valueInc*2
r023_n  LABEL word
    ORG r023_n+th_nameSize/th_valueInc*2
r024_n  LABEL word
    ORG r024_n+th_nameSize/th_valueInc*2
D_n  LABEL word
    ORG D_n+th_nameSize/th_valueInc*2
r025_n  LABEL word
    ORG r025_n+th_nameSize/th_valueInc*2
E_n  LABEL word
    ORG E_n+th_nameSize/th_valueInc*2
r026_n  LABEL word
    ORG r026_n+th_nameSize/th_valueInc*2
r027_n  LABEL word
    ORG r027_n+th_nameSize/th_valueInc*2
r028_n  LABEL word
    ORG r028_n+th_nameSize/th_valueInc*2
r029_n  LABEL word
    ORG r029_n+th_nameSize/th_valueInc*2
execute_Turing_machine_n  LABEL word
    ORG execute_Turing_machine_n+th_nameSize/th_valueInc*2
Machine_n  LABEL word
    ORG Machine_n+th_nameSize/th_valueInc*2
Cell_n  LABEL word
    ORG Cell_n+th_nameSize/th_valueInc*2
State_n  LABEL word
    ORG State_n+th_nameSize/th_valueInc*2
while_State_is_not_STOP_n  LABEL word
    ORG while_State_is_not_STOP_n+th_nameSize/th_valueInc*2
execute_Rule_for_Cell_at_State_n  LABEL word
    ORG execute_Rule_for_Cell_at_State_n+th_nameSize/th_valueInc*2
exit_execute_Rule_n  LABEL word
    ORG exit_execute_Rule_n+th_nameSize/th_valueInc*2
Value_n  LABEL word
    ORG Value_n+th_nameSize/th_valueInc*2
Rule_n  LABEL word
    ORG Rule_n+th_nameSize/th_valueInc*2
unknown_Rule_n  LABEL word
    ORG unknown_Rule_n+th_nameSize/th_valueInc*2
Error____unknown_Rule_or_missing_Move_for_State__Cell__Value_n  LABEL word
    ORG Error____unknown_Rule_or_missing_Move_for_State__Cell__Value_n+th_nameSize/th_valueInc*2
new_left_Cell_n  LABEL word
    ORG new_left_Cell_n+th_nameSize/th_valueInc*2
NewCell_n  LABEL word
    ORG NewCell_n+th_nameSize/th_valueInc*2
new_right_Cell_n  LABEL word
    ORG new_right_Cell_n+th_nameSize/th_valueInc*2
print_Turing_tape_n  LABEL word
    ORG print_Turing_tape_n+th_nameSize/th_valueInc*2
exit_print_tape_n  LABEL word
    ORG exit_print_tape_n+th_nameSize/th_valueInc*2
Skip_print_n  LABEL word
    ORG Skip_print_n+th_nameSize/th_valueInc*2
true_n  LABEL word
    ORG true_n+th_nameSize/th_valueInc*2
print_value_for_each_Cell_n  LABEL word
    ORG print_value_for_each_Cell_n+th_nameSize/th_valueInc*2

th_assigned_n LABEL WORD
    ORG  th_assigned_n+((th_nameSize-th_first_assignable)/th_valueInc)*th_nameSize/th_valueInc*2-2  ;allocate remaining th_nameSize names as th_unknownValue(0)
th_last_assigned_n LABEL WORD
    dw  1 dup(?)  ; force allocation of names without tripping the post-masm-6.11 performance bug for megabyte array allocation

;end of names

.data
align 16 ;segment alignment is 16
thid8714_lbl  LABEL dword
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    offset Three_Turing_machines@thid8714
  DD    offset select_Machine@thid8714
  DD    0
  DD    0
  DD    offset define_Turing_machines@thid8714
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    offset Right@thid8714
  DD    0
  DD    offset Left@thid8714
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    offset Stay@thid8714
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    0
  DD    offset execute_Turing_machine@thid8714
  DD    0
  DD    0
  DD    0
  DD    offset while_State_is_not_STOP@thid8714
  DD    offset execute_Rule_for_Cell_at_State@thid8714
  DD    offset exit_execute_Rule@thid8714
  DD    0
  DD    0
  DD    offset unknown_Rule@thid8714
  DD    0
  DD    offset new_left_Cell@thid8714
  DD    0
  DD    offset new_right_Cell@thid8714
  DD    offset print_Turing_tape@thid8714
  DD    offset exit_print_tape@thid8714
  DD    0
  DD    0
  DD    offset print_value_for_each_Cell@thid8714
th_assigned_lbl LABEL DWORD
  assigned_size EQU (th_nameSize-th_first_assignable)/th_valueInc*4  ;allocate remaining th_nameSize labels as th_unknownValue(0)
  ORG  th_assigned_lbl+assigned_size ; avoid post-masm-6.11 performance bug for megabyte array allocation
; end of thid8714_lbl

; Variables for NameAsmRun
th_next_assignable  DW th_first_assignable     ;next assignable value.  +2 for NoName
th_previous_clock_msec  DD 0  ;msec since process start
th_runs_remaining       DW 0
; Constants for NameAsmRun::fn@initialize
th_runs             DW 1
th_assigned_lbl2 DD th_assigned_lbl   ; for memfill in th@initialize
th_assigned_lbl_size  DD assigned_size    ;number of bytes to fill with 0
th_first_name2 DD th_first_name
th_names      DD th_names2
th_name_table DD 0       ; GlobalAlloc pointer set by fn@initialize, ltoc th_name_table, th_names
th_names2     LABEL byte ; NL(10) overwritten with '\0' by fn@initialize
  DB "NON",10
  DB "DEC",10
  DB "INC",10
  DB "LIT",10
  DB "NAME",10
  DB "NEW",10
  DB "VAR",10
  DB "thid-00080-th-0798-8714",10
  DB "Three Turing machines",10
  DB "select Machine",10
  DB "Turing machine",10
  DB "3-state busy beaver",10
  DB "define Turing machines",10
  DB "incrementer",10
  DB "firstCell",10
  DB "c001",10
  DB "Cells",10
  DB "1",10
  DB "Right",10
  DB "c002",10
  DB "Left",10
  DB "c003",10
  DB "firstState",10
  DB "go",10
  DB "Blank",10
  DB "B",10
  DB "r001",10
  DB "Write",10
  DB "Move",10
  DB "NextState",10
  DB "r002",10
  DB "Stay",10
  DB "STOP",10
  DB "c010",10
  DB "0",10
  DB "a",10
  DB "r010",10
  DB "b",10
  DB "r011",10
  DB "c",10
  DB "r012",10
  DB "r013",10
  DB "r014",10
  DB "r015",10
  DB "5-state busy beaver",10
  DB "c020",10
  DB "A",10
  DB "r020",10
  DB "r021",10
  DB "C",10
  DB "r022",10
  DB "r023",10
  DB "r024",10
  DB "D",10
  DB "r025",10
  DB "E",10
  DB "r026",10
  DB "r027",10
  DB "r028",10
  DB "r029",10
  DB "execute Turing machine",10
  DB "Machine",10
  DB "Cell",10
  DB "State",10
  DB "while State is not STOP",10
  DB "execute Rule for Cell at State",10
  DB "exit execute Rule",10
  DB "Value",10
  DB "Rule",10
  DB "unknown Rule",10
  DB "Error -- unknown Rule or missing Move for State, Cell, Value",10
  DB "new left Cell",10
  DB "NewCell",10
  DB "new right Cell",10
  DB "print Turing tape",10
  DB "exit print tape",10
  DB "Skip print",10
  DB "true",10
  DB "print value for each Cell",10
  DB 0
