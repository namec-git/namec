
Namec -- name code for programming with arbitrary names

      Version 2018.1 2018/09/14 1.0.0
  
      Documentation:
        namec --help; namec --syntax
        http://github.com/namec-git/namec/tree/master/doc/namec-help.txt
        http://github.com/namec-git/namec/tree/master/doc/pseudocode.rtf

      Available from:
        <http://qhull.org/download/namec-2018.1-1.0.0.zip> with namec.exe
        <http://github.com/namec-git/namec> (git@github.com:namec-git/namec.git)
 
Purpose

  Name code is for programming with arbitrary names.  Name code has a flexible syntax 
  with trivial semantics.  Everything is a name.  Its hello world program is a test 
  of name-machine.nc.  Its traditional hello world is:

     say hello->
     hello world=
  
  The distribution includes programs for regular expressions, three Turing machines, 
  a name machine, and various tests.  As well as executing name code, Namec compiles
  name code for name-machine.nc or assembles name code for Masm32.  
  
  Currently, name code is not practical.  It does not support integers, floats,
  strings, booleans, expressions, parameters, types, or external programs.  The 
  assembly code is wasteful of memory.  It ignores registers and the stack.

  Despite these limitations, name code programs can read like pseudocode.
  
Challenges

  1) Write name code to implement th_NEW without using th_INC (name-machine.nc)
  
  2) With (1), start the name code clock.  It prints th_NEW values indefinitely. 
     Be careful of memory.  The clock needs to run for at least a month.
  
  3) Write name code for 32-bit arithmetic.
  
  4) Write a verifier for well-formed name code.
  
Environment requirements

  Namec is a C++ program based on the Qt framework (qt.io).  
  
  The distribution includes a Windows executable for Intel hosts, compiled
  with Visual C++ 2017.  The generated assembly code requires Masm32 for 
  Windows installed in /masm32/ (www.masm32.com).  Define environment 
  variable $QTDIR.  Review PlatformToolset in *.vcxproj. 
  
  Namec is copyrighted software.  Please read COPYING.txt, LICENSE.txt,
  and REGISTER.txt before using or distributing Namec.

To cite Namec, please use

  Barber, C.B., "Name code for understandable programs,"
  http://github.com/namec-git/namec/tree/master/doc/pseudocode.rtf, 2018.

-----------------
Distributed files

  README.txt                // An introduction to Namec
  COPYING.txt               // Namec copyright notice 
  LICENSE.txt               // Apache License, Version 2.0
  REGISTER.txt              // Namec registration
  namec.sh                  // Shell program for maintaining Namec
  .gitignore                // git ignore file

  bin/namec.exe             // Windows executable for Intel hosts
  bin/turing-sample.exe     // Turing machine for 5-state busy beaver
  bin/Qt5Core.dll           // Qt dll for namec.exe
  
  build/namec/namec.vcxproj // A DevStudio project for namec

  doc/namec-help.txt        // Namec arguments, examples, and name code syntax
  doc/pseudocode.rtf        // An introduction to name code with programs

  src/Changes-namec.txt     // Change history for Namec
  src/thid-app.pri          // Include file for Qt projects 
  src/thid-warn.pri         // Include warnings for Qt projects

  src/nameasm/turing-sample.asm // Assembly code for bin/turing-sample.exe
  src/nameasm/turing-sample_data.asm 
  src/nameasm/nameasm.vcxproj // A DevStudio project for nameasm and /masm32
  src/nameasm/NameAsmRun.asm // Assembly runtime for Masm32 name code
  src/nameasm/NameAsmRun.inc // Symbols for name.asm and NameAsmRun.asm

  src/namec/namec.pro       // Qt project file for Namec
  src/namec/namec.cpp       // The Namec program
  src/namec/RoadUtility_mini.* // Utility functions for Namec

  src/road/RoadArg64.*      // 64-bit args for error reporting
  src/road/RoadError.*      // Report an error 
  src/road/RoadEvent.*      // An event (a call, log, warning, or error)
  src/road/RoadEventPool.*  // Event history
  src/road/RoadEventStack.* // Event stack
  src/road/RoadLogEvent.*   // A logged event in RoadEventPool

  src/thidcode/ThAsmCode.*  // Write Masm32 code for a ThCode program
  src/thidcode/ThCode.*     // Generate a byte code program from name code
  src/thidcode/ThCoreCode.* // Compile core code from a ThCode program
  src/thidcode/ThExecuteCode.*  // Execute a ThCode program

  src/thidcore/ThCall.*     // Log a call event to RoadEventPool and stderr
  src/thidcore/ThError.*    // Throw an error.  Log to RoadEventPool and stderr
  src/thidcore/ThLog.*      // Log an event to RoadEventPool and stderr
  src/thidcore/ThWarn.*     // Log a warning to RoadEventPool and stderr
  src/thidcore/thesa.h      // Precompiled header file
  src/thidcore/tnamespace.h // Namespace defined symbols
  src/thidcore/tglobal.h    // Globally defined macros

  test/hello-test.nc        // hello world test program
  test/hello-test-core.nc   // hello world test program for name-machine.nc
  test/hello-test-core-compile.nc // hello world for compiling name-machine.nc

  test/namec-test.nc        // Test print instructions
  test/namec-test-core.nc   // Test core instructions
  test/namec-test-errors.nc // Test syntax errors
  test/namec-test-literals.nc // Test literal names
  test/namec-test-name-field.nc // Test name.field syntax
  test/namec-test-try.nc    // Test try instructions
  
  test/name-machine.nc      // a name code machine for core code (*.nc.nc)
  test/regexp-dfa.nc        //  a regular expression recognizer
  test/regexp-test.nc       //  Tests for regexp-dfa.nc
  test/turing-machine.nc    // a universal turing machine
  test/turing-samples.nc    // Sample turing machines from rosetta-code.org

-----------------
Author:

  C. Bradford Barber
  bradb@shore.net
  
  If you find Namec useful, please let me know.
