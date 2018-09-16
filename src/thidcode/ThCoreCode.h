/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThCoreCode.h#6 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThCoreCode_H
#define ThCoreCode_H

#include "thidcore/thesa.h"
#include "thidcode/ThCode.h"

namespace Thesa {

#//!\name Defined here
    class ThCoreCode;

class ThCoreCode : public ThCode {
    Q_OBJECT //! Owned by Thid?

public:

    enum {
	FirstReserved= 2,			//!< First reserved tag for ReservedCompileNames
    };

#//!\name Fields

private:
    int                 next_arg;               //!< Supplies unique argument names 'a...' for writeCoreInstruction
    int                 op_argA;                //!< ArgA for operator
    QTextStream         write_text;             //!< QTextStream for write text to a QFile, QByteArray, QString, or char *
    QHash<QByteArray, int> reserved_tags;       //!< Unique tags for ReservedCompileNames, starting at 2
    QSet<QString>       literal_operands;       //!< th_LIT.literal in core code
    QSet<QPair<NameIdentifier, NameIdentifier>> literal_label_operands;  //!< literal labels in core code for Op:goto and Op:ifEquals, etc.

#//!\name Class constants
public:
    static const char * CoreCodePrefix;         //!< "th_Core" prefix for instruction names
    static const char * CoreCodeStop;           //!< "th_STOP" for last instruction
    static const char * LiteralSuffix;          //!< First suffix for literal core code
    static const char * th_NameCodeProgram;     //!< "th_NameCodeProgram" is the first name code instruction
    static const char * th_NEW_value;           //!< "th_NEW_value" represents 'th_NEW' in  Op:assignLiteral and Op:ifEquals
    static const QByteArray OtherLiteralOperands; //!< Other literals used by ThCoreCode for literal_operands
    static const QByteArray ReservedCompileNames; 

#//!\name Constructors, etc.
public:
                        ThCoreCode(ThDisk *disk, QObject *parent= 0);
    virtual             ~ThCoreCode() {} // Q_OBJECT
private:
                        Q_DISABLE_COPY(ThCoreCode);
public:

#//!\name GetSet
    QChar               operationArgChar() { return QChar('A'+next_arg-op_argA); }
    QByteArray		toCoreCodeUtf8(NameIdentifier name);

#//!\name Methods
    void                collectLiteralOperands(int start);
    void                errorNotCoreArgument(int instructionOffset, ArgType arg) const;
    void                unpackAsArrayIndex(const OperandIdentifierList &operands, QString *nameA, QString *fieldA, QString *nameB, QString *fieldB);
    void                writeArgPair(int instructionOffset, const QString &instructionSuffix, QChar letter, const QString &nameA, const QString &fieldA, const QString &nameB, const QString &fieldB);
    void                writeCoreCode(int start, const QString &filename, const QString &extraCode);
    void                writeCoreInstruction(const ThCode::Instruction &i);
    void                writeNext(int instructionOffset, const QString &instructionSuffix, int nextOffset, const QString &nextSuffix);
    void                writeOperation(int instructionOffset, const QString &instructionSuffix, const QString &operation);
    void                writeLiteralCoreInstructions(int start);

#//!\name Class methods
    static bool         isValidLiteral(const QByteArray &literal);

};//ThCoreCode

}//namespace Thesa

#endif ThCoreCode_H
