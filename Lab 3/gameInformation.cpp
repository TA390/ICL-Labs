#include<iostream>

#include"gameInformation.h"

using namespace std;


/******************************************************************************/
/******************************************************************************/

/* initialise all static variable */

string GameInformation::threat_to_white_king = NOT_IN_CHECK;
string GameInformation::threat_to_black_king = NOT_IN_CHECK;

string GameInformation::white_king_position = "E1";
string GameInformation::black_king_position = "E8";

string GameInformation::king_castling = NO_CASTLING;
short GameInformation::castling_direction = 0;


/******************************************************************************/
/******************************************************************************/

/* Helper function for all classes to parse a move */

bool GameInformation::parse_input(string input) const
{
  /* an input string must be of length 2 */
  const int STR_LENGTH = 2;

  if(input.size() != STR_LENGTH)
  {
    return false;
  } 

  if(input[FILE_INDEX] < File::A || input[FILE_INDEX] > File::H ||
     input[RANK_INDEX] < Rank::FIRST || input[RANK_INDEX] > Rank::LAST)
  {
    return false;
  } 

  return true;
}

/******************************************************************************/

/* update 'source' to the next position determined by 'col_step' & row_step' */

void GameInformation::move_piece(short col_step, short row_step, string &source)
{
  source[FILE_INDEX] += col_step;
  source[RANK_INDEX] += row_step;
}

/******************************************************************************/
/******************************************************************************/

void GameInformation::set_king_check(Colour king, string threat)
{
  bool check = (threat != "") ? true : false;

  if(king == Colour::White)
  {
    white_king_check = check;
    threat_to_white_king = threat;
  }
  else
  {
    black_king_check = check;
    threat_to_black_king = threat;
  }
}

/******************************************************************************/

void GameInformation::set_king_position(Colour king, string position)
{
  if(king == Colour::White)
  {
    white_king_position = position;
  }
  else
  {
    black_king_position = position;
  }
}

/******************************************************************************/

bool GameInformation::king_check_status(Colour king) const
{
  if(king == Colour::White)
  {
    return white_king_check;
  }
  else
  {
    return black_king_check;
  }
}

/******************************************************************************/

string GameInformation::king_position(Colour king) const
{
  if(king == Colour::White)
  {
    return white_king_position;
  }
  else
  {
    return black_king_position;
  }
}

/******************************************************************************/

string GameInformation::threatening_piece(Colour player) const
{
  if(player == Colour::White)
  {
    return threat_to_white_king;
  }
  else
  {
    return threat_to_black_king;
  }
}

/******************************************************************************/

void GameInformation::set_king_castling(string rook_to_castle)
{
  king_castling = rook_to_castle;
}

/******************************************************************************/

string GameInformation::get_king_castling() const
{
  return king_castling;
}

/******************************************************************************/

void GameInformation::set_castling_direction(short direction)
{
  GameInformation::castling_direction = direction;
}

/******************************************************************************/

short GameInformation::get_castling_direction() const
{
  return GameInformation::castling_direction;
}

/******************************************************************************/
