#include <iostream>

#include "errors.h"
#include "host.h"
#include "message.h"

using namespace std;

/******************************************************************************/

/* Initialise the base class passing the Type, host address and Router pointer
   as a nullptr */
Host::Host(const int _address)
  :Local_network{Type::HOST, _address, nullptr}
{

}

/******************************************************************************/

/* Test for an attempt to connect to a Host, otherwise let the Router passed as
   an argument ('object') to connect the Host */

bool Host::connectTo(Local_network& object)
{

  if(object.get_type() == Type::HOST)
  {
     return Error{}.connect_failure(*this, object);
  }  
  else
  {
    return object.connectTo(*this);
  }

}

/******************************************************************************/

/* Test for an attempt to disconnect from a Host, otherwise let the Router
   passed as an argument ('object') disconnect the Host */

bool Host::disconnect(Local_network& object)
{

  if(object.get_type() == Type::HOST)
  {
     return Error{}.disconnect_failure(*this, object);
  }
  else
  {
    return object.disconnect(*this);
  }

}

/******************************************************************************/

/* Send a 'message' to a connected Router ('network_obj') */

bool Host::send_to(const Message &message, Local_network *network_obj) const
{
  /* Test for a valid pointer and an attempt to send directly to a Host */
  if(network_obj != nullptr && network_obj->get_type() != Type::HOST)
  {
    return network_obj->send_to(message, network_obj);
  }

  return Error{}.message_failure(get_address(), message.get_receiver());
}

/******************************************************************************/

/* Create a message object and pass it to the connected router */

bool Host::send(const int host, const char *message)
{
  Connections_iter my_router = router_begin();

  if(my_router != this->end())
  {
    Message my_message{get_address(), host, message};

    print_sent_message(my_message);

    return send_to(my_message, my_router->second);
  }

  return Error{}.message_failure(get_address(), host);
}

/******************************************************************************/

/* Print message to be sent */

void Host::print_sent_message(const Message &message)
{
  std::cout << "Host " << message.get_sender() << " sent a message to Host "
	    << message.get_receiver() << ": " 
	    << message.get_message() << std::endl <<std::endl;
}

/******************************************************************************/
