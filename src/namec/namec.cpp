/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/namec/namec.cpp#16 $$Change: 2478 $
** $Date: 2018/09/14 $$Author: bbarber $
**
****************************************************************************/

#include "thidcore/thesa.h"

//!\todo If build thidtest, need to rebuild namec.  Otherwise seg fault before main()

#include "thidcode/ThAsmCode.h"
#include "thidcode/ThCoreCode.h"
#include "thidcode/ThExecuteCode.h"
#include "road/RoadEventStack.h"

#include <QtCore/QTextStream>
#include <QtCore/QCommandLineParser>

namespace Thesa {

void 
printHelp(const QCoreApplication &app){
    QString help= 
    //   1                       25                                                     80 
        "\n"
        "namec -- name code for programming with arbitrary names\n";
    help += QStringLiteral("  ") += app.applicationVersion() += "\n";
    help += 
        "Options:\n"
        "  -s, --syntax          -- Syntax for name code\n"
        "  -e, --execute         -- Execute name code\n"
        "  -C, --compile=file    -- Compile core code to file\n"
        "  -X, --extra-code=file -- Extra code for compile\n"
        "  -A, --assemble=file   -- Assembly code for Masm32\n"
        "  -V, --version         -- Version\n"
        "  -?, -h, --help        -- This message\n"
        "Logging:\n"
        "  -v, --verbose         -- Log events, '-v -v -v' for details\n"
        "  -t, --trace=N         -- Log steps(1), trace(2), detail(3,4)\n"
        "  -i, --ignore-errors   -- Ignore syntax errors, try errors, and missing calls\n"
        "  -S, --log-start=N     -- Start logging at event N\n"
        "  -L, --log-depth=N     -- Log up to depth N\n"
        "  -M, --log-max=N       -- Maximum events at one depth\n"
        "  -o, --log-stdout      -- Log to stdout instead of stderr\n"
        "  -n, --no-program      -- Do not log program\n"
        "Hello test of name code:\n"
        "  bin/namec test/hello-test.nc --execute --log-stdout --no-program\n"
        "  bin/namec test/hello-test-core.nc test/name-machine.nc -e -o -n\n"
        "  bin/namec test/hello-test-core-compile.nc  test/name-machine.nc --compile test/name-machine.nc.nc -n\n"
        "  bin/namec test/name-machine.nc.nc test/name-machine.nc -e -o -n\n"
        "  bin/namec test/hello-test.nc --assemble src/nameasm/name.asm -n; namec.sh nameasm; bin/nameasm.exe\n"
        "namec examples:\n"
        "  bin/namec test/regexp-test.nc test/regexp-dfa.nc -e -n\n"
        "  bin/namec test/turing-samples.nc test/turing-machine.nc -e -n -o\n"
        "  (cd test && ../bin/namec namec-test-core.nc namec-test-name-field.nc namec-test-literals.nc namec-test-try.nc namec-test.nc -e -n -o)\n"
        "  bin/namec test/namec-test-errors.nc -e -i -o\n"
        "\n"
        ;
    OutStream << help;
}//printHelp

void 
printHelpSyntax(){
    QString help= 
    //   1                    22     Less than 80 characters, up to column 89.          80
        "=============================================================\n"
        "== namec -- Name code for programming with arbitrary names ==\n"
        "=============================================================\n"
        "\n"
        "A name code program is a sequence of instructions.  An instruction\n"
        "is printable text with spaces.  There is one instruction per line.\n"
        "Three or more instructions may be separated by '//':\n"
        "  instruction\n"
        "  instruction // instruction // instruction\n"
        "  instruction // comment to end of line\n"
        "  // comment to end of line\n"
        "\n"
        "A name code program uses labels, names, fields, literals, and values:\n"
        "  A label is arbitrary text.  It identifies an instruction\n"
        "  A program fails if it does not contain a label (see below)\n"
        "  The first label of a program is its name\n"
        "  A name is arbitrary text.  It contains fields\n"
        "  A field is arbitrary text.  A field has a value\n"
        "  A literal is arbitrary text.  It is a value\n"
        "  The value of a field is unknown until it is assigned\n"
        "  A value is a literal, name, field, or label depending on context (see below)\n"
        "  'th_NEW' is a reserved name.  It identifies unknown values\n"
        "\n"
        "The core instructions are:\n"
        "  label->                                        // label this instruction\n"
        "  name.field= literal                            // assign literal value\n"
        "  name.field[name.field]= name.field[name.field] // assign value\n"
        "  ->label                                        // goto label or fail\n"
        "  ->name.field                                   // goto name.field or continue\n"
        "  name.field[name.field]==name.field ->label     // if equal, goto label\n"
        "  name.field[name.field]==th_NEW ->label         // if unknown, goto label\n"
        "  name.field= th_NEW                             // assign a new, unique value\n"
        "\n"
        "Name code instructions are derived from the core instructions:\n"
        "  A program has an external identifier 'thid-...'\n"
        "  A variable is a field of the program's external identifier\n"
        "  Let N be a name, F a field, and V a variable\n"
        "  A term T is N.F or, depending on context, N, F, or V (see below)\n"
        "  A reference R is T or T[T]\n"
        "  An indexed term is followed by a bracket (T[...])\n"
        "  An index term is bracketed (...[T])\n"
        "  if T is not N.F\n"
        "    T is a variable V if it was assigned a value in the program\n"
        "    T is a name N if it is an indexed term\n"
        "    T is a field F if it is an index term\n"
        "  A literal is a value\n"
        "  N.F has a value\n"
        "  N[F] is the same as N.F\n"
        "  A value R' is either R or a literal\n"
        "  An indexed name N[F] is a total function from names to names\n"
        "\n"
        "The name code instructions are:\n"
        "  label->              // label this instruction or program\n"
        "  R= R'                // assign value to R\n"
        "  R= th_NEW            // assign a new, unique value\n"
        "  -> label             // goto label or fail\n"
        "  -> R                 // goto label at R or continue\n"
        "  R' == R'    ->label  // if equals, goto label or fail\n"
        "  R' != R'    ->label  // if not equals, goto label\n"
        "  R == th_NEW ->label  // if unknown, goto label\n"
        "  R != th_NEW ->label  // if known, goto label\n"
        "  program              // call program or fail\n"
        "  program     ->label  // try program. If OK, goto label\n"
        "  R           ->label  // try program at R. If OK, goto label\n"
        "  .                    // do nothing\n"
        "  ->                   // return to caller of program\n"
        "  !>                   // fail this program\n"
        "  !> R'                // fail by throwing an error\n"
        "  R' !>                // catch an error after a try\n"
        "  =!                   // breakpoint\n"
        "\n"
        "Print instructions visualize the results of a program:\n"
        "  R' =                 // print value followed by a new line\n"
        "  R' =,                // print value followed by a comma and space\n"
        "  R' =_                // print value followed by a space\n"
        "  R'=_                 // print value\n"
        "  R =!                 // print fields of name.field\n"
        "  =                    // print a new line\n"
        "  =,                   // print a comma\n"
        "  =_                   // print a space\n"
        "\n"
        "Multiple values may be separated by '=,' or '=_ ' (with space):\n"
        "  print comma-list with spaces=, Variable=, name.field=, Variable[Index]=,\n"
        "  print comma-list without spaces=,name.field=,literal=\n"
        "  print value-list with spaces =_ name.field =_ Variable.field=\n"
        "  print value-list without spaces=, Variable=_ ==_ literal=_\n"
        "\n"
        "Details to distinguish names and instructions:\n"
        "  The goal is to allow arbitrary, descriptive names\n"
        "  Leading spaces, trailing spaces, and extra spaces are ignored\n"
        "  [[...]] is an arbitrary name, field, variable, literal, or label.  One per line\n"
        "\n"
        "The instruction delimiters are '=', '==', '!=', '->', and '//':\n"
        "  Instructions are listed in precedence order\n"
        "  Multiple instructions are separated by two or more '//'\n"
        "  Comments start with '//'\n"
        "  Goto instructions start with '->'\n"
        "  Label instructions end with '->'\n"
        "  Throw instructions start with '!>'\n"
        "  Catch instructions end with '!>'\n"
        "  IfEquals instructions contain the first '==' and the last '->label'\n"
        "  IfNotEquals instructions contain the first '!=' and the last '->label'\n"
        "  IfNotEquals is selected if '!=' is before '=='\n"
        "  IfEquals and IfNotEquals ignore '===', '!!=',  or '!=='\n"
        "  Try instructions contain the last '->label'\n"
        "  Labels do not start or end with either '->' or '!>'\n"
        "  Print instructions end in '=', '=,', '=_', or '=!'\n"
        "  Print instructions do not end in '=='\n"
        "  Multiple print instructions are separated by '=,' or '=_ ' (with space)\n"
        "  Indexed assign instructions contain the first '[...]=' that is not '=='\n"
        "  Assign instructions contain the first '=' that is not '=='\n"
        "  Everything else is a call instruction\n"
        "\n"
        "The reference delimiters are '.', '[', and ']':\n"
        "  A name.field is the first '.' that is not '..'\n"
        "  An indexed reference T[T] is the first '[' and ends with ']'\n"
        "  Name.field for catch, goto, print, throw, and try is restricted\n"
        "  A restricted name.field does not contain ' ', '.', '=', '[', or ']'\n"
        "\n"
        "Reserved names start with 'thid-' or 'th_' followed by a digit or capital:\n"
        "  thid-... is an external identifier\n"
        "  th_NEW indicates an unknowable unique value\n"
        "    Name code could use unknown values instead of th_NEW\n"
        "  th_INC is a function to create unique values for th_NEW\n"
        "  th_NAME is a place holder for a literal name, field, or literal ([[...]])\n"
        "  th_END ignores the rest of a program\n"
        "\n"
        "The remaining reserved names are for generating core code (--compile):\n"
        "  th_LBL0000,th_LBL0001,... are generated labels for core code\n"
        "  th_LIT is a function for literals in core code:\n"
        "    th_LIT.literal= literal\n"
        "  th_TMP, th_TMP2,... are temporary variables in core code\n"
        "  th_VAR is a function for temporary variables in core code:\n"
        "    th_LIT.th_VAR[th_LIT.th_TMP]= th_LIT.name[x.y]\n"
        "\n"
        "To be done:\n"
        "  call program       // call is not implemented for --compile\n"
        "  try program ->label // try is not implemented for --assemble or --compile\n"
        "  !>                 // fail is an error for --assemble or --compile\n"
        "  !> literal         // throw a literal is a print followed by fail\n"
        "  !> name.field      // catch a reference is not implemented\n"
        "  literal !>         // catch a literal is not implemented\n"
        "  name.field !>      // catch a reference is not implemented\n"
        "\n"
        "Enhancements\n"
        "  Name code is currently not practical.  Assemble should use registers\n"
        "  Enhancements will include expressions, parameters, and program blocks\n"
        "  Programs and names will be externally defined with thid-... identifiers\n"
        "====================================================================\n";
        ;
    OutStream << help;
}//printHelpSyntax

//! Return programOffset for name code files and stdin
//! Substitutes InStream (stdin) for "-"
int 
appendProgram(ThCode *code, const QCommandLineParser &options)
{
    bool stdinProcessed= false;
    int program= NoIndex;
    foreach(const QString &filePath, options.positionalArguments()){
        int program2;
        if(filePath=="-"){
            code->setFileName("stdin");
            program2= code->appendStream("thid-00080-th-0798-8714", &InStream); //Thesa: test thid
            stdinProcessed= true;
        }else{
            QFile file(filePath);
            if(!file.exists()){
                throw ThError(12058, "namec: file '%s' not found.  Current directory is '%s'", 0,0, filePath, QDir::currentPath());
            }
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                throw ThError(12059, "namec: file '%s' exists, but could not open for read text: %s", 0,0, filePath, file.errorString());
            }
            QTextStream fileStream(&file);
            code->setFileName(filePath);
            program2= code->appendStream("thid-00080-th-0798-8714", &fileStream); //Thesa: test thid
        }
        if(program==NoIndex){
            program= program2;
        }
    }
    if(!stdinProcessed && InStream.device()->bytesAvailable()>0){
        code->setFileName("stdin");
        int program2= code->appendStream("thid-00080-th-0798-8714", &InStream); //Thesa: test thid
        if(program==NoIndex){
            program= program2;
        }
    }
    code->appendCodeOp(ThCode::Return);
    if(code->errorCount()!=0){
        if(code->isIgnoreErrors()){
            ThWarn(10469, "namec: %d errors.  Continuing due to --ignore-errors", code->errorCount());
        }else{
            throw ThError(10479, "namec: %d errors.  Override with option '--ignore-errors/-i'", code->errorCount());
        }
    }
    code->resolveCalls(program);
    if(!options.isSet("no-program")){
        code->printProgram(program);
    }
    return program;
}//appendProgram

void
configureLogging(const QCommandLineParser &options)
{
    bool ok;
    int eventPriority= RoadEvent::Always;
    int logLevel= 0;  
    if(options.isSet("trace")){
        QString intVal= options.value("trace");
        int logLevel= intVal.toInt(&ok);
        switch(logLevel){
        case 0:
            eventPriority= RoadEvent::Always;
            break;
        case 1:
            eventPriority= RoadEvent::Steps;
            break;
        case 2:
            eventPriority= RoadEvent::Normal;
            break;
        case 3:
            eventPriority= RoadEvent::Detail;
            break;
        case 4:
            eventPriority= RoadEvent::Detail2;
            break;
        default:
            ok= false;
            break;
        }
        if(!ok){
            throw ThError(12050, "namec: expecting --trace 0..4.  Got '%s'", intVal);
        }
        if(options.isSet("verbose")){
            throw ThError(12051, "namec: cannot set --verbose with --trace");
        }
    }else if(options.isSet("verbose")){
        QStringList optionNames= options.optionNames();
        logLevel= optionNames.count("verbose")+optionNames.count("v");
        switch(logLevel){
        case 1:
            eventPriority= RoadEvent::Steps;
            break;
        case 2:
            eventPriority= RoadEvent::Normal;
            break;
        case 3:
            eventPriority= RoadEvent::Detail;
            break;
        default:
            eventPriority= RoadEvent::Detail2;
        }
    }
    // invoke RoadEventStack to insure OutStream initialization
    RoadEventStack *eventStack= RoadEventStack::currentEventStack();
    if(options.isSet("log-stdout")){
        RoadEventStack::setIsLogStdout();
        QIODevice *device= OutStream.device(); // Defined in RoadEventStack
        RoadEventStack::setErrStream(device); 
    }
    eventStack->setLogLevel(eventPriority>>RoadEvent::EventPriorityShift);
    if(options.isSet("log-start")){
        QString intVal= options.value("log-start");
        int n= intVal.toInt(&ok);
        if(!ok || n<1){
            throw ThError(12052, "namec: expecting 1.. for --log-start.  Got '%s'", intVal);
        }
        eventStack->setFirstLogEvent(n);
    }
    if(options.isSet("log-depth")||options.isSet("log-max")){
        //!\todo Implement --log-depth, --log-max
        throw ThError(12054, "namec: --log-depth and --log-max are not implemented");
    }
}//configureLogging

void 
readOptions(const QCoreApplication &app, QCommandLineParser *options)
{
    if(app.arguments().count()==1){
        printHelp(app);
        exit(0);
    }
    QCommandLineOption assemble(QStringList() << "A" << "assemble", "description", "file");
    options->addOption(assemble);
    QCommandLineOption compile(QStringList() << "C" << "compile", "description", "file");
    options->addOption(compile);
    QCommandLineOption extra(QStringList() << "X" << "extra-code", "description", "file");
    options->addOption(extra);
    QCommandLineOption execute(QStringList() << "e" << "execute");
    options->addOption(execute);
    QCommandLineOption tla(QStringList() << "T" << "tla", "description", "file");
    options->addOption(tla);
    QCommandLineOption help(QStringList() << "?" << "h" << "help");
    options->addOption(help);
    QCommandLineOption syntax(QStringList() << "s" << "syntax");
    options->addOption(syntax);
    QCommandLineOption version(QStringList() << "V" << "version", "description");
    options->addOption(version);
    QCommandLineOption trace(QStringList() << "t" << "trace", "description", "N");
    options->addOption(trace);
    QCommandLineOption verbose(QStringList() << "v" << "verbose");
    options->addOption(verbose);
    QCommandLineOption ignoreErrors(QStringList() << "i" << "ignore-errors");
    options->addOption(ignoreErrors);
    QCommandLineOption start(QStringList() << "S" << "log-start", "description", "N");
    options->addOption(start);
    QCommandLineOption level(QStringList() << "L" << "log-depth", "description", "N");
    options->addOption(level);
    QCommandLineOption max(QStringList() << "M" << "log-max", "description", "N");
    options->addOption(max);
    QCommandLineOption out(QStringList() << "o" << "log-stdout");
    options->addOption(out);
    QCommandLineOption noProgram(QStringList() << "n" << "no-program");
    options->addOption(noProgram);
    if(!options->parse(app.arguments())){
        logStream() << "namec: " << options->errorText() << "\n";
        logStream() << "Usage: namec [OPTION]... [FILE.nc]...\n";
        logStream() << "Try 'namec --help' for more information\n";
        exit(1);
    }
    if(options->isSet(syntax)){
        printHelpSyntax();
        exit(0);
    }
    if(options->isSet(help)){
        if(options->positionalArguments().contains("syntax")){
            printHelpSyntax();
        }else{
            printHelp(app);
        }
        exit(0);
    }
}//readOptions

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("C.B. Barber");
    QCoreApplication::setOrganizationDomain("thesa.com");
    QCoreApplication::setApplicationName("namec");
    QCoreApplication::setApplicationVersion("http://github.io/namec-git  2018.1  2018/09/14 1.0.0");
    QCommandLineParser options;
    readOptions(app, &options);
    int status= 0;
    try{
        configureLogging(options);
        bool isAsm= options.isSet("assemble");
        bool isCompile= options.isSet("compile");
        bool isExecute= options.isSet("execute");
        bool isExtra= options.isSet("extra-code");
        if(!(isAsm||isCompile||isExecute)){
            throw ThError(12053, "namec: expecting at least one  --execute/-e, --assemble/-A=file, --compile/-C=file.  None found");
        }
        if(isExtra && !isCompile){
            throw ThError(12053, "namec: expecting --compile/-C=file  with --extra-file/-X=file '%s'.  Got --execute or --assemble", options.value("extra-file"));
        }
        bool isIgnoreErrors= options.isSet("ignore-errors");
        if(isAsm){
            QString outAsm= options.value("assemble");
            QString outAsmData= outAsm;
            int baseExtension= outAsmData.lastIndexOf(QRegularExpression("[\\.\\\\/]"));
            if(baseExtension==NoIndex || outAsmData.at(baseExtension)!='.'){
                baseExtension= outAsmData.size();
            }
            outAsmData.insert(baseExtension, "_data");

            ThAsmCode code(NULL); // no ThDisk;
            if(isIgnoreErrors){
                code.setIsIgnoreErrors();
            }
            int program= appendProgram(&code, options);
            logStream() << "== assemble ==\n"; 
            code.writeAsmCode(program, outAsm, outAsmData);
        }
        if(isCompile){
            ThCoreCode code(NULL); // no ThDisk;
            if(isIgnoreErrors){
                code.setIsIgnoreErrors();
            }
            int program= appendProgram(&code, options);
            logStream() << "== compile ==\n"; 
            code.writeCoreCode(program, options.value("compile"), options.value("extra-code"));
        }
        if(isExecute){
            ThExecuteCode code(NULL); // no ThDisk;
            if(isIgnoreErrors){
                code.setIsIgnoreErrors();
            }
            int program= appendProgram(&code, options);
            logStream() << "== execute ==\n"; 
            code.executeProgram(program);
        }
    }catch(const std::exception &e){
        logStream() << "FAIL!  : exception thrown\n";
        logStream() << e.what() << endl;
        status= 1; //!<\todo Set return status to error code
    }
    if(!RoadError::emptyGlobalLog()){
        logStream() << RoadError::stringGlobalLog() << endl;
        RoadError::clearGlobalLog();
    }
    return status;
}//Thesa::main

}//Thesa

int main(int argc, char *argv[])
{
    try{
        return Thesa::main(argc, argv);
    }catch(const std::exception &e){
        Thesa::ErrStream << "FAIL!  : namec did not catch error\n";
        Thesa::ErrStream << e.what() << endl;
    }catch(...){
        Thesa::ErrStream << "FAIL!  : namec threw a segfault or unknown error, not std::exception" << endl;
    }
    return -1;
}//main
