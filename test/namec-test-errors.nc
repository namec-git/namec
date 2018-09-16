//================
// namec-test-errors.nc tests namec for errors
//
// See 'namec.sh test'
// $Id: //main/2005/thid/test/namec-test-errors.nc#3 $$Change: 2471 $
// $Date: 2018/09/07 $$Author: bbarber $
//================

test errors->
  [[==> See EXPECTED_ERRORS in 'namec.sh test-nc']]=

  test variable errors->
    V= v
    U= u
    V.U=a
    a.V=a
    V.x[a]=a
    x.V[a]=a
    a[V.x]=a
    a[x.V]=a

  test printFields->
     // 'thid-00080-th-0798-8714' is the name of this program.  Its fields are its labels and variables
     thid-00080-th-0798-8714=! // print fields of a name

  test literal errors->
    a.b
    a.b= a[]

  test explicit literals [[...->
    a.b=a  a.b= [[not a literal  name]]  a.c=a
    a [[not a literal field]]= b
    [[literal name]].c== [[ literal name]] lkjasdf  -> Error not a literal try 
    
  test th_NEW errors->
    th_NEW= a
    th_NEW.a= x
    a[th_NEW]= y
    th_NEW==a    ->Error th_NEW must be rhs
    x.th_NEW!=y  ->Error x.th_NEW!=y
    x.new= th_NEW.xyz
    x.new= th_NEW[xyz]
    x.new= x.th_NEW
    x.new= x[th_NEW]
    x.new= x[th_NEW.a]
    x.new= x[a.th_NEW]
 
test th_NEW as name.field, may be a future enhancement->
     x.new==th_NEW.xyz   -> th_NEW.xyz assigned
     x.new==th_NEW[xyz]  -> th_NEW[xyz] assigned
    x.new==x.th_NEW     -> x.th_NEW assigned 
     x.new==x[th_NEW]    -> x[th_NEW] assigned 
     x.new==x[th_NEW.a]  -> x[th_NEW.a] assigned 
     x.new==x[a.th_NEW]  -> x[a.th_NEW] assigned 
     x.new==x.x[th_NEW]    -> x.x[th_NEW] assigned 
     x.new==th_NEW[x.x]    -> th_NEW[x.x] assigned 
 
  test label errors->
    -> an instruction cannot start and end with ->
    
  test try errors->
      UnknownTry== ->With option '--ignore-errors', warn on try or call of an unknown label
      UnknownCall

