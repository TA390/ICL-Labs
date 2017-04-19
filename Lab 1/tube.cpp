#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>


using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) 
{
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) 
  {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows)
{
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) 
{
  char line[MAX_MAP_LINE];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,MAX_MAP_LINE);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,MAX_MAP_LINE);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) 
{
  bool success = get_map_dimensions(filename, height, width);

  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);

  ifstream input(filename);

  char line[MAX_MAP_LINE];
  char space[] = " ";

  for (int r = 0; r<height; r++) 
  {
    input.getline(line, MAX_MAP_LINE);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }

  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) 
{
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in 
   Question 3 */
const char *error_description(int code)
{
  switch(code) 
  {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) 
{
  const char *strings[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
  for (int n=0; n<8; n++) 
  {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}


/******************************************************************************/
/*                                 QUESTION 1                                 */
/******************************************************************************/

/* Function to locate 'target' and set 'row' and 'col' to target's index
   position */
bool get_symbol_position(char **map, const int row_height, const int col_width, 
			 const char target, int &row, int &col)
{
  assert(map && "char **map - passed to get_symbol_position()");

  for(row=0; row<row_height; row++)
    for(col=0; col<col_width; col++)
      if(map[row][col] == target)
	return true;

  row = col = -1; 
  return false;
}

/******************************************************************************/
/*                             END OF QUESTION 1                              */
/******************************************************************************/


/******************************************************************************/
/*                                 QUESTION 2                                 */
/******************************************************************************/

/* Function to find and return the character symbol of 'name' */
char get_symbol_for_station_or_line(const char *name)
{ 
  assert(name && "char *name - passed to get_symbol_for_station_or_line()");
  const char no_station_or_line = ' ';
  Files data_files;

  /* Search all files inside the array */
  for(int i=0; i<NUMBER_OF_FILES; i++) 
    if(char symbol = search_file_for_name(name, data_files.file_array[i])) 
      return symbol;
  
  return no_station_or_line;
}


/* INTERNAL HELPER FUNCTION */
/* Function to test the state of an ifstream */
void test_stream_state(ifstream &in_stream, const char *file_name)
{
  if(!in_stream)
  {
    cerr << "*The file " << file_name; 
    cerr << " could not be opened* - the program has been terminated\n"; 
    exit(1);
  }
}


/* INTERNAL HELPER FUNCTION */
/* Function to perform a case insensitive compare */
int case_insensitive_cmp(const char* str_1, const char* str_2, bool on=true)
{
  if(!on)
    return strcmp(str_1, str_2);

  for(; tolower(*str_1) == tolower(*str_2); str_1++, str_2++)
    if(*str_1 == '\0')
      break;

  return *str_1 - *str_2;
}


/* INTERNAL HELPER FUNCTION */
/* Function to search for 'name' inside 'file' and return name's character
   symbol */
char search_file_for_name(const char *name, const char *file)
{
  
  char line[MAX_LINE_LENGTH];

  ifstream in_stream{file};
  test_stream_state(in_stream, file);

  /* Read a character and check the streams state */
  for(char station_or_line_symbol=0; in_stream >> station_or_line_symbol; ) 
  {    
    /* Remove any amount of whitespace */
    in_stream >> ws;
    
    in_stream.getline(line, MAX_LINE_LENGTH);

    if(case_insensitive_cmp(name, line) == 0) 
      return station_or_line_symbol;
  }

  return false;
}

/******************************************************************************/
/*                             END OF QUESTION 2                              */
/******************************************************************************/



/******************************************************************************/
/*                                 QUESTION 3                                 */
/******************************************************************************/


/* ALL INTERNAL HELPER FUNCTIONS FOR QUESTION 3 THROW AN ERROR CODE IF AN ERROR
   IS DETECTED. THIS NEGATES THE NEED FOR MULTIPLE IF STATEMENTS IN THE FUNCTION
   BODY OF VALIDATE_ROUTE TO TEST THE RETURN VALUE OF EACH HELPER FUNCTION */


/* Function to validate each move made on a journey and return the number of
   line changes made. Return an error code for an invalid journey */
int validate_route(char **map, const int height, const int width, 
		   const char *start_station, const char *route, 
		   char *destination)
{
  assert(map && "char **map - passed to validate_route()");
  assert(start_station && "char *start_station - passed to validate_route()");
  assert(route && "char *route - passed to validate_route()");
  assert(destination && "char *destination - passed to validate_route()");
  
  /* Return value for an unknown error */
  const int ERROR_UNKNOWN = -100;

  Train_line line_changes;
  Files data_files;
  int row_position = 0;
  int col_position = 0;
  
  using Error_code = int;

  try
  {
    const char start_station_symbol = validate_start_station(start_station);
    
    get_symbol_position(map, height, width, start_station_symbol, 
			row_position, col_position);

    /* Assigns 'current_direction' the next direction */
    for(Route route_token{route, ','}; route_token.get_direction();)
    {

      char previous_position_on_track = map[row_position][col_position];

      travel_on_map(row_position, col_position, route_token.current_direction);

      is_route_out_of_bounds(height, width, row_position, col_position);

      char current_position_on_track = map[row_position][col_position];

      is_position_off_track(current_position_on_track);

      is_line_change_valid(previous_position_on_track, 
			   current_position_on_track);

      is_move_backtracking(previous_position_on_track,
			   route_token.previous_direction, 
			   route_token.current_direction);

      /* Increment member variable 'number_of_changes' for each valid change */
      line_changes(current_position_on_track);

    }  

    validate_end_station(destination, map[row_position][col_position], 
			 data_files.file_array[STATIONS]);

    return line_changes.number_of_changes;

  }
  /* catch all error codes and return it */
  catch(Error_code &error)
  {
    return error;
  }
  /* catch any unexpected errors */
  catch(...)
  {
    return ERROR_UNKNOWN;
  }

}


/* INTERNAL HELPER FUNCTION */
/* Function to test that the starting position is a station */
char validate_start_station(const char *start_station)
{
  Files data_files;

  /* Retrieve 'start_station' character symbol or return 0 */
  char station_symbol = search_file_for_name(start_station, 
					     data_files.file_array[STATIONS]); 

  return (station_symbol) ? station_symbol : throw ERROR_START_STATION_INVALID;
}


/* INTERNAL HELPER */
/* ROUTE DEFINITIONS */

/* Initialise all variables */
Route::Route(const char *_route, const char _delimiter)
  :delimiter{_delimiter}, route_index{0}, current_direction{INVALID_DIRECTION},
   previous_direction{INVALID_DIRECTION}
{
  route = new char[strlen(_route) +1];
  strcpy(route, _route);
}


/* Create sub strings within 'route' by replacing each delimiter with a '\0' */
char Route::get_direction()
{
    previous_direction = current_direction;
    const char *direction = &route[route_index];

    for(; route[route_index]; route_index++)
    {
      if(route[route_index] == delimiter)
      {
	/* Test for misplaced or multiple delimiters one after another */
	if(route_index == 0 || route[route_index +1] == '\0' || 
	   route[route_index +1] == delimiter)
	  break;
	  
	route[route_index] = '\0';
	route_index++;
	break;
      }
    }

    current_direction = string_to_direction(direction);
    return *direction;
}

Route::~Route()
{
    delete[] route;
}
/* END OF ROUTE DEFINITIONS */


/* INTERNAL HELPER FUNCTION */
/* Function to change position on the map, determined by the 'direction' */
void travel_on_map(int &row_position, int &col_position, Direction direction)
{
    switch(direction)
    {
    case N:
      row_position--;
      break;
    case NE:
      row_position--;
      col_position++;
      break;
    case E:
      col_position++; 
      break;
    case SE:
      row_position++;
      col_position++;
      break;
    case S:
      row_position++; 
      break;
    case SW:
      row_position++;
      col_position--;
      break;
    case W:
      col_position--; 
      break;
    case NW:
      row_position--;
      col_position--;
      break;
    default:
      throw ERROR_INVALID_DIRECTION;
    }
}


/* INTERNAL HELPER FUNCTION */
/* Function to test if the current position is within the dimensions of the
   array */
void is_route_out_of_bounds(const int row_height, const int col_width, 
			    const int current_row, const int current_col)
{
  if(current_row < 0 || current_row >= row_height || 
     current_col < 0 || current_col >= col_width)
    throw ERROR_OUT_OF_BOUNDS;
}


/* INTERNAL HELPER FUNCTION */
/*  Function to test if the current position is on a track  */
void is_position_off_track(const char position_on_map)
{
  if(isspace(position_on_map))
    throw ERROR_OFF_TRACK;
}


/* INTERNAL HELPER FUNCTION */
/* Function checks that a backwards move has not been made on the line. */
void is_move_backtracking(const char previous_position,
			  Direction current_direction, 
			  Direction previous_direction)
{
  if(previous_direction != INVALID_DIRECTION)
  {
    const int number_of_directions = 8;

    /* Set backtracking direction (directions are in a clockwise order) */
    const int backtrack = (current_direction + number_of_directions/2) %
                           number_of_directions;

    if(backtrack == previous_direction && !isalnum(previous_position))
      throw ERROR_BACKTRACKING_BETWEEN_STATIONS;
  }
}


/* INTERNAL HELPER FUNCTION */
/* Function to test for line hopping */
void is_line_change_valid(const char previous_position, 
			  const char current_position)
{
  if(!isalnum(current_position) && !isalnum(previous_position))
    if(previous_position != current_position)
      throw ERROR_LINE_HOPPING_BETWEEN_STATIONS;
}


/* INTERNAL HELPER */
/* TRAIN_LINE DEFINITIONS */

/* Initialise variables */
Train_line::Train_line()
  :number_of_changes{0}, line{0}
{
}

/* Increment 'number_of_changes' if a line change occurred */
void Train_line::operator()(const char current_position)
{
  if(!isalnum(current_position) && (line != current_position))
  { 
    if(line != 0)
      number_of_changes++;

    line = current_position;
  } 
}
/* END OF LINE_CHANGES DEFINITIONS */


/* INTERNAL HELPER FUNCTION */
/* Function to test that the route ends at a station */
void validate_end_station(char *destination, const char end_station_symbol, 
			  const char *file)
{
  ifstream in_stream{file};
  test_stream_state(in_stream, file);

  /* Read a character and check stream state */
  for(char current_station_symbol = 0; in_stream >> current_station_symbol; ) 
  { 
    if(end_station_symbol == current_station_symbol) 
    {
      /* Remove any whitespace */
      in_stream >> ws;
      in_stream.getline(destination, MAX_LINE_LENGTH);

      return;
    }

    /* Ignore the rest of the line each time a match is not found */
    in_stream.ignore(MAX_LINE_LENGTH, '\n'); 
  }

  throw ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
}

/******************************************************************************/
/*                             END OF QUESTION 3                              */
/******************************************************************************/
