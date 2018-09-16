//================
//== namec-test-try.nc tests the try command '... -> on success'.  It is not implemented for --compile or --assemble
//================

test try->
  
    try ok      ->try ok succeeded
    !>Error: 'try ok' failed
    try ok succeeded->
    
    a.a= try ok
    a.a         ->indirect try succeeded
    !>Error: indirect try through 'a.a' failed
    indirect try succeeded->
    
    T= try ok
    T           ->indirect try variable succeeded
    !>Error: indirected try variable failed
    indirect try variable succeeded->

    try fail    ->Error: 'try fail' didn't fail
    a.b= try fail
    a.b         ->Error: indirect try 'a.b' didn't fail
    T= try fail
    T           ->Error: indirect try 'T' didn't fail

    [[odd literal]].c== [[ continued]].b= try ok
    [[odd literal]].c== [[ continued]].b -> literal indirect try succeeded
    !>Error: indirect try through a literal failed
    literal indirect try succeeded->

    labels for Op:ifEquals or Op:try allow all characters->
    try literal ok -> Op:try -- !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,./ ZXCVBNM<>?
    !> Error: 'try literal ok' failed
    Op:try -- !@#$%^*()_+ qwertyuiop[]\ QWERTYUIOP{}| asdfghjkl;' ASDFGHJJKL:" zxcvbnm,./ ZXCVBNM<>? ->
    ->continue with namec test

try ok->
    ->

try fail->
    !>

try literal ok->
    ->

continue with namec test->
     namec-test-try completed successfully=    

  