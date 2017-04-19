#ifndef HOST_H
#define HOST_H

/******************************************************************************/

#include "local_network.h"

/******************************************************************************/


/* Host class represents a computer on the network */
class Host : public Local_network
{
 public:

  /* Initialises the base class Local_network with the '_address' passed as an
     argument */
  explicit Host(const int _address);

  /* Function to connect a Host to a Router. The function takes a Local_network
     parameter 'object' and connects the Host if this 'object' is a Router. The
     function returns true if successful and false otherwise */
  bool connectTo(Local_network &object);

  /* Function to disconnect a Host from its Router. The parameter 'object'
     corresponds to the object in which the Host is to be disconnected from. The
     function returns true if disconnected successfully and false otherwise */
  bool disconnect(Local_network &object);

  /* Function to create a Message from the parameters 'host' for the host
     destination address and 'message' for the message to be delivered */
  bool send(const int host, const char *message);


 private:

  /* Overridden function used to send a message to connected objects (Router).
     The parameter 'message' is a reference to the Message and 'network_obj'
     is the connected Router which will forward on the message. The function
     returns true if a message was delivered successfully and false otherwise */
  bool send_to(const Message &message, Local_network *network_obj) const;

  /* Internal helper function to print the contents of a message being sent */
  void print_sent_message(const Message &message);

  /* Prevent copying */
  Host(Host &);
  Host &operator=(Host &);

};


/******************************************************************************/


#endif
