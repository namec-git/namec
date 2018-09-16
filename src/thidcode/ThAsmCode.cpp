/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThAsmCode.cpp#18 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#include "thidcore/thesa.h"

#include "thidcode/ThAsmCode.h"

#include <QtCore/QTextStream>

namespace Thesa {

#//Class constants

    const QByteArray ThAsmCode::AsmType= "th-asm-type";

    //! Check ReservedAsmNames against masm documentation
    const QByteArray ThAsmCode::ReservedAsmNames=  // sorted with shorter names first and '\n' delimiters
        "\nabs"
        "\nalias"
        "\nalign"
        "\nand"
        "\nassume"
        "\nbyte"
        "\nc"
        "\ncall"
        "\ncatstr"
        "\ncomm"
        "\ncomment"
        "\ncontext"
        "\ndb"
        "\ndf"
        "\ndq"
        "\ndt"
        "\ndup"
        "\ndw"
        "\ndword"
        "\necho"
        "\nelse"
        "\nelseif"
        "\nelseif2"
        "\nend"
        "\nendif"
        "\nendm"
        "\nendp"
        "\nends"
        "\neq"
        "\nexitm"
        "\nextern"
        "\nexterndef"
        "\nextrn"
        "\nfor"
        "\nforc"
        "\nfword"
        "\nge"
        "\ngoto"
        "\ngroup"
        "\ngt"
        "\nhigh"
        "\nhigh32"
        "\nhighword"
        "\nif"
        "\nif2"
        "\nifb"
        "\nifdef"
        "\nifdif"
        "\nifdifi"
        "\nife"
        "\nifidn"
        "\nifidni"
        "\nifnb"
        "\nifndef"
        "\nimagerel"
        "\ninclude"
        "\nincludelib"
        "\ninstr"
        "\ninvoke"
        "\nirc"
        "\nirp"
        "\nlabel"
        "\nle"
        "\nlength"
        "\nlengthof"
        "\nlocal"
        "\nlow"
        "\nlow32"
        "\nlowword"
        "\nlroffset"
        "\nlt"
        "\nmacro"
        "\nmask"
        "\nmmword"
        "\nmod"
        "\nname"
        "\nne"
        "\nnop"
        "\nnot"
        "\noffset"
        "\nopattr"
        "\noption"
        "\nor"
        "\norg"
        "\noword"
        "\npage"
        "\npopcontext"
        "\nproc"
        "\nproto"
        "\nptr"
        "\npublic"
        "\npurge"
        "\npushcontext"
        "\nreal10"
        "\nreal4"
        "\nreal8"
        "\nrecord"
        "\nrepeat"
        "\nrept"
        "\nreturn"
        "\nsbyte"
        "\nsdword"
        "\nsectionrel"
        "\nseg"
        "\nshl"
        "\nshort"
        "\nshr"
        "\nsize"
        "\nsizeof"
        "\nsizstr"
        "\nsubstr"
        "\nsubtitle"
        "\nsword"
        "\ntbyte"
        "\ntest"
        "\nthis"
        "\ntitle"
        "\ntxtequ"
        "\ntype"
        "\ntypedef"
        "\nunion"
        "\nwhile"
        "\nwidth"
        "\nword"
        "\nxmmword"
        "\nxor"
        "\n"
        ;

    const QByteArray ThAsmCode::ProgramTag= "@";

#//!\name Constructors, etc.

ThAsmCode::
ThAsmCode(ThDisk *disk, QObject *parent/*= 0*/)
: ThCode(disk, parent)
, asm_comment()
, asm_arg_width(0)
, is_check(false)
, is_skip_comment(false)
, program_labels()
, write_text()
, write_text2()
{
}//constructor

#//!\name GetSet

//! Return UTF8 string for name in ASM format, with name suffix "_n"
QByteArray ThAsmCode::
toAsmName(const QByteArray &ba)
{
    QByteArray result= ba;
    if(!result.isEmpty()){
        result.append("_n");
    }
    return result;
}//toAsmName

//! Return UTF8 string for name in ASM format
//! Convert spaces to '_'
//! Convert each initial '_' to "__"
//! Disallow '_' adjacent to ' '
QByteArray ThAsmCode::
toAsmUtf8(NameIdentifier name)
{
    QByteArray ba(toUtf8(name));
    if(ba.contains("_ ") || ba.contains(" _")){
        throw ThError(12072, "ThAsmCode toAsmUtf8: names for AsmCode cannot contain adjacent '_' and ' ' characters -- '%s'", 0,0, ba);
    }
    if(ba.startsWith("n_ ")){
        throw ThError(12090, "ThAsmCode toAsmUtf8: names for AsmCode cannot start with the name prefix 'n_' -- '%s'", 0,0, ba);
    }
    if(ba.startsWith("thid-")){
        QByteArray suffix= ba.right(4);
        while(unique_thids.value(suffix)!=ba){
            if(!unique_thids.contains(suffix)){
                unique_thids.insert(suffix,ba);
                break;
            }else if(suffix.count()==4){
                suffix.append('a');
            }else{
                suffix[4]= suffix[4]+1;  //!<\todo handle more than 26 thid collisions
            }
        }
        ba= "thid" + suffix;
    }
    ba.replace(' ', '_');
    for(int i= 0; i<ba.size(); ++i){
        char c= ba[i];
        if(!isalnum(c) && c!='_' && c!='$' && c!='@'){  //!<\todo what other MASM characters are OK as labels?
            ba[i]= AsmOtherChar;
        }
    }
    if(!ba.isEmpty() && isdigit(ba[0])){
        ba.insert(0, "th_");
    }
        // same as ThCoreCode::toNameCodeUtf8
    int reserved= ReservedAsmNames.indexOf(ba.toLower());
    while(reserved>0){  // includes NoIndex and th_noName
        if(ReservedAsmNames.at(reserved-1)=='\n' && ReservedAsmNames.at(reserved+ba.size())=='\n'){
            ba.append('_');
            break;
        }
        reserved= ReservedAsmNames.indexOf(ba.toLower(), reserved+1);
    }
    return ba;
}//toAsmUtf8

//! Return UTF8 string for name in ASM format, with programLabels suffix "_lbl"
QByteArray ThAsmCode::
toAsmUtf8Labels(NameIdentifier name)
{
    QByteArray ba= toAsmUtf8(name);
    if(!ba.isEmpty()){
        ba.append("_lbl");
    }
    return ba;
}//toAsmUtf8Labels

//! Return UTF8 string for name in ASM format, with name suffix "_n"
QByteArray ThAsmCode::
toAsmUtf8Name(NameIdentifier name)
{
    QByteArray ba= toAsmUtf8(name);
    if(!ba.isEmpty()){
        ba.append("_n");
    }
    return ba;
}//toAsmUtf8Name

#//!\name Methods

//! Unpack OperandIdentifierList into the corresponding ArrayIndex N.N[N.N]
void ThAsmCode:: 
unpackAsArrayIndex(const OperandIdentifierList &operands, QByteArray *nameA, QByteArray *fieldA, QByteArray *nameB, QByteArray *fieldB)
{
    ArgType type= argTypeForOperands(operands);
    NameIdentifier name;
    NameIdentifier field;
    switch(type){
    case NoArg:
        nameA->clear();
        fieldA->clear();
        nameB->clear();
        fieldB->clear();
        break;
    case Literal:
        *nameA= th_LIT;
        *fieldA= th_LIT;
        *nameB= th_LIT;
        field= getNameField(operands.at(0), &name);
        *fieldB= toAsmUtf8(field);
        break;
    case Field:
        *nameA= th_LIT;
        *nameB= th_LIT;
        field= getNameField(operands.at(0), &name);
        *fieldA= toAsmUtf8(name);
        *fieldB= toAsmUtf8(field);
        break;
    case IndirectField:
        *nameA= th_LIT;
        field= getNameField(operands.at(0), &name);
        *fieldA= toAsmUtf8(field);
        field= getNameField(operands.at(1), &name);
        *nameB= toAsmUtf8(name);
        *fieldB= toAsmUtf8(field);
        break;
    case Array:
        field= getNameField(operands.at(0), &name);
        *nameA= toAsmUtf8(name);
        *fieldA= toAsmUtf8(field);
        field= getNameField(operands.at(1), &name);
        *nameB= th_LIT;
        *fieldB= toAsmUtf8(field);
        break;
    case ArrayIndex:
        field= getNameField(operands.at(0), &name);
        *nameA= toAsmUtf8(name);
        *fieldA= toAsmUtf8(field);
        field= getNameField(operands.at(1), &name);
        *nameB= toAsmUtf8(name);
        *fieldB= toAsmUtf8(field);
        break;
    default:
        throw ThError(12102, "ThAsmCode unpackAsArrayIndex: unknown argType 0x%X for operands of %d name-fields", type, operands.count());
    }
}//unpackAsArrayIndex

//!\see printProgram
void ThAsmCode::
writeAsmCode(int start, const QString &filename, const QString &filenameData)
{
    QFile file(filename);
    QFileInfo fileinfo(file);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){  //!<\todo Should QIODevice::Text also be set?
        throw ThError(12064, "ThAsmCode writeAsmCode: cannot open asmCode file '%s' for writing -- %s", 0,0, fileinfo.absoluteFilePath(), file.errorString());
    }
    write_text.setDevice(&file);
    write_text << "; written by 'namec --assemble'\n";
    write_text << ".486\n" << ".model flat, stdcall\n";
    write_text << "        include NameAsmRun.inc\n";
    write_text << "        extrn  fn@breakpoint:near, fn@err_goto_ax:near, fn@err_notKnown:near, fn@error:near, fn@initialize:near, fn@print:near, fn@printComma:near, fn@print_fields:near, fn@print_name:near\n";
    write_text << "        option oldstructs   ; allow name.field\n";
    write_text << "        option noscoped     ; otherwise OpLabel is not viewable by .data segment. Alternative is label:: instead of label:\n";
    write_text << "        option casemap:none ; case-sensitive\n";
    int basename= filenameData.lastIndexOf("/");
    QString outAsmData= filenameData.mid(basename+1);
    write_text << "        include " << outAsmData << "\n";

    write_text << ".code\n";
    write_text << "fn@main_base: ; th@initialize reports base address\n";
    write_text << "fn@main proc\n";

    clearProgramName();
    ThAsmCode::Instruction i(this, start);
    while(i.isValid()){
        writeAsmInstruction(i);
        i.nextInstruction();
    }
    writeAsmUnknownLabels(start);
    write_text << "fn@main endp\n";
    write_text << "END\n";
    write_text.flush();
    if(write_text.status()!=QTextStream::Ok){
        throw ThError(12066, "ThAsmCode writeAsmCode: failed to flush buffered output to '%s' -- %s", 0,0, fileinfo.absoluteFilePath(), file.errorString());
    }
    file.close();
    writeAsmData(filenameData);
}//writeAsmCode

//! Write ASM data for fields and names
//! Writes to write_text2 in case need to append additional data in the future
void ThAsmCode::
writeAsmData(const QString &filenameData)
{
    QFile file2(filenameData);
    QFileInfo fileinfo2(file2);
    if(!file2.open(QIODevice::WriteOnly|QIODevice::Truncate)){  //!<\todo Should QIODevice::Text also be set?
        throw ThError(12065, "ThAsmCode writeAsmData: cannot open asmCode data file '%s' for writing -- %s", 0,0, fileinfo2.absoluteFilePath(), file2.errorString());
    }
    write_text2.setDevice(&file2);

    write_text2 << ".data\n";
    write_text2 << "        public th_first_assignable\n";
    write_text2 << "\n";

    int nextValue= AsmValueInc;
    for(NameIdentifier name; name<nameCount(); name.next()){
        QByteArray asmField= toAsmUtf8(name);
        if(!asmField.isEmpty()){
            write_text2 << asmField << "  EQU " << nextValue << "\n";
            nextValue += AsmValueInc;
        }
    }
    int firstAssignable= nextValue;  // th_first_assignable
    write_text2 << "\n";
    write_text2 << "th_first_assignable EQU " << firstAssignable << "\n";
    write_text2 << "\n";
    write_text2 << ".data?\n";
    write_text2 << "align 16 ;segment alignment is 16\n";
    write_text2 << "th_first_name  LABEL word\n";
    write_text2 << "  ORG th_first_name+th_nameSize/th_valueInc*2\n";

    for(NameIdentifier name; name<nameCount(); name.next()){
        QByteArray asmName= toAsmUtf8Name(name);
        if(!asmName.isEmpty()){
            write_text2 << asmName << "  LABEL word\n";
            write_text2 << "    ORG " << asmName << "+th_nameSize/th_valueInc*2\n";
        }
    }
    write_text2 << "\n";
    write_text2 << "th_assigned_n LABEL WORD\n";
    write_text2 << "    ORG  th_assigned_n+((th_nameSize-th_first_assignable)/th_valueInc)*th_nameSize/th_valueInc*2-2  ;allocate remaining th_nameSize names as th_unknownValue(0)\n";
    write_text2 << "th_last_assigned_n LABEL WORD\n";
    write_text2 << "    dw  1 dup(?)  ; force allocation of names without tripping the post-masm-6.11 performance bug for megabyte array allocation\n";
    write_text2 << "\n";

    write_text2 << ";end of names\n";
    write_text2 << "\n";
    write_text2 << ".data\n";
    write_text2 << "align 16 ;segment alignment is 16\n";
    if(program_labels.count()!=1){
        throw ThError(12105, "ThAsmCode writeAsmData: expecting one program_labels.  Got %d program_labels.  More than 1 label not supported yet", program_labels.count());
    }
    foreach(NameIdentifier programName, program_labels){
        QHash<NameIdentifier, OperandIdentifier> labels;
        foreach(NameIdentifierPair programLabel, labelIndex().keys()){
            if(programLabel.first==programName){
                labels.insert(programLabel.second, labelIndex().value(programLabel));
            }
        }
        write_text2 << toAsmUtf8Labels(programName) << "  LABEL dword\n";
        QByteArray programThid= toAsmUtf8(programName);
        for(NameIdentifier field; field<nameCount(); field.next()){
            if(labels.contains(field)){
                write_text2 << "  DD    offset " << toAsmUtf8(field) << ProgramTag << programThid << "\n"; //!< Combine with writeAsmLabelOperand -- same string different stream
            }else{
                write_text2 << "  DD    0\n";
            }
        }
        //!\todo Need different names and 0-fill for multiple th_assigned_lbl
        write_text2 << "th_assigned_lbl LABEL DWORD\n";
        write_text2 << "  assigned_size EQU (th_nameSize-th_first_assignable)/th_valueInc*4  ;allocate remaining th_nameSize labels as th_unknownValue(0)\n";
        write_text2 << "  ORG  th_assigned_lbl+assigned_size ; avoid post-masm-6.11 performance bug for megabyte array allocation\n";
        write_text2 << "; end of " << toAsmUtf8Labels(programName) << "\n\n";
    }

    write_text2 << "; Variables for NameAsmRun\n";
    write_text2 << "th_next_assignable  DW th_first_assignable     ;next assignable value.  +2 for NoName\n";
    write_text2 << "th_previous_clock_msec  DD 0  ;msec since process start\n";
    write_text2 << "th_runs_remaining       DW 0\n";

    write_text2 << "; Constants for NameAsmRun::fn@initialize\n";
    write_text2 << "th_runs             DW 1\n";
    write_text2 << "th_assigned_lbl2 DD th_assigned_lbl   ; for memfill in th@initialize\n";
    write_text2 << "th_assigned_lbl_size  DD assigned_size    ;number of bytes to fill with 0\n";
    write_text2 << "th_first_name2 DD th_first_name\n";
    write_text2 << "th_names      DD th_names2\n";
    write_text2 << "th_name_table DD 0       ; GlobalAlloc pointer set by fn@initialize, ltoc th_name_table, th_names\n";
    write_text2 << "th_names2     LABEL byte ; NL(10) overwritten with '\\0' by fn@initialize\n";
    for(NameIdentifier name; name<nameCount(); name.next()){
        QByteArray asmName= toUtf8(name);
        if(asmName.startsWith("th_")){
            asmName.remove(0, 3);
        }
        asmName= asmName.replace('\'', '_').replace('"', '_'); // quote and double-quote invalid in MASM strings
        if(asmName.isEmpty()){
            write_text2 << "  DB \"NON\",10\n";
        }else{
            write_text2 << "  DB \"" << asmName << "\",10\n";
        }
    }

    write_text2 << "  DB 0\n";

    write_text2.flush();
    if(write_text2.status()!=QTextStream::Ok){
        throw ThError(12067, "ThAsmCode writeAsmData: failed to flush buffered output to '%s' -- %s", 0,0, fileinfo2.absoluteFilePath(), file2.errorString());
    }
    file2.close();
}//writeAsmData

//! Write end-of-line with optional asm_comment
//! No comment if is_skip_comment
//! Clears asm_comment and asm_arg_width and is_skip_comment
void ThAsmCode::
writeAsmEndLine()
{
    if(is_skip_comment){
        asm_comment.clear();
        is_skip_comment= false;
    }
    if(asm_comment.isEmpty()){
        write_text << "\n";
    }else{
        int spaceCount= AsmCommentTab - asm_arg_width;
        if(spaceCount<1){
            spaceCount= 1;
        }
        write_text << QString(spaceCount, ' ') << ";; " << asm_comment << "\n";
        asm_comment.clear();
    }
    asm_arg_width= 0;
}//writeAsmEndLine

//! Write assembler code to write_text
//! If is_optimize_literalName, ignores the initialization of 'i' (i.e., assignLiteral to the literal names)
//! if KnownName..., optimizes tests for assignable values.
//! if KnownName..., errors if assign to a notknown field or a known field of a notknown name, allows reading a notknown value
//! Handles equality tests of notknown names OK, but not identify tests v.A[v.B]==v.A[v.B]
//! non-const due to writeAsmCodeOperandList...  (getValue and next_unknown)
//! \see executeInstruction
void ThAsmCode::
writeAsmInstruction(const ThCode::Instruction &i)
{
    NameIdentifier name;
    NameIdentifier field;
    NameIdentifier nameB;
    NameIdentifier fieldB;
    int instructionOffset= i.offset();
    OperandIdentifierList operandsA= i.operandsA();
    OperandIdentifierList operandsB= i.operandsB();
    OperandIdentifierList operandsC= i.operandsC();
    ArgType argA= i.argTypeA();
    ArgType argB= i.argTypeB();
    ArgType argC= argTypeForOperands(operandsC);  //!<\todo replace with argTypeC() when redo Op for full labelOperand
    if(argC!=NoArg){
        field= getNameField(i.labelOperand(), &name);
        if(name==programName()){  //!<\todo Fix special-case of argC literal labels
            argC= Literal;
        }
    }

    QString iOffset= QString("i@%1:  ").arg(instructionOffset, 4, 10, QChar('0'));
    asm_arg_width= 0;
    if(!operandsB.isEmpty()){
        fieldB= getNameField(operandsB.first(), &nameB);
    }
    switch(i.op()){
    case AssignAB:
        asm_comment= toString(operandsA) % "= " % toString(operandsB);
        if(argB==Literal && fieldB==thNew()){       // ArgA= th_NEW
            write_text << "  call  fn@newAX ;>ax";
            asm_arg_width += 21;
            writeAsmEndLine();
            writeDestination("  mov   ", instructionOffset, argA, operandsA);
            write_text << ",ax\n";
        }else if(argB==Literal){                    // ArgA= fieldB
            writeDestination("  mov   ", instructionOffset, argA, operandsA);
            writeSource(",", instructionOffset, argB, operandsB);
            writeAsmEndLine();
        }else if(argB==Field && nameB==thInc()){    // ArgA= th_INC[fieldB]
            QByteArray fieldBstr= toAsmUtf8Name(fieldB);
            write_text << "  mov   ax," << fieldBstr;
            asm_arg_width += 11 + fieldBstr.size();
            writeAsmEndLine();
            write_text << "  call  fn@incAX ;>ax\n";
            writeDestination("  mov   ", instructionOffset, argA, operandsA);
            write_text << ",ax\n";
        }else if(argB==IndirectField && fieldB==thInc()){    // ArgA= th_INC[name.field]
            writeSource("  mov   ax,", instructionOffset, Field, OperandIdentifierList() << operandsB[1]);
            writeAsmEndLine();
            write_text << "  call  fn@incAX ;>ax\n";
            writeDestination("  mov   ", instructionOffset, argA, operandsA);
            write_text << ",ax\n";
        }else{                                      // ArgA= ArgB
            writeSource("  mov   ax,", instructionOffset, argB, operandsB);
            writeAsmEndLine();
            if(is_check && argA!=Field){  // Storing a th_NEW does not break the model.  Skip for fields only, otherwise diagnosis is difficult
                write_text << "  and   ax,ax\n";
                write_text << "  jz    fn@err_notKnown\n";
            }
            writeDestination("  mov   ", instructionOffset, argA, operandsA);
            write_text << ",ax\n";
        }
        break;
    case Breakpoint:
        asm_comment= "=!";
        write_text << "  call  fn@breakpoint";
        asm_arg_width += 21;
        writeAsmEndLine();
        break;
    case CallA:
        asm_comment= toString(operandsA);
        if(argA!=Literal){  // Always Literal ThidId.Title by appendCallCode
            throw ThError(12091, "ThAsmCode writeAsmInstruction: 'call' is only defined for literal names.  Use 'try' (... ->success) for indexed access.");
        }
        write_text << "  call  ";
        writeAsmLabelOperands(instructionOffset, argA, operandsA);
        writeAsmEndLine();
        break;
    case CatchB:
    case ThrowB:
    case TryA:
        throw ThError(12076, "ThAsmCode writeAsmInstruction: try, catch, and throw are not implemented.  Op 0x%X at instruction %d", i.op(), i.offset());
    case Fail:
        asm_comment= "!>";
        write_text << "  call  fn@error ; never returns";
        asm_arg_width += 32;
        writeAsmEndLine();
        break;
    case GotoA:
        field= getNameField(operandsA.first(), &name);
        if(operandsA.count()==1 && name==programName() && !isVariable(name, field)){  //!<\todo Review detection of program labels.  Should it be encoded in Op.  This convention also used for executeProgram.gotoA
            if(!program_labels.contains(programName())){
                program_labels.append(programName()); // table generated by writeAsmData
            }
            asm_comment= "->" % QString(toUtf8(field));
            write_text << "  jmp   ";
            asm_arg_width += 8;
            writeAsmLabelOperands(instructionOffset, argA, operandsA);  //!< \todo How to write labels
            writeAsmEndLine();
            break;
        }else{
            asm_comment= "->" % toString(operandsA);
            write_text << "  xor   ebx,ebx";
            asm_arg_width += 15;
            writeAsmEndLine();
            writeSource("  mov   bx,", instructionOffset, argA, operandsA);
            write_text << "\n  and   ebx,ebx ; always check\n";
            write_text << "  mov   edi,@F\n";
            write_text << "  jz    fn@err_notKnown\n";
            write_text << "@@:\n";
            write_text << "  mov   eax," << toAsmUtf8Labels(programName()) << "[ebx*2]\n";
            write_text << "  and   eax,eax ; fall through on unknown label\n";
            write_text << "  jz    @F\n";
            write_text << "  jmp   eax\n";
            write_text << "@@:\n";
        }
        break;
    case IfEqualsAB:
    case IfNotEqualsAB:
        asm_comment= toString(operandsA) % (i.op()==IfEqualsAB ? " == " :  " != ") % toString(operandsB) % " ->";
        field= getNameField(i.labelOperand(), &name);
        if(name==programName()){
            asm_comment += toUtf8(field);
        }else{
            asm_comment += toString(i.labelOperand());
        }
        if(argB==Literal){
            if(argA==Literal){
                writeSource("  mov   ax,", instructionOffset, argA, operandsA);
                writeAsmEndLine();
                if(fieldB==thNew()){
                    write_text << "  cmp   ax,th_new\n";
                }else{
                    writeSource("  cmp   ax,", instructionOffset, argB, operandsB);
                    write_text << "\n";
                }
            }else if(fieldB==thNew()){
                writeDestination("  cmp   ", instructionOffset, argA, operandsA);
                write_text << ",th_new" ;
                asm_arg_width += 7;
                writeAsmEndLine();
            }else{
                writeDestination("  cmp   ", instructionOffset, argA, operandsA);
                writeSource(",", instructionOffset, argB, operandsB);
                writeAsmEndLine();
            }
        }else if(argA==Literal){
            writeDestination("  cmp   ", instructionOffset, argB, operandsB);
            writeSource(",", instructionOffset, argA, operandsA);
            writeAsmEndLine();
        }else{
            writeSource("  mov   ax,", instructionOffset, argB, operandsB);
            writeAsmEndLine();
            if(is_check){
                write_text << "  and   ax,ax\n";
                write_text << "  jz    fn@err_notKnown\n";
            }
            writeDestination("  cmp   ", instructionOffset, argA, operandsA);
            write_text << ",ax\n";
        }
        if(argC==Literal){  //!< \todo GotoAB tests name==programName().  Which is correct?
            if(!program_labels.contains(programName())){
                program_labels.append(programName()); // table generated by writeAsmData
            }
            write_text << (i.op()==IfEqualsAB ? "  je    " : "  jne   ");
            writeAsmLabelOperand(instructionOffset, i.labelOperand());
            write_text << "\n";
        }else{
            //!\todo IfEquals/IfNotEquals only allows literal labels.  Disallow name.field?
            write_text << (i.op()==IfEqualsAB ? "  jne    " : "  je   ");
            write_text << "skip_" << ++next_label << "\n";
            write_text << "  xor   ebx,ebx\n";
            writeSource("  mov   bx,", instructionOffset, argC, operandsC);
            write_text << "\n  and   ebx,ebx ; always check\n";
            write_text << "  mov   edi,@F\n";
            write_text << "  jz    fn@err_notKnown\n";
            write_text << "@@:\n";
            write_text << "  mov   eax," << toAsmUtf8Labels(programName()) << "[ebx*2]\n";
            write_text << "  and   eax,eax\n";
            write_text << "  jz    fn@err_unknownLabel\n";
            write_text << "  jmp   eax\n";
            write_text << "@@:\n";
            write_text << "skip_" << next_label << ":\n";
        }
        break;
    case LabelA:
        {
            OperandIdentifier label= operandsA.first(); //!<\todo Add check.  Assumes 'Literal' with programName.field
            field= getNameField(label, &name);
            setProgramName(name);
            if(!program_labels.contains(name)){
                program_labels.append(name);
            }
            asm_comment=  QString(toUtf8(field)) % "->";
            write_text << iOffset;
            asm_arg_width += iOffset.size();
            writeAsmEndLine();
            writeAsmLabelOperand(instructionOffset, label);
            write_text << ":\n";
        }
        break;
    case NOP:
        asm_comment=  ".";
        write_text << iOffset;
        asm_arg_width += iOffset.size();
        writeAsmEndLine();
        break;
    case PrintA:
        {
            bool isSkipComment= false;
            Op next= i.nextOp();
            if(next==PrintComma || next==PrintCommaSpace){
                asm_comment= toString(operandsA) % "=,";
                isSkipComment= true;
            }else if(next==PrintSpace){
                asm_comment= toString(operandsA) % " =_";
                isSkipComment= true;
            }else if(next==PrintNewLine){
                asm_comment= toString(operandsA) % "=";
                isSkipComment= true;
            }else{
                asm_comment= toString(operandsA) % "=_";
            }
            writeSource("  mov   ax,", instructionOffset, argA, operandsA);
            writeAsmEndLine();
            write_text << "  call  fn@print\n";
            if(isSkipComment){
                is_skip_comment= true;
            }
        }
        break;
    case PrintComma:
        asm_comment= "=,";
        write_text << "  call  fn@printComma";
        asm_arg_width += 21;
        writeAsmEndLine();
        break;
    case PrintCommaSpace:
        asm_comment= "=, ";
        write_text << "  call  fn@printCommaSP";
        asm_arg_width += 23;
        writeAsmEndLine();
        break;
    case PrintNewLine:
        asm_comment= "=";
        write_text << "  call  fn@printNL";
        asm_arg_width += 18;
        writeAsmEndLine();
        break;
    case PrintSpace:
        asm_comment= "=_";
        write_text << "  call  fn@printSP";
        asm_arg_width += 18;
        writeAsmEndLine();
        break;
    case PrintFieldsA:
        asm_comment= toString(operandsA) % "=!";
        writeSource("  mov   ax,", instructionOffset, argA, operandsA);
        writeAsmEndLine();
        write_text << "  push  eax\n";
        write_text << "  call  fn@print\n";
        write_text << "  pop   eax\n";
        write_text << "  call  fn@print_fields\n";
        break;
    case Return:
        asm_comment= "->";
        write_text << "  ret   ";
        asm_arg_width += 8;
        writeAsmEndLine();
        break;
    default:
        throw ThError(12068, "ThAsmCode writeAsmInstruction: Unknown operator %d for instruction ip %d", i.op(), instructionOffset);
    }
}//writeAsmInstruction

void ThAsmCode::
writeAsmLabelOperand(int instructionOffset, OperandIdentifier labelOperand)
{
    NameIdentifier labelName;
    NameIdentifier labelField= getNameField(labelOperand, &labelName); //!<\todo IfUnless is programName().label while goto/call is implicit programName()
    if(labelName==NoName || labelField==NoName){
        throw ThError(12069, "ThAsmCode writeAsmLabelOperand: expecting labelOperator at instruction ip %d.  Either name/program %d or field %i is undefined", instructionOffset, labelName, qint64(labelField));
    }
    QString name= toAsmUtf8(labelName);
    QString field= toAsmUtf8(labelField);
    if(labelName==programName()){
        write_text << field << ProgramTag << name;  // label@thid
    }else{
        write_text << name << "." << field;
    }
    asm_arg_width += name.size()+field.size()+1;
}//writeAsmLabelOperand

void ThAsmCode::
writeAsmLabelOperands(int instructionOffset, ArgType arg, const OperandIdentifierList &operands)
{
    if(operands.count()==1){
        writeAsmLabelOperand(instructionOffset, operands[0]);
    }else{
        ThWarn(12073, "ThAsmCode writeAsmLabelOperands: indirect labels not valid for instruction at ip %d ArgType 0x%X.  Use gotoA or tryA", instructionOffset, arg);
    }
}//writeAsmLabelOperands

//! write Asm code for unknown labels
void ThAsmCode::
writeAsmUnknownLabels(int start)
{
    //!\todo similar code to determine program labels in writeAsmData
    if(program_labels.count()!=1){
        throw ThError(10466, "ThAsmCode writeAsmUnknownLabels: expecting one program_labels.  Got %d program_labels.  More than 1 label not supported yet", program_labels.count());
    }
    QSet<NameIdentifier> labels;
    foreach(NameIdentifier programName, program_labels){
        foreach(NameIdentifierPair programLabel, labelIndex().keys()){
            if(programLabel.first==programName){
                labels.insert(programLabel.second);
            }
        }
        // one instance by TH10466
        QByteArray programThid= toAsmUtf8(programName);
        ThAsmCode::Instruction i(this, start);
        while(i.isValid()){
            NameIdentifier program;
            NameIdentifier label;
            if(i.op()==IfEqualsAB || i.op()==IfNotEqualsAB){
                label= getNameField(i.labelOperand(), &program);
            }else if(i.op()==GotoA){
                if(i.argTypeA()==Literal){
                    NameIdentifier field= getNameField(i.operandsA().at(0), &program);
                    if(!isVariable(program, field)){ // not a Variable
                        label= field;
                    }
                }
            }
            if(label.isValid()){
                if(program==programName && !labels.contains(label)){
                    write_text << toAsmUtf8(label)  << ProgramTag << programThid <<  ":\n";
                    write_text << "  mov   ax," << toAsmUtf8(label) << "\n";
                    write_text << "  jmp   fn@err_unknownAX\n";
                    labels.insert(label);
                }
            }
            i.nextInstruction();
        }
    }
}//writeAsmUnknownLabels

//! Write OperandIdentifierList as a Destination
//! Preserve EAX
//! Reserve EDI for return addresses
//! May return ESI and EBX
void ThAsmCode::
writeDestination(const char *prefix, int instructionOffset, ArgType arg, const OperandIdentifierList &operands)
{
    QByteArray nameA,fieldA,nameB,fieldB;
    unpackAsArrayIndex(operands, &nameA, &fieldA, &nameB, &fieldB);
    switch(arg){
    case Literal:
        write_text << prefix << fieldB;
        asm_arg_width += int(strlen(prefix)) + fieldB.size();
        break;
    case Field:
        fieldA= toAsmName(fieldA);
        write_text << prefix << fieldA << "." << fieldB;
        asm_arg_width += int(strlen(prefix)) + fieldA.size() + fieldB.size() + 1;
        break;
    case IndirectField: // N[N.N]
        write_text << "  xor   ebx,ebx";
        asm_arg_width += 15;
        writeAsmEndLine();
        write_text << "  mov   bx," << toAsmName(nameB) << "." << fieldB << "\n";
        if(is_check){
            write_text << "  and   ebx,ebx\n";
            write_text << "  mov   edi,@F\n";
            write_text << "  jz    fn@err_notKnown\n";
            write_text << "@@:\n";
        }
        write_text << prefix << toAsmName(fieldA) << "[ebx]";
        break;
    case Array: // N.N[N]
        write_text << "  xor   ebx,ebx";
        asm_arg_width += 15;
        writeAsmEndLine();
        write_text << "  mov   bx," << toAsmName(nameA) << "." << fieldA << "\n";
        if(is_check){
            write_text << "  and   ebx,ebx\n";
            write_text << "  mov   edi,@F\n";
            write_text << "  jz    fn@err_notKnown\n";
            write_text << "@@:\n";
        }
        write_text << "  shl   ebx,th_valueBits-1 ;convert value to name\n";
        write_text << prefix << "th_first_name[ebx]." << fieldB;
        break;
    case ArrayIndex:  // N.N[N.N]
        write_text << "  xor   ebx,ebx";
        asm_arg_width += 15;
        writeAsmEndLine();
        write_text << "  mov   bx," << toAsmName(nameA) << "." << fieldA << "\n";
        if(is_check){
            write_text << "  and   ebx,ebx\n";
            write_text << "  mov   edi,@F\n";
            write_text << "  jz    fn@err_notKnown\n";
            write_text << "@@:\n";
        }
        write_text << "  shl   ebx,th_valueBits-1 ;convert value to name\n";
        write_text << "  xor   esi,esi\n";
        write_text << "  mov   si," << toAsmName(nameB) << "." << fieldB << "\n";
        if(is_check){
            write_text << "  and   esi,esi\n";
            write_text << "  jz    fn@err_notKnown\n";
        }
        write_text << prefix << "th_first_name[ebx+esi]";
        break;
    default:
        throw ThError(12103, "ThAsmCode writeDestination: unknown arg 0x%X at instruction ip %d", arg, instructionOffset);
    }
}//writeDestination

//! Write load of OperandIdentifierList
//! Reserve EDI for return addresses
//! Preserves EAX
//! If operands.count()==2, returns ECX.EDX instead of EBX.ESI
void ThAsmCode::
writeSource(const char *prefix, int instructionOffset, ArgType arg, const OperandIdentifierList &operands)
{
    QByteArray nameA,fieldA,nameB,fieldB;
    unpackAsArrayIndex(operands, &nameA, &fieldA, &nameB, &fieldB);
    switch(arg){
    case Literal:
        write_text << prefix << fieldB;
        asm_arg_width += int(strlen(prefix)) + fieldB.size();
        break;
    case Field:
        fieldA= toAsmName(fieldA);
        write_text << prefix << fieldA << "." << fieldB;
        asm_arg_width += int(strlen(prefix)) + fieldA.size() + fieldB.size() + 1;
        break;
    case IndirectField: // N[N.N]
        write_text << "  xor   edx,edx";
        asm_arg_width += 15;
        writeAsmEndLine();
        write_text << "  mov   dx," << toAsmName(nameB) << "." << fieldB << "\n";
        if(is_check){
            write_text << "  and   edx,edx\n";
            write_text << "  jz    fn@err_notKnown\n";
        }
        write_text << prefix << toAsmName(fieldA) << "[edx]";
        break;
    case Array: // N.N[N]
        write_text << "  xor   ecx,ecx";
        asm_arg_width += 15;
        writeAsmEndLine();
        write_text << "  mov   cx," << toAsmName(nameA) << "." << fieldA << "\n";
        if(is_check){
            write_text << "  and   ecx,ecx\n";
            write_text << "  jz    fn@err_notKnown\n";
        }
        write_text << "  shl   ecx,th_valueBits-1 ;convert value to name\n";
        write_text << prefix << "th_first_name[ecx]." << fieldB;
        break;
    case ArrayIndex:  // N.N[N.N]
        write_text << "  xor   ecx,ecx";
        asm_arg_width += 15;
        writeAsmEndLine();
        write_text << "  mov   cx," << toAsmName(nameA) << "." << fieldA << "\n";
        if(is_check){
            write_text << "  and   ecx,ecx\n";
            write_text << "  mov   edi,@F\n";
            write_text << "  jz    fn@err_notKnown\n";
            write_text << "@@:\n";
        }
        write_text << "  shl   ecx,th_valueBits-1 ;convert value to name\n";
        write_text << "  xor   edx,edx\n";
        write_text << "  mov   dx," << toAsmName(nameB) << "." << fieldB << "\n";
        if(is_check){
            write_text << "  and   edx,edx\n";
            write_text << "  jz    fn@err_notKnown\n";
        }
        write_text << prefix << "th_first_name[ecx+edx]";
        break;
    default:
        throw ThError(12104, "ThAsmCode writeSource: unknown arg 0x%X at instruction ip %d", arg, instructionOffset);
    }
}//writeSource

#//!\name Class methods

//! True if prefix indicates an assignable source value will be required
//! True if ends in ','
bool ThAsmCode::
isAssignableSource(const char *prefix)
{
    size_t n= int(strlen(prefix));
    if(n>0 && prefix[n-1]==','){
        return true;
    }
    return false;
}//isAssignableSource

//! True if prefix indicates an assignable source value to register AX
//! True if prefix == '  mov   ax,'
bool ThAsmCode::
isAssignableSourceMovAx(const char *prefix)
{
    if(strcmp(prefix, "  mov   ax,")==0){
        return true;
    }
    return false;
}//isAssignableSourceMovAx

}//namespace Thesa

