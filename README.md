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
  - Create Move and Move_list
  - for queen, knight, bishop, rook:
    - split into quiet moves, capture moves
  - for king
    - quiet moves, capture moves
    - castling moves
  - for pawns
    - quiet moves
      - single push, double push, pawn promotion
    - capture moves
      - regular capture, promotion capture, ep capture
- finish move gen
- add perft testing <--------- FIRST MILESTONE (doesn't need evaluation, or board state really tbh but whatever)

# Minimax, Negamax, Alpha-Beta pruning
- board game score IS NOT measured relative, i.e. + is good for white, - is good for black. If it was relative, + is
good for whatever side, and - is bad for whatever side
- minimax works by:
  - for white: evaluate the children nodes, find the score that maxes the board score
  - for black: evaluate the children nodes, find the score that minimises the board score
- alpha-beta pruning
  - alpha represents the best max score reached so far, beta represents the best min score reached so far
  - at a max node, you choose the max score, but you call the min function. this means you are returning the best min
score. compare this value to the current; if it is more than, then the search has already found a better min path, so 
you can prune. Then you can check if alpha needs updating (if the val is more than the alpha), and eventually return val.
    - repeat this but opposite for the min node.
  - this means you will be checking (if val <= beta) {prune} on the max node, but algorithms implement it as
alpha >= beta. This is because, if you update the alpha value before checking val against beta, you are either:
    - checking the new val against the beta, or
    - checking the currently higher alpha against the beta - if the alpha was already higher than the beta, this branch
wouldn't even be reached. if the alpha wasn't, then the result is the same as if you checked val. This is why alpha >= beta works.
- why does negamax work
  - since chess is zerosum, +3 for white is the same as -3 black. Effectively, eval(white) == -eval(black)
  - in minimax, we have to first maximise, then minimise, due to the board score. However, instead of minimising, we can
flip black's score (multiply by -1) and maximise that.
  - This means that if we modify the static eval to return an eval relative to the side, we can focus on only maximising
Then, when we pass it to the upper function, it must be negated (for example, if we eval white's position as +3 and pass
it up to black, black needs to see it as -3; the better for white, the worse for black)
- how to implement alpha beta
  - from the max node perspective, alpha is the best max score, beta is the best min score
  - from the previous explanation, we have used negamax to reduce the original minimax algorithm to only concern the
maximising part
  - therefore, we just need to implement the alpha beta logic of the maximising function: alpha >= beta, then prune
  - consider, if on a white node, the val is +3 (good), the alpha is +4 and the beta +5
  - an alpha of 4 means white is guaranteed a 4; a beta of 5 means white can't force anything more than 5
  - therefore for black, black can't force anything more than -4 (black beta); black is guaranteed a -5 (black alpha)
  
# Evalution