#include<iostream>

#include"gameInformation.h"
#include"chessPiece.h"

using namespace std;

/******************************************************************************/

void GameInformation::set_king_check(Colour king, bool check)
{
  if(king == Colour::White)
  {
    white_king_check = check;
  }
  else
  {
    black_king_check = check;
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

bool GameInformation::king_check_status(Colour king)
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

string GameInformation::king_position(Colour king)
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
