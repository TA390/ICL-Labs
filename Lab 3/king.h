#ifndef KING
#define KING


#include"chessPiece.h"

/******************************************************************************/

class King: public ChessPiece{

 public:

  /* Constructor to initilaise the base class */
  King(Colour _player, std::string _position, Board *_game_board);

  /* Overridden function - Function to test if a move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the piece can make the move and false otherwise */
  bool move(std::string destination);


 private:

  /* Max move of a king */
  const short MAX_MOVE;

  /* INTERNAL HELPER FUNCTION */

  /* Overridden function - Function to test if the type of move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the type of move is one the piece can make and false otherwise */
  bool is_move_valid(std::string destination);

  /* Function to test if the move is within the max move of a king. The 
     parameter 'file_move' is the distance to move along the file. The
     parameter 'rank_move' is the distance to move along the rank. The
     function return true if the move is available and false otherwise */
  bool is_move_available(short file_move, short rank_move) const;

  /* Overridden function - Function to test if a king can capture the
     threatening piece. The parameter 'THREAT' is the position of the
     threatening piece. The function returns true if the king can capture the
     piece and false otherwise */
  bool remove_threat(const std::string THREAT);

  /* Overridden function - Function to set attribute 'position' in the base
     class. The parameter 'new_position' is the value to assign */
  void set_position(std::string new_position);


  /* CASTLING */
  /* Function to test if a castling move has been attempted. The parameters
     'FILE_MOVE' and 'RANK_MOVE' are the number of step to take along the file
     and rank to make the move. The function returns true if any valid move
     is attempted and false otherwise */
  bool is_castling_available(const short FILE_MOVE, const short RANK_MOVE);

  /* Function to test if a castling move is valid from the king's perspective.
     The parameter 'FILE_MOVE' is the move to be made along the file. The
     function returns true if the move is executed successfully and false
     otherwise */
  bool castle_move(const short FILE_MOVE);

  /* Function to test if the castling move is valid from the rook's perspective.
     The parameter 'end_file' is the end of the file (the start position of the 
     rook). The parameter 'direction' is the direction the king is moving to
     perform the move. The function returns true if the move is available
     and false otherwise */
  bool castling_available(std::string end_file, short direction);

};

/******************************************************************************/


#endif
