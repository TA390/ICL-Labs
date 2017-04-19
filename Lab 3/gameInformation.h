#ifndef GAMEINFORMATION
#define GAMEINFORMATION


#include<vector>

/******************************************************************************/

enum Colour{White, Black};
enum Piece{Rooks, Knights, Bishops, Queens, Kings, Pawns};
enum Move{Horizontal, Vertical, Diagonal, Horse, Invalid};

/******************************************************************************/

const short FILE_INDEX = 0;
const short RANK_INDEX = 1;

/* An empty string is used to show that a king in not in check */
const std::string NOT_IN_CHECK = "";

/* An empty string is used to show that a king is not castling */
const std::string NO_CASTLING = "";

/******************************************************************************/

/* the first and last file, initialised to their ASCII character values */
enum File{A='A', H='H'};

/* the first and last rank, initialised to their ASCII character values */
enum Rank{FIRST='1', LAST='8'};

/******************************************************************************/


class GameInformation{

 public:

  /* Function which takes a string 'input' and checks that it corresponds to a
     valid position on the board. The function returns true if 'input' is a
     position on the board and false otherwise */
  bool parse_input(std::string input) const;

  /* Function to update 'source' to a new position on the board. The parameters
     'col_step' and 'row_step' are the values to be added to the character
     representing the file and the character representing the rank */
  void move_piece(short col_step, short row_step, std::string &source);

  /* KING INFORMATION */
  /* Function to set the 'king_check' attribute (prefixed by its colour) and
     the location of the piece threatening. The parameter 'king' is the colour
     of the king to set. The parameter 'threat' is the position of the
     threatening piece (or an empty string if no threat) */
  void set_king_check(Colour king, std::string threat);

  /* Function to set the 'king_position' attribute (prefixed by its colour).
     The parameter 'king' is the colour of the king to set. The parameter
     'position' is the current position of the king */
  void set_king_position(Colour king, std::string position);

  /* Function to return true if a king is in check and false if it is not. The
     parameter king is the colour of the king whose status is returned */
  bool king_check_status(Colour player) const;

  /* Function to return the current position of the king. The parameter
     'player' is the colour of the king to be returned */
  std::string king_position(Colour player) const;

  /* Function to return the position of the piece threatening a player's king.
     The parameter 'player' is the colour of the king under threat */
  std::string threatening_piece(Colour player) const;


  /* CASTLING */
  /* Function to set the attribute 'king_castling'. The parameter
     'rook_to_castle' is the position of the rook to perform the move with */
  void set_king_castling(std::string rook_to_castle);

  /* Function to return the attribute 'king_castling' which is the position
     of the rook to perform the move with */
  std::string get_king_castling() const;


  /* Function to set the attribute 'castling_direction' which is the direction
     the king is moving to perform the move. The parameter direction is a 
     value of 1 or -1 (left or right) */
  void set_castling_direction(short direction);

  /* Function to return the attribute 'castling_direction' */
  short get_castling_direction() const;



 private:

  /* Arrtibutes to signal whether a king is in check */
  bool white_king_check;
  bool black_king_check;

  /* Attributes to store the position on the threatening piece. Made static
     to be access by all pieces when trying to remove a threat on their king */
  static std::string threat_to_white_king;
  static std::string threat_to_black_king;

  /* Attributes to store the current position of a king. Made static as 
     it is required by all pieces to check if they can defend their own king */
  static std::string white_king_position;
  static std::string black_king_position;


  /* CASTLING */
  /* the position of the rook to castle with */
  static std::string king_castling;

  /* the direction the king is moving to perform the castling move */
  static short castling_direction;


};

/******************************************************************************/


#endif
