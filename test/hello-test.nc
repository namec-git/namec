hello test of the core name machine, name-machine.nc->
  ->test th_NEW
  ->Error: goto failed
  
  test th_NEW->
  A.y==th_NEW    ->A.y is th_NEW, an unknown value
  ->Error: A.y is known
  A.y is th_NEW, an unknown value->
  A.y= 

  assign th_NEW->  
  A.y= th_NEW
  A.y==th_NEW    ->Error: A.y==th_NEW
  A.y==w         ->Error: A.x==w or any other known value
  A.y=
  
  th_INC implements th_NEW->
  A.y= th_INC[A.y] 
  A.y=
  
  A.y= say hello
  ->A.y
  ->Error: goto A.y failed
  
  say hello->
  A.x= Hello World
  B.name= A
  B.field= x
  A.y= B.name[B.field]
  A.y==A.x       ->assigned B.name[B.field] 
  ->Error: A.y!=A.x
  assigned B.name[B.field]->

  print A.y->
  A.y=
