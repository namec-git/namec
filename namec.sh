# namec.sh -- maintain namec distribution

MASM32="C:\masm32"  # '\' required for '/libpath:'
ML=$MASM32/bin/ml.exe
LINK=$MASM32/bin/link.exe
TH_HOME="${TH_HOME:-$PWD}"
NAMEC="$TH_HOME/bin/namec"
isnone=1
zip_file=namec-2018.1-1.0.0.zip

while [[ $isnone == 1 || "$1" != "" ]]; do
    if [[ "$1" == "clean-all" ]]; then
        shift
        echo Clean build/x64/namec/*/
        cd $TH_HOME/build && rm -rf x64/namec/*/*
        echo Clean src/nameasm/x64/ Debug/ *.obj *.lst
        cd $TH_HOME/src/nameasm && rm -rf x64/ Debug/* *.obj *.lst
        echo Clean test/*.x *.nc.nc *.name.asm
        cd $TH_HOME/test && rm -f *.x *.nc.nc *.name.asm
        cd $TH_HOME
    elif [[ "$1" == "nameasm" ]]; then
        shift
        rm -f bin/nameasm.* src/nameasm/*.obj src/nameasm/*.lst 
        cd src/nameasm
        $ML //I:"$MASM32\include" //Fl //nologo //w //c //coff name.asm
        $ML //I:"$MASM32\include" //Fl //nologo //w //c //coff NameAsmRun.asm
        echo $PWD
        # Define libs from DevStudio nameasm/Debug/link.command.1.tlog.  Removed uuid.lib 
        # LIBS="masm32.lib msvcrt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib odbc32.lib odbccp32.lib"
        LIBS="masm32.lib msvcrt.lib kernel32.lib"
        # $LINK //out:"..\..\bin\nameasm.exe" //machine:x86 //subsystem:console //opt:noref //nologo //nodefaultlib //libpath:"$MASM32\lib" $LIBS name.obj NameAsmRun.obj | grep -v "total image size"
        # For DevStudio link, need PATH with %MSIDE%;%MSVCBIN%;
        #   MSVCBIN=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin
        # link //out:"..\..\bin\nameasm.exe" //manifest //nxcompat //dynamicbase //machine:x86 //safeseh:no //subsystem:console //opt:noref //nologo //nodefaultlib //libpath:"$MASM32\lib" $LIBS name.obj NameAsmRun.obj | grep -v "total image size"
        $LINK //out:"..\..\bin\nameasm.exe"                                      //machine:x86              //subsystem:console //opt:noref //nologo //nodefaultlib //libpath:"$MASM32\lib" $LIBS name.obj NameAsmRun.obj | grep -v "total image size"
        # Ignored -- warning LNK4084: total image size 536956928 exceeds max (268435456); image may not run
        cd ../..
        ls -l $PWD/bin/nameasm.exe
    elif [[ "$1" == "namec-zip" ]]; then
        shift
        namec.sh clean-all
        ($NAMEC --help && $NAMEC --syntax) >doc/namec-help.txt
        bin/namec test/hello-test.nc -A src/nameasm/name.asm -n
        rm $zip_file
        FILES="namec.sh COPYRIGHT.txt LICENSE.txt README.txt REGISTER.txt \
          .gitignore bin/namec.exe bin/turing-sample.exe \
          build/namec/namec.vcxproj doc/namec-help.txt doc/pseudocode.rtf \
          src/thid-app.pri src/thid-warn.pri src/namec src/nameasm src/road \
          src/thidcode src/thidcore/thesa.h src/thidcore/tglobal.h src/thidcore/tnamespace.h \
          src/thidcore/ThCall.* src/thidcore/ThError.* src/thidcore/ThLog.* \
          src/thidcore/ThWarn.* test/*.nc \
          "
        cat >exclude.x <<EXCLUDEZIP
x.*
*.x
*.log
*.filters
*.obj
*.user
build\turing\*
src\images\*
src\nameasm\*\*
src\nameasm\name.*
src\nameasm\name_data.*
src\nameasm\NameAsmRun.lst
src\road\RoadTest.*
src\road\RoadUtility.*
src\thidcode\thidcode.pro
test\namec*.nc.nc
test\regexp*.nc.nc
test\turing*.nc.nc
test\*\*
EXCLUDEZIP
        wzzip -P -r -u $zip_file -x@exclude.x $FILES
        wzunzip -ybc -t $zip_file | grep -v " OK"
        ls -l $zip_file
    elif [[ "$1" == "test-all" ]]; then
        shift
        $NAMEC --help | tee test/test-all.x
        $NAMEC --syntax | tee -a test/test-all.x
        CMD="namec.sh test-namec test-execute test-compile test-assemble"
        OUT="test/test-all.x"
        echo ======================
        echo "== $CMD"
        echo == Output in $PWD/$OUT
        echo ======================
        echo Running tests...
        $CMD | grep -B4 -A6 -E 'Unexpected results|OK:' | tee -a $OUT
        RESULT=$(grep OK: $OUT | wc -l)        
        if [[ $RESULT -ne 4 ]]; then
            echo Error test-all: Expecting four tests.  Only $RESULT tests are OK
            exit 1
        fi
        echo OK: test-all passed
    elif [[ "$1" == "test-assemble" ]]; then
        TEST_RESULT="OK: $1 passed"
        shift
        echo Deleting previous *-nameasm.exe files
        rm -f bin/*-nameasm.exe
        pushd $TH_HOME/test
        for TEST in hello-test.nc 'hello-test-core.nc name-machine.nc' 'regexp-test.nc regexp-dfa.nc' 'namec-test-core.nc namec-test-name-field.nc namec-test-literals.nc namec-test.nc' 'turing-samples.nc turing-machine.nc'; do
            LAST=${TEST##* }
            CMD="$NAMEC $TEST --assemble ../src/nameasm/name.asm -n -o"
            OUT=$LAST.x
            echo ======================
            echo "== $CMD"
            echo == Output in $PWD/$OUT
            echo ======================
            EXPECTED_RESULTS="^== assemble =="
            RESULT=$($CMD | tee $OUT | tail -1 | grep -E "$EXPECTED_RESULTS")
            if [[ -s $OUT && "$RESULT" != "" ]]; then
                echo $RESULT
                cp ../src/nameasm/name.asm $LAST.name.asm
                cd $TH_HOME
                CMD="namec.sh nameasm"
                OUT=test/$LAST.nameasm.x
                echo ======================
                echo "== $CMD"
                echo == Output in $PWD/$OUT
                echo ======================
                RESULT=$($CMD | tee $OUT)
                if [[ -x bin/nameasm.exe ]]; then
                    mv bin/nameasm.exe bin/$LAST-nameasm.exe
                    ls -l bin/$LAST-nameasm.exe
                    if [[ "$LAST" == "turing-machine.nc" ]]; then
                        cp src/nameasm/name.asm src/nameasm/$LAST-name.asm
                        cp src/nameasm/name_data.asm src/nameasm/$LAST-name_data.asm
                    fi
                else
                    TEST_RESULT="Error test-assemble: One or more masm32/link failed -- see $OUT"
                    echo Error test-assemble: unexpected results
                    echo ...
                    tail -n 5 $OUT
                fi
                cd test
            else
                TEST_RESULT="Error test-assemble: One or more --assemble failed -- see $OUT"
                echo Error test-assemble: unexpected results
                echo ...
                tail -n 5 $OUT
            fi
        done
        echo ===========================================
        echo == Run assembled programs, bin/*-nameasm ==
        echo ===========================================
        for TEST in ../bin/*-nameasm.exe; do
            LAST=${TEST##*/}
            CMD="$TEST"
            OUT=$LAST.x
            echo ======================
            echo "== $CMD"
            echo == Output in $PWD/$OUT
            echo ======================
            EXPECTED_RESULTS="All tests passed|OK, no errors detected, status 0|^1$|Hello World"
            RESULT=$($CMD | tee $OUT | tail -15 | grep -E "$EXPECTED_RESULTS")
            if [[ -s $OUT && "$RESULT" != "" ]]; then
                echo "$RESULT"
            else
                TEST_RESULT="Error test-assemble: One or more assemble tests failed -- see $OUT"
                echo Error test-assemble: unexpected results
                echo ...
                cat $OUT | tail -10
            fi
        done        
        CMD="../bin/turing-sample.exe"
        OUT="turing-sample.x"
        echo ======================
        echo "== $CMD"
        echo == Output in $PWD/$OUT
        echo ======================
        RESULT=$($CMD | tee $OUT | grep '^1$' | wc -l)
        head -n 10 $OUT
        echo ...
        if [[ $RESULT -eq 4098 ]]; then
            echo "Result contains $RESULT '1's"
        else
            TEST_RESULT="Error test-assemble: One or more assemble tests failed -- see $OUT"
            echo "Error test-assemble: expecting 4098 '1's.  Got $RESULT"
        fi
        
        echo ======================
        echo $TEST_RESULT
        popd
    elif [[ "$1" == "test-compile" ]]; then
        TEST_RESULT="OK: $1 passed"
        shift
        pushd $TH_HOME/test
        for TEST in hello-test.nc 'hello-test-core-compile.nc name-machine.nc' 'regexp-test.nc regexp-dfa.nc' 'namec-test-core.nc namec-test-name-field.nc namec-test-literals.nc namec-test.nc' 'turing-samples.nc turing-machine.nc'; do
            LAST=${TEST##* }
            CMD="$NAMEC $TEST --compile $LAST.nc -n -o"
            OUT=$LAST.x
            echo ======================
            echo "== $CMD"
            echo == Output in $PWD/$OUT
            echo ======================
            EXPECTED_RESULTS="^== compile ==|Op:fail compiled as unknown-Op|Op:call compiled as a warning"
            RESULT=$($CMD | tee $OUT | tail -1 | grep -E "$EXPECTED_RESULTS")
            if [[ -s $OUT && "$RESULT" != "" ]]; then
                echo $RESULT
                CMD="$NAMEC $LAST.nc name-machine.nc -e -o"
                OUT=$LAST.nc.x
                echo ======================
                echo "== $CMD"
                echo == Output in $PWD/$OUT
                echo ======================
                EXPECTED_RESULTS="All tests passed|OK, no errors detected, status 0|^1$|Hello World|Op:call is not implemented"
                RESULT=$($CMD | tee $OUT | tail -15 | grep -E "$EXPECTED_RESULTS")
                if [[ -s $OUT && "$RESULT" != "" ]]; then
                    echo "$RESULT"
                else
                    TEST_RESULT="Error test-compile: One or more tests failed -- see $OUT"
                    echo Error test-compile: unexpected results
                    echo ...
                    cat $OUT | tail -10
                fi
            else
                TEST_RESULT="Error test-compile: One or more --compile failed -- see $OUT"
                echo Error test-compile: unexpected results
                echo ...
                cat $OUT | tail -5
            fi
        done
        echo ======================
        echo $TEST_RESULT
        popd
    elif [[ "$1" == "test-execute" ]]; then
        TEST_RESULT="OK: $1 passed"
        shift
        pushd $TH_HOME/test
        for TEST in hello-test.nc 'hello-test-core.nc name-machine.nc' 'regexp-test.nc regexp-dfa.nc'; do
            LAST=${TEST##* }
            CMD="$NAMEC $TEST --execute -n -o"
            OUT=$LAST.x
            echo ======================
            echo "== $CMD"
            echo == Output in $PWD/$OUT
            echo ======================
            EXPECTED_RESULTS="All tests passed|Hello World"
            RESULT=$($CMD | tee $OUT | tail -1 | grep -E "$EXPECTED_RESULTS")
            if [[ "$RESULT" != "" ]]; then
                echo "$RESULT"
            else
                TEST_RESULT="Error test-execute: One or more tests failed -- see $OUT"
                echo Error test-execute: unexpected results
                echo ...
                cat $OUT | tail -5
            fi
        done
        SAMPLE="Turing machine= 3-state busy beaver"
        if ! grep "^ *$SAMPLE" turing-samples.nc >/dev/null; then
            TEST_RESULT="Error test-execute: turing-samples.nc is misconfigured"
            echo ======================
            echo test/turing-samples.nc is not configured as
            echo $SAMPLE
        else
            CMD="$NAMEC turing-samples.nc turing-machine.nc -e -n -o"
            OUT=turing-machine.nc.x
            echo ======================
            echo "== $CMD"
            echo == Output in $PWD/$OUT
            echo ======================
            RESULT=$($CMD | tee $OUT | grep '^1$' | wc -l)
            if [[ $RESULT -eq 6 ]]; then
                cat $OUT
            else
                TEST_RESULT="Error test-execute: One or more tests failed -- see $OUT"
                echo Error test-execute: unexpected results
                echo ...
                cat $OUT | tail -5
            fi
        fi
        echo ======================
        echo $TEST_RESULT
        popd
    elif [[ "$1" == "test-namec" ]]; then
        TEST_RESULT="OK: $1 passed"
        shift
        pushd $TH_HOME/test
        OUT=namec-errors.x
        CMD="$NAMEC namec-test-errors.nc -e -i -o"
        echo ======================
        echo "== $CMD"
        echo == Output in $PWD/$OUT
        echo ======================
        EXPECTED_ERRORS="^== |[0-9]+ (call|dump|labl|prnl|rtrn)| asgn .*(V= v|U= u)|addArgument|addOperand|appendAssignCode|appendCallCode|appendInstruction|duplicate warnings|exception thrown|EXPECTED_ERRORS|external calls|ignore-errors|^ipOffset|namec-test-errors|splitArgument|th_NAME|th-0798-8714|tryAppendInstruction|U->|V->"
        RESULT=$($CMD | tee $OUT | grep -vE "$EXPECTED_ERRORS")
        if [[ -s $OUT && "$RESULT" == "" ]]; then
            echo namec-test-errors.nc reported errors as expected
        else
            TEST_RESULT="Error test-namec: Unexpected errors from namec-test-errors.nc"
            echo Error test-namec: Unexpected results
            cat $OUT | grep -vE "$EXPECTED_ERRORS"
        fi
        OUT2=namec-test.x
        CMD2="$NAMEC namec-test-core.nc namec-test-name-field.nc namec-test-literals.nc namec-test-try.nc namec-test.nc -e -n -o" 
        echo ======================
        echo "== $CMD2"
        echo == Output in $PWD/$OUT2
        echo ======================
        RESULT="$($CMD2 | tee $OUT2 | tail -1)"
        if [[ "$RESULT" == "OK, no errors detected, status 0" ]]; then
            echo $RESULT
        else
            TEST_RESULT="Error test-namec: Unexpected errors from -- $CMD2"
            echo Error test-namec: Unexpected results
            echo ...
            tail -n 10 $OUT2
        fi
        echo ======================
        echo $TEST_RESULT
        popd
    else
        if [[ "$1" != "" ]]; then 
            echo "Error namec.sh: unknown option '$1'"
        fi
        echo "namec.sh [options]"
        echo "  clean-all -- Clean all non-essential files (*.obj, *.lst, test/*.x, test/*.nc.nc, etc.)"
        echo "  nameasm -- Assemble src/nameasm/name.asm with MASM32"
        echo "  namec-zip -- WinZip namec for distribution"
        echo "  test-all -- Non-zero exit if a test fails.  Includes help"
        echo "  test-assemble -- Test --assemble on namec programs"
        echo "  test-compile -- Test --compile on namec programs"
        echo "  test-execute -- Test --execute on namec programs"
        echo "  test-namec -- Test namec errors and instructions (namec-test*.nc)"
        exit
    fi
    isnone=0
done
