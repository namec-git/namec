/****************************************************************************
**
** Copyright (C) 2011-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThCode.cpp#87 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#include "thidcore/thesa.h"

#include "thidcode/ThCode.h"

#include "road/RoadEventStack.h" // logStream()

#ifndef TH_NO_THDISK
#include "thidcore/ThDepot.h"
#include "thidcore/ThDisk.h"
#include "thidcore/Thid.h"
#include "thidcore/ThidData.h"
#include "thidgui/ThTextDocument.h"

#include <QtCore/QBitArray>
#include <QtCore/QChar>
#include <QtCore/QTextStream>
#include <QtGui/QTextBlock>
#include <QtGui/QTextBlockFormat>
#include <QtGui/QTextCursor>
#endif

namespace Thesa {

#//Class constants
    const QByteArray ThCode::th_INC= "th_INC";
    const QByteArray ThCode::th_LIT= "th_LIT";
    const QByteArray ThCode::th_NAME= "th_NAME";
    const QByteArray ThCode::th_NEW= "th_NEW";
    const QByteArray ThCode::th_VAR= "th_VAR";
    const QByteArray ThCode::th_RESERVED_PREFIX= "th_";
    const QByteArray ThCode::th_UNIQUE_LABEL= "th_LBL0000";
    const QByteArray ThCode::th_TMP= "th_TMP";
    const QByteArray ThCode::th_NEXT_NEW= "-1";
    const QString ThCode::NonInstructionCharactersRE= "^$";  //!\todo define NonInstructionChars 

const int ThCode:: 
s_operands_for_arg[8]= {  // count of arguments for ThCode::ArgType
    0, //NoArg
    0, //NoArg2
    1, //Literal
    1, //Field
    2, //IndirectField
    2, //Array
    2, //ArrayIndex
    0, //unused-arg
};

#//!\name ThCode::Instruction

ThCode::Instruction::
Instruction(ThCode *code) 
: th_code(code)
, current_offset(0)
, next_offset(code->programCodeSize())
, operands_A()
, operands_B()
, label_operand()
, code_op(NOP)
, argType_A(NoArg)
, argType_B(NoArg)
{
    getInstruction(code->programCode());
}//Instruction

ThCode::Instruction::
Instruction(ThCode *code, int program) 
: th_code(code)
, current_offset(program)
, next_offset(code->programCodeSize())
, operands_A()
, operands_B()
, label_operand()
, code_op(NOP)
, argType_A(NoArg)
, argType_B(NoArg)
{
    getInstruction(code->programCode()+program);
}//Instruction

ThCode::Instruction::
Instruction(const Instruction &other) 
: th_code(other.th_code)
, current_offset(other.current_offset)
, next_offset(other.next_offset)
, operands_A(other.operands_A)
, operands_B(other.operands_B)
, label_operand(other.label_operand)
, code_op(other.code_op)
, argType_A(other.argType_A)
, argType_B(other.argType_B)
{
}//Instruction

ThCode::Instruction & ThCode::Instruction::
operator=(const ThCode::Instruction &other) 
{
    th_code= other.th_code;
    current_offset= other.current_offset;
    next_offset= other.next_offset;
    operands_A= other.operands_A;
    operands_B= other.operands_B;
    label_operand= other.label_operand;
    code_op= other.code_op;
    argType_A= other.argType_A;
    argType_B= other.argType_B;
    return *this;
}//operator=

ThCode::Op ThCode::Instruction::
nextOp() const
{
    Op result; 
    if(isLastInstruction()){
        result= Return; // implicit return at end of program
    }else{
        const char *ip= th_code->programCode()+next_offset;
        checkMore(ip);
        char c= *ip;
        result= ThCode::toOpCode(c);
    }
    return result;
}//nextOp

//! Convert label_operand to OperandIdentitiferList
ThCode::OperandIdentifierList ThCode::Instruction::
operandsC() const
{ 
    OperandIdentifierList result; 
    if(!label_operand.isEmpty()){
        result << label_operand; 
    }
    return result; 
}//operandsC

void ThCode::Instruction::
checkMore(const char *p) const
{
    if(p<programCode() || p>=programCodeEnd()){
        throw ThError(10485, "ThCode checkMore: no more arguments.  Expecting 0..%d.  Got %d.", offset(programCodeEnd()), offset(p));
    }
}

//! Returns !isValid at end-of-code
void ThCode::Instruction::
getInstruction(const char *p)
{
    // p is checked below
    int newOffset= offset(p);
    argType_A= NoArg;
    argType_B= NoArg;
    operands_A.clear();
    operands_B.clear();
    label_operand= OperandIdentifier(NoOperand);
    if(newOffset>=th_code->programCodeSize()){
        current_offset= th_code->programCodeSize();  // !isValid()
        code_op= NOP;
        return;
    }
    checkMore(p);
    current_offset= newOffset;
    const char *ip= p;
    char c= *ip++;
    // same as toOpCode()
    char argFlags= c&OpNoArg;
    if(argFlags==0){
        code_op= Op(c);  // no arguments
    }else if((argFlags&ArgTypeMask)==0){  // ArgType for ArgA must be set
        code_op= Op((c&OpArgAMask)+OpArgA);
        argType_A= ArgType((c>>ArgAShift)&ArgTypeMask);
        checkMore(ip);
        ip= th_code->fillOperands(argType_A, ip, &operands_A);
        if(code_op==TryA){
            checkMore(ip);;
            OperandIdentifierList labelOperands;
            //!\todo why Literal for 'try' shouldn't argType_A==Field be an error?
            ip= th_code->fillOperands(Literal, ip, &labelOperands);  //!\todo Faster if load one operand.  Also in checkLabel
            label_operand= labelOperands[0];
        }
    }else if((argFlags>>ArgAShift)==0){  // ArgType for ArgB must be set
        code_op= Op((c&OpArgBMask)+OpArgB);
        argType_B= ArgType(c&ArgTypeMask);
        checkMore(ip);
        ip= th_code->fillOperands(argType_B, ip, &operands_B);
    }else{  // ArgA and ArgB must be arguments
        code_op= Op((c&OpArgABMask)+OpArgAB);
        argType_A= ArgType((c>>ArgAShift)&ArgTypeMask);
        argType_B= ArgType(c&ArgTypeMask);
        checkMore(ip);
        ip= th_code->fillOperands(argType_A, ip, &operands_A);
        checkMore(ip);
        ip= th_code->fillOperands(argType_B, ip, &operands_B);
        if(code_op==IfEqualsAB || code_op==IfNotEqualsAB){
            checkMore(ip);
            OperandIdentifierList labelOperands;
            ip= th_code->fillOperands(Literal, ip, &labelOperands);  //!\todo Faster if load one operand.  Also in checkLabel
            label_operand= labelOperands[0];
        }
    }
    next_offset= offset(ip);
}//getInstruction

#//!\name ThCode::const_iterator

ThCode::const_iterator::
const_iterator(const ThCode *code, int program)
: current_instruction(const_cast<ThCode *>(code), program)
{
}//const_iterator

#//!\name Constructors, etc.

ThCode::
ThCode(ThDisk *disk, QObject *parent/*= 0*/)
: QObject(parent)
, program_title()
, program_name()
, file_line(0)
, file_name()
, next_label()
, program_variables()
, utf8_names()
, name_offsets()
, program_operands()
, program_code()
, label_index()
, name_index()
, operand_index()
, thid_disk(disk)
, th_name_value()
, th_name_count(0)
, error_count(0)
, th_inc(NoName)
, th_lit(NoName)
, th_name(NoName)
, th_new(NoName)
, th_var(NoName)
, is_ignore_errors(false)
, is_literal_thlit(false)
, is_well_formed(true)
{
    next_label= th_UNIQUE_LABEL;
    utf8_names.append('\0');
    name_offsets.append(0);
    program_operands << NameIdentifier(NoName) << NameIdentifier(NoName); // NoOperand(0)
    th_inc= addName(th_INC);
    th_lit= addName(th_LIT);
    th_name= addName(th_NAME);
    th_new= addName(th_NEW);
    th_var= addName(th_VAR);  // must be last reserved name, otherwise addName will warn
}//constructor

#//!\name GetSet

//! Return labelOffset for literal label program.field
//! Returns ErrorReturn (-1) if none
//! Assumes labels in label_index are valid
int ThCode::
getLiteralLabelOffset(NameIdentifier program, NameIdentifier labelName) const
{
    OperandIdentifier labelOperand= label_index.value(qMakePair(program, labelName));
    if(!labelOperand){
        RoadError::Setting setting(RoadError::ErrorExpected);
        ThWarn(10361, "ThCode getLiteralLabelOffset: labelName not found for program %d labelName %d '%s'", program, labelName, toUtf8(labelName));
        return ErrorReturn;
    }
    return getLiteralLabelOffset(labelOperand);
}//getLiteralLabelOffset

//! Get offset for valid label.  
//! Assumes that it is a label (checkLabel).
//! \see getNameFieldFromLabelOffset for the inverse operation for a bad labelOffset
int ThCode::
getLiteralLabelOffset(OperandIdentifier operand) const
{
    int indx= operand*2+LabelOffset;
    if(operand.isEmpty() || indx>=program_operands.count()){
        throw ThError(10357, "ThCode getLiteralLabelOffset: operand %d for label is out-of-bounds", operand);
    }
    int low= program_operands[indx];
    int high= program_operands[indx+1];
    int result= low+(high<<Word16Shift);
    return result;
}//getLiteralLabelOffset

//! Return literal NameIdentifier for OperandIdentifier
//! Do no use for label operands.  They set the name to the label's program 
ThCode::NameIdentifier ThCode::
getLiteral(OperandIdentifier operand) const
{
    NameIdentifier name;
    NameIdentifier field= getNameField(operand, &name);
    if(name.isValid()){
        throw ThError(12039, "ThCode getLiteral: expecting a literal for operand %d.  Got %s.%s", operand,0, toUtf8(name), toUtf8(field));
    }
    return field;
}//getLiteral

//! Return name and field for operand
ThCode::NameIdentifier ThCode::
getNameField(OperandIdentifier operand, NameIdentifier *name) const
{
    int indx= operand*2;
    if(operand.isEmpty() || indx<0 || indx+1>=program_operands.count()){
        throw ThError(10360, "ThCode getNameField: operand %d for nameField is out-of-bounds", operand);
    }
    *name= program_operands[indx+1];
    return program_operands[indx];
}//getNameField

//! A label in program_operands is a program/labelName followed by a LabelOffset instead of the name/field for the next operand
//! This method returns the name/field if it appears to be a valid operand, otherwise field is NoName
//! \see getLiteralLabelOffset
ThCode::NameIdentifier ThCode::
getNameFieldFromLabelOffset(int labelOffset, NameIdentifier *name) const
{
    NameIdentifierInt nameInt= labelOffset>>Word16Shift;
    NameIdentifierInt fieldInt= labelOffset&Word16Mask;
    if(nameInt<0 || nameInt>=name_offsets.count() || fieldInt<0 || fieldInt>=name_offsets.count()){
        nameInt= NoName;
        fieldInt= NoName;
    }
    *name= NameIdentifier(nameInt);
    return NameIdentifier(fieldInt);
}//getNameFieldFromLabelOffset

//! Return OperandIdentifier for name.field
//! Returns NoOperand if not found
ThCode::OperandIdentifier ThCode::
getOperand(const QString &name, const QString &field) const
{
    NameIdentifier n= getName(name);
    NameIdentifier f= getName(field);
    NameIdentifierPair nameField= qMakePair(n, f);
    OperandIdentifier result= operand_index.value(nameField);
    return result;
}//getOperand

//! Not const due to getValue()
//! Same as \see checkLabel
bool ThCode::       
isLabelValid(OperandIdentifier label)
{
    int labelOffset= getLiteralLabelOffset(label);
    if(labelOffset==NoLabelOffset){
        RoadError::Setting setting(RoadError::ErrorExpected);
        ThWarn(12063, "ThCode isLabelValid: label is not defined for '%s'", toString(label));
        return false;
    }
    if(labelOffset<0 || labelOffset>program_code.size()-3){
        RoadError::Setting setting(RoadError::ErrorExpected);
        ThWarn(10487, "ThCode isLabelValid: labelOffset %d is not in 0..%d for label '%s'", labelOffset, program_code.size()-3, toString(label));
        return false;
    }
    char c= program_code[labelOffset];
    Op op= ThCode::toOpCode(c);
    if(op!=LabelA){
        RoadError::Setting setting(RoadError::ErrorExpected);
        ThWarn(10488, "ThCode isLabelValid: labelOffset %d does not point to a label operator.  Got op 0x%X", labelOffset, op);
        return false;
    }
    ArgType argA= ArgType((c>>ArgAShift)&ArgTypeMask);
    OperandIdentifierList labelOperands;
    fillOperands(argA, program_code.constData()+labelOffset+1, &labelOperands);
    if(labelOperands[0]!=label){
        RoadError::Setting setting(RoadError::ErrorExpected);
        ThWarn(10489, "ThCode isLabelValid: expecting label operand %d at labelOffset %i.  Got label operand %d", label, labelOperands[0], qint64(label));
        return false;
    }
    return true;
}//isLabelValid

//! True if OperandIdentifier is literal
//! Do not use for label operands.  They use the name of the label's program
bool ThCode::
isLiteral(OperandIdentifier i) const
{
    NameIdentifier name;
    NameIdentifier field= getNameField(i, &name);
    return name.isEmpty() && !field.isEmpty() && !isVariable(field);
}//isLiteral

//! True if argument is V or N.N or R[R] for Goto, Try, Throw, or Catch
//! False if argument contains ' ', '=', or extra '.', '[', or ']' 
//! Returns isName if N and not V
//! Assumes argument is trimmed
//! Non-const due to variables and addName()
bool ThCode::
isRestrictedNameField(const QString &argument, bool *isName/*= NULL*/)
{
    if(argument.endsWith(']')){
        if(isName!=NULL){
            *isName= false;
        }
        QStringList nameFields= argument.split('[');
        if(nameFields.count()==2){
            bool isName;
            if(ThCode::isRestrictedNameField(nameFields.at(0).trimmed(), &isName) || isName){  // One-level recursive.  Does not contain '['
                QString index=nameFields.at(1);
                index.chop(1);
                if(ThCode::isRestrictedNameField(index.trimmed(), &isName) || isName){  // One-level recursive.  Does not contain '['
                    return true;
                }
            }
        }
    }else{
        QStringList names= argument.split('.');
        if(ThCode::isPlainName(names.at(0))){
            if(names.count()==1){
                NameIdentifier var= addName(names.at(0));
                if(isVariable(var)){
                    return true;
                }
                if(isName!=NULL){
                    *isName= true;
                }
            }else if(names.count()==2){
                return ThCode::isPlainName(names.at(1));
            }
        }
    }
    return false;
}//isRestrictedNameField

void ThCode::
setProgramName(const QString &s)
{
    NameIdentifier program= addName(s);
    setProgramName(program);
}//setProgramName

//! Return string for operand. 
//!\todo Combine with printOperandNames
QString ThCode::
toString(OperandIdentifier operand) const
{
    NameIdentifier name;
    NameIdentifier field= getNameField(operand, &name);
    if(name.isEmpty()){
        return QString(toUtf8(field));
    }else if(field.isEmpty()){
        return QString(toUtf8(name))%QString(".?");
    }else if(name==program_name){
        return QString(toUtf8(field));
    }else{
        return QString(toUtf8(name))%QString(".")%QString(toUtf8(field));
    }
}//toString operand

//! Return string for operand. 
//!\todo Combine with printOperands
QString ThCode::
toString(const OperandIdentifierList &operands) const
{
    QString result;
    switch(operands.count()){
    case 1:
        result= toString(operands[0]);
        break;
    case 2:
        result= toString(operands[0]) % QString("[") % toString(operands[1]) % QString("]");
        break;
    default:
        throw ThError(12060, "ThCode toString: expecting operand list of one or two name-fields.  Got %d name-fields", operands.count());
    }
    return result;
}//toString operands

//! Return UTF8 string for name. 
const char * ThCode::
toUtf8(NameIdentifier name) const
{
    if(name>=name_offsets.count()){
        throw ThError(10362, "ThCode toUtf8: name %d is out-of-bounds (end %d)", name, name_offsets.count());
    }
    int result= name_offsets.value(name);
    if(result<0 || result>=utf8_names.size()){
        throw ThError(10363, "ThCode toUtf8: name offset %d is out-of-bounds (end %d)", result, utf8_names.size());
    }
    return utf8_names.constData()+result;
}//toUtf8

//! Warn if reserved name outside of ThCode constructor (th_XYZ or th_123)
//!\todo Review independent use of reserved names
void ThCode::
warnIfReservedName(const QString &name) const
{
    if(!th_var.isEmpty() && ThCode::isReservedName(name)){ // allow ThCode constructor to add reserved names
        if(RoadEventStack::isDetailLogging()){ //!<\todo review when switch to full time logging
            ThWarn(12114, "ThCode warnIfReservedName: name '%s' is reserved for name code.  It starts with '%s' followed by upper-case or digit",0,0, name, th_RESERVED_PREFIX);
        }
    }
}//warnIfReservedName

#//!\name Methods

//! Return QList of OperandIdentifier's for an instruction argument
//! If R is N, N.N, or V, then matches R, R[R], or R.N; otherwise an error
//! OK if N is th_NEW and isAllowedThNew; otherwise an error
//! Assumes argument is trimmed()
ThCode::OperandIdentifierList ThCode:: 
addArgument(const QString &argument, bool isAllowedThNew)
{
    errorIfFrozen();
    bool isArray; // addOperand does not report errors for N.V and V.V (!isArray)
    QStringList names= splitArgument(argument, &isArray);
    OperandIdentifierList result;
    OperandIdentifier fieldWithVariable;
    switch(names.count()){
    case 2:                     // One or two names -- V, N, N.N, V.N, V.V, N[N], N[V], V[N], V[V]
        result << addOperand(names.at(0), names.at(1), isAllowedThNew, &fieldWithVariable);
        if(!fieldWithVariable.isEmpty()){
            result << fieldWithVariable;
        }
        break;
    case 4:                     // Three or four names -- R[R] with at least one N.N
        result << addOperand(names.at(0), names.at(1), isAllowedThNew);
        result << addOperand(names.at(2), names.at(3), isAllowedThNew);
        break;
    default:
        throw ThError(10496, "ThCode addArgument: Expecting 1..4 names for '%s' as 2 or 4 names.  Got %d names", names.count(), 0, argument);
    }
    if(!isArray && result.count()==2){      // Report error for N.V or V.V
        NameIdentifier name;
        NameIdentifier field= getNameField(result.at(1), &name);
        if(name.isValid() && isVariable(field)){   
            throw ThError(12093, "ThCode addArgument: expecting N[V] or V[V].  Got '%s' with variable '%s'", 0,0, argument, toUtf8(field));
        }
    }
    return result;
}//addArgument

//! Add operand for Call or literal Try
ThCode::OperandIdentifier ThCode::
addCallOperand(NameIdentifier programName, NameIdentifier thidTitle)
{
    errorIfFrozen();
    if(!programName.isEmpty()){
#ifndef TH_NO_THDISK
        //!\todo Assumes programName is either a ThDepot or specifies a ThDepot.  What about 't_ifequals'?
        Thid *thid= NULL;
        try{
            //!\todo Assumes programName is a thidIdString.  Should textual names be allowed?
            ThidId id(toUtf8(programName)); 
            //!\todo ThidInfo is just as good and faster
            thid= thid_disk->getThidByLabel(id.toDepotId(), toUtf8(thidTitle));
        }catch(const ThError &e){
            ThError::warn(e);
        }catch(const std::exception &e){
            ThWarn(10787, "ThCode low-level error from getDepotByLabel or getThidByLabel: %s", 0,0, e.what());
        }
        if(thid && thid->isFromDisk()){  //!\todo set code only if has code
            QString thidIdString= thid->toThidIdString();
            NameIdentifier thidIdentifier= addName(thidIdString);
            return addOperand(thidIdentifier, thidTitle);
        }
#endif //!TH_NO_THDISK
    }
    //!\todo if cannot find thidTitle, adds a call to thidTitle in context of programName (its Id)
    return addOperand(programName, thidTitle);
}//addCallOperand

void ThCode::
addLiteralLabel(OperandIdentifier label, int labelOffset)
{
    errorIfFrozen();
    if(labelOffset<0 || labelOffset>nextCodeOffset()){
        throw ThError(10472, "ThCode addLiteralLabel: label '%s' offset %d is out-of-bounds.  End %d", labelOffset, nextCodeOffset(), toString(label));
    }
    int labelIndex= label*2+LabelOffset;
    if(labelIndex+1 < program_operands.size()){ // Already defined
        NameIdentifier lowOffset= program_operands.value(labelIndex);
        NameIdentifier highOffset= program_operands.value(labelIndex+1);
        if(lowOffset==NoLabel && highOffset==NoLabel){
            program_operands[labelIndex]= NameIdentifier(labelOffset&Word16Mask);
            program_operands[labelIndex+1]= NameIdentifier(labelOffset>>Word16Shift);
        }else if(isLabelValid(label)){
            throw ThError(10355, "ThCode addLiteralLabel: label '%s' already defined", toString(label));
        }else{
            throw ThError(10471, "ThCode addLiteralLabel: operand '%s' is not a valid label", toString(label));
        }
    }else if(labelIndex == program_operands.size()){ // New offset
        program_operands << NameIdentifier(labelOffset&Word16Mask);
        program_operands << NameIdentifier(labelOffset>>Word16Shift);
    }else{
        checkLabel(label); // Errors due to bad offset
    }
    NameIdentifier program;
    NameIdentifier field= getNameField(label, &program);
    label_index.insert(qMakePair(program, field), label);
}//addLiteralLabel

ThCode::OperandIdentifier ThCode::
addLiteralLabelOperand(NameIdentifier program, NameIdentifier field)
{
    errorIfFrozen();
    if(program==NoName){
        throw ThError(12048, "ThCode addLiteralLabelOperand: expecting a program ThidId for label '%s'.  None defined",0,0, toUtf8(field));
    }
    OperandIdentifier labelOperand= addOperand(program, field);
    addLiteralLabelOperand(labelOperand);
    return labelOperand;
}//addLiteralLabelOperand

void ThCode::
addLiteralLabelOperand(OperandIdentifier label)
{
    errorIfFrozen();
    if(label<(program_operands.size()/2)-1){ // Already defined
        int labelOffset= getLiteralLabelOffset(label);
        if(labelOffset!=NoLabelOffset){
            checkLabel(label); // throws an error if bad
        }
    }else{
        program_operands << NameIdentifier(NoLabel) << NameIdentifier(NoLabel);
    }
}//addLiteralLabelOperand

ThCode::NameIdentifier ThCode::
addName(const QString &s)
{
    errorIfFrozen();
    NameIdentifier result;
    if(s.isEmpty()){
        result= NameIdentifier(NoName);
    }else{
        QString s2= s;
        result= getName(s2);
        if(result==th_name && !th_name_value.isEmpty()){
            ++th_name_count;
            s2= th_name_value;
            result= getName(s2);
        }
        if(result.isEmpty()){
            warnIfReservedName(s2);
            int nextNameOffset= utf8_names.size();
            QByteArray name= s2.toUtf8();
            utf8_names.append(name);
            utf8_names.append('\0'); // last name will have another \0 after it
            result= NameIdentifier(name_offsets.size());
            name_offsets.append(nextNameOffset);
            name_index.insert(s2, result);
        }
    }
    return result;
}//addName


ThCode::OperandIdentifier ThCode::
addOperand(NameIdentifier name, NameIdentifier field)
{
    errorIfFrozen();
    NameIdentifierPair nameField= qMakePair(name, field);
    OperandIdentifier result= operand_index.value(nameField);
    if(result.isEmpty()){
        if(name.isEmpty() && field.isEmpty()){
            throw ThError(12112, "ThCode addOperand: cannot add the NoOperand(0) for NoName.NoName");
        }
        int nextOperand= program_operands.count();
        result= OperandIdentifier(nextOperand/2);
        program_operands << field << name;
        operand_index.insert(nameField, result);
    }
    return result;
}//addOperand

//! Return OperandIdentifier for name.field
//! If isAllowedThNew, OK if name or field is th_NEW; otherwise throws an error if name or field is th_NEW
//! Assumes fieldWithVariable is set for one or two names.  It is NULL for N.N[N], N[N.N], or N.N[N.N]
//! Sets fieldWithVariable if two names with a variable.  Caller reports error for N.V or V.V
//! Otherwise an error if two or more names and name or field is a variable
ThCode::OperandIdentifier ThCode::
addOperand(QString name, QString field, bool isAllowedThNew, OperandIdentifier *fieldWithVariable/*= NULL*/)
{
    errorIfFrozen();
    if(name.isEmpty()&&field.isEmpty()){
        throw ThError(10037, "ThCode addOperand: Both name and field are empty strings, \"\".  Can not parse");
    }
    if(fieldWithVariable!=NULL){
        *fieldWithVariable= OperandIdentifier();
    }
    NameIdentifier nameIdentifier= addName(name);
    NameIdentifier fieldIdentifier= addName(field);
    if(isAllowedThNew){
        if(nameIdentifier==th_new || (fieldIdentifier==th_new && nameIdentifier.isValid())){
            throw ThError(12097, "ThCode addOperand: 'th_NEW' as a name.field is not supported.  Got '%s.%s'", 0,0, name, field);
        }else if(fieldIdentifier==th_new && fieldWithVariable==NULL){
            throw ThError(12117, "ThCode addOperand: 'th_NEW' as a th_NEW[name.field] or name.field[th_NEW] is not supported");
        }
    }else if(nameIdentifier==th_new || fieldIdentifier==th_new){
        throw ThError(12115, "ThCode addOperand: 'th_NEW' may only be used as the right-hand side of Assign or IfEquals.  Got '%s.%s'", 0,0, name, field);
    }
    if(nameIdentifier.isEmpty()){
        if(isVariable(fieldIdentifier)){                            // V, N.N[V], or V[N.N]
            nameIdentifier= program_name;
        }
    }else if(isVariable(nameIdentifier) && fieldWithVariable!=NULL){ // two names V[N] or V.N
        if(isVariable(fieldIdentifier)){                            // V[V] or V.V
            // Caller reports error for V.V
            *fieldWithVariable= addOperand(program_name, fieldIdentifier);
        }else{                                                                      // V[N] or V.N
            *fieldWithVariable= addOperand(NameIdentifier(), fieldIdentifier);
        }
        fieldIdentifier= nameIdentifier;
        nameIdentifier= program_name;
    }else if(isVariable(fieldIdentifier)){                          // Two to four names with N.V
        if(fieldWithVariable==NULL){                                                // Three or four names, V.V[R], N.V[R], R[N.V], or R[V.V]
            throw ThError(12081, "ThCode addOperand: expecting V[R] or R[V].  Got '%s.%s' as N.V or V.V in R[R]", 0,0, name, field);
        }else{                                                                      // N.V or N[V] 
            // Caller reports error for N.V
            *fieldWithVariable= addOperand(program_name, fieldIdentifier);
            fieldIdentifier= nameIdentifier;
            nameIdentifier= NameIdentifier();
        }
    }else if(isVariable(nameIdentifier)){                           // Three or four names, V.N[R], R[V.N]
        throw ThError(12080, "ThCode addOperand: expecting V[R] or R[V].  Got '%s.%s' as V.N[R], or R[V.N]", 0,0, name, field);
    }
    return addOperand(nameIdentifier, fieldIdentifier);
}//addOperand

//! Add literal OperandIdentifier as a Variable for the current progran_name
//! Returns OperandIdentifierList for program_name.Variable
//!\todo should addLiteralLabelOperand() also imply program_name instead of passing it as an argument
ThCode::OperandIdentifierList ThCode:: 
addProgramVariable(OperandIdentifier lit)
{
    OperandIdentifierList result;
    NameIdentifier name; // NoName
    NameIdentifier variable= getNameField(lit, &name);
    program_variables.insert(NameIdentifierPair(program_name, variable));
    OperandIdentifier programVariable= addOperand("", toUtf8(variable), !IsAllowedThNew); // returns program_name.variable due to updated program_variables
    result << programVariable;
    return result;
}//addProgramVariable

void ThCode::
appendAssignCode(const QString &argumentA, const QString &argumentB)
{
    int n= nameCount();
    int n2= operandCount();
    OperandIdentifierList operandsA= addArgument(argumentA, !IsAllowedThNew);
    bool isNewArgA= (nameCount()>n);
    bool isNewOperandA= (operandCount()>n2);
    OperandIdentifierList operandsB= addArgument(argumentB, IsAllowedThNew);

    ArgType argA= argTypeForOperands(operandsA);
    ArgType argB= argTypeForOperands(operandsB);
    if(argA==Literal){  // V= ... defines a program Variable (th_VAR).  Must be first, no preceding use of N.field or N 
        if(!isNewArgA && !is_literal_thlit){
            NameIdentifier var= getName(argumentA);
            if(!isVariable(var)){
                OperandIdentifier first= firstLiteralOrName(var);
                if(first.isValid() && (!isNewOperandA || first!=operandsA.at(0))){
                    throw ThError(12088, "ThCode appendAssignCode: expecting a new variable for assign ('%s=...'), but '%s' was previously used as a literal (N, R[N]) or a name (N.field)",0,0, argumentA, argumentA); 
                }
            }
        }
        operandsA= addProgramVariable(operandsA.at(0));
        argA= argTypeForOperands(operandsA);
    }
    uchar c= ThCode::toByteCode(AssignAB, argA, argB);
    program_code.append(c);
    appendOperands(operandsA);
    appendOperands(operandsB);
}//appendAssignCode

//! Append Call code for a callTitle
//! Error if could be a try (name.field -> success, or V -> success)
//! \see appendLabelCode and appendTryCode
void ThCode::
appendCallCode(const QString &argument)
{
    if(ThCode::isRestrictedNameField(argument)){
        throw ThError(10497, "ThCode appendCallCode: Call appears to be an indirect call through V, N.N, or R[R]. Got '%s'.  If indirect call, add label for try (N.N ->on success).  If call, add ' ', '.', '=', '[', ']', or '[[...]]'", argument);
    }
    NameIdentifier callTitle= addName(argument);
    OperandIdentifier operand= addCallOperand(program_name, callTitle); //!\todo Review conversion to ThidId if ThDisk in context of program_name; otherwise, it is a local label
    addLiteralLabelOperand(operand);
    uchar c= ThCode::toByteCode(CallA, Literal, NoArg);
    program_code.append(c);
    appendOperandIdentifier(operand);
}//appendCallCode

//! Append goto label
//! Labels are relative to the current program_name
void ThCode::
appendGotoCode(const QString &argument)
{
    if(ThCode::isRestrictedNameField(argument)){
        OperandIdentifierList operands= addArgument(argument, !IsAllowedThNew);
        ArgType argA= argTypeForOperands(operands);
        uchar c= ThCode::toByteCode(GotoA, argA, NoArg);
        program_code.append(c);
        appendOperands(operands);
    }else{
        checkLabelName(argument);
        NameIdentifier labelName= addName(argument);
        OperandIdentifier labelOperand= addLiteralLabelOperand(program_name, labelName);
        uchar c= ThCode::toByteCode(GotoA, ArgType::Literal, NoArg);
        program_code.append(c);
        appendOperandIdentifier(labelOperand);
    }
}//appendGotoCode

void ThCode::
appendIfEqualsCode(Op opCode, const QString &argumentA, const QString &argumentB, const QString &label)
{
    if(opCode!=IfEqualsAB && opCode!=IfNotEqualsAB){
        throw ThError(10336, "ThCode appendIfEqualsCode: expecting IfEquals or IfNotEquals for opCode.  Got 0x%X", opCode);
    }
    checkLabelName(label);
    NameIdentifier labelName= addName(label);
    OperandIdentifier labelOperand= addLiteralLabelOperand(program_name, labelName);
    OperandIdentifierList operandsA= addArgument(argumentA, !IsAllowedThNew);
    OperandIdentifierList operandsB= addArgument(argumentB, IsAllowedThNew);
    ArgType argA= argTypeForOperands(operandsA);
    ArgType argB= argTypeForOperands(operandsB);
    uchar c= ThCode::toByteCode(opCode, argA, argB);
    program_code.append(c);
    appendOperands(operandsA);
    appendOperands(operandsB);
    appendOperandIdentifier(labelOperand);
}//appendIfEqualsCode

//! Append instruction to ThCode
//! Determines opCode and arguments
void ThCode:: 
appendInstruction(const QString &s)
{
    errorIfFrozen();  //!<\todo Should all of the 'append...' calls be private?  Otherwise should add an errorIfFrozen() check.
    if(s.contains("\n")){
        throw ThError(10481, "ThCode appendInstruction: use appendProgram for multi-line instructions:\n...%s...", s);
    }
    QString instruction= s;
    if(s.contains("[[") && s.contains("]]")){  // [[..]]
        instruction= replaceLiteralWithThName(s); 
    }
    if(instruction.contains("//")){
        QStringList instructions= instruction.split("//");
        instruction= instructions.at(0).simplified().trimmed();
        if(instruction.isEmpty()){                              // A leading '//' is always a comment 
            return;   // comment line
        }
        if(instructions.count()-1 >= MultipleInstructions){     // Two or more '//' separate multiple instructions
            foreach(const QString &s2, instructions){
                appendInstruction(s2);  // one level of recursion
            }
            return; // multiple instructions
        }
    }else{
        instruction= instruction.simplified().trimmed();
        if(instruction.isEmpty()){
            return;  // blank line
        }
    }
    if(instruction.contains(QRegularExpression(ThCode::NonInstructionCharactersRE))){
        throw ThError(12094, "ThCode appendInstruction: instruction contains one or more non-printable characters.  Got '%s'", instruction);
    }
    // in precedence order, first isMatched wins
    bool isMatched= false;
    // Fixed instructions
    bool isPrintLiteralSpace= false;
    bool isPrintNewLine= false;
    bool isPrintLiteralComma= false;
    bool isNop= false;
    bool isReturn= false;
    bool isFail= false;
    bool isBreakpoint= false;
    if(instruction.count()<=2){
        isMatched= true;
        if(instruction=="=_"){                   // '=_'
            isPrintLiteralSpace= true;
        }else if (instruction=="="){                            // '=_'
            isPrintNewLine= true;
        }else if(instruction=="=,"){                            // '=,'
            isPrintLiteralComma= true;
        }else if(instruction=="="){                            // '=_'
            isPrintNewLine= true;
        }else if(instruction=="."){                            // '.'
            isNop= true;
        }else if(instruction=="->"){                           // '->'
            isReturn= true;
        }else if(instruction=="!>"){                           // '!>'
            isFail= true;
        }else if(instruction=="=!"){                           // '=!'
            isBreakpoint= true;
        }else{
            isMatched= false;
        }
    }
    // Labeled instructions
    bool isGoto= false;
    bool isLabel= false;
    bool isThrow= false;
    bool isCatch= false;
    bool isEquals= false;
    bool isNotEquals= false;
    bool isTry= false;
    QString lhs;
    QString rhs;
    if(!isMatched){
        isGoto= instruction.startsWith("->");                           // '->asdf'
        isLabel= instruction.endsWith("->");                           // 'asdf->';
        isThrow= instruction.startsWith("!>");                    // '!>error' or '!>name.field';
        isCatch= instruction.endsWith("!>");                // 'error!>'
        if(isGoto+isLabel+isThrow+isCatch>1){
            throw ThError(12096, "ThCode appendInstruction: instruction starts and ends with '->' or '!>'.  Is it Op:goto (->...), Op:label (...->), Op:throw (!>...), or Op:catch (...!>)?  Got '%s'", instruction);
        }
        if(isGoto+isLabel+isThrow+isCatch==1){
            isMatched= true;
        }else{
            int equalsIndex= instruction.indexOf(QRegularExpression("[^=!][!=]=[^=].*->.")); 
            if(equalsIndex!=NoIndex){
                ++equalsIndex; // skip preceding character
                int gotoIndex= instruction.lastIndexOf("->", -2);
                lhs= instruction.left(equalsIndex).trimmed();
                rhs= instruction.mid(equalsIndex+2, gotoIndex-equalsIndex-2).trimmed();
                QString equals= instruction.mid(equalsIndex, 2); 
                isEquals= (instruction.at(equalsIndex)==QChar('='));        // 'asdf == asdf ->asdf'
                isNotEquals= !isEquals;             // 'asdf != asdf ->asdf'
                isMatched= true;
            }else if(instruction.contains(QRegularExpression("->"))){      // 'try program ->label'
                isTry= true;
                isMatched= true;
            }
        }
    }
    // Print instructions
    bool isPrintComma= false;
    bool isPrintSpace= false;
    bool isPrintValue= false;
    bool isPrintLine= false;
    bool isPrintFields=  false;
    if(!isMatched){
        if(instruction.endsWith("=,")){                     // 'name.field=,'
            isPrintComma= true;
        }else if(instruction.endsWith(" =_")){                     // 'name.field =_'
            isPrintSpace= true;
        }else if(instruction.endsWith("=_")){                      // 'name.field=_'
            isPrintValue= true;
        }else if(instruction.endsWith("=")){                    // 'name.field='
            if(!instruction.endsWith("==")){    // '... literal=='  //!<\todo Should there be a warning or a log?
                isPrintLine= true;
            }
        }else if(instruction.endsWith("=!")){                   // 'name=!'
            isPrintFields= true;
        }
        isMatched= isPrintComma|isPrintSpace|isPrintValue|isPrintLine|isPrintFields;
    }
    // Assign instructions
    bool isAssign= false;
    if(!isMatched){
        int bracketIndex=  instruction.indexOf(QChar('['));
        int assignIndex= instruction.indexOf(QRegularExpression("\\] ?=[^=]"));
        if(assignIndex==NoIndex || bracketIndex==NoIndex || assignIndex<bracketIndex){
            assignIndex= instruction.indexOf(QRegularExpression("[^=]=[^=]"));
        }
        assignIndex= instruction.indexOf(QChar('='), assignIndex);
        if(assignIndex!=NoIndex){
            lhs= instruction.left(assignIndex).trimmed();
            rhs= instruction.mid(assignIndex+1).trimmed();
            isAssign= true;
            isMatched= true;
        }
    }
    bool isCall= !isMatched;

    int parseCount= isAssign+isBreakpoint+isCall+isCatch+isEquals+isFail+isGoto+isLabel+isNop+isNotEquals+isPrintComma+isPrintFields+isPrintLine+isPrintLiteralComma+isPrintLiteralSpace+isPrintNewLine+isPrintSpace+isPrintValue+isReturn+isThrow+isTry;
    if(parseCount!=1){
        throw ThError(12082, "ThCode appendInstruction: expecting one parse of '%s'.  Got %d 'isAssign,etc.'", parseCount,0, instruction);
    }
    if(isAssign){                       // x= y
        appendAssignCode(lhs, rhs);
    }else if(isBreakpoint){             // !=
        appendCodeOp(Breakpoint);
    }else if(isCall){                   // x
        appendCallCode(instruction);
    }else if(isCatch){         // ...!>
        instruction.chop(2);
       // appendCatchCode(instruction.trimmed());
        throw ThError(12077, "ThCode appendInstruction: catch is not implemented.  Use try ('...->...') and fail ('!>') instead.  Got %s", instruction);
    }else if(isEquals||isNotEquals){    // 'a == b -> label'   w/ lhs, rhs
        int gotoIndex= instruction.lastIndexOf("->");  // Defined by the isEquals/isNotEquals regexp
        QString label= instruction.right(instruction.count()-gotoIndex-2).trimmed();
        if(lhs.isEmpty() || rhs.isEmpty() || label.isEmpty()){
            throw ThError(10464, "ThCode appendInstruction: 'Empty operand or label.  Expecting '...== ...->...' or '...!= ...->...'.  Got %s", instruction);
        }
        if(instruction.at(gotoIndex-1)==QChar('-')){
            throw ThError(12079, "ThCode appendInstruction: instruction contains '-->'.  Was ifEquals or ifNotEquals intended ('...==... ->...')?  Got '%s'", instruction);
        }
        Op op= (isEquals ? IfEqualsAB : IfNotEqualsAB);
        appendIfEqualsCode(op, lhs, rhs, label);
    }else if(isFail){             // !>
        appendCodeOp(Fail);
    }else if(isGoto){                   // ->...
        instruction.remove(0, 2);
        appendGotoCode(instruction.trimmed());
    }else if(isLabel){                  //...->
        instruction.chop(2);
        appendLabelCode(instruction.trimmed());
    }else if(isNop){                    // .
        appendCodeOp(NOP);
    }else if(isPrintComma){             // ...=,
        instruction.chop(2);
        appendPrintCode(PrintA, instruction.trimmed());
        appendCodeOp(PrintCommaSpace);
    }else if(isPrintFields){            // ...=!
        instruction.chop(2);
        appendPrintCode(PrintFieldsA, instruction.trimmed());
    }else if(isPrintLine){              // ...=
        instruction.chop(1);
        appendPrintCode(PrintA, instruction.trimmed());
        appendCodeOp(PrintNewLine);
    }else if(isPrintLiteralComma){      // =, (comma)
        appendCodeOp(PrintComma);
    }else if(isPrintLiteralSpace){      // =_ (space)
        appendCodeOp(PrintSpace);
    }else if(isPrintNewLine){      // = (newline)
        appendCodeOp(PrintNewLine);
    }else if(isPrintSpace){             // ... =_ 
        instruction.chop(2);
        if(instruction.isEmpty()){
            appendCodeOp(PrintSpace);
        }else{
            appendPrintCode(PrintA, instruction.trimmed());
            appendCodeOp(PrintSpace);
        }
    }else if(isPrintValue){             // ...=_
        instruction.chop(2);
        appendPrintCode(PrintA, instruction.trimmed());
    }else if(isReturn){                 // ->
        appendCodeOp(Return);
    }else if(isThrow){                  // !>...
        instruction.remove(0, 2);
        if(ThCode::isRestrictedNameField(instruction)){
            throw ThError(10468, "ThCode appendInstruction: throw of name-field is not implemented.  Got '!>%s'", instruction);
        }else{
            //!\todo implement appendThrowCode(instruction.trimmed()) and handle variables
            appendPrintLiteral(instruction.trimmed());
            appendCodeOp(PrintNewLine);
            appendCodeOp(Fail);
        }
    }else if(isTry){
        QStringList gotoLabel= RoadUtility::splitLast(instruction, QString("->"));
        if(gotoLabel[0].endsWith(QChar('-'))){
            throw ThError(10351, "ThCode appendInstruction: instruction contains '-->'.  Was try intended ('... ->...')?  Got '%s'", instruction);
        }
        appendTryCode(gotoLabel[0].trimmed(), gotoLabel[1].trimmed());
    }
    if(!th_name_value.isEmpty()){  // set by replaceLiteralWithThName
        QString nameValue= th_name_value;
        th_name_value.clear();
        if(th_name_count==0){
            throw ThError(10030, "ThCode appendInstruction: expecting use of literal '[[%s]]', but 'th_NAME' was not resolved '%s'",0,0, nameValue, instruction);
        }else if(th_name_count>1){
            throw ThError(10369, "ThCode appendInstruction: more than one th_NAME for literal '[[%s]]' in '%s'",0,0, nameValue, instruction);
        }
    }
}//appendInstruction

//! Append simplified argument as Label
//! \see appendCallCode
void ThCode::
appendLabelCode(const QString &argument)
{
    checkLabelName(argument);
    NameIdentifier labelName= addName(argument);
    appendLabelCode(labelName);
}//appendLabelCode arg

void ThCode::
appendLabelCode(NameIdentifier labelName)
{
    OperandIdentifier labelOperand= addLiteralLabelOperand(program_name, labelName);
    int labelOffset= nextCodeOffset();
    uchar c= ThCode::toByteCode(LabelA, Literal, NoArg);
    program_code.append(c);
    appendOperandIdentifier(labelOperand);
    addLiteralLabel(labelOperand, labelOffset);
}//appendLabelCode name

void ThCode::
appendOperandIdentifier(OperandIdentifier operand)
{
    char low= char(operand&ByteMask);
    char high= char(operand>>ByteShift);
    program_code.append(low);
    program_code.append(high);
}//appendOperandIdentifier

void ThCode::
appendOperands(OperandIdentifierList operands)
{
    foreach(OperandIdentifier operand, operands){
        appendOperandIdentifier(operand);
    }
}//appendOperands

//! Append printA or printFieldsA code for one or more values in a QString 
//! Two level recursive if values separated by '=_ ' and '=,'
//! One level recursive if values separated by '=_ ' or '=,'
//! A name.field contains one '.'; otherwise it is a literal
//! Assumes argument is simplified
void ThCode::
appendPrintCode(ThCode::Op code, const QString &argument)
{
    if(code!=PrintA && code!=PrintFieldsA){
        throw ThError(10345, "ThCode appendPrintCode: expecting PrintA or PrintFieldsA.  Got 0x%X", code);
    }
    if(argument.contains("=,")){
        QStringList printCommas= argument.split("=,");
        for(int i= 0; i<printCommas.count(); ++i){
            QString arg= printCommas.at(i).simplified();
            if(!arg.isEmpty()){
                appendPrintCode(code, arg);
            }
            bool isLast= (i==printCommas.count()-1);
            if(code==PrintA && !isLast){
                if(printCommas.at(i+1).startsWith(" ")){
                    appendCodeOp(PrintCommaSpace);
                }else{
                    appendCodeOp(PrintComma);
                }
            }
        }
    }else if(argument.contains("=_ ")){
        QStringList printCommas= argument.split("=_ ");
        for(int i= 0; i<printCommas.count(); ++i){
            QString arg= printCommas.at(i).simplified();
            if(!arg.isEmpty()){
                appendPrintCode(code, arg);
            }
            bool isLast= (i==printCommas.count()-1);
            if(code==PrintA && !isLast){
                if(printCommas.at(i).endsWith(" ")){
                    appendCodeOp(PrintSpace);
                }
            }
        }
    }else{
        OperandIdentifierList operands;
        if(ThCode::isRestrictedNameField(argument)){
            operands= addArgument(argument, IsAllowedThNew);
        }else{
            operands << addOperand("", argument, IsAllowedThNew);
        }
        ArgType argA= argTypeForOperands(operands);
        uchar c= ThCode::toByteCode(code, argA, NoArg);
        program_code.append(c);
        appendOperands(operands);
    }
}//appendPrintCode

void ThCode::
appendPrintLiteral(const QString &argument)
{
    if(!argument.isEmpty()){
        OperandIdentifierList operands;
        operands << addOperand("", argument, IsAllowedThNew);
        uchar c= ThCode::toByteCode(PrintA, Literal, NoArg);
        program_code.append(c);
        appendOperands(operands);
    }
}//appendPrintLiteral

#ifndef TH_NO_THDISK

//! Append Thid program to ThCode.  Its title may be specified
//! Thid not const due to getTextDocument
//! Returns program offset for Thid
int ThCode:: 
appendProgram(Thid *thid, NameIdentifier title/*= NoName*/)
{
    const ThTextDocument *doc= thid->getTextDocument(ThModelId::ModelThid);
    return appendProgram(thid, doc, title);
}//appendProgram

//! Returns program offset for Thid with ThTextDocument.  Its title may be specified (e.g., resolveCall)
//! Clears programName at end
int ThCode:: 
appendProgram(Thid *thid, const ThTextDocument *text, NameIdentifier title/*= NoName*/)
{
    int program= nextCodeOffset();
    if(!thid){
        throw ThError(10483, "ThCode appendProgram: thid not defined for appendProgram");
    }
    try{
        setProgramName(thid->toThidIdString());
        setFileName(thid->toThidIdString());
        if(title.isEmpty()){
            appendLabelCode(thid->title(text->localeId()));
        }else{
            appendLabelCode(title);  //!\todo title from resolveCall is not correct since a program may have multiple names.  but OK for now
        }
        appendTextDocument(text);
        appendCodeOp(Return);
        clearProgramName();
    }catch(...){
        clearProgramName();
        //!<\todo Need to remove programTitle on error
        throw;
    }
    return program;
}//appendProgram
#endif // TH_NO_THDISK

//! Append QTextStream to ThCode and return program offset
//! Sets programId, clears programName/programId on exit
int ThCode:: 
appendStream(const QString &programId, QTextStream *stream)
{
    setProgramName(programId);
    int program= nextCodeOffset();
    while(true){ // atEnd() not supported on stdin
        QString line= stream->readLine();
        if(line.isNull()){   //!< EOF \todo replace with while(readLineInto) (Qt 5.5)
            break;
        }
        if(line.contains(QRegularExpression("^[ \t]*th_END"))){
            ThWarn(12057, "ThCode appendStream: early exit due to 'th_END'");
            break;
        }
        ++file_line;
        tryAppendInstruction(line);
    }
    clearProgramName();
    return program;
}//appendStream

#ifndef TH_NO_THDISK
void ThCode:: 
appendTextDocument(const ThTextDocument *text)
{
    if(!programName().isValid()){
        throw ThError(12049, "ThCode appendTextDocument: expecting previously defined programName().  None was defined"); //!<\todo What is 'name' of a TextDocument?
    }
    QTextCursor c= text->find(QRegExp("^code$"));
    if(c.isNull()){
        //!\todo Report identifier for text
        throw ThError(10482, "ThCode appendTextDocument: text is missing 'code' with indented program:\n...'text."); //!<\todo What is 'name' of a TextDocument?
    }
    int codeIndent= c.blockFormat().indent();
    QTextBlock block= c.block().next();
    file_line= c.blockNumber(); //!<\todo Should file_line by lineNumber instead of blockNumber?
    while(block.isValid()){
        int indent= block.blockFormat().indent();
        QString line= block.text();
        if(indent<codeIndent){
            break;
        }else if(indent==codeIndent && line.count()!=0 && !line.at(0).isSpace()){
            break;
        }else if(line.contains(QRegularExpression("^ *th_END"))){
            ThWarn(12095, "ThCode appendTextDocument: early exit due to 'th_END'");
            break;
        }else{
            tryAppendInstruction(line);
        }
        block= block.next();
        ++file_line;
    }
}//appendTextDocument
#endif // TH_NO_THDISK

//! Append Try code for a callTitle or name.field
//! \see appendLabelCode and appendCallCode
void ThCode::
appendTryCode(const QString &argument, const QString &label)
{
    if(ThCode::isRestrictedNameField(argument)){
        OperandIdentifierList operands= addArgument(argument, !IsAllowedThNew);
        ArgType argA= argTypeForOperands(operands);
        uchar c= ThCode::toByteCode(TryA, argA, NoArg);
        program_code.append(c);
        appendOperands(operands);
    }else{
        NameIdentifier callTitle= addName(argument);
        OperandIdentifier operand= addCallOperand(program_name, callTitle);
        addLiteralLabelOperand(operand);
        uchar c= ThCode::toByteCode(TryA, Literal, NoArg);
        program_code.append(c);
        appendOperandIdentifier(operand);
    }
    checkLabelName(label);
    NameIdentifier labelName= addName(label);
    OperandIdentifier labelOperand= addLiteralLabelOperand(program_name, labelName);
    appendOperandIdentifier(labelOperand);
}//appendTryCode

//! Return ArgType/ArgTypeMask for operands
ThCode::ArgType ThCode:: 
argTypeForOperands(const OperandIdentifierList &operands) const
{
    ArgType arg;
    switch(operands.count()){
    case 0:
        arg= NoArg;
        break;
    case 1: 
        arg= isLiteral(operands[0]) ? Literal 
                                    : Field;
        break;
    case 2:
        arg= isLiteral(operands[0]) ? IndirectField  //!\todo Should a[b] => a.b?
           : isLiteral(operands[1]) ? Array 
                                    : ArrayIndex;
        break;
    default:
        throw ThError(12047, "ThCode argTypeForOperands: expecting one or two OperandIdentifiers.  Got %d", operands.count());
    }
    return arg;
}//argTypeForOperands

//! Not const due to getValue()
//! Same as \see isLabelValid
void ThCode::       
checkLabel(OperandIdentifier label)
{
    int labelOffset= getLiteralLabelOffset(label);
    if(labelOffset==NoLabelOffset){
        throw ThError(10384, "ThCode checkLabel: label not defined '%s'", toString(label));
    }
    const char *symptom= "";
    ArgType argA= NoArg;
    if(labelOffset<0 || labelOffset>=program_code.size()-2){
        symptom= "labelOffset is out-of-bounds";  //keep short
    }else{
        char c= program_code[labelOffset];
        Op op= ThCode::toOpCode(c); 
        argA= ArgType((c>>ArgAShift)&ArgTypeMask);
        if(op!=LabelA){
            symptom= "labelOffset is not a labelOp";
        }
    }
    if(*symptom!='\0'){
        NameIdentifier name;
        NameIdentifier field= getNameFieldFromLabelOffset(labelOffset, &name); 
        if(field.isValid()){
            throw ThError(10390, "ThCode checkLabel: expected label operand %d with a ThidId.  Got '%s'.  It appears to be a name-field operand instead of a label operand.  %s.  LabelOffset %d", label, labelOffset, toString(label), symptom);
        }else{
            throw ThError(10382, "ThCode checkLabel: expected label operand %d for label '%s'.  %s.  LabelOffset %d", label, labelOffset, toString(label), symptom);
        }
    }
    OperandIdentifierList labelOperands;
    fillOperands(argA, program_code.constData()+labelOffset+1, &labelOperands);
    if(labelOperands[0]!=label){
        throw ThError(10383, "ThCode checkLabel: Mismatched label.  Expected operand %d at labelOffset %d.  Got operand %lli", label, labelOffset, qint64(labelOperands[0]));
    }
}//checkLabel

// Check syntax of label
// Assumes label is trimmed
void ThCode::       
checkLabelName(const QString &label) const
{
    if(label.contains(QRegularExpression("^(->|!>)|(->|!>)$"))){
        throw ThError(12119, "ThCode checkLabelName: a label cannot start with '->' or '!>' or end with '->' or '!>'.  Got '%s'", label);
    }
}//checkLabels

// only called by ThExecuteCode
void ThCode::       
checkLabels()
{
    foreach (OperandIdentifier label, label_index.values()){
        checkLabel(label);
    }
}//checkLabels

void ThCode::
errorIfFrozen()
{
    if(isFrozen()){
        throw ThError(11879, "ThCode errorIfFrozen: code is frozen.  Cannot change the code");
    }
}//errorIfFrozen

//! Fill operands for getInstruction()
const char * ThCode::
fillOperands(ArgType arg, const char *ip, OperandIdentifierList *operands) const
{
    int count= s_operands_for_arg[arg];
    if(ip+count>program_code.constData()+nextCodeOffset()){
        throw ThError(10387, "ThCode fillOperands: Operands for Argument %d overflow the program", arg);
    }
    operands->clear();
    for(int i= 0; i<count; i++){
        uchar low= uchar(*ip++);
        uchar high= uchar(*ip++);
        int indx= uint(low)+ (uint(high)<<ByteShift);
        if(2*indx>=program_operands.count()){
            throw ThError(10353, "ThCode fillOperands: operand %d for arg %d is out-of-bounds", i, arg);
        }
        operands->append(OperandIdentifier(indx));
    }
    return ip;
}//fillOperands

//! Return OperandIdentifier for first use as a literal or name
//! Returns NoOperand if none
//! Used by appendAssignCode to verify V=... is the first use of V
ThCode::OperandIdentifier ThCode::
firstLiteralOrName(NameIdentifier name)
{
    OperandIdentifier nameResult; 
    OperandIdentifier literalResult; 
    bool isLit= false;
    QHash<NameIdentifierPair, OperandIdentifier>::const_iterator i;
    for (i = operand_index.constBegin(); i != operand_index.constEnd(); ++i){
        const NameIdentifierPair &nameField= i.key();
        OperandIdentifier operand= i.value();
        if(nameField.first==name){
            if(nameResult.isEmpty() || nameResult.toInt()>operand.toInt()){
                nameResult= operand;
            }
        }else if(nameField.first==th_lit && nameField.second==name){
            isLit= true;
        }else if(nameField.first.isEmpty() && nameField.second==th_lit){
            is_literal_thlit= true;
        }else if(nameField.first.isEmpty() && nameField.second==name){
            if(literalResult.isEmpty() || literalResult.toInt()>operand.toInt()){
                literalResult= operand;
            }
        }
    }
    if(is_literal_thlit){
        ThLog(40695, "ThCode firstLiteralOrName: ignoring mixed use of literals and variables.  'th_LIT' was used as a literal, e.g., for --compile core code");
        return nameResult;
    }
    if(isLit){
        ThLog(40694, "ThCode firstLiteralOrName: assume only literal use of '%s' is for 'th_LIT.%s'",0,0, toUtf8(name), toUtf8(name));
        return nameResult;
    }
    OperandIdentifier result= nameResult; 
    if(result.isEmpty() || result.toInt()>literalResult.toInt()){
        result= literalResult;
    }
    return result;
}//firstLiteralOrName

//! Freeze ThCode and disallow further additions
//! Anonymous names may be added via getAssignableValue
//! \see isFrozen
void ThCode::
freezeProgram()
{
    name_index.clear();
    operand_index.clear();
}//freezeProgram

//! non-const due to printOperandList...  (getValue and next_unknown)
void ThCode::
printInstruction(const ThCode::Instruction &i)
{
    switch(i.op()){
    case AssignAB:
        logStream() << i.offset() << " asgn ";
        printOperands(i.operandsA());
        logStream() << "= ";
        printOperands(i.operandsB());
        printOperandLists(i);
        break;
    case Breakpoint:
        logStream() << i.offset() << " brkp\n";
        break;
    case CallA:
        logStream() << i.offset() << " call ";
        printOperands(i.operandsA());
        printOperandLists(i);
        break;
    case Fail:
        logStream() << i.offset() << " erro\n";
        break;
    case GotoA:
        logStream() << i.offset() << " goto ";
        printOperands(i.operandsA());
        printOperandLists(i);
        break;
    case IfEqualsAB:
        logStream() << i.offset() << " ifeq ";
        printOperands(i.operandsA());
        logStream() << " == ";
        printOperands(i.operandsB());
        logStream() << " ->";
        printOperandNames(i.labelOperand());
        printOperandLists(i);
        break;
    case LabelA:
        logStream() << i.offset() << " labl ";
        printOperands(i.operandsA());
        printOperandLists(i);
        break;
    case NOP:
        logStream() << i.offset() << " nop\n";
        break;
    case PrintA:
        logStream() << i.offset() << " prnt ";
        printOperands(i.operandsA());
        printOperandLists(i);
        break;
    case PrintFieldsA:
        logStream() << i.offset() << " dump ";
        printOperands(i.operandsA());
        printOperandLists(i);
        break;
    case PrintComma:
        logStream() << i.offset() << " prco\n";
        break;
    case PrintCommaSpace:
        logStream() << i.offset() << " prcs\n";
        break;
    case PrintNewLine:
        logStream() << i.offset() << " prnl\n";
        break;
    case PrintSpace:
        logStream() << i.offset() << " prsp\n";
        break;
    case Return:
        logStream() << i.offset() << " rtrn\n";
        break;
    case TryA:
        logStream() << i.offset() << " try  ";
        printOperands(i.operandsA());
        logStream() << " ->";
        printOperandNames(i.labelOperand());
        printOperandLists(i);
        break;
    case IfNotEqualsAB:
        logStream() << i.offset() << " ifne ";
        printOperands(i.operandsA());
        logStream() << " != ";
        printOperands(i.operandsB());
        logStream() << " -> ";
        printOperandNames(i.labelOperand());
        printOperandLists(i);
        break;
    default:
        throw ThError(10490, "ThCode printInstruction: Unknown operator %d for instruction ip %d", i.op(), i.offset());
    }
}//printInstruction

//! non-const due to getValue and next_unknown
void ThCode::
printOperandList(OperandIdentifierList operands)
{
    if(operands.count()>0){
        NameIdentifier name;
        NameIdentifier field= getNameField(operands[0], &name);
        logStream() << " {" << operands[0].toInt() << ": " << name.toInt() << "." << field.toInt();
        NameIdentifier value;
        if(hasNameValue(name, field)){
            value= getValue(name, field);
            logStream() << " = " << value.toInt();
        }
        logStream() << "}";
        if(operands.count()>1){
            NameIdentifier nameA;
            NameIdentifier fieldA= getNameField(operands[1], &nameA);
            logStream() << "[" << operands[1].toInt() << ": " << nameA.toInt() << "." << fieldA.toInt();
            NameIdentifier valueA;
            if(hasNameValue(nameA, fieldA)){
                valueA= getValue(nameA, fieldA);
                logStream() << " = " << valueA.toInt();
            }
            NameIdentifier valueB;
            if(value.isValid()){
                // name.field always has a value
                valueB= getValue(value, valueA);
                logStream() << " " << value.toInt() << "." << valueA.toInt() << "= " << valueB.toInt();
            }
            logStream() << "]";
            if(operands.count()>2){
                NameIdentifier nameC;
                NameIdentifier fieldC= getNameField(operands[2], &nameC);
                if(nameC!=NoName){
                    ThWarn(12036, "ThCode printInstruction: expecting a field for operand 2.  Got %d.%d", nameC, fieldC);
                }else{
                    logStream() << "." << fieldC.toInt();
                    if(value.isValid()){
                        NameIdentifier valueC= getValue(valueB, fieldC);
                        logStream() << "  " << valueB.toInt() << "." << fieldC.toInt() << "= " << valueC.toInt();
                    }
                }
                if(operands.count()>3){
                    ThWarn(11863, "ThCode printInstruction: expecting 0..3 operands.  Got %d operands", operands.count());
                }
            }
        }
    }
}//printOperandList


void ThCode::
printOperandLists(Instruction i)
{
    printOperandList(i.operandsA());
    printOperandList(i.operandsB());
    if(!i.labelOperand().isEmpty()){
        NameIdentifier name;
        NameIdentifier field= getNameField(i.labelOperand(), &name);
        logStream() << " {label-" << i.labelOperand().toInt() << ": " << name.toInt() << "." << field.toInt() << "}";
    }
    logStream() << "\n";
}//printOperandLists

void ThCode::
printOperandNames(OperandIdentifier operand) const
{
    NameIdentifier name;
    NameIdentifier field= getNameField(operand,  &name);
    if(name.isEmpty()){
        logStream() << toUtf8(field);
    }else if(field.isEmpty()){
        logStream() << toUtf8(name) << ".?";
    }else{
        logStream() << toUtf8(name) << "." << toUtf8(field);
    }
}//printOperandNames

void ThCode::
printOperands(const OperandIdentifierList &operands) const
{
    switch(operands.count()){
    case 1:
        printOperandNames(operands[0]);
        break;
    case 2:
        printOperandNames(operands[0]);
        logStream() << "[";
        printOperandNames(operands[1]);
        logStream() << "]";
        break;
    case 3:
        printOperandNames(operands[0]);
        logStream() << "[";
        printOperandNames(operands[1]);
        logStream() << "].";
        printOperandNames(operands[2]);
        break;
    }
}//printOperands

//! \see executeProgram, writeNameCode
void ThCode::
printProgram(int start)
{
    logStream() << "== program ==\n";
    logStream() << "ipOffset op operands operandIds [results].  Ids are {operandId: nameId.fieldId = valueId}\n";
    ThCode::Instruction i(this, start);
    while(i.isValid()){
        printInstruction(i);
        i.nextInstruction();
    }
}//printProgram

//! Rewrite instruction with longest [[th_NAME]]
//! Sets th_name_value and clears th_name_count
//! Allows arbitrary names
QString ThCode:: 
replaceLiteralWithThName(const QString &s)
{
    QString result= s.trimmed();
    if(!result.startsWith("//")){           // ignore comments
        int firstBrace= result.indexOf("[[");
        int lastBrace= result.lastIndexOf("]]");
        if(firstBrace!=NoIndex && lastBrace!=NoIndex && firstBrace+2<lastBrace){
            QString prefix= result.left(firstBrace);
            QString suffix= result.right(result.count()-lastBrace-2);
            QString testResult= prefix + th_NAME + suffix;
            QString testName= result.mid(firstBrace+2, lastBrace-firstBrace-2);
            bool isMultiInstructions= false;
            int firstComment= testResult.indexOf("//");
            if(firstComment!=NoIndex && testResult.lastIndexOf("//")>=firstComment+2){  // Cannot use count() due to overlapping instances.
                isMultiInstructions= true;
            }
            bool isName= true;
            if(testName.contains("//")){
                if(isMultiInstructions){
                    throw ThError(12078, "ThCode replaceLiteralWithThName: cannot mix multiple instructions with [[literal // characters]] -- '%s'", result);
                }
                // literal [[.. // .. ]] must include full-token.  May be in comment ...//... [[kjsdlfj//lkjsdf]]
            }else if(testResult.endsWith("->")){
                isName= false; // labels are literal anyway
            }else if(testResult.contains(QRegularExpression(".[=!]=.*->.*th_NAME"))){
                isName= false; // labels are literal anyway
            }
            if(isName){
                if(isMultiInstructions){
                    // appendInstruction will recall replaceLiteralWithThName() for each instruction
                }else if(firstComment!=NoIndex && firstComment<firstBrace){
                    // replace [[...]] in comment with th_NAME (it may include '//')
                    result= testResult; 
                }else{ 
                    result= testResult;
                    th_name_value= testName;
                    th_name_count= 0;
                }
            }
        }
    }
    return result;
}//replaceLiteralWithThName

void ThCode::
resolveCall(Op op, int codeOffset, OperandIdentifier label)
{
    int labelOffset= getLiteralLabelOffset(label);
    if(labelOffset==NoLabelOffset){ //label is not in code
        NameIdentifier thidIdentifier;
        NameIdentifier title= getNameField(label, &thidIdentifier);
#ifdef TH_NO_THDISK
        ThError e(12106, "ThCode resolveCall: external calls not implemented.  Define '%s' for %s at instruction %d", codeOffset,0, toUtf8(title), (op==Op::CallA ? "call" : "try")); //!<\todo Implement resolveCall for Thids
        if(isIgnoreErrors()){
            ++error_count;
            ThError::warn(e);
        }else{
            throw e;
        }
#else
        Q_UNUSED(codeOffset)
        Q_UNUSED(op)
        if(!thidIdentifier.isEmpty()){  // label is thidIdentifer.title as set by addCallOperand via getThidByLabel
            ThidId id(toUtf8(thidIdentifier));
            Thid *thid= thid_disk->getThidById(id);
            QString thidLabel= thid->label();
            NameIdentifier labelTitle= getName(thidLabel);
            if(title==labelTitle){
                ThCall call(40691, "ThCode resolveCall: append program for %s '%s'",0,0, id, thidLabel);
                int ip= appendProgram(thid, title);  //title is the title for id
                Q_UNUSED(ip); // ip is label for title
            }else{
                QString titleString= toUtf8(title);
                //!\todo Review calls to unknown thids.  Should they be caught in addCallOperand or caught here?
                ThLog(40692, "ThCode resolveCall: expecting Thid for '%s' but not found in addCallOperand.  Caller was '%s'",0,0, titleString, thidLabel);
            }
        }
#endif // TH_NO_THDISK
    }
}//resolveCall

//! resolve calls from start to end, including loaded programs
void ThCode::
resolveCalls(int start)
{
    ThCall call(40689, "ThCode resolveCalls: resolve calls in program at %d", start);
    ThCode::Instruction i(this, start);
    if(i.op()!=LabelA){
        //!\todo Allow a program without a leading label
        throw ThError(10486, "ThCode resolveCalls: expecting label ('...->') at start of program.  Got opCode 0x%X", i.op());
    }
    NameIdentifier program; // for error reporting
    NameIdentifier field= getNameField(i.operandsA()[0], &program); //!\todo -- does getNameField replace the old code -- loadNameField(i.argTypeA(), i.operandsA(), &programName);
    Q_UNUSED(field);
    NameIdentifier name;
    int errorCount= 0;
    while(i.isValid()){
        if(i.op()==CallA || i.op()==TryA){
            if(i.argTypeA()==ThCode::Literal){
                OperandIdentifier label= i.operandsA().at(0);
                try{
                    resolveCall(i.op(), i.offset(), label);
                }catch(const ThError &e){
                    ThError::warn(e);
                    ++errorCount;
                }
            }
        }
        i.nextInstruction();
    }
    if(errorCount!=0){
        throw ThError(12037, "ThCode resolveCalls: reported %d errors in %s", errorCount,0, toUtf8(program));
    }
}//resolveCalls

//! Return QStringList for an instruction argument as 1 or 2 pairs of strings
//! N, N.N, R[R] with first '.' (not ..), first '[' and last ']'
//! Sets isArray if defined and argument is '..[..]'
//! Errors if non-conforming argument
//! Assumes argument is trimmed()
QStringList ThCode:: 
splitArgument(const QString &argument, bool *isArray/*= NULL*/)
{
    if(argument.isEmpty()){
        //!\todo Report context for "Empty argument" from splitArgument
        throw ThError(10465, "ThCode splitArgument: Empty argument for label, assignment, goto, or ifEquals.  Can not parse");
    }
    if(isArray!=NULL){
        *isArray= false;
    }
    QStringList references;
    if(argument.endsWith("]")){
        int start= (argument.startsWith(QChar('[')) ? 1 : 0);
        references= RoadUtility::splitFirst(argument, QChar('['), start);
        if(references.count()==2){
            references[1].chop(1);
            if(references.at(1).trimmed().isEmpty()){
                throw ThError(10354, "ThCode splitArgument: Empty index for '%s[...]'.  Can not parse", references.at(0));
            }
            if(references.at(0).trimmed().isEmpty()){
                throw ThError(10034, "ThCode splitArgument: Empty name for '...[%s]'.  Can not parse", references.at(1));
            }
            if(isArray!=NULL){
                *isArray= true;
            }
        } //else references.count()==1
    }else{
        references << argument;
    }
    QStringList result;
    bool wasName= false;  // True if not 'name.field'
    foreach(QString reference, references){
        //reference cannot be empty by TH10465, TH10354, and TH10034
        //!\todo review leading '.' and '[' as a name character -- make it an error?
        int fieldIndex= reference.indexOf(QRegularExpression("[^.]\\.[^.]"));
        if(fieldIndex==NoIndex){
            if(wasName){
                result= QStringList() << result.at(1) << reference.trimmed();
                // must be references.at(1) with references.count()==2
            }else{
                result << QString("") << reference.trimmed();
                wasName= true;
            }
        }else{
            ++fieldIndex; // skip first char
            QString name= reference.left(fieldIndex).trimmed();
            QString field= reference.mid(fieldIndex+1).trimmed();
            if(name.isEmpty()){
                throw ThError(12062, "ThCode splitArgument: name.field without name in '%s'.  Can not parse", argument);
            }
            if(field.isEmpty()){
                throw ThError(12118, "ThCode splitArgument: name.field without field in '%s'.  Can not parse", argument);
            }
            result << name << field;
        }
    }
    return result;
}//splitArgument

void ThCode::
tryAppendInstruction(const QString &line)
{
    try{
        appendInstruction(line);
    }catch(const ThError &e){
        ThLog(40696, RoadEvent::Always, "ThCode tryAppendInstruction: error at instruction %d\n%s(%d):%s", program_code.size(), file_line, file_name, line);
        ThError::warn(e);
        ++error_count;
    }
}//tryAppendInstruction


#//!\name Class methods

//! True if a plain name w/o ' ', '.', '=', '[', or ']'
bool ThCode::
isPlainName(const QString &name)
{
    bool result= false;
    QString s= name.trimmed();
    if(!s.isEmpty() && !s.contains(QRegularExpression(" |=|\\.|\\[|\\]"))){ //!\todo Should be a defined constant
        result= true;
    }
    return result;
}//isPlainName

bool ThCode::
isReservedName(const QString &name)
{
    if(name.startsWith(th_RESERVED_PREFIX)){
        int i= th_RESERVED_PREFIX.size();
        if(name.size()>i && (name.at(i).isUpper() || name.at(i).isDigit())){
            return true;
        }
    }
    return false;
}//isReservedName

//! Return next name [static]
//! "" => "-1"
//! "...0000" => "...0001"
//! "...9999" => "...10000"
//! "..." => "...2"
QString ThCode::
nextName(const QString &name)
{
    QString result;
    if(name.isEmpty()){
        result= th_NEXT_NEW;
    }else{
        int last= name.size()-1;
        if(name.at(last).isDigit()){
            while(last>=0 && name.at(last)=='9'){
                --last;
            }
            int n= name.size()-last;
            if(last==-1){
                result= "1" + QString(n+1, '0');
            }else if(name.at(last).isDigit()){
                QChar nextDigit(name.at(last).unicode()+1);
                if(last==0){
                    result= nextDigit + QString(n-1, '0');
                }else{
                    result= name.left(last) + nextDigit + QString(n-1, '0');
                }
            }else{
                result= name.left(last+1) + '1' + QString(n+1, '0');
            }
        }else{
            result= name+"2";
        }
    }
    return result;
}//nextName

//! Convert opCode, argA, and argB into a byteCode [static]
uchar ThCode::
toByteCode(ThCode::Op opCode, ThCode::ArgType argA, ThCode::ArgType argB)
{
    if(argA<0 || argA>ArgTypeMask || argA==1 || argB<0 || argB>ArgTypeMask || argB==1){
        throw ThError(12074, "ThCode toByteCode: expecting argType 0 or 0x02..0x%x.  Got argA 0x%X argB 0x%X", argA, argB, quint64(ArgTypeMask));
    }
    uchar c;
    if(argA==NoArg){
        if(argB==NoArg){
            c= opCode;
        }else{
            c= (opCode&OpArgBMask)+argB;
        }
    }else if(argB==NoArg){
        c= (opCode&OpArgAMask)+(argA<<ArgAShift);
    }else{
        c= (opCode&OpArgABMask)+(argA<<ArgAShift)+argB;
    }
    return c;
}//toByteCode

//! Convert byteCode into opCode with argA, argB, or argAB as needed for arguments [static]
//! same as getInstruction
ThCode::Op ThCode::
toOpCode(uchar byteCode)
{
    uchar argFlags= byteCode&OpNoArg;
    if(argFlags==0){
        return Op(byteCode);
    }else if((argFlags&ArgTypeMask)==0){
        return Op((byteCode&OpArgAMask)+OpArgA);
    }else if((argFlags>>ArgAShift)==0){
        return Op((byteCode&OpArgBMask)+OpArgB);
    }else{
        return Op((byteCode&OpArgABMask)+OpArgAB);
    }
}//toOpCode

}//namespace Thesa

