//================
//== namec-test-literals.nc -- test namec's conventions for literals
//== see 'namec --help syntax'
//================

test namec's conventions for literals, labels, names, and fields->

test label literals->
  a==a ->  labels   ignore extra spaces  *&*^  asdf
  !> Error: previous instruction wasn't an Op:ifEquals
  labels ignore   extra   spaces *&*^ asdf  -> 
  
  a==a -> labels for Op:ifEquals allow all characters -- !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,./ ZXCVBNM<>?
  !> Error: previous instruction wasn't an Op:ifEquals
  // Op:try tested in namec-test-try.nc

  labels for Op:ifEquals allow all characters -- !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,./ ZXCVBNM<>?->

  -> labels for Op:goto allow all characters except dot and trailing braces -- !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,_/ ZXCVBNM<>?
  !> Error: previous instruction wasn't a goto
  labels for Op:goto allow all characters except dot and trailing braces    -- !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,_/ ZXCVBNM<>? ->
  
  -> labels for Op:goto may include ->; but not Op:ifEquals and Op:try
  !> Error: previous instruction wasn't an Op:goto
  labels for Op:goto may include ->; but not Op:ifEquals and Op:try ->

test literal name.fields->

  test literal->
    X2= literal==  // Literals may end with '==' without being printed
    X2!=literal== ->Error: literal==
    // Otherwise, literals may not end with '->' or '!>' or '..='

    // Literals may not end with brackets ([...]) or contain an embedded '.'
    X=[[1234567890-= !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,./ ZXCVBNM<>?]]
    X!=[[1234567890-= !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,./ ZXCVBNM<>?]] ->Error: all characters
    Y=   1234567890-= !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,_/ ZXCVBNM<>? literal
    Y!=  1234567890-= !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,_/ ZXCVBNM<>? literal ->Error all but embedded . and trailing [...] is OK in literals
    //X=
    //Y=

    V= literal with multiple dots askdfh..asdklfjalskfjlskdjf

    X3= ^&*876]
    X3!= ^&*876] ->Error: X3!= ^&*876]

    ^&*87[6]= .6.
    X4= ^&*87[6]
    X4!= ^&*87[6] ->Error X3!= ^&*87[6]
    X4!= .6. ->Error X3!= .6.
    
    !@#$.7.bac= [7
    X5= !@#$.7.bac
    X5!= !@#$.7.bac ->Error: X5!= !@#$.7
    X5!= [7 ->Error X5!= [7

    a.b.c.d= 8   // First '.' identifies the field
    X6= a.b.c.d
    X6!= a.b.c.d    ->Error: X6!= a.b.c.d
    X6!=8           ->Error: X6!=8        
    a[ [[b.c.d]] ]!=8   ->Error: a[ [[b.c.d ]!=8

    X7= []   // Leading [ is character
    X7!= []     ->Error: X7!= []

    X8= a[
    X8!= a[     -> Error: X8!= a[

  test literal [[th_NAME]]->
    a.b= [[literal name]]
    a.b!=literal name   -> Error a.b!=literal name

    a.b= [[literal// name]]
    a.b!=[[literal// name]]   -> Error a.b!=literal comment name

    a.b=a // a.b= [[literal name]] // a.c=a
    a.b!=literal name   -> Error a.b!=literal name in multi-statement
    a.c!=a              -> Error a.c!=a in multi-statement

    a.b==a.b            -> continue test [[literal]]
    continue test [[literal]]->

    a.[[literal .]]= [[zyx]]= b // literal to last ]]= z
    a.[[literal .]]= [[zyx]]= b // literal to last ]] != z  -> Error literal field to last

    [[literal .]]= [[zyx]]= b // literal to last ]].a= z
    [[literal .]]= [[zyx]]= b // literal to last ]].a != z  -> Error name literal to last

    a.b=  [[literal .]]= [[zyx]]= b // literal to last ]]
    a.b!= [[literal .]]= [[zyx]]= b // literal to last ]]  -> Error value literal to last
    
    [[ literal name]].c= 5
    [[ literal name]].c!= 5     -> Error ' literal name'.c== 5
    [[literal name]].c= 4
    [[literal name]].c!= 4      -> Error literal name.c != 4
    literal name.c== [[ literal name]].c  -> Error literal spaces not included 

    a.b[ [[literal name]].c]= 3
    a.b[literal name.c]!= 3     -> Error a.b[literal name.c]
    a.b[4]!= 3                  -> Error a.b[4]
    
    [[literal name]].c[2]= 3
    [[literal name]].c[2]!= 3   -> Error literal name.c[2]
    4[2]!= 3                    -> Error literal 4[2]
    
  namec-test-literals completed successfully=    
  
