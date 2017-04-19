#ifndef TUBE_H
#define TUBE_H


enum Direction {N, NE, E, SE, S, SW, W, NW, INVALID_DIRECTION};

/* error codes for Question 3 */
#define ERROR_START_STATION_INVALID -1 
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width);

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width);

/* pre-supplied helper function to describe errors for Question 3 */
const char *error_description(int code);

/* presupplied helper function for converting string to Direction enum */
Direction string_to_direction(const char *token);

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows);


/******************************************************************************/
/*                              GLOBAL CONSTANTS                              */
/******************************************************************************/

/* List of all files contained in the array for indexing specific files.  */
enum {STATIONS, LINES, NUMBER_OF_FILES};

/* Struct containing an array of all files */
struct Files{

  const char* file_array[NUMBER_OF_FILES]
  {  
    "stations.txt", 
    "lines.txt"
  };

};

/* Max line length when searching a file line by line */
const int MAX_LINE_LENGTH = 128;

/* Max line length (column width) for the 2D array */
const int MAX_MAP_LINE = 512;

/******************************************************************************/
/*                          END OF GLOBAL CONSTANTS                           */
/******************************************************************************/



/******************************************************************************/
/*                                 QUESTION 1                                 */
/******************************************************************************/

/* Function to locate 'target' in the 2D array 'map'. The parameters 'height'
   and 'width' are the array dimensions. The function assigns 'row' and 'col'
   the index position of 'target' and returns true. If 'target' was not located
   return false and set the value of 'row' and 'col' to -1. */
bool get_symbol_position(char **map, const int height, const int width, 
			 const char target, int &row, int &col);

/******************************************************************************/
/*                             END OF QUESTION 1                              */
/******************************************************************************/



/******************************************************************************/
/*                                 QUESTION 2                                 */
/******************************************************************************/

/* Function returns the character symbol, which corresponds to 'name'. If 'name'
   could not be found return the space character (' ') */
char get_symbol_for_station_or_line(const char *name);


/* INTERNAL HELPER FUNCTION */
/* Function to test the state of an ifstream. If the state is not good, display
   an error message and terminate the program. */
void test_stream_state(ifstream &in_stream, const char *file_name);


/* INTERNAL HELPER FUNCTION */
/* Function to perform a case insensitive search. The function compares 'str_1'
   against 'str_2' and returns 0 if they match. The parameter 'on' is set true
   by default to perform a case insensitive compare. Setting this to false will
   call strcmp() and perform a case sensitive compare */
int case_insensitive_cmp(const char* str_1, const char* str_2, bool on);


/* INTERNAL HELPER FUNCTION */
/* Function to search for 'name' inside 'file' and return the character symbol
   which corresponds to 'name'. Return false if 'name' was not found */
char search_file_for_name(const char *name, const char *file);

/******************************************************************************/
/*                             END OF QUESTION 2                              */
/******************************************************************************/



/******************************************************************************/
/*                                 QUESTION 3                                 */
/******************************************************************************/

/* Function to check that 'route' is a valid journey within 'map'. The
   parameters 'height' and 'width' are the row height and column width of the
   array. The function starts the journey from 'start_station' and sets
   'destination' to the name of the final station (if a valid station). If
   successful the function returns the number of line changes made on a journey.
   If unsuccessful it returns the relevant error code. */
int validate_route(char **map, const int height, const int width, 
		   const char *start_station, const char *route, 
		   char *destination);


/* INTERNAL HELPER FUNCTION */
/* Function to test whether the starting position 'start_station' is a valid
   station. 
   throw ERROR_START_STATION_INVALID to signal failure */
char validate_start_station(const char *start_station);


/* INTERNAL HELPER */
/* Struct to break up the directions inside the string 'route' */
struct Route{

  /*Make a copy of route and initialise all variables */
  Route(const char *route, const char delimiter);

  /* Convert the string route into sub strings by adding '\0' in place of the
     delimiters. Return the first character of the next sub string */
  char get_direction();

  char *route;
  const char delimiter;
  /* Keep track of the current location within the string route */
  int route_index;
  Direction current_direction;
  Direction previous_direction;

  /* Delete all resources acquired */
  ~Route();

};


/* INTERNAL HELPER FUNCTION */
/* Function takes a 'direction' and sets 'row_position' and 'col_position' to 
   the new position within the map.
   throw ERROR_INVALID_DIRECTION to signal failure */
void travel_on_map(int &row_position, int &col_position, Direction direction);


/* INTERNAL HELPER FUNCTION */
/* Function to test if the current position is within the dimensions of the
   array. The parameters 'row_height' and 'col_width' are the dimensions of the
   array. The parameters 'current_row' and 'current_col' is the current
   row and column index position */
void is_route_out_of_bounds(const int row_height, const int col_width, 
			    const int current_row, const int current_col);


/* INTERNAL HELPER FUNCTION */
/* Function to test that the current position within the 2D array map is on
   the track or at a station.
   throw ERROR_OFF_TRACK to signal failure */
void is_position_off_track(const char position_on_map);


/* INTERNAL HELPER FUNCTION */
/* Function checks that a backwards move has not been made on the line while
   not at a station.
   throw ERROR_BACKTRACKING_BETWEEN_STATIONS to signal failure */
void is_move_backtracking(const char previous_position,
			  Direction previous_direction, 
			  Direction current_direction);


/* INTERNAL HELPER FUNCTION */
/* Function to test whether an invalid line change has occurred.
   throw ERROR_LINE_HOPPING_BETWEEN_STATIONS to signal failure */
void is_line_change_valid(const char previous_position, 
			  const char current_position);


/* Struct to keep count of all line changes and the current train line */
struct Train_line{
  
  Train_line();

  /* Increment 'number_of_changes' for each line change */
  void operator()(const char current_position);

  int number_of_changes;
  char line;

};


/* INTERNAL HELPER FUNCTION */
/* Function to test that the final destination is a station. The function tests
   whether 'end_station_symbol' is a valid station and assigns 'destination' its
   stations name. The parameter 'file' is the name of the file to be searched 
   throw ERROR_ROUTE_ENDPOINT_IS_NOT_STATION to signal failure */
void validate_end_station(char *destination, const char end_station_symbol, 
			  const char *file);

/******************************************************************************/
/*                             END OF QUESTION 3                              */
/******************************************************************************/


#endif
