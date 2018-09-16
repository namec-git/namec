test Regexps for core dfa program->

  DFA regexps tests= Test01
  DFA regexps logging= skip // skip or detail
  
  //================
  //== Define DFAs for Regexps
  //================

  //===== accept or reject '(0*1)*' 
  Regexps.(0*1)* =      (0*1)* maybe 1
  
  0.(0*1)* maybe 1=     (0*1)* repeat 0
  1.(0*1)* maybe 1=     (0*1)* maybe 1
  STOP.(0*1)* maybe 1= accept
  
  0.(0*1)* repeat 0=    (0*1)* repeat 0
  1.(0*1)* repeat 0=    (0*1)* maybe 1
  STOP.(0*1)* repeat 0= reject
  
  //===== accept or reject '(01|10)' 
  Regexps.(01|10) =     (01|10) 0 or 1

  0.(01|10) 0 or 1=     (01|10) one 1
  1.(01|10) 0 or 1=     (01|10) one 0
  STOP.(01|10) 0 or 1=  reject

  0.(01|10) one 1=      reject
  1.(01|10) one 1=      (01|10) stop
  STOP.(01|10) one 1=   reject

  0.(01|10) one 0=      (01|10) stop
  1.(01|10) one 0=      reject
  STOP.(01|10) one 0=   reject

  0.(01|10) stop=       reject
  1.(01|10) stop=       reject
  STOP.(01|10) stop=    accept
  
  //================
  //== Tests for Regexps defined above
  //================
  Test01.test-string= 01
    Test01.test-regexp= (0*1)*
    Test01.test-result= accept
    Next.Test01= Test02
  FirstSymbol.01= 0
    Next.01= 1
  FirstSymbol.1= 1
    Next.1= STOP
  FirstSymbol.STOP= STOP

  //================
  Test02.test-string= 001
    Test02.test-regexp= (0*1)*
    Test02.test-result= accept
    Next.Test02= Test03
  FirstSymbol.001= 0
    Next.001= 01
  
  //================
  Test03.test-string= 1001
    Test03.test-regexp= (0*1)*
    Test03.test-result= accept
    Next.Test03= Test04
  FirstSymbol.1001= 1
    Next.1001= 001
  
  //================
  Test04.test-string= 1
    Test04.test-regexp= (0*1)*
    Test04.test-result= accept
    Next.Test04= Test05

  //================
  Test05.test-string= 0
    Test05.test-regexp= (0*1)*
    Test05.test-result= reject
    Next.Test05= Test06
  FirstSymbol.0= 0
    Next.0= STOP
  
  //================
  Test06.test-string= 00
    Test06.test-regexp= (0*1)*
    Test06.test-result= reject
    Next.Test06= Test07
  FirstSymbol.00= 0
    Next.00= 0
  
  //================
  Test07.test-string= 00
    Test07.test-regexp= (01|10)
    Test07.test-result= reject
    Next.Test07= Test08
  
  //================
  Test08.test-string= STOP
    Test08.test-regexp= (01|10)
    Test08.test-result= reject
    Next.Test08= Test09
  
  //================
  Test09.test-string= STOP
    Test09.test-regexp= (0*1)*
    Test09.test-result= accept
    Next.Test09= Test10
  
  //================
  Test10.test-string= 01
    Test10.test-regexp= (01|10)
    Test10.test-result= accept
    Next.Test10= Test11
  
  //================
  Test11.test-string= 101
    Test11.test-regexp= (01|10)
    Test11.test-result= reject
    Next.Test11= Test12
  FirstSymbol.101= 1
    Next.101= 01

  //================
  Test12.test-string= 10
    Test12.test-regexp= (01|10)
    Test12.test-result= accept
    Next.Test12= STOP
  FirstSymbol.10= 1
    Next.10= 0

