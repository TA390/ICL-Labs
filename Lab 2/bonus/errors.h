#ifndef ERRORS_H
#define ERRORS_H


/******************************************************************************/

class Local_network;
class ostream;

/******************************************************************************/

enum class Type;

/******************************************************************************/


/* struct Error is used to output error messages */

struct Error
{

  /* RETURN VALUE - all error functions return false */

  /* Function to output a message when a connection fails */
  bool connect_failure(const Local_network &object_1,
		       const Local_network &object_2) const;

  /* Function to output a message when a disconnection fails */
  bool disconnect_failure(const Local_network &object_1,
			  const Local_network &object_2) const;

  /* Function to output a message when an attempt to send a message fails */
  bool message_failure(const int host_1, const int host_2) const;

  /* Function to output a message when the routing of a message fails  */
  bool message_failure(const int router) const;
  
};


/******************************************************************************/
/*                          INTERNAL HELPER FUNCTIONS                         */
/******************************************************************************/

const char *type_to_str(const Type object_type);

/******************************************************************************/

std::ostream& operator<<(std::ostream &out_stream, const Local_network &object);

/******************************************************************************/


#endif
