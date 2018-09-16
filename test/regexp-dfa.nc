  //================
  //== Test program for DFA regexps
  //== DFA regexps tests = first test
  //== DFA regexps logging = skip or detail
  //================

  define arguments->
    Test= DFA regexps tests
    Log=  DFA regexps logging
    
  initialize variables->
    TestResult= All tests passed

  for each Test->
    Test==STOP                          ->exit for each Test
    String= Test.test-string
    Regexp= Test.test-regexp
    ExpectedResult= Test.test-result

    print Test->
        Test=, ExpectedResult=, String=, for=, Regexp=,    

    //================
    //== test String with Regexp
    //================
    test String with Regexp->
        State= Regexps[Regexp]
        Suffix= String
        Log==skip                       ->skip logging Regexp
            Logging= 
            Test=, State=, Suffix=
        skip logging Regexp->
        for each Symbol in String->
            Symbol= FirstSymbol[Suffix]
            State= Symbol[State]
            Log==skip                   ->skip logging String
                Suffix=, Symbol=, State=
            skip logging String->
            State==accept ->exit for each Symbol
            State==reject ->exit for each Symbol
            Suffix= Next[Suffix]
                                        ->for each Symbol in String
      
        exit for each Symbol->
            ExpectedResult==State       ->pass
            FAIL=
            TestResult= One or more tests failed
                                        ->next Test
        pass->
            PASS=
                                        ->next Test
    next Test->        
        Test= Next[Test]
                                        ->for each Test
      
    exit for each Test->
  TestResult=

