/****************************************************************************
**
** Copyright (C) 2011-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcode/ThCode.h#72 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThCode_H
#define ThCode_H

#include "thidcore/thesa.h"

namespace Thesa {

#//!\name Defined here
/***
    A ThCode is a byte compressed Thesa program
***/
    class ThCode;
    //!   ThCode::NameIdentifier
    //!   ThCode::NameIdentifierList
    //!   ThCode::OperandIdentifier
    //!   ThCode::OperandIdentifierList;

#//!\name Used here
    class ThDepot;
    class ThDisk;
    class Thid;
    class ThTextDocument;

class ThCode : public QObject {
    Q_OBJECT //! Owned by Thid?

public:

    enum {
        LogAll=2,                   //!< Logging \todo Move to logger
        LogDetail=3,                //!< Lots of logging \todo Move to logger
        MaximumName=    MaximumInt, //!< Maximum value for a NameIdentifier.  Used for next_unknown
        NoLabelOffset=  0xffffffff, //!<  Label offset not defined for operand (i.e., two NoLabel's)
        NoName=         0x0000,     //!< Indicates no record or field for this operand ("")
        ErrorReturn=    -1,         //!< Returned by getLiteralLabelOffset if label does not exist
        FailReturn=     -2,         //!< Returned by opIfEquals on fail
        IsAllowedThNew= true,       //!< Flag for addOperand isAllowedThNew
        MultipleInstructions= 2,    //!< Two or more '//' separate multiple instructions  
    };

    enum Offsets : quint16 {
        LabelOffset=    2,          //!< Labels are stored as N.N followed by lowOffset highOffset into program
        NoLabel=        0xffff,     //!<  indicates the Label wasn't seen yet
        NoOperand=      0x0000,     //!< Indicates no record or field for this operand ("")
        MaxOffset=      0xfffe,     //!< Maximum index into program_operands (16 bits)
        MaxOperands=    6,          //!< Maximum number of operands for an Op
        NoReturn=       0xffff,     //!< Indicates missing label in operand_offsets
        ReturnOffset=   2,          //< Offset in program_operands to double wide offset into program_code
    };//Offsets

    /** At most six ArgTypes since codes 0 and 1 are reserved for Ops
       // 'N' indicates a name/field/literal, 'R' indicates a name.field
       // ArgTypes with 1-4 names and without [..]
       N            Literal             N
       N.N          Field               R
       N.N.N        Array               R[N]
       N.N.N.N      Error
       // ArgTypes with 2-4 names and with ..[..]
       N[N]         ==                  N.N
       N[N.N]       IndirectField       N[R]
       N[N.N.N]     Error
       N.N[N]       ==                  N.N.N
       N.N[N.N]     ArrayIndex          R[R]
       N.N.N[N]     Error
*/
    //! Byte code for getInstruction()
    //! followed by ArgA, ArgB, Label for fillOperands()
    //! \see toOpCode and toByteCode for conversions with OpArgA, OpArgB, or OpArgAB as needed
    //!\todo Review Op encoding since an arg is 1..4 names instead of 1..5 names
    enum Op : uchar {               
        ArgTypeMask=    0x07,       //!< ArgType (0x02..0x07) for argB
         ArgAShift=     3,          //!< shift ArgType for argA [0x38] 
        OpNoArg=        0x36,       //!< No arguments (0x30 and 0x06 are zero, ArgTypes 0x00 and 0x01/0x08 are not used)
         NOP=           0x00,       //!< . do nothing
         Breakpoint=    0x01,       //!< =! BREAKPOINT
         Return=        0x08,       //!< -> RETURN
         Fail=          0x09,       //!< !> FAIL
         PrintComma=    0x40,       //!< '=,' or '^=,$'
         PrintCommaSpace=0x41,      //!< '=, ' or '=,$'
         PrintNewLine=  0x49,       //!< '=$'
         PrintSpace=    0x48,       //!< ' =_', ' =_$', or '^=_$'
                                    //!< print w/o separator is '=_$' or '^=_.*=_$'
                        // eight additional codes for ~OpNoArg (NoArg, 0x8?, 0xC?)
       // 'N' indicates a literal, 'R' indicates a name/field/literal or a name.field
        OpArgAMask=     0xc7,       //!< Operator with one argument A (ArgType 0x30 is non-zero, 0x06 is zero)
         OpArgA=        0x30,       // distinguish argA operators 
         // unused opA opcode 0x30, reserve for one or two argument extension codes (two bytes)
         LabelA=        0x40+OpArgA,   //!< [0x70] R->   Label N or N.N, a literal label may have a program_name
         GotoA=         0x80+OpArgA,   //!< [0xb0] ->R R[R] R[R]
         // unused opA opcode 0xc0, 
         CallA=         0x01+OpArgA,   //!< [0x38] N
         TryA=          0x41+OpArgA,   //!< [0x78] R -> N (not implemented for assemble or compile)
         PrintA=        0x81+OpArgA,   //!< [0xb8] R=, R= or R=_   Print value, usually followed by a PrintCommaSP,etc.
         PrintFieldsA=  0xc1+OpArgA,   //!< [0xf8] R=! Display ArgA and its fields
        OpArgBMask=     0xf8,       //!< Operator with one argument B (ArgType 0x30 is zero, 0x06 is non-zero)
         OpArgB=        0x06,       // distinguish argB operators 
         ThrowB=        0x40+OpArgB,   //!< [0x46] !> R (not implemented)
         CatchB=        0x80+OpArgB,   //!< [0x86] R !> (not implemented)  
                        // five unused opB opcodes 0x00, 0xc0, 0x08, 0x48, 0x88, 0xc8
        OpArgABMask=    0xc0,       //!< Operator with two arguments A and B (ArgType 0x30 is non-zero, 0x06 is non-zero)
         OpArgAB=       0x36,       // Distinguish two or three argument operators 
                        // unused opAB opcode 0x00, 
         AssignAB=      0x40+OpArgAB,  //!< [0x76] ArgA= ArgB
         IfEqualsAB=    0x80+OpArgAB,  //!< [0xb6] ArgA==ArgB ->R
         IfNotEqualsAB= 0xc0+OpArgAB,  //!< [0xf6] ArgA!=ArgB ->R
    };//Op

    enum ArgType : uchar {              //!< argType_A is 0x38 and argType_B is 0x07
        NoArg=          0x0,            //!< codes 0x00 and 0x01 reserved for Op instead of ArgType
         //noArg2       0x1,
         Literal=       0x2,            //!< N (N)
         Field=         0x3,            //!< N.N or N[N] (R) -- except for ... (a name)
         IndirectField= 0x4,            //!< N[N.N] (N[R])
         Array=         0x5,            //!< N.N[N] (R[N]) or V.N
         ArrayIndex=    0x6,            //!< N.N[N.N] (R[R])
         //unused       0x7,
    };//ArgType

    //! Identifier for a 'name' (i.e., offset into name_offsets and ThExecuteCode::name_records

    class NameIdentifier {
    private:
        NameIdentifierInt   name_identifier;
    public:
                        NameIdentifier() : name_identifier(NoName) {  } // i.e., ""
        explicit        NameIdentifier(NameIdentifierInt i) : name_identifier(i) {  } // i.e., ""
                        NameIdentifier(const NameIdentifier &other) : name_identifier(other.name_identifier) { }
        NameIdentifier &operator=(const NameIdentifier &other) { name_identifier= other.name_identifier; return *this; }
                        ~NameIdentifier() {  }

        bool            isEmpty() const { return (name_identifier==NoName); }
        bool            isThNew() const { return (name_identifier==NoName); } //!< A th_NEW field has the NoName identifier
        bool            isValid() const { return (name_identifier!=NoName); }
        void            next() { ++name_identifier; }
        NameIdentifierInt toInt() const { return name_identifier; }
                        operator NameIdentifierInt() const { return name_identifier; }
        bool            operator==(const NameIdentifier &other) const { return other.name_identifier==name_identifier; }
        bool            operator!=(const NameIdentifier &other) const { return other.name_identifier!=name_identifier; }
    };//class ThCode::NameIdentifier
    typedef QList<NameIdentifier>  NameIdentifierList;
    typedef QPair<NameIdentifier, NameIdentifier> NameIdentifierPair;

    //! Identifier for a 'operand' (index*2 into program_operands to a pair of NameIdentifier)
    class OperandIdentifier {
    private:
        OperandIdentifierInt  operand_identifier;
    public:
                        OperandIdentifier() : operand_identifier(NoOperand) {  } // i.e., ""
        explicit        OperandIdentifier(OperandIdentifierInt i) : operand_identifier(short(i)) {  } // i.e., ""
                        OperandIdentifier(const OperandIdentifier &other) : operand_identifier(other.operand_identifier) { }
        OperandIdentifier &operator=(const OperandIdentifier &other) { operand_identifier= other.operand_identifier; return *this; }
                        ~OperandIdentifier() {  }

        bool            isEmpty() const { return (operand_identifier==NoOperand); }
        bool            isValid() const { return (operand_identifier!=NoOperand); }
        OperandIdentifierInt  toInt() const { return operand_identifier; }
                        operator OperandIdentifierInt() const { return operand_identifier; }
        bool            operator==(const OperandIdentifier &other) const { return other.operand_identifier==operand_identifier; }
        bool            operator!=(const OperandIdentifier &other) const { return other.operand_identifier!=operand_identifier; }

    };//class ThCode::OperandIdentifier
    typedef QList<OperandIdentifier>  OperandIdentifierList;

    class Instruction {
    private:
        ThCode *        th_code;
        int             current_offset;     //!< Offset into program_code.  Always non-negative.
        int             next_offset;        //!< Next offset into program_code.  Always non-negative.
        OperandIdentifierList operands_A;   //!< Operands for arg_A
        OperandIdentifierList operands_B;   //!< Operands for arg_B
        OperandIdentifier label_operand;    //!< label operand
        Op              code_op;            //!< operator
        ArgType         argType_A;          //!< ArgType for arg_A
        ArgType         argType_B;          //!< ArgType for arg_B
    public:
                        Instruction(ThCode *code);
                        Instruction(ThCode *code, int program);
                        Instruction(const Instruction &other);
        Instruction &   operator=(const Instruction &other);

        ArgType         argTypeA() const { return argType_A; }
        ArgType         argTypeB() const { return argType_B; }
        bool            isLastInstruction() const { return next_offset >= th_code->programCodeSize(); }
        bool            isValid() const { return current_offset < th_code->programCodeSize(); }
        OperandIdentifier labelOperand() const { return label_operand; }
        int             nextOffset() const { return next_offset; }
        Op              nextOp() const;
        int             offset() const { return current_offset; }
        int             offset(const char *p) const { return (int)(p-programCode()); }
        Op              op() const { return code_op; }
        OperandIdentifierList operandsA() const { return operands_A; }
        OperandIdentifierList operandsB() const { return operands_B; }
        OperandIdentifierList operandsC() const;
        bool            operator==(const Instruction &other) const { return current_offset==other.current_offset && th_code==other.th_code; }
        bool            operator!=(const Instruction &other) const { return current_offset!=other.current_offset || th_code!=other.th_code; }
        const char *    programCode()const { return th_code->programCode(); }  //!< Invalid after appendProgram
        const char *    programCodeEnd()const { return th_code->programCodeEnd(); }  //!< Invalid after appendProgram
        void            setNextOffset(int offset) { next_offset= offset; }

        void            checkMore(const char *p) const;
        void            getInstruction(const char *p);
        void            nextInstruction() { getInstruction(th_code->programCode()+next_offset); }
    };//Instruction

    //! Derived from ThTuples::const_iterator using just a next pointer
    //! Not used
    class const_iterator
    {
    private:
        Instruction     current_instruction;
    public:

        typedef std::forward_iterator_tag   iterator_category;
        typedef Instruction                 value_type;
        // No declaration for difference_type, pointer, and reference

                        const_iterator(const ThCode *code, int program);
                        /* implicit copy constructor, etc. */

        const Instruction &operator*() const { return current_instruction; }
        bool            operator==(const const_iterator &other) const { return (current_instruction==other.current_instruction); }
        bool            operator!=(const const_iterator &other) const { return (current_instruction!=other.current_instruction); }
        const_iterator & operator++() { current_instruction.nextInstruction(); }
        // operator++(int), operator--, operator+, operator- not implemented
    };//ThTuples::const_iterator


#//!\name Fields

private:
    QString             program_title;
    NameIdentifier      program_name;           //!< Program name for compiling into program_code and executing 'goto'
    int                 file_line;              //!< Current line number in file_name for tryAppendInstruction
    QString             file_name;              //!< Current file name for tryAppendInstruction
    QString             next_label;             //!< Next unique label (th_LBL0000)
    QSet<NameIdentifierPair> program_variables;     //!< Local variables (th_VAR) by program_name.
    QByteArray          utf8_names;             //!< Null-terminated titles for names in program_operands.  Offset 0 is '\0'.
    QList<int>          name_offsets;           //!< Offsets into utf8_names, Offset 0 is '\0'.
    NameIdentifierList  program_operands;       //!< Operands as name.field NameIdentifier's (i.e., two UTF8 strings.
        //!< The first is the field or literal.  The second is the name or "".
        //!< For literal labels, the next two NameIdentifier's (LabelOffset) are an offset into program_code, low word first.
        //!<   NoLabel (NoIndex) if label not found yet.
    QByteArray          program_code;           //!< Byte codes for program as sequence of Op+ArgTypes+OperandIndentifiers
    QHash<NameIdentifierPair, OperandIdentifier> label_index; //!< Index for labels (program.literal) and programs (name)
    QHash<QString, NameIdentifier> name_index;  //!< NameIdentifier index for titles in utf8_names
						//!< Cleared when program_code is frozen.
    QHash<NameIdentifierPair, OperandIdentifier> operand_index; //!< name.field index for program_operands.
                                                //!< Cleared when program_code is frozen.
                                                //!< const char * does not work since QPair is const T &
    ThDisk *            thid_disk;              //!< ThDisk for resolving names
    QString             th_name_value;          //!< Replacement for th_NAME
    int                 th_name_count;          //!< Replacement count for th_NAME
    int                 error_count;            //!< Errors reported by tryAppendInstruction
    NameIdentifier      th_inc;
    NameIdentifier      th_lit;
    NameIdentifier      th_name;
    NameIdentifier      th_new;
    NameIdentifier      th_var;
    bool                is_ignore_errors;       //!< True if ignoring errors in tryAppendInstruction and ThExecuteCode::opTry
    bool                is_literal_thlit;       //!< True th_LIT used as a literal, particularly for --compile of core code
                                                //!< If true, mixed uses of variable as literal are ignored
    bool                is_well_formed;         //!< True if all values are known except for explicit th_NEW

#//!\name Class constants
    static const int    s_operands_for_arg[8];
public:
    static const QByteArray th_INC;        //!< Reserved "th_INC" for unique, incremented value
    static const QByteArray th_NAME;       //!< Reserved "th_NAME" for one arbitrary name per line
    static const QByteArray th_NEXT_NEW;   //!< Reserved nextName suffix is "-1"
    static const QByteArray th_LIT;        //!< Reserved "th_LIT" for literals
    static const QByteArray th_NEW;        //!< Reserved "th_NEW" for ThCode::NoName
    static const QByteArray th_VAR;        //!< Reserved "th_VAR" for local variables
    static const QByteArray th_RESERVED_PREFIX;  //!< Reserved names start with "th_" followed by an upper-case letter
    static const QByteArray th_TMP;        //!< Reserved th_VAR temporaries are "th_TMP..." w/ ThCode::nextName()
    static const QByteArray th_UNIQUE_LABEL; //!< Reserved labels are "th_LBL0000" w/ ThCode::nextName()
    static const QString NonInstructionCharactersRE;

#//!\name Constructors, etc.
                        ThCode(ThDisk *disk, QObject *parent= 0);
    virtual             ~ThCode() {} // Q_OBJECT
private:
                        Q_DISABLE_COPY(ThCode);
public:

#//!\name GetSet
    void                clearProgramName() { program_name= NameIdentifier(NoName); }
    ThDisk *            disk() const { return thid_disk; }
    int                 errorCount() const { return error_count; }
    int                 getLiteralLabelOffset(NameIdentifier program, NameIdentifier labelName) const;
    int                 getLiteralLabelOffset(OperandIdentifier operand) const;
    NameIdentifier      getLiteral(OperandIdentifier operand) const;
    NameIdentifier      getNameField(OperandIdentifier operand, NameIdentifier *name) const;
    NameIdentifier      getNameFieldFromLabelOffset(int labelOffset, NameIdentifier *name) const;
    NameIdentifier      getName(const QString &name) const { return name_index.value(name); }
    OperandIdentifier   getOperand(const QString &name, const QString &field) const;
    bool                isEmpty() const { return program_code.isEmpty(); }
    bool                isIgnoreErrors() const { return is_ignore_errors; }
    bool                isFrozen() const { return (name_index.isEmpty() && name_offsets.count()>1); }
    bool                isLabelValid(OperandIdentifier label);
    bool                isLiteral(OperandIdentifier i) const;
    bool                isRestrictedNameField(const QString &argument, bool *isName= NULL);
    bool                isVariable(NameIdentifier name) const { return program_variables.contains(NameIdentifierPair(program_name, name)); }
    bool                isVariable(NameIdentifier program, NameIdentifier name) const { return program_variables.contains(NameIdentifierPair(program, name)); }
    bool                isWellFormedProgram() const { return is_well_formed; }
    const QHash<NameIdentifierPair, OperandIdentifier> & labelIndex() const { return label_index; }
    int                 nameCount() const { return name_offsets.count(); }
    void                newNextLabel() { next_label= ThCode::nextName(next_label); }
    int                 nextCodeOffset() const { return program_code.count(); }
    QString             nextLabel() const { return next_label; }
    int                 operandCount() const { return operand_index.count(); }
    const char         *programCode() const { return program_code.data(); }
    const char         *programCodeEnd() const { return program_code.data()+program_code.count(); }
    int                 programCodeSize() const { return program_code.count(); }
    NameIdentifier      programName() const { return program_name; }
    void                setFileName(const QString &s) { file_name= s; file_line= 0; }
    void                setIsIgnoreErrors() { is_ignore_errors= true; }
    void                setProgramName(NameIdentifier name) { program_name= name; }
    void                setProgramName(const QString &s);
    void                setTitle(const QString &s) { program_title= s; }
    NameIdentifier      thInc() const { return th_inc; }
    NameIdentifier      thLit() const { return th_lit; }
    NameIdentifier      thNew() const { return th_new; }
    NameIdentifier      thVar() const { return th_var; }
    QString             title() const { return program_title; }
    QString             toString(OperandIdentifier operand) const;
    QString             toString(const OperandIdentifierList &operands) const;
    const char *        toUtf8(NameIdentifier name) const;
    void                warnIfReservedName(const QString &name) const;

#//!\name Virtual methods
    //! getValue and hasNameValue defined for ThExecuteCode only.  Used by ThExecuteCode and ThCode::printOperandList
    virtual NameIdentifier getValue(NameIdentifier name, NameIdentifier field) { return NameIdentifier(); } 
    virtual bool        hasNameValue(NameIdentifier name, NameIdentifier field) { return false; }

#//!\name Methods
    OperandIdentifierList addArgument(const QString &argument, bool isAllowedThNew);
    OperandIdentifier   addCallOperand(NameIdentifier programName, NameIdentifier thidTitle);
    void                addLiteralLabel(OperandIdentifier label, int offset);
    OperandIdentifier   addLiteralLabelOperand(NameIdentifier program, NameIdentifier label);
    void                addLiteralLabelOperand(OperandIdentifier label);
    NameIdentifier      addName(const QString &s);
    OperandIdentifier   addOperand(NameIdentifier name, NameIdentifier field);
    OperandIdentifier   addOperand(QString name, QString field, bool isAllowedThNew, OperandIdentifier *fieldWithVariable= NULL);
    OperandIdentifierList addProgramVariable(OperandIdentifier literal);
    void                appendAssignCode(const QString &argumentA, const QString &argumentB);
    void                appendCallCode(const QString &argument);
    void                appendCodeOp(Op code) { program_code.append(char(code)); }
    void                appendGotoCode(const QString &argument);
    void                appendIfEqualsCode(Op opCode, const QString &argumentA, const QString &argumentB, const QString &label);
    void                appendInstruction(const QString &s);
    void                appendLabelCode(const QString &argument);
    void                appendLabelCode(NameIdentifier labelName);
    void                appendOperandIdentifier(OperandIdentifier operand);
    void                appendOperands(OperandIdentifierList operands);
    void                appendPrintCode(Op printOp, const QString &argument);
    void                appendPrintLiteral(const QString &argument);
    int                 appendProgram(Thid *thid, NameIdentifier title= NameIdentifier(NoName));
    int                 appendProgram(Thid *thid, const ThTextDocument *text, NameIdentifier title= NameIdentifier(NoName));
    int                 appendStream(const QString &program, QTextStream *stream);
    void                appendTextDocument(const ThTextDocument *text);
    void                appendTryCode(const QString &argument, const QString &label);
    ArgType             argTypeForOperands(const OperandIdentifierList &operands) const;
    void                checkLabel(OperandIdentifier label);
    void                checkLabelName(const QString &label) const;
    void                checkLabels();
    void                errorIfFrozen();
    const char *        fillOperands(ArgType arg, const char *ip, OperandIdentifierList *operands) const;
    OperandIdentifier   firstLiteralOrName(NameIdentifier name);
    void                freezeProgram();
    void                printInstruction(const ThCode::Instruction &i);
    void                printOperandList(OperandIdentifierList operands);
    void                printOperandLists(Instruction i);
    void                printOperandNames(OperandIdentifier operand) const;
    void                printOperands(const OperandIdentifierList &operands) const;
    void                printProgram(int start);
    QString             replaceLiteralWithThName(const QString &s);
    void                resolveCall(Op op, int codeOffset, OperandIdentifier label);
    void                resolveCalls(int start);
    QStringList         splitArgument(const QString &argument, bool *isArray= NULL);
    void                tryAppendInstruction(const QString &line);

#//\name Class methods
    static bool         isPlainName(const QString &name);
    static bool         isReservedName(const QString &name);
    static QString      nextName(const QString &name);
    static uchar        toByteCode(Op opCode, ArgType argA, ArgType argB);
    static Op           toOpCode(uchar byteCode);
};//ThCode

}//namespace Thesa

#endif ThCode_H
