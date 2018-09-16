//================
//== namec-test.nc -- test goto, call, and prints OK if no errors
//================

namec test->

test goto of a variable->
    G= goto variable
    ->G
    G=
    !>Error: ->G not found
  
  goto variable->
    // OK, continue with test

  test call->
    last call
    0=
    ->
    !> Error: should not get here due to preceeding return '->'

last call->
    .   // nop
    =!  // breakpoint
    // Test print instructions and multi-instructions.  
    // Use '...=!' to print a name and its fields.  See namec-test-errors.nc
    AnyErrors= no
    =   // print a blank line
    OK=, AnyErrors =_ errors det=_ ected=_
    =, // =_ // status =_
    ->
