  execute Turing machine->
  
  Machine= Turing machine
  Machine=
  Cell= Machine.firstCell
  State= Machine.firstState
  Blank= Machine.Blank
  
  while State is not STOP->
    execute Rule for Cell at State->
        State==STOP                     ->exit execute Rule
        Value= Cells[Cell]
        Rule= Value[State]
        // Rule= // debug
                                        ->Move[Rule]
        
        unknown Rule->
            State=, Cell=, Value=,
            Error -- unknown Rule or missing Move for State, Cell, Value=
                                        !>
        Left->
            Cells[Cell]= Write[Rule]
            State= NextState[Rule]
            Left[Cell]==th_NEW          ->new left Cell
            Cell= Left[Cell] 
                                        ->execute Rule for Cell at State
        new left Cell->
            NewCell= th_NEW
            Cells[NewCell]= Blank
            Right[NewCell]= Cell
            Left[Cell]= NewCell 
            Cell= NewCell
            Machine.firstCell= Cell
                                        ->execute Rule for Cell at State
        Stay->
            Cells[Cell]= Write[Rule]
            State= NextState[Rule]
                                        ->execute Rule for Cell at State
        Right->
            Cells[Cell]= Write[Rule]
            State= NextState[Rule]
            Right[Cell]==th_NEW         ->new right Cell
            Cell= Right[Cell] 
                                        ->execute Rule for Cell at State
        new right Cell->
            NewCell= th_NEW
            Cells[NewCell]= Blank
            Left[NewCell]= Cell
            Right[Cell]= NewCell 
            Cell= NewCell
                                        ->execute Rule for Cell at State
        exit execute Rule->
    
  print Turing tape->
    Skip print==true                    ->exit print tape
    Cell= Machine.firstCell
    print value for each Cell->
        Cells[Cell]=
        Right[Cell]==th_NEW             ->exit print tape
        Cell= Right[Cell]
                                        ->print value for each Cell
    exit print tape->

  STOP=

        