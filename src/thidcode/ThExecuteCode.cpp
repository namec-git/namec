/****************************************************************************
**
** Copyright (C) 2011-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThExecuteCode.cpp#20 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#include "thidcore/thesa.h"

#include "thidcode/ThExecuteCode.h"

#include "road/RoadEventStack.h"

#ifndef TH_NO_THDISK
#include "thidcore/ThSettings.h"
#endif

#if defined(Q_CC_MSVC)  // Disable warning for 0x80 as type 'char'
#pragma warning(disable: 4996) /* 'sprintf': This function or variable may be unsafe */
#endif

namespace Thesa {

#//!\name Constructors, etc.

ThExecuteCode::
ThExecuteCode(ThDisk *disk, QObject *parent/*= 0*/)
: ThCode(disk, parent)
, log_level(0)
, next_name(0)
, next_unknown(MaximumInt)
, name_values()
, unknown_values()
{
    int defaultLogLevel= RoadEventStack::currentLogLevel();
#ifdef TH_NO_THDISK
    log_level= defaultLogLevel;
#else
    log_level= ThSettings::intValue("codeLevel", defaultLogLevel); // LogAll logs ops, LogDetail logs opPrint, etc
#endif
}//constructor


#//!\name GetSet

//! True if known value for field in name
bool ThExecuteCode::
hasNameValue(NameIdentifier name, NameIdentifier field) const
{
    if(name.isValid() && name<name_values.count()){
        const NameValues &values= name_values[name];
        return values.contains(field);
    }
    return false;
}//hasNameValue

#//!\name Methods

void  ThExecuteCode::
assignValue(NameIdentifier name, NameIdentifier field, NameIdentifier value)
{
    if(name.isEmpty() || field.isEmpty()){
        throw ThError(10366, "ThExecuteCode assignValue: Can not assign to empty name %d or field %d", name, field);
    }
    while(name>=name_values.count()){
        name_values.append(NameValues());
    }
    NameValues &values= name_values[name];
    values.insert(field, value);
}//assignValue

//! Not used.  \todo executeInstruction does not assign programName().  Execute requires a label
bool ThExecuteCode::
executeInstruction(QString line)
{
    int instruction= nextCodeOffset();
    appendInstruction(line);
    appendInstruction("==");
    return executeProgram(instruction);
}//executeInstruction

//! Return true on success, false on failure
//! Defines programName() if 'start' is a label
//! \see writeAsmInstruction
bool ThExecuteCode::
executeProgram(int start/*= 0*/)
{
    ThCall call(40690, RoadEvent::Detail, "ThExecuteCode executeProgram: execute Program at %d", start);    
    checkLabels();  //!\todo checkLabels() only needed for new labels -- Keep a count of undefined labels instead.

    clearProgramName();   //!<\todo Review use of clearProgramName().  Should it be part of every scan?
    ThExecuteCode::Instruction i(this, start);
    if(i.op()==LabelA){
        //!\todo Should LabelA be required?   Otherwise programName() is carried forward from caller
        //!\todo Should Field be required for LabelA?   What does a variable programName() and field mean? 
        NameIdentifier program;
        NameIdentifier field= loadNameField(i.argTypeA(), i.operandsA(), &program);
        Q_UNUSED(field);  //!\todo 'field' is the title for a program.  Should it be recorded?
        setProgramName(program);
    }

    bool isFailed= false;
    while(i.isValid()){
        NameIdentifier nameA;
        NameIdentifier fieldA;
        NameIdentifier nameB;
        NameIdentifier fieldB;

        if(next_unknown!=MaximumName){  // Reset unknown names for each instruction
            next_unknown= MaximumName;
            unknown_values.clear();
        }
        switch(i.op()){
            case AssignAB:
                {
                    fieldA= loadAssignableNameField(i.argTypeA(), i.operandsA(), &nameA);
                    fieldB= getNameField(i.operandsB().first(), &nameB);
                    NameIdentifier valueB;
                    if(i.argTypeB()==Literal){
                        if(fieldB==thNew()){ 
                            valueB= newNameWithValues(); 
                        }else{
                            valueB= fieldB;
                        }
                    }else if(i.argTypeB()==Field && nameB==thInc()){            // A= th_INC[fieldB]
                        //!\todo th_INC only implemented for Assign.  How about IfEquals and Print?  
                        if(hasNameValue(thInc(), fieldA)){
                            valueB= getValue(thInc(), fieldA);
                        }else{
                            valueB= newIncValue(fieldB);
                            assignValue(thInc(), fieldA, valueB);
                        }
                    }else if(i.argTypeB()==IndirectField && fieldB==thInc()){   // A= th_INC[nameA.fieldB]
                        fieldB= getNameField(i.operandsB().at(1), &nameB);
                        NameIdentifier fieldValue= getAssignableValue(nameB, fieldB);
                        if(hasNameValue(thInc(), fieldValue)){
                            valueB= getValue(thInc(), fieldValue);
                        }else{
                            valueB= newIncValue(fieldValue);
                            assignValue(thInc(), fieldValue, valueB);
                        }
                    }else{
                        fieldB= loadAssignableNameField(i.argTypeB(), i.operandsB(), &nameB);
                        valueB= getAssignableValue(nameB, fieldB);
                    }
                    opAssign(i.offset(), nameA, fieldA, valueB);
                }
                break;
            case Breakpoint:
                opBreakpoint(i.offset());
                break;
            case CallA:
                //!\todo How does a Literal become thid-...-th-....title?  Looks like a Literal label is N.F
                if(i.argTypeA()!=Literal){  // not possible, checked by appendCallCode
                    throw ThError(10478, "ThExecuteCode executeProgram: 'call' is only defined for literal names.  Use 'try' (... ->label) for indexed access.");
                }
                fieldA= loadNameField(i.argTypeA(), i.operandsA(), &nameA);
                if(!opCall(i.offset(), nameA, fieldA)){
                    isFailed= true;   //!\todo trap errors for opCall
                    break;
                }
                break;
            case CatchB:
            case ThrowB:
                throw ThError(12075, "ThExecuteCode executeProgram: catch and throw not implemented.  Op 0x%X at instruction %d", i.op(), i.offset());
            case Fail:
                opReturnError(i.offset());
                isFailed= true;  
                break;
            case GotoA:
                {
                    fieldA= loadAssignableNameField(i.argTypeA(), i.operandsA(), &nameA);
                    NameIdentifier labelField;
                    if(nameA==programName()){
                        if(isVariable(nameA, fieldA)){ 
                            labelField= getAssignableValue(nameA, fieldA);
                        }else{
                            labelField= fieldA;
                        }
                    }else{
                        labelField= getAssignableValue(nameA, fieldA);
                    }
                    int gotoOffset= opGotoLabel(i.offset(), programName(), labelField);
                    if(gotoOffset>=0){
                        i.setNextOffset(gotoOffset);
                    }else{
                        isFailed= true;  
                        break;
                    }
                }
                break;
            case IfEqualsAB:
            case IfNotEqualsAB:
                {
                    fieldA= loadNameField(i.argTypeA(), i.operandsA(), &nameA);
                    fieldB= loadNameField(i.argTypeB(), i.operandsB(), &nameB);
                    NameIdentifier valueB= getValue(nameB, fieldB);
                    int gotoOffset= opIfEquals(i.offset(), i.op(), nameA, fieldA, valueB, i.labelOperand());
                    if(gotoOffset>=0){
                        i.setNextOffset(gotoOffset);
                    }else if(gotoOffset==FailReturn){
                        //do nothing if fails test
                    }else if(gotoOffset==ErrorReturn){
                        isFailed= true;  
                        break;
                    }else{
                        throw ThError(10475, "ThExecuteCode executeProgram: Negative return (%d) from ifUnlessGoto at ip %d", gotoOffset, i.offset());
                    }
                }
                break;
            case LabelA:
                fieldA= loadNameField(i.argTypeA(), i.operandsA(), &nameA);
                opLabel(i.offset(), nameA, fieldA);
                break;
            case NOP:
                break;
            case PrintA:
                fieldA= loadNameField(i.argTypeA(), i.operandsA(), &nameA);
                opPrint(i.offset(), nameA, fieldA);
                break;
            case PrintComma:
                OutStream << ',';
                break;
            case PrintCommaSpace:
                OutStream << ", ";
                break;
            case PrintFieldsA:
                fieldA= loadNameField(i.argTypeA(), i.operandsA(), &nameA);
                opPrintFields(i.offset(), nameA, fieldA);
                break;
            case PrintNewLine:
                OutStream << '\n';
                break;
            case PrintSpace:
                OutStream << ' ';
                break;
            case Return:
                opReturn(i.offset());
                return true;
            case TryA:
                {
                    if(i.argTypeA()==Literal){
                        fieldA= loadNameField(i.argTypeA(), i.operandsA(), &nameA);
                    }else{
                        fieldA= loadAssignableNameField(i.argTypeA(), i.operandsA(), &nameA);
                        NameIdentifier indirectLabel= getAssignableValue(nameA, fieldA);
                        nameA= NameIdentifier(programName());  //!\todo what is programName for indirect Try to an external routine?
                        fieldA= indirectLabel;
                    }
                    int gotoOffset= opTry(i.offset(), nameA, fieldA, i.labelOperand());
                    if(gotoOffset>=0){
                        i.setNextOffset(gotoOffset);
                    }else if(gotoOffset==FailReturn){
                        //do nothing if fails test
                    }else if(gotoOffset==ErrorReturn){
                        isFailed= true;  
                        break;
                    }else{
                        throw ThError(10476, "ThExecuteCode executeProgram: negative return (%d) for try...success at ip %d", gotoOffset, i.offset());
                    }
                }
                break;
            default:
                throw ThError(10480, "ThExecuteCode executeProgram: Unknown operator 0x%X for instruction ip %d", i.op(), i.offset());
        }
        if(isFailed){
            ThLog(40693, "ThExecuteCode executeProgram: return false from program %d %s", start,0, toUtf8(programName())); //!<\todo Could track the reason for failure, but it appears to be obvious
            return false;
        }
        i.nextInstruction();
    }
    return true;
}//executeProgram

//! Return a value for assigning to a name.field
//! Creates a new name if needed
ThExecuteCode::NameIdentifier ThExecuteCode::
getAssignableValue(NameIdentifier name, NameIdentifier field)
{
    if(field.isEmpty()){
        throw ThError(10032, "ThExecuteCode getAssignableValue: empty fields do not have a (assignable) value.  Name is %d", name);
    }
    if(name.isEmpty()){
        return field;  // e.g., 'y' in A.x[y]= c
    }
    if(field.toInt()>next_unknown || name.toInt()>next_unknown){
        throw ThError(10033, "ThExecuteCode getAssignableValue: unknown fields (%d) or names (%d) do not have an assignable value.", field, name);
    }
    while(name>=name_values.count()){
        name_values.append(NameValues());
    }
    NameValues &values= name_values[name];
    NameIdentifier result= values.value(field);
    if(!result){
        if(isWellFormedProgram()){
            throw ThError(10376, "ThExecuteCode getAssignableValue: expecting assignable value for '%s.%s' (%d,%d).  None defined", name, field, toUtf8(name), toUtf8(field));
        }else{
            result= newNameWithValues(); 
            values.insert(field, result);
        }
    }
    return result;
}//getAssignableValue

ThExecuteCode::NameIdentifier ThExecuteCode::
getAssignableValue(OperandIdentifier operand)
{
    NameIdentifier name;
    NameIdentifier field= getNameField(operand, &name);
    return getAssignableValue(name, field);
}//getAssignableValue OperandIdentifier

//! Returns value for name.field
//! Not valid for a LabelA with a programName()
//! If undefined, returns an new, temporarily unknown value from unknown_values;
//! Unknown values exist for an instruction only, they may not be assigned as a name, field, or value (getAssignableValue)
ThCode::NameIdentifier ThExecuteCode::
getValue(NameIdentifier name, NameIdentifier field)
{
    NameIdentifier result;
    if(!field){
        throw ThError(10470, "ThExecuteCode getValue: empty fields do not have a value.  Name is %d", name);
    }
    if(!name){
        result= field;
    }else if(name>=name_values.count()){
        result= NameIdentifier(NoName);
    }else{
        const NameValues &values= name_values[name];
        result= values.value(field);
    }
    if(result.isEmpty()){
        NameIdentifierPair p= qMakePair(name, field);
        result= unknown_values.value(p);
        if(result.isEmpty()){
            result= NameIdentifier(next_unknown--);
            unknown_values.insert(p, result);
        }
    }
    return result;
}//getValue

ThCode::NameIdentifier ThExecuteCode::
getValue(OperandIdentifier operand)
{
    NameIdentifier name;
    NameIdentifier field= getNameField(operand, &name);
    return getValue(name, field);
}//getValue OperandIdentifier

ThCode::NameIdentifier ThExecuteCode::
getValue(const QString &argument)
{
    NameIdentifier name;
    NameIdentifier field;
    NameIdentifier value;
    NameIdentifier offset;
    NameIdentifier result;
    OperandIdentifierList operands= addArgument(argument, !ThCode::IsAllowedThNew);
    switch(operands.count()){
        case 1:
            result= getValue(operands[0]);
            break;
        case 2:
            name= getValue(operands[0]);
            field= getValue(operands[1]);
            result= getValue(name, field);
            break;
        case 3:
            name= getValue(operands[0]);
            field= getValue(operands[1]);
            offset= getValue(operands[2]);
            value= getValue(name, field);
            result= getValue(value, offset);
            break;
        default:
            throw ThError(10474, "ThExecuteCode  getValue: too many components (%d).  Expecting R, R[R], or R[R].N.  Got '%s'", argument);
    }
    return result;
}//getValue QString

//! Return names of name and field.  Name may be NoName.  
ThExecuteCode::NameIdentifier ThExecuteCode::
loadAssignableNameField(ArgType arg, OperandIdentifierList operands, NameIdentifier *name)
{
    NameIdentifier field;
    switch(arg){
    case Array:
        *name= getAssignableValue(operands[0]);
        field= getAssignableValue(operands[1]);
        break;
    case ArrayIndex:
        *name= getAssignableValue(operands[0]);
        field= getAssignableValue(operands[1]);
        break;
    case Field:
        field= getNameField(operands[0], name);
        break;
    case IndirectField:
        {
            NameIdentifier nullName;
            *name= getNameField(operands[0], &nullName);
            if(!nullName.isEmpty() || (*name).isEmpty()){
                throw ThError(10386, "ThExecuteCode loadAssignableNameField: IndirectField is name %d or empty field %d", nullName, *name);
            }
            field= getAssignableValue(operands[1]);
        }
        break;
    case Literal:
        field= getNameField(operands[0], name);
        break;
    }
    return field;
}//loadAssignableNameField

//! Return names of name and field.  Name may be NoName.  
//! Not const due to getValue
ThExecuteCode::NameIdentifier ThExecuteCode::
loadNameField(ArgType arg, OperandIdentifierList operands, NameIdentifier *name)
{
    NameIdentifier field;
    switch(arg){
    case Array:
        *name= getValue(operands[0]);
        field= getValue(operands[1]);
        break;
    case ArrayIndex:
        *name= getValue(operands[0]);
        field= getValue(operands[1]);
        break;
    case Field:
        field= getNameField(operands[0], name);
        break;
    case IndirectField:
        {
            NameIdentifier nullName;
            *name= getNameField(operands[0], &nullName);
            field= getValue(operands[1]);
        }
        break;
    case Literal:
        field= getNameField(operands[0], name);
        break;
    }
    return field;
}//loadNameField

void ThExecuteCode::
log(int level, int code, const char * fmt, int ipOffset)
{
    if(log_level>=level){
        ThLog(code, fmt, ipOffset);
    }
}//log

void ThExecuteCode::
log(int level, int code, const char * fmt, int ipOffset, int intA)
{
    if(log_level>=level){
        ThLog(code, fmt, ipOffset, intA);
        ThExecuteCode::Instruction i(this, ipOffset);
        printInstruction(i);
    }
}//log

void ThExecuteCode::
log(int level, int code, const char * fmt, int ipOffset, int intA, long long longA)
{
    if(log_level>=level){
        ThLog(code, fmt, ipOffset, intA, longA);
        ThExecuteCode::Instruction i(this, ipOffset);
        printInstruction(i);
    }
}//log

void ThExecuteCode::
log(int level, int code, const char * fmt, int ipOffset, int intA, int intB, int intC)
{
    if(log_level>=level){
        ThExecuteCode::Instruction i(this, ipOffset);
        printInstruction(i);
        ThLog(code, fmt, ipOffset, intA, qint64(intB), qint64(intC));
        QString extra;
        if(intA>0 && intA<nameCount()){
            extra.append("execute: ").append(toUtf8(NameIdentifier(intA)));
            if(intB>0 && intB<nameCount()){
                if(intC>0 && intC<nameCount()){
                    extra.append(".").append(toUtf8(NameIdentifier(intB)));
                    extra.append("= ").append(toUtf8(NameIdentifier(intC)));
                }else{
                    extra.append(", ").append(toUtf8(NameIdentifier(intB)));
                }
            }
            logStream() << extra << "\n";
        }
    }
}//log

//! Log code 20001..20015
void ThExecuteCode::
log(int level, int code, const char * fmt, int ipOffset, int intA, float floatA, long long longA)
{
    if(log_level>=level){
        ThLog(code, fmt, ipOffset, intA, floatA, longA);
        ThExecuteCode::Instruction i(this, ipOffset);
        printInstruction(i);
    }
}//log

//! Return a new value as increment of field
//! Implements th_INC[i]
ThCode::NameIdentifier ThExecuteCode::
newIncValue(NameIdentifier field)
{
    QByteArray name= toUtf8(field);
    QString newName= ThCode::nextName(name);
    NameIdentifier oldName= getName(newName);
    if(oldName.isValid()){
        throw ThError(12089, "ThExecuteCode newIncValue: expecting new value for th_INC[%s].  '%s' is already in use as name %d", oldName,0, name, newName);
    }
    // duplicates newNameWithValues 
    NameIdentifier result= addName(newName);
    while(result>=name_values.count()){
        name_values.append(NameValues());  // Creates a NameValues for all of the names, but these are empty (one pointer)
    }
    return result;
}//newIncValue

ThCode::NameIdentifier ThExecuteCode::
newNameWithValues()
{
    char name[12];
    sprintf(name, "n%.4d", ++next_name);  //!< \todo check for next_name overflow, no intrinsic limit
    NameIdentifier result= addName(QString(name));
    while(result>=name_values.count()){
        name_values.append(NameValues());  // Creates a NameValues for all of the names, but these are empty (one pointer)
    }
    return result;
}//newNameWithValues

void ThExecuteCode::
opAssign(int ipOffset, NameIdentifier name, NameIdentifier field, NameIdentifier value)
{
    log(LogAll, 20001, "execute: %d Assign %d.%lli= %lli", ipOffset, name, field, value);
    assignValue(name, field, value);
}//opAssign

void ThExecuteCode::
opBreakpoint(int ipOffset)
{
    log(LogAll, 20002, "execute: %d Breakpoint", ipOffset);
    //!\todo Implement opBreakpoint
}//opBreakpoint

bool ThExecuteCode::
opCall(int ipOffset, NameIdentifier name, NameIdentifier field)
{
    log(LogAll, 20003, "execute: %d Call %d.%lli", ipOffset, name, field, NoName);
    int labelCall= getLiteralLabelOffset(name, field);
    if(labelCall>=0){
        NameIdentifier saveProgram= programName();  //!\todo use an object to save restore programName()?
        bool status= executeProgram(labelCall);
        setProgramName(saveProgram);
        return status;
    }
    opPrint(ipOffset, NameIdentifier(NoName), field);  //!\todo What should opCall do on missing target?  Was 'name' (aka program) defined?
    return true;
}//opCall

int ThExecuteCode::
opGotoLabel(int ipOffset, NameIdentifier program, NameIdentifier field)
{
    log(LogAll, 20005, "execute: %d Goto %d.%lli", ipOffset, program, field, NoName);

    int labelGoto= getLiteralLabelOffset(program, field);
    if(labelGoto==ErrorReturn){
        opPrint(ipOffset, NameIdentifier(NoName), field);
    }
    return labelGoto;
}//opGoto

//! Return label's offset if true
//! Return ErrorReturn (-1) if label does not exist
//! Return FailReturn (-2) if false
//! Returns false if both values are undefined.  Incorrect for a.x==a.x
int ThExecuteCode::
opIfEquals(int ipOffset, Op op, NameIdentifier name, NameIdentifier field, NameIdentifier value, OperandIdentifier label)
{
    bool result;
    NameIdentifier valueA= getValue(name, field);
    if(value==thNew()){
        if(valueA>next_unknown){
            valueA= value;
        }
    }
    if(op==IfEqualsAB){
        result= (valueA==value);
        log(LogAll, 20007, "execute: %d IfEquals %d==%lli ->%lli", ipOffset, valueA,  value, label);
    }else{
        result= (valueA!=value);
        log(LogAll, 20008, "execute: %d IfNotEquals %d!=%lli ->%lli", ipOffset, valueA,  value, label);
    }
    if(result){
        NameIdentifier labelName;
        NameIdentifier labelField= getNameField(label, &labelName); //!<\todo IfEquals/IfNotEquals is programName().label while goto/call is implicit programName() 
        if(labelName!=programName()){
            //!\todo IfEquals/IfNotEquals only allows literal labels.  Disallow name.field?
            labelField= getValue(labelName, labelField);
        }
        int labelGoto= getLiteralLabelOffset(programName(), labelField);
        if(labelGoto==ErrorReturn){
            opPrint(ipOffset, programName(), labelField);
        }
        return labelGoto;
    }
    return FailReturn;
}//opIfEquals

void ThExecuteCode::
opLabel(int ipOffset, NameIdentifier program, NameIdentifier field)
{
    log(LogAll, 20009, "execute: %d Label %d.%lli", ipOffset, program, field, NoName);
    ThLog(40697, RoadEvent::Steps, "ThExecuteCode opLabel: %s", toUtf8(field));
}//opLabel

void ThExecuteCode::
opPrint(int ipOffset, NameIdentifier name, NameIdentifier field)
{
    log(LogAll, 20010, "execute: %d Print %d.%lli", ipOffset, name, field, NoName);
    NameIdentifier value= getValue(name, field);
    printValue(value);
}//opPrint

void ThExecuteCode::
opPrintFields(int ipOffset, NameIdentifier name, NameIdentifier field)
{
    log(LogAll, 20004, "execute: %d PrintFields %d.%lli", ipOffset, name, field, NoName);
    printNameFieldValue(name, field);
}//opPrintFields

void ThExecuteCode::
opReturn(int ipOffset)
{
    log(LogAll, 20012, "execute: %d Return", ipOffset);
}//opReturn

void ThExecuteCode::
opReturnError(int ipOffset)
{
    log(LogAll, 20014, "execute: %d Return error", ipOffset);
}//opReturnError


//! Return label's offset if call succeeds
//! Return ErrorReturn (-1) if label does not exist
//! Return FailReturn (-2) if call returns an error (i.e., fails)
int ThExecuteCode::
opTry(int ipOffset, NameIdentifier name, NameIdentifier field, OperandIdentifier label)
{
    log(LogAll, 20015, "execute: %d Try %d.%lli ->%lli", ipOffset, name, field, label);

    int gotoOffset;
    try{
        if(opCall(ipOffset, name, field)){    //!<\todo Call outside of current program?
            gotoOffset= getLiteralLabelOffset(label);   //!<\todo try/if/unless is programName().label while goto/call is implicit programName() 
            if(gotoOffset==ErrorReturn){
                opPrint(ipOffset, NameIdentifier(NoName), getValue(label));
            }
        }else{
            gotoOffset= FailReturn;
        }
    }catch(const ThError &e){
        if(isIgnoreErrors()){
            ThError::warn(e);
            gotoOffset= FailReturn;
        }else{
            throw;
        }
    }
    return gotoOffset;
}//opTry

void ThExecuteCode::
printNameFields(NameIdentifier name) const
{
    if(name.isEmpty()){
        OutStream << "NoName\n";
    }else if(name==NoLabel){
        OutStream << "NoLabel\n";
    }else if(name<0){
        OutStream << "NegativeName(" << name << ")\n";
    }else if(name>=name_values.count()){  // includes unknown names
        printValue(name);
        OutStream << "-> contains no Fields\n";
    }else{
        const NameValues &r= name_values[name];
        if(r.isEmpty()){
            OutStream << toUtf8(name) << "-> isEmpty\n";
        }else{
            OutStream << toUtf8(name) << "->\n";
            foreach(NameIdentifier field, r.keys()){
                OutStream << "    " << toUtf8(field) << "-> " << toUtf8(r.value(field)) << "\n";
            }
        }
    }
}//printNameFields

void ThExecuteCode::
printNameFieldValue(NameIdentifier name, NameIdentifier field)
{
    if(name.isEmpty()){
        printNameFields(field);
    }else if(field.isEmpty()){
        printValue(name);
        OutStream << ".NoName-> always empty\n";
    }else{
        printValue(name);
        OutStream << ".";
        printValue(field);
        OutStream << " == ";
        NameIdentifier value= getValue(name, field);
        printValue(value);
        OutStream << "->\n";
        printNameFields(value);
    }
}//printNameFieldValue

//! Print value to OutStream
void ThExecuteCode::
printValue(NameIdentifier value) const
{
    if(value>next_unknown){
        OutStream << "u" << value%1000;
    }else{
        OutStream << toUtf8(value);
    }
}//printValue

#//!\name Class methods

}//namespace Thesa

