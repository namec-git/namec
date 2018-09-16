hello-test-core-compile.nc is hello-test-core.nc without th_NEW tests->

  //===================
  //== Literals and Labels
  //===================
  i0-lit00.op= Op:assignLiteral
    i0-lit00.nameA= th_LIT
    i0-lit00.fieldA= x
    i0-lit00.fieldB= x
    i0-lit00.next= i0-lit01
  i0-lit01.op= Op:assignLiteral
    i0-lit01.nameA= th_LIT
    i0-lit01.fieldA= th_INC
    i0-lit01.fieldB= th_INC
    i0-lit01.next= i0-lit02
  i0-lit02.op= Op:assignLiteral
    i0-lit02.nameA= th_LIT
    i0-lit02.fieldA= A
    i0-lit02.fieldB= A
    i0-lit02.next= i0-lit03
  i0-lit03.op= Op:assignLiteral
    i0-lit03.nameA= th_LIT
    i0-lit03.fieldA= w
    i0-lit03.fieldB= w
    i0-lit03.next= i0-lit04
  i0-lit04.op= Op:assignLiteral
    i0-lit04.nameA= th_LIT
    i0-lit04.fieldA= skip test th_NEW and th_INC; they do not compile OK
    i0-lit04.fieldB= skip test th_NEW and th_INC; they do not compile OK
    i0-lit04.next= i0-lit05
  i0-lit05.op= Op:assignLiteral
    i0-lit05.nameA= th_LIT
    i0-lit05.fieldA= y
    i0-lit05.fieldB= y
    i0-lit05.next= i0
  //===================
  //== Core code
  //===================
  i0.op= Op:label
    i0.fieldA= [[hello test of the core name machine, name-machine.nc]]
    i0.next= i3
  i3.op= Op:goto
    i3.nameA= th_LIT
    i3.fieldA= skip test th_NEW and th_INC; they do not compile OK
    i3.next= i6
  i6.op= Op:goto
    i6.fieldA= Error: goto failed
    i6.next= i9
  i9.op= Op:label
    i9.fieldA= skip test th_NEW and th_INC; they do not compile OK
    i9.next= i65
  i65.op= Op:assignLiteral
    i65.nameA= A
    i65.fieldA= y
    i65.fieldB= say hello
    i65.next= i70
  i70.op= Op:goto
    i70.nameA= A
    i70.fieldA= y
    i70.next= i73
  i73.op= Op:goto
    i73.fieldA= [[Error: goto A.y failed]]
    i73.next= i76
  i76.op= Op:label
    i76.fieldA= say hello
    i76.next= i79
  i79.op= Op:assignLiteral
    i79.nameA= A
    i79.fieldA= x
    i79.fieldB= Hello World
    i79.next= i84
  i84.op= Op:assignLiteral
    i84.nameA= B
    i84.fieldA= name
    i84.fieldB= A
    i84.next= i89
  i89.op= Op:assignLiteral
    i89.nameA= B
    i89.fieldA= field
    i89.fieldB= x
    i89.next= i94
  i94.op= Op:assignValue
    i94.nameA= th_LIT
    i94.fieldA= A
    i94.nameB= th_LIT
    i94.fieldB= y
    i94.nameC= B
    i94.fieldC= name
    i94.nameD= B
    i94.fieldD= field
    i94.next= i101
  i101.op= Op:ifEquals
    i101.nameA= th_LIT
    i101.fieldA= A
    i101.nameB= th_LIT
    i101.fieldB= y
    i101.nameC= A
    i101.fieldC= x
    i101.fieldD= [[assigned B.name[B.field]]]
    i101.next= i108
  i108.op= Op:goto
    i108.fieldA= [[Error: A.y!=A.x]]
    i108.next= i111
  i111.op= Op:label
    i111.fieldA= [[assigned B.name[B.field]]]
    i111.next= i114
  i114.op= Op:label
    i114.fieldA= [[print A.y]]
    i114.next= i117
  i117.op= Op:print
    i117.nameA= th_LIT
    i117.fieldA= A
    i117.nameB= th_LIT
    i117.fieldB= y
    i117.next= i120
  i120.op= Op:printNewLine
    i120.next= i121
  i121.op= Op:return
    i121.next= th_STOP
  
  //===================
  //== Name code starts here
  //===================
  th_LIT.th_NEW_value= th_NEW_value  // if as name code, would allocate a new value
  th_NameCodeProgram= i0-lit00
