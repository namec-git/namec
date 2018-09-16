//================
//== namec-test-core.nc - test core instructions
//================

test core name machine using core instructions->

  test AssignLiteral->
    lit.a= a
    lit.b= b
    lit.v= v
    lit.w= w
    lit.x= x
    a.b= w
    lit.a[lit.b]==lit.w ->OK a.b==w
    ->Error failed 'a.b=w'
    OK a.b==w->
    lit.a[lit.b]==lit.a  ->Error a.b==a or any literal not w

  test AssignValue ->
    c.d= x
    e.f= y
    g.h= z
    y.z= v
    a.b[c.d]= e.f[g.h]
    a.b[c.d]==y.z       ->OK a.b[c.d]==y.z
    ->Error failed 'a.b[c.d]= y.z'
    OK a.b[c.d]==y.z->
    lit.w[lit.x]==lit.v ->OK w.x==v
    ->Error failed 'a.b[c.d]= e.f[g.h]'
    OK w.x==v->
    a.b[c.d]==lit.v      ->OK a.b[c.d]==v
    ->Error failed 'a.b[c.d]==v'
    OK a.b[c.d]==v->
    a.b[c.d]==lit.a     ->Error a.b[c.d]==a or any literal not v
      
  test == ->
    // see [[test AssignValue]]
    lit.lit= lit
    lit.lit[lit.lit]==lit.lit  ->OK lit
    ->Error failed lit
    OK lit->
    
  test th_NEW core->
    lit.new= new
    lit.x[lit.new]==th_NEW      ->OK x.new
    ->Error failed x.new
    OK x.new->
    x.new= th_NEW
    lit.x[lit.new]==th_NEW      ->Error: x.new==th_NEW
    x.new==z                    ->Error: x.new not unique
    ->test goto label
    ->Error failed goto label 

  test goto name.field->
    x.label= test goto label
    ->x.label
    ->Error failed goto name field 
  
  test goto label->
    // goto label tested above

  namec-test-core completed successfully=    
