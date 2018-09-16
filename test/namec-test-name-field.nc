//================
//== namec-test-name-field.nc -- Test core instructions for name.field variations
//================

test name field->

  test AssignValue name field ->
    a.y= x
    a[a.y]= w
    a[a.y]!=w   ->Error a[a.y]!=w
    a.x!=w      ->Error a.x!=w

    w.x= u
    w.u= v
    w.x[w.u]= t
    w.x[w.u]!=t ->Error w.x[w.u]!=t
    u.v!=t      ->Error u.v!=t

    a.y[a]= u.v
    a.y[a]!=u.v  ->Error a.y[a]= u.v
    x.a!=t       ->Error x.a!=t
    
    b.c= a.y[a]
    b.c!=a.y[a]  ->Error b.c!=a.y[a]
    b.c!=t       ->Error b.c!=t

    c.d= a[a.y]
    c.d!=a[a.y]  ->Error c.d!=a[a.y]
    c.d!=w       ->Error c.d!=w

    x.x= y
    e.f= a.y[a.y]    
    e.f!=a.y[a.y] ->Error e.f!=a.y[a.y]
    e.f!=y       ->Error e.f!=y

  test AssignValue variables ->
    V= z
    V==z        ->ok2
    !> Error V==z
    ok2->
    
    W= V
    W == V      ->ok3
    !> Error W==V
    ok3->
    W!=z        ->Error W!=z
    
    V.a= x
    V.a != x    ->Error V.a!= x
    V[a]= V.a
    V[a]==x     ->ok4
    !> Error V a!=x
    ok4->
    
    W= a[a.y]
    a[a.y]!=W   ->Error a[a.y]!=W
    a.x!=W      ->Error a.x!=W
    w!=W        ->Error w!=W
    W!=a[a.y]   ->Error W!=a[a.y]
    W!=a.x      ->Error W!=a.x

    W[a.y]= u    
    W[a.y]!=u   ->Error W[a.y]!=u
    w.x!=u      ->Error w.x!=u

    U= W[a.y]
    W[a.y]!=U   ->Error W[a.y]!=U
    U!=u        ->Error U!=u
    
    W[U]=v
    W[U]!=v     ->Error W[U]!=v
    w.u!=v      ->Error w.u!=v

    V= W[U]
    V!=W[U]     ->Error V!=W[U]
    v!=V        ->Error v!=V

  test == and != ->
    // see [[test AssignValue]]
    w!=w        ->Error w!=w
    1==2        ->Error 1=2
    
  test ifEquals th_NEW->
    x2.new!=th_NEW ->Error x2.new!=th_NEW
    x2.new==th_NEW ->assign x2.new
    !>Error not new
    
    assign x2.new->
    x2.new= th_NEW
    x2.new==th_NEW ->Error: x2.new==th_NEW

  namec-test-name-field completed successfully=    
  
