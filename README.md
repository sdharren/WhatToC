# WhatToC
WhatToDo, but with better planning, additional features, and in C++ because Python was unbelievably slow

# To do:
- add Zobrist key to board state
  - A key for each piece, on each square (12 X 64)
  - a key for each EP square (64)
  - a key for each castling right combo (16)
  - a key for the side
- add piece list to board state
- how to initialise board:
  - read relevant information from FEN
  - initialise occupancies
  - initialise Zobrist
  - initialis piece list
- leave space for it in the game state and intialisation process
- board functions:
  - put piece, remove piece, move piece
  - set ep square
  - swap side
  - update castling rights
  - increment Zobrist and evaluation methods on each "move"
- finish generating all pseudolegal moves
- finish move gen
- add perft testing <--------- FIRST MILESTONE (doesn't need evaluation, or board state really tbh but whatever)



- being search understanding 
- read up on evaluation? 