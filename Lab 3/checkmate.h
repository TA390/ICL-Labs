#ifndef GAMEINFORMATION
#define GAMEINFORMATION

/******************************************************************************/

enum Colour: bool{White, Black};
enum Piece : short{Rooks, Knights, Bishops, Kings, Queens, Pawns};
enum Move{Horizontal, Vertical, Diagonal, Horse, Invalid};

/******************************************************************************/

const short FILE_INDEX = 0;
const short RANK_INDEX = 1;

/******************************************************************************/

class GameInformation{

 public:

  void set_king_check(Colour king, bool check);
  void set_king_position(Colour king, std::string position);

  bool king_check_status(Colour player);
  std::string king_position(Colour player);

 private:
  
  bool white_king_check;
  bool black_king_check;

  std::string white_king_position;
  std::string black_king_position;

};

/******************************************************************************/


#endif
