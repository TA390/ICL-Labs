#include<iostream>

#include"chessPiece.h"
#include"queen.h"

using namespace std;

/******************************************************************************/

Queen::Queen(Colour _player, string _position, Board *_game_board)
  :ChessPiece(Piece::Queens, _player, _position, _game_board)
{
}

/******************************************************************************/

bool Queen::move(string destination)
{
  if(is_move_valid(destination) == false)
  {
    return false;
  }

  if(piece_on_path(get_position(), destination) == true)
  {  
    return false;
  }

  return true;
}

/******************************************************************************/

bool Queen::is_move_valid(string destination)
{
  switch(move_type(get_position(), destination))
  {
    case Move::Vertical:
    case Move::Horizontal:
    case Move::Diagonal:

      return true;

    default:
      
      return false;
  }
}

/******************************************************************************/
