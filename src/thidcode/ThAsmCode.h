/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThAsmCode.h#9 $$Change: 2472 $
** $Date: 2018/09/07 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThAsmCode_H
#define ThAsmCode_H

#include "thidcore/thesa.h"
#include "thidcode/ThCode.h"

namespace Thesa {

#//!\name Defined here
    class ThAsmCode;

class ThAsmCode : public ThCode {
    Q_OBJECT //! Owned by Thid?

public:

    enum {
        AsmValueInc= 2,             //!< Fields and values are even integers starting at 2
        AsmCommentTab= 48,          //!< Tab to Asm comments from operands (e.g., "  mov   ") \todo Should be a ThSetting
    };

    enum Chars : char {
        AsmOtherChar= '_',  //!< indicates a name-char that is not an alphanum, ' ', or '_'.  For asm compatibility
    };

#//!\name Fields

private:
    QString             asm_comment;            //!< Comment for end-of-line
    int                 asm_arg_width;          //!< Asm argument width for comment tab, updated by writeAsmOperands, toAsmUtf8, etc.
    bool                is_check;               //!< True if checking well-formed program (assumes well-formed)
    bool                is_skip_comment;        //!< True if skipping one asm_comment in writeAsmEndLine()
    int                 next_label;             //!< Supplies unique label names 'true_...'
    QList<NameIdentifier> program_labels;       //!< List of programs that need labels for indirect goto or try \todo Should it be a QSet instead of QList?
    QHash<QByteArray, QByteArray> unique_thids; //!< Assign suffix to ThidId for toAsmUtf8
    QTextStream         write_text;             //!< QTextStream for write text to a QFile, QByteArray, QString, or char *
    QTextStream         write_text2;            //!< A second QTextStream for write text to a QFile, QByteArray, QString, or char *

#//!\name Class constants
	static const QByteArray AsmType;			//!< "th-asm-type.X= literal/known" Indicates literal and known names 
    static const QByteArray ReservedAsmNames; 
public:
	static const QByteArray ProgramTag;			//!< '@' between label and program

#//!\name Constructors, etc.
                        ThAsmCode(ThDisk *disk, QObject *parent= 0);
    virtual             ~ThAsmCode() {} // Q_OBJECT
private:
                        Q_DISABLE_COPY(ThAsmCode);
public:

#//!\name GetSet
    QByteArray          toAsmName(const QByteArray &ba);
    QByteArray          toAsmUtf8(NameIdentifier name);
    QByteArray          toAsmUtf8Labels(NameIdentifier name);
    QByteArray          toAsmUtf8Name(NameIdentifier name);

#//!\name Methods
    void                unpackAsArrayIndex(const OperandIdentifierList &operands, QByteArray *nameA, QByteArray *fieldA, QByteArray *nameB, QByteArray *fieldB);
    void                writeAsmCode(int start, const QString &filename, const QString &filenameData);
    void                writeAsmData(const QString &filenameData);
    void                writeAsmEndLine();
    void                writeAsmInstruction(const ThCode::Instruction &i);
    void                writeAsmLabelOperand(int instructionOffset, OperandIdentifier operand);
    void                writeAsmLabelOperands(int instructionOffset, ArgType arg, const OperandIdentifierList &operands);
    void                writeAsmUnknownLabels(int start);
    void                writeDestination(const char *prefix, int instructionOffset, ArgType arg, const OperandIdentifierList &operands);
    void                writeSource(const char *prefix, int instructionOffset, ArgType arg, const OperandIdentifierList &operands);

#//\name Class methods
    static bool         isAssignableSource(const char *prefix);
    static bool         isAssignableSourceMovAx(const char *prefix);

};//ThAsmCode

}//namespace Thesa

#endif ThAsmCode_H
