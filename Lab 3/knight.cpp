#include<iostream>

#include"chessPiece.h"
#include"knight.h"

using namespace std;

/******************************************************************************/

Knight::Knight(Colour _player, string _position, Board *_game_board)
  :ChessPiece(Piece::Knights, _player, _position, _game_board)
{
}

/******************************************************************************/

bool Knight::move(string destination)
{
  if(is_move_valid(destination) == false)
  {
    return false;
  }

  return (same_player(destination) == false);
}

/******************************************************************************/

bool Knight::is_move_valid(string destination)
{

  switch(move_type(get_position(), destination))
  {
    case Move::Horse:

      return true;

    default:
      
      return false;
  }
}

/******************************************************************************/

/* Overridden function to set the movement values for a knight */

vector<string> Knight::move_list()
{
  short movements[] = {1,2};
  return move_grid(movements);
}

/******************************************************************************/

/* change the order and values of the movements array to create a different
   move each time */

template<int SIZE>
void Knight::create_next_move(short movements[SIZE], 
			      const short COUNT, const short MID_POINT)
{
  if((COUNT+1) == MID_POINT)
  {
    swap(movements[FILE_INDEX], movements[RANK_INDEX]);
  }
  
  const short negate = -1;
  /* flip the values from positive to negative to keep changing the moves */
  movements[COUNT % SIZE] *= negate;
}

/******************************************************************************/
