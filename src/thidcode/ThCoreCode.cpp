/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThCoreCode.cpp#11 $$Change: 2478 $
** $Date: 2018/09/14 $$Author: bbarber $
**
****************************************************************************/

#include "thidcore/thesa.h"

#include "thidcode/ThCoreCode.h"

#include <QtCore/QTextStream>

namespace Thesa {

#//!\name Class constants

    const char * ThCoreCode::CoreCodePrefix= "th_Core";
    const char * ThCoreCode::CoreCodeStop= "th_STOP";
    const char * ThCoreCode::LiteralSuffix= "-lit0000";
    const char * ThCoreCode::th_NameCodeProgram= "th_NameCodeProgram";
    const char * ThCoreCode::th_NEW_value= "th_NEW_value";

    const QByteArray ThCoreCode::OtherLiteralOperands=   // from 'writeArgPair.*th_LIT' unsorted with '\n' delimiters  
        "th_VAR\n"
        "th_TMP\n"
        "th_LIT";  // last name with a '/n/

    const QByteArray ThCoreCode::ReservedCompileNames=  // sorted with shorter names first and '\n' delimiters  
		"\nLabels"
		"\n"
		;

#//!\name Constructors, etc.

ThCoreCode::
ThCoreCode(ThDisk *disk, QObject *parent/*= 0*/)
: ThCode(disk, parent)
, next_arg(0)
, op_argA(0)
, write_text()
, reserved_tags()
, literal_operands()
{
}//constructor

#//!\name GetSet

//! Return UTF8 string for name in NameCode format
//! To avoid name clashes
//! ..Converts thids to thid-..._001 
//! ..Appends unique_NNN to names in ReservedCompileNames
//! If not a valid literal for assignLiteral, adds [[...]]
QByteArray ThCoreCode::
toCoreCodeUtf8(NameIdentifier name)
{
    QByteArray ba(toUtf8(name));
    if(ba.startsWith("thid-")){
        if(ba.endsWith("_001")){
            throw ThError(12056, "ThCoreCode toCoreCodeUtf8: more than one --compile is not implemented.  Name code already contains '%s'",0,0, ba);
        }
        ba.append("_001");
    }else if(ba.startsWith(CoreCodePrefix)){
        throw ThError(12126, "ThCoreCode toCoreCodeUtf8: more than one --compile is not implemented.  Name code already contains th_Core... '%s'", 0, 0, ba);
        //!\todo Report error if compile name-machine.nc with core code using th_NEW or th_INC.  It does not work.
    }else if(reserved_tags.contains(ba)){
	ba.append(QByteArray::number(reserved_tags.value(ba)));
    }else{
	// same as ThAsmCode::toAsmUtf8
	int reserved= ReservedCompileNames.indexOf(ba);
	while(reserved>0){
            if(ReservedCompileNames.at(reserved-1)=='\n' && ReservedCompileNames.at(reserved+ba.size())=='\n'){
                int reservedTag= FirstReserved;
                while(true){
                    QByteArray reserved(ba);
                    reserved.append(QByteArray::number(reservedTag));
                    NameIdentifier reservedName= getName(reserved);
                    if(reservedName.isEmpty()){
                        reserved_tags.insert(ba, reservedTag);
                        break;
                    }
                    ++reservedTag;
                }//while true
                ba.append(QByteArray::number(reserved_tags.value(ba)));
                break;
            }
            reserved= ReservedCompileNames.indexOf(ba, reserved+1);
	}//while ReservedCompileNames
    }
    if(!ThCoreCode::isValidLiteral(ba)){
        ba= "[[" + ba + "]]";
    }
    return ba;
}//toCoreCodeUtf8

#//!\name Methods

//! Collect literal operands and literal labels
//! Results are literal_operands and literal_label_operands 
void ThCoreCode::
collectLiteralOperands(int start)
{
    foreach(const QByteArray &s, ThCoreCode::OtherLiteralOperands.split('\n')){
        literal_operands.insert(s);
    }
    clearProgramName();
    ThCoreCode::Instruction i(this, start);
    NameIdentifier program;
    NameIdentifier name;
    NameIdentifier field;
    QString nameA,fieldA,nameB,fieldB,nameC,fieldC,nameD,fieldD;
    while(i.isValid()){
        if(i.op()==LabelA){
            field= getNameField(i.operandsA()[0], &program);
            setProgramName(program);
        }else if(i.op()==AssignAB && i.argTypeA()==Field && i.argTypeB()==Literal){
            // do nothing
        }else{
            unpackAsArrayIndex(i.operandsA(), &nameA, &fieldA, &nameB, &fieldB);
            unpackAsArrayIndex(i.operandsB(), &nameC, &fieldC, &nameD, &fieldD);
            if(nameA==ThCode::th_LIT){
                literal_operands.insert(fieldA);
            }
            if(nameB==ThCode::th_LIT){
                literal_operands.insert(fieldB);
            }
            if(nameC==ThCode::th_LIT){
                literal_operands.insert(fieldC);
            }
            if(nameD==ThCode::th_LIT){
                literal_operands.insert(fieldD);
            }
            if(i.op()==GotoA || i.op()==CallA){
                if(i.argTypeA()==Literal){
                    field= getNameField(i.operandsA()[0], &name);
                    literal_label_operands.insert(QPair<NameIdentifier, NameIdentifier>(name, field));
                }
                //!\todo Support non-literal Goto and Call.  Are all labels needed?
            }else if(i.op()==IfEqualsAB || i.op()==IfNotEqualsAB){
                field= getNameField(i.labelOperand(), &name);
                if(name==programName()){
                    literal_label_operands.insert(QPair<NameIdentifier, NameIdentifier>(name, field));
                }
            }
        }
        i.nextInstruction();
    }
}//collectLiteralOperands

void ThCoreCode::
errorNotCoreArgument(int instructionOffset, ArgType arg) const
{
    //!\todo convert instructions into core format
    throw ThError(12038, "ThCoreCode errorNotCoreArgument: unexpected argumentType 0x%X at instruction %d", arg, instructionOffset);
}//errorNotCoreArgument

//! Unpack OperandIdentifierList into the corresponding ArrayIndex N.N[N.N]
void ThCoreCode:: 
unpackAsArrayIndex(const OperandIdentifierList &operands, QString *nameA, QString *fieldA, QString *nameB, QString *fieldB)
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
        *fieldB= toCoreCodeUtf8(field);
        break;
    case Field:
        *nameA= th_LIT;
        *nameB= th_LIT;
        field= getNameField(operands.at(0), &name);
        *fieldA= toCoreCodeUtf8(name);
        *fieldB= toCoreCodeUtf8(field);
        break;
    case IndirectField:
        *nameA= th_LIT;
        field= getNameField(operands.at(0), &name);
        *fieldA= toCoreCodeUtf8(field);
        field= getNameField(operands.at(1), &name);
        *nameB= toCoreCodeUtf8(name);
        *fieldB= toCoreCodeUtf8(field);
        break;
    case Array:
        field= getNameField(operands.at(0), &name);
        *nameA= toCoreCodeUtf8(name);
        *fieldA= toCoreCodeUtf8(field);
        field= getNameField(operands.at(1), &name);
        *nameB= th_LIT;
        *fieldB= toCoreCodeUtf8(field);
       break;
    case ArrayIndex:
        field= getNameField(operands.at(0), &name);
        *nameA= toCoreCodeUtf8(name);
        *fieldA= toCoreCodeUtf8(field);
        field= getNameField(operands.at(1), &name);
        *nameB= toCoreCodeUtf8(name);
        *fieldB= toCoreCodeUtf8(field);
        break;
    default:
        throw ThError(12113, "ThCoreCode unpackAsArrayIndex: unknown argType 0x%X for operands of %d name-fields", type, operands.count());
    }
}//unpackAsArrayIndex

//! Write name and field for a pair of arguments
//! If "", skips the second argument's name and/or field
//! Record th_LIT.literal fields in literal_operands
void ThCoreCode:: 
writeArgPair(int instructionOffset, const QString &instructionSuffix, QChar letter, const QString &nameA, const QString &fieldA, const QString &nameB, const QString &fieldB)
{
    write_text << "  " << CoreCodePrefix << instructionOffset << instructionSuffix << ".name" << letter << "= " << nameA << "\n";
    write_text << "  " << CoreCodePrefix << instructionOffset << instructionSuffix << ".field" << letter << "= " << fieldA << "\n";
    QChar nextLetter(letter.unicode()+1);
    if(!nameB.isEmpty()){
        write_text << "  " << CoreCodePrefix << instructionOffset << instructionSuffix << ".name" << nextLetter << "= " << nameB << "\n";
    }
    if(!fieldB.isEmpty()){
        write_text << "  " << CoreCodePrefix << instructionOffset << instructionSuffix << ".field" << nextLetter << "= " << fieldB << "\n";
    }
}//writeArgPair

//!\see ThCoreCode::printProgram
void ThCoreCode:: 
writeCoreCode(int start, const QString &filename, const QString &extraCode)
{
    QFile extraCodeFile;
    if(!extraCode.isEmpty()){
        extraCodeFile.setFileName(extraCode);
        QFileInfo extraCodeFileinfo(extraCodeFile);
        if(!extraCodeFile.open(QIODevice::ReadOnly)){  //!<\todo Should QIODevice::Text also be set?  
            throw ThError(12055, "ThCoreCode writeCoreCode: cannot open extra nameCode file '%s' for reading -- %s", 0,0, extraCodeFileinfo.absoluteFilePath(), extraCodeFile.errorString());
        }
    }
    QFile file(filename);
    QFileInfo fileinfo(file);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){  //!<\todo Should QIODevice::Text also be set?  
        throw ThError(12040, "ThCoreCode writeCoreCode: cannot open nameCode file '%s' for writing -- %s", 0,0, fileinfo.absoluteFilePath(), file.errorString());
    }
    write_text.setDevice(&file);
    QString programLabel(filename);
    int ext= programLabel.lastIndexOf('.');
    if(ext!=NoIndex){
        programLabel.truncate(ext);
    }
    int base= programLabel.lastIndexOf('/');
    if(base!=NoIndex){
        programLabel= programLabel.mid(base+1);
    }
    write_text << programLabel << "->\n\n";
    write_text << "  //===================\n";
    write_text << "  //== Literals and Labels\n";
    write_text << "  //===================\n";
    collectLiteralOperands(start);
    writeLiteralCoreInstructions(start);
    write_text << "  //===================\n";
    write_text << "  //== Core code\n";
    write_text << "  //===================\n";
    clearProgramName();
    ThCoreCode::Instruction i(this, start);
    while(i.isValid()){
        writeCoreInstruction(i);
        i.nextInstruction();
    }
    write_text << "\n  //===================\n";
    write_text << "  //== Name code starts here\n";
    write_text << "  //===================\n";
    QByteArray startSuffix= (literal_operands.isEmpty() && literal_label_operands.isEmpty() ? "" : LiteralSuffix);
    write_text << "  th_LIT." << th_NEW_value << "= " << th_NEW_value << "  // if as name code, would allocate a new value\n";
    write_text << "  " << th_NameCodeProgram << "= " << CoreCodePrefix << start << startSuffix << "\n";
    if(!extraCode.isEmpty()){
        QByteArray extra= extraCodeFile.readAll();
        write_text << extra;
    }
    write_text.flush();
    if(write_text.status()!=QTextStream::Ok){
        throw ThError(12041, "ThCoreCode writeCoreCode: failed to flush buffered output to '%s' -- %s", 0,0, fileinfo.absoluteFilePath(), file.errorString());
    }
    file.close();
}//writeCoreCode

//! Write name code to write_text
//! non-const due to writeCoreCodeOperandList...  (getValue and next_unknown)
void ThCoreCode::
writeCoreInstruction(const ThCode::Instruction &i)
{
    int instructionOffset= i.offset();
    ArgType argA= i.argTypeA();
    OperandIdentifierList operandsA= i.operandsA();
    ArgType argB= i.argTypeB();
    OperandIdentifierList operandsB= i.operandsB();
    OperandIdentifierList operandsC= i.operandsC();
    ArgType argC= argTypeForOperands(operandsC);  //!<\todo Should labelOperand allow Array,etc?  Could use OpArgB or switch to 2-bit OpArg mask
    QString nameA,fieldA,nameB,fieldB,nameC,fieldC,nameD,fieldD,nameE,fieldE,nameF,fieldF;
    unpackAsArrayIndex(operandsA, &nameA, &fieldA, &nameB, &fieldB);
    unpackAsArrayIndex(operandsB, &nameC, &fieldC, &nameD, &fieldD);
    unpackAsArrayIndex(operandsC, &nameE, &fieldE, &nameF, &fieldF);

    QString suffix;
    QString nextTemp= th_TMP;
    switch(i.op()){
    case AssignAB:
        if(argB==Literal && fieldD==th_NEW){
            fieldD= th_NEW_value;
        }
        if(argA==Field && argB==Literal){
            writeOperation(instructionOffset, suffix, "Op:assignLiteral");
            writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, "", fieldD);
        }else if(argB==Literal && fieldD==th_NEW_value){  
            // Core code does not allow assignValue of th_NEW.  Convert to assignLiteral of th_NEW 
            writeOperation(instructionOffset, suffix, "Op:assignLiteral");
            writeArgPair(instructionOffset, suffix, 'A', th_VAR, nextTemp, "", th_NEW_value);
            fieldA= th_VAR;
            fieldB= nextTemp;  // last use of nextTemp
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', nameA, fieldA, nameB, fieldB);
            writeArgPair(instructionOffset, suffix, 'C', th_LIT, th_VAR, th_LIT, nextTemp);
        }else{
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', nameA, fieldA, nameB, fieldB);
            writeArgPair(instructionOffset, suffix, 'C', nameC, fieldC, nameD, fieldD);
        }
        break;
    case Breakpoint:
        writeOperation(instructionOffset, suffix, "Op:breakpoint");
        break;
    case CallA:
        //!\todo Implement CallA as core code
        ThWarn(12120, "ThCoreCode writeCoreInstruction: Op:call compiled as a warning.  Instruction %d", i.offset());
        writeOperation(instructionOffset, suffix, "Op:call");
        writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, "", "");
        break;
    case CatchB:
        //!\todo Implement CatchB as core code
        throw ThError(12121, "ThCoreCode writeCoreInstruction: Op:catch not implemented for --compile.  Instruction %d", i.offset());
        writeOperation(instructionOffset, suffix, "Op:catch");
        writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, "", "");
        break;
    case Fail:
        //!\todo Implement Fail as core code
        ThWarn(12122, "ThCoreCode writeCoreInstruction: Op:fail compiled as unknown-Op.  Instruction %d", i.offset());
        writeOperation(instructionOffset, suffix, "Op:fail");
        break;
    case GotoA:
        if(argA!=Field && argA!=Literal){
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', th_LIT, th_VAR, th_LIT, nextTemp);
            writeArgPair(instructionOffset, suffix, 'C', nameA, fieldA, nameB, fieldB);
            fieldA= th_VAR;
            fieldB= nextTemp;  // last use of nextTemp
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
        }
        writeOperation(instructionOffset, suffix, "Op:goto");
        writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, "", "");
        break;
    case IfEqualsAB:
    case IfNotEqualsAB:
        if(argB==Literal && fieldD==th_NEW){
            fieldD= th_NEW_value;
        }
        if(argB!=Literal){
            if(nameC==th_NEW){
                nameC= th_NEW_value;
            }
            if(fieldC==th_NEW){
                fieldC= th_NEW_value;
            }
            if(nameD==th_NEW){
                nameD= th_NEW_value;
            }
            if(fieldD==th_NEW){
                fieldD= th_NEW_value;
            }
        }
        if(argB!=Field && argB!=Literal){
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', th_LIT, th_VAR, th_LIT, nextTemp);
            writeArgPair(instructionOffset, suffix, 'C', nameC, fieldC, nameD, fieldD);
            fieldC= th_VAR;
            fieldD= nextTemp;
            nextTemp= ThCode::nextName(nextTemp);
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
        }
        if(argC!=Field && argC!=Literal){
            //!\todo IfEquals/IfNotEquals only allows literal labels.  Disallow name.field and other options?
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', th_LIT, th_VAR, th_LIT, nextTemp);
            writeArgPair(instructionOffset, suffix, 'C', nameE, fieldE, nameF, fieldF);
            fieldE= th_VAR;
            fieldF= nextTemp; // last use of nextTemp
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
        }
        if(i.op()==IfNotEqualsAB){
            writeOperation(instructionOffset, suffix, "Op:ifEquals");
            writeArgPair(instructionOffset, suffix, 'A', nameA, fieldA, nameB, fieldB);
            writeArgPair(instructionOffset, suffix, 'C', fieldC, fieldD, toUtf8(programName()), nextLabel());
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
            writeOperation(instructionOffset, suffix, "Op:goto");
            writeArgPair(instructionOffset, suffix, 'A', fieldE, fieldF, "", "");
            nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
            writeOperation(instructionOffset, suffix, "Op:label");
            writeArgPair(instructionOffset, suffix, 'A', toUtf8(programName()), nextLabel(), "", "");
            newNextLabel();
        }else{
            writeOperation(instructionOffset, suffix, "Op:ifEquals");
            writeArgPair(instructionOffset, suffix, 'A', nameA, fieldA, nameB, fieldB);
            writeArgPair(instructionOffset, suffix, 'C', fieldC, fieldD, fieldE, fieldF);
        }
        break;
    case LabelA:
        setProgramName(nameA);
        writeOperation(instructionOffset, suffix, "Op:label");
        writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, "", "");
        break;
    case NOP:
        writeOperation(instructionOffset, suffix, "Op:assignLiteral");
        writeArgPair(instructionOffset, suffix, 'A', th_LIT, "nop", "", "nop");
        break;
    case PrintA:
        writeOperation(instructionOffset, suffix, "Op:print");
        writeArgPair(instructionOffset, suffix, 'A', nameA, fieldA, nameB, fieldB);
        break;
    case PrintComma:
        writeOperation(instructionOffset, suffix, "Op:printComma");
        break;
    case PrintCommaSpace:
        writeOperation(instructionOffset, suffix, "Op:printCommaSpace");
        break;
    case PrintFieldsA:
        writeOperation(instructionOffset, suffix, "Op:printFields");
        writeArgPair(instructionOffset, suffix, 'A', nameA, fieldA, nameB, fieldB);
        break;
    case PrintNewLine:
        writeOperation(instructionOffset, suffix, "Op:printNewLine");
        break;
    case PrintSpace:
        writeOperation(instructionOffset, suffix, "Op:printSpace");
        break;
    case Return:
        //!\todo Implement Return as core code
        writeOperation(instructionOffset, suffix, "Op:return");
        // writeOperation(instructionOffset, suffix, "Op:assignLiteral");
        // writeArgPair(instructionOffset, suffix, 'A', th_LIT, "return", "", "return");
        break;
    case ThrowB:
        //!\todo Implement ThrowB as core code
        throw ThError(12124, "ThCoreCode writeCoreInstruction: Op:throw not implemented for --compile.  Instruction %d", i.offset());
        writeOperation(instructionOffset, suffix, "Op:throw");  
        writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, "", "");
        break;
    case TryA:
        //!\todo Implement TryA as core code
        throw ThError(12125, "ThCoreCode writeCoreInstruction: Op:try not implemented for --compile.  Instruction %d", i.offset());
        if(argA!=Field && argA!=Literal){
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', th_LIT, th_VAR, th_LIT, nextTemp);
            writeArgPair(instructionOffset, suffix, 'C', nameA, fieldA, nameB, fieldB);
            fieldA= th_VAR;
            fieldB= nextTemp;
            nextTemp= ThCode::nextName(nextTemp);
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
        }
        if(argB!=Field && argB!=Literal){
            writeOperation(instructionOffset, suffix, "Op:assignValue");
            writeArgPair(instructionOffset, suffix, 'A', th_LIT, th_VAR, th_LIT, nextTemp);
            writeArgPair(instructionOffset, suffix, 'C', nameC, fieldC, nameD, fieldD);
            fieldC= th_VAR;
            fieldD= nextTemp; // last use of nextTemp
            QString nextSuffix= ThCode::nextName(suffix);
            writeNext(instructionOffset, suffix, instructionOffset, nextSuffix);
            suffix= nextSuffix;
        }
        writeOperation(instructionOffset, suffix, "Op:try");
        writeArgPair(instructionOffset, suffix, 'A', fieldA, fieldB, fieldC, fieldD);
        break;
    default:
        throw ThError(12046, "ThCoreCode writeCoreInstruction: Unknown operator %d for instruction ip %d", i.op(), i.offset());
    }
    if(i.isLastInstruction()){
        writeNext(instructionOffset, suffix, NoIndex, CoreCodeStop);
    }else{
        writeNext(instructionOffset, suffix, i.nextOffset(), "");
    }
}//writeCoreInstruction

//! Write 'next' field for core instruction.  Ignores nextOffset if NoIndex
void ThCoreCode::
writeNext(int instructionOffset, const QString &instructionSuffix, int nextOffset, const QString &nextSuffix)
{
    write_text << "  " << CoreCodePrefix << instructionOffset << instructionSuffix << ".next= ";
    if(nextOffset==NoIndex){
        write_text << nextSuffix << "\n";
    }else{
        write_text << CoreCodePrefix << nextOffset << nextSuffix << "\n";
    }
}//writeNext

void ThCoreCode::
writeOperation(int instructionOffset, const QString &instructionSuffix, const QString &operation)
{
    write_text << "  " << CoreCodePrefix << instructionOffset << instructionSuffix << ".op= " << operation << "\n";
}//writeOperation

//! write Op:assignLiteral instructions for literals and labels
//! \todo Should th_LIT be built into the core engine or implemented via th_LIT.x= x as defined?
//! \todo Should literal labels (->label) include the program name (thid-...)?   ->A.x must be a literal.  Currently setting like th_LIT which is odd
void ThCoreCode::
writeLiteralCoreInstructions(int start)
{
    QString suffix= ThCoreCode::LiteralSuffix;
    int n= literal_operands.count()+literal_label_operands.count();
    int i= 0;
    foreach(const QString &lit, literal_operands){
        ++i;
        if(lit!=ThCode::th_NEW && lit!=ThCoreCode::th_NEW_value){
            writeOperation(start, suffix, "Op:assignLiteral");
            writeArgPair(start, suffix, 'A', ThCode::th_LIT, lit, "", lit);
            QString nextSuffix= (i==n ? "" : ThCode::nextName(suffix));
            writeNext(start, suffix, start, nextSuffix);
            suffix= nextSuffix;
        }
    }
    QSet<QPair<NameIdentifier, NameIdentifier>>::const_iterator nameField;
    for (nameField= literal_label_operands.begin(); nameField != literal_label_operands.end(); ++nameField){
        QByteArray program= toCoreCodeUtf8((*nameField).first);
        QByteArray lit= toCoreCodeUtf8((*nameField).second);
        writeOperation(start, suffix, "Op:assignLiteral");
        writeArgPair(start, suffix, 'A', program, lit, "", lit);
        QString nextSuffix= (++i==n ? "" : ThCode::nextName(suffix));
        writeNext(start, suffix, start, nextSuffix);
        suffix= nextSuffix;
    }
}//writeLiteralCoreInstructions

#//!\name Class methods


//! True if valid literal
//! False if extra spaces, embedded ., trailing [...], [[...]], trailing '=' '=,' '=_' '=!'
//! False if '->' (invalid as 'a.b= literal', would be valid as 'a.b==literal ->label')
bool ThCoreCode::
isValidLiteral(const QByteArray &literal)
{
    if(literal.startsWith(' ') || literal.endsWith(' ') || literal.contains("  ")){
        return false;
    }
    int startDot= (literal.startsWith('.') ? 1 : 0);
    int firstDot= literal.indexOf('.', startDot);
    if(firstDot!=NoIndex && firstDot!=literal.count()-1){
        return false;
    }
    if(literal.endsWith(']')){
        int startBrace= (literal.startsWith('[') ? 1 : 0);
        if(literal.indexOf('[', startBrace)!=NoIndex){
            return false;
        }
    }
    int startLiteral= literal.indexOf("[[");
    if(startLiteral!=NoIndex && startLiteral<literal.lastIndexOf("]]")){
        return false;
    }
    if(literal.endsWith("=") || literal.endsWith("=,") || literal.endsWith("=_") || literal.endsWith("=!")){
        return false;
    }
    if(literal.contains("->")){
        return false;
    }
    return true;
}//isValidLiteral

}//namespace Thesa

