Three Turing machines->

  select Machine->
      // Turing machine= incrementer
      Turing machine= 3-state busy beaver
      // Turing machine= 5-state busy beaver
      // Skip print= true  // true for timing runs, or grep -v ^.$

  define Turing machines->
  
  // Define the incrementer Turing machine
  incrementer.firstCell= c001
      Cells.c001= 1
        Right.c001= c002
      Cells.c002= 1
        Left.c002= c001
        Right.c002= c003
      Cells.c003= 1
        Left.c003= c002
      incrementer.firstState= go
      incrementer.Blank= B

      // transitions  
      1.go= r001
          Write.r001= 1
          Move.r001= Right
          NextState.r001= go

      B.go= r002
          Write.r002= 1
          Move.r002= Stay
          NextState.r002= STOP

  // 3-state busy beaver Turing machine
  3-state busy beaver.firstCell= c010
      Cells.c010= 0
      3-state busy beaver.firstState= a
      3-state busy beaver.Blank= 0

      // transitions
      0.a= r010  
          Write.r010= 1
          Move.r010= Right
          NextState.r010= b

      1.a= r011  
          Write.r011= 1
          Move.r011= Left
          NextState.r011= c

      0.b= r012  
          Write.r012= 1
          Move.r012= Left
          NextState.r012= a

      1.b= r013  
          Write.r013= 1
          Move.r013= Right
          NextState.r013= b

      0.c= r014  
          Write.r014= 1
          Move.r014= Left
          NextState.r014= b

      1.c= r015  
          Write.r015= 1
          Move.r015= Stay
          NextState.r015= STOP

  // 5-state busy beaver Turing machine
  5-state busy beaver.firstCell= c020
      Cells.c020= 0
      5-state busy beaver.firstState= A
      5-state busy beaver.Blank= 0

      // transitions
      0.A= r020  
          Write.r020= 1
          Move.r020= Right
          NextState.r020= B
      1.A= r021  
          Write.r021= 1
          Move.r021= Left
          NextState.r021= C

      0.B= r022  
          Write.r022= 1
          Move.r022= Right
          NextState.r022= C
      1.B= r023  
          Write.r023= 1
          Move.r023= Right
          NextState.r023= B

      0.C= r024  
          Write.r024= 1
          Move.r024= Right
          NextState.r024= D
      1.C= r025  
          Write.r025= 0
          Move.r025= Left
          NextState.r025= E

      0.D= r026  
          Write.r026= 1
          Move.r026= Left
          NextState.r026= A
      1.D= r027  
          Write.r027= 1
          Move.r027= Left
          NextState.r027= D

      0.E= r028  
          Write.r028= 1
          Move.r028= Stay
          NextState.r028= STOP
      1.E= r029  
          Write.r029= 0
          Move.r029= Left
          NextState.r029= A

