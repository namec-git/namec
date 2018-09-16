/****************************************************************************
**
** Copyright (C) 2011-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThExecuteCode.h#11 $$Change: 2472 $
** $Date: 2018/09/07 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThExecuteCode_H
#define ThExecuteCode_H

#include "thidcore/thesa.h"
#include "thidcode/ThCode.h"

namespace Thesa {

#//!\name Defined here
    class ThExecuteCode;
    //!   ThExecuteCode::NameValues
    //!   ThExecuteCode::NameValuesList

#//!\name Used here

class ThExecuteCode : public ThCode {
    Q_OBJECT //! Owned by Thid?

public:

    enum {
    };

    class NameValues {

    private:
        QHash<NameIdentifier, NameIdentifier> name_value;  //!< Mapping from field to value for a name
    public:
        NameValues() : name_value() {  } // i.e., ""
        NameValues(const NameValues &other) : name_value(other.name_value) { }
        NameValues &    operator=(const NameValues &other) { name_value= other.name_value; return *this; }
        ~NameValues() {  }

        bool            contains(NameIdentifier field) const { return name_value.contains(field); }
        void            insert(NameIdentifier field, NameIdentifier value) { name_value.insert(field, value); }
        bool            isEmpty() const { return name_value.isEmpty(); }
        NameIdentifierList keys() const { return name_value.keys(); }
        NameIdentifier  value(NameIdentifier field) const { return name_value.value(field); }
    };//class ThCode::NameValues
    typedef QList<NameValues>  NameValuesList;

#//!\name Fields

private:
    int                 log_level;              //!< Logging if >0
    int                 next_name;              //!< Supplies unique names 'n...' for newNameWithValues
    int                 next_unknown;           //!< Next unknown name for unknown_values (starting with maxint);
    NameValuesList      name_values;           //!< NameValues for each name's fields
    QHash<NameIdentifierPair, NameIdentifier> unknown_values; //!< Temporary, next_unknown values from getValue().  Never equal (opIfEquals)

#//!\name Class constants

public:
#//!\name Constructors, etc.
                        ThExecuteCode(ThDisk *disk, QObject *parent= 0);
    virtual             ~ThExecuteCode() {} // Q_OBJECT
private:
                        Q_DISABLE_COPY(ThExecuteCode);
public:

#//!\name GetSet
    bool                hasNameValue(NameIdentifier name, NameIdentifier field) const;
    int                 logLevel() const { return log_level; }
    void                setLogLevel(int i) { log_level= i; }

#//!\name Methods
    void                assignValue(NameIdentifier name, NameIdentifier field, NameIdentifier value);
    void                printNameFields(NameIdentifier name) const;
    void                printNameFieldValue(NameIdentifier name, NameIdentifier field);
    bool                executeInstruction(QString line);
    bool                executeProgram(int start= 0);
    NameIdentifier      getAssignableValue(NameIdentifier name, NameIdentifier field);
    NameIdentifier      getAssignableValue(OperandIdentifier operand);
    NameIdentifier      getValue(NameIdentifier name, NameIdentifier field);
    NameIdentifier      getValue(OperandIdentifier operand);
    NameIdentifier      getValue(const QString &argument);
    NameIdentifier      loadAssignableNameField(ArgType arg, OperandIdentifierList operands, NameIdentifier *name);
    NameIdentifier      loadNameField(ArgType arg, OperandIdentifierList operands, NameIdentifier *name);
    void                log(int level, int code, const char * fmt, int ipOffset);
    void                log(int level, int code, const char * fmt, int ipOffset, int intA);
    void                log(int level, int code, const char * fmt, int ipOffset, int intA, long long longA); // also matches intB
    void                log(int level, int code, const char * fmt, int ipOffset, int intA, int intB, int intC);
    void                log(int level, int code, const char * fmt, int ipOffset, int intA, float floatA, long long longA);
    NameIdentifier      newDecValue(NameIdentifier field);
    NameIdentifier      newIncValue(NameIdentifier field);
    NameIdentifier      newNameWithValues();
    void                opAssign(int ipOffset, NameIdentifier name, NameIdentifier field, NameIdentifier value);
    void                opBreakpoint(int ipOffset);
    bool                opCall(int ipOffset, NameIdentifier name, NameIdentifier field);
    void                opPrintFields(int ipOffset, NameIdentifier name, NameIdentifier field);
    int                 opGotoLabel(int ipOffset, NameIdentifier program, NameIdentifier field);
    int                 opIfEquals(int ipOffset, Op op, NameIdentifier name, NameIdentifier field, NameIdentifier value, OperandIdentifier label);
    void                opLabel(int ipOffset, NameIdentifier program, NameIdentifier field);
    void                opPrint(int ipOffset, NameIdentifier name, NameIdentifier field);
    void                opReturn(int ipOffset);
    void                opReturnError(int ipOffset);
    int                 opTry(int ipOffset, NameIdentifier name, NameIdentifier field, OperandIdentifier label);
    void                printValue(NameIdentifier value) const;

#//\name Class methods
    static bool         isAssignableSource(const char *prefix);
    static bool         isAssignableSourceMovAx(const char *prefix);

};//ThExecuteCode

}//namespace Thesa

#endif ThExecuteCode_H
