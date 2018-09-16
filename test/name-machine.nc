core machine for th_NameCodeProgram ->
  Program= th_NameCodeProgram
  
  print Program->
    Instruction= Program
    print each Instruction->
        Instruction==th_STOP            ->exit print Program
        Instruction=, Instruction.op=, Instruction.nameA=, Instruction.fieldA=, Instruction.nameB=, Instruction.fieldB=, Instruction.nameC=, Instruction.fieldC=, Instruction.nameD=, Instruction.fieldD=
        Instruction= Instruction.next
        ->print each Instruction
    exit print Program->
  
  resolve labels in Program->
    Instruction= Program

    resolve each Instruction->
        Instruction==th_STOP            ->exit resolve labels
        Instruction.op==Op:label        ->resolve Op:label
        Instruction= Instruction.next
        ->resolve each Instruction

        resolve Op:label->
            Label= Instruction.fieldA
            Labels[Label]= Instruction
            resolve=, Label=, Labels[Label]=
            Instruction= Instruction.next
            ->resolve each Instruction

    exit resolve labels->
  
  execute Program->
    ============= =
    == execute == =
    ============= =
    Instruction= Program
    ReturnInstruction= th_STOP
    NewValue= v_0000
    
    execute each Instruction->
        Instruction==th_STOP            ->exit execute
        // Instruction=, //DEBUG
        ->Instruction.op
        Instruction.op=, Error: unknown Instruction op=
        !>

        Op:assignLiteral->
            Name= Instruction.nameA
            Field= Instruction.fieldA
            Literal= Instruction.fieldB
            Literal==th_NEW_value       ->assign new value
            Name[Field]= Literal
            ->execute next Instruction
            
            assign new value->
                NewValue= th_INC[NewValue]
                Name[Field]= NewValue
                ->execute next Instruction

        Op:assignValue->
            NameA= Instruction.nameA
            FieldA= Instruction.fieldA
            ValueA= NameA[FieldA]

            NameB= Instruction.nameB
            FieldB= Instruction.fieldB
            ValueB= NameB[FieldB]

            NameC= Instruction.nameC
            FieldC= Instruction.fieldC
            ValueC= NameC[FieldC]

            NameD= Instruction.nameD
            FieldD= Instruction.fieldD
            ValueD= NameD[FieldD]
            ValueC==th_INC              ->assign inc value

            ValueCD= ValueC[ValueD]
            // *A=, NameA=, FieldA=, = =_ ValueA=, *B=, NameB=, FieldB=, = =_ ValueB=, *C=, NameC=, FieldC=, = =_ ValueC=, *D=, NameD=, FieldD=, = =_ ValueD=, *CD=, ValueCD= //DEBUG
            ValueA[ValueB]= ValueCD            
            ->execute next Instruction
            
            assign inc value->
            NewValue= th_INC[ValueD]
            th_DEC[NewValue]= ValueD
            ValueA[ValueB]= NewValue            
            ->execute next Instruction

        Op:breakpoint->
            =!
            ->execute next Instruction

        Op:call->
            Error: Op:call is not implemented for core code -- =_
            Instruction.fieldA=
            //-> 
            // FIXUP temporary enhancement to core code for namec-test.nc.  Replace with equivalent core code.  Allow embedded calls.
            ReturnInstruction!=th_STOP  ->Error: embedded calls not implemented
            ReturnInstruction= Instruction.next
            ->Op:goto

        Op:goto->
            NameA= Instruction.nameA
            FieldA= Instruction.fieldA
            Label= NameA[FieldA]
            Labels[Label]==th_NEW   ->report unknown goto
            Instruction= Labels[Label]
            ->execute each Instruction

            report unknown goto->
                Instruction=, NameA=, FieldA=, Label=, Error: unknown label for Op::goto in core instructions=
                !>

        Op:ifEquals->
            NameA= Instruction.nameA
            FieldA= Instruction.fieldA
            ValueA= NameA[FieldA]

            NameB= Instruction.nameB
            FieldB= Instruction.fieldB
            ValueB= NameB[FieldB]

            NameC= Instruction.nameC
            FieldC= Instruction.fieldC
            ValueC= NameC[FieldC]

            ValueC==th_NEW_value        ->test equals th_NEW

            test equals->
            ValueA[ValueB]==ValueC      ->is equal
            ->execute next Instruction
            
            test equals th_NEW->
            ValueA[ValueB]==th_NEW      ->is equal
            ->execute next Instruction
            
            is equal->
                Label= Instruction.fieldD
                Labels[Label]==th_NEW   ->report unknown label
                Instruction= Labels[Label]
                ->execute each Instruction

            report unknown label->
                Instruction=, Label=, Error: unknown label for Op::ifEquals in core instructions=
                !>
                
        Op:label->
            Instruction.fieldA=
            ->execute next Instruction

        Op:print->
            NameA= Instruction.nameA
            FieldA= Instruction.fieldA
            ValueA= NameA[FieldA]

            NameB= Instruction.nameB
            FieldB= Instruction.fieldB
            ValueB= NameB[FieldB]

            ValueA[ValueB]==th_NEW     ->print new
            ValueA[ValueB]=_
            ->execute next Instruction
            
            print new->
                NEW=_
                ->execute next Instruction

        Op:printComma->
            ,=_
            ->execute next Instruction
            
        Op:printCommaSpace->
            , =_
            ->execute next Instruction
            
        Op:printSpace->
            =_
            ->execute next Instruction
            
        Op:printNewLine->
            =
            ->execute next Instruction

        Op:return->
            Instruction= ReturnInstruction
            ReturnInstruction= th_STOP
            ->execute each Instruction

    execute next Instruction->
        Instruction= Instruction.next
        ->execute each Instruction

    exit execute->
