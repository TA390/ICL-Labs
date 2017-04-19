#include <iostream>
#include <random>
#include <limits>

#include "local_network.h"
#include "message.h"

/******************************************************************************/

/* Initialise all const attributes and set 'msg_id' to 0 */
Local_network::Local_network(const Type _object_type, const int _address, 
			     Router *const _my_router_ptr)
  :id{_object_type, _address}, my_router_ptr{_my_router_ptr}, msg_id{0}
{
  /* Insert the created object into its own std::map 'connections' */
  insert_connection(this);
}

/******************************************************************************/

const Type Local_network::get_type() const
{
  return id.first;
}

/******************************************************************************/

const int Local_network::get_address() const
{
  return id.second;
}

/******************************************************************************/

const Identity Local_network::get_id() const
{
  return id;
}

/******************************************************************************/

Router *const Local_network::get_router_ptr() const
{
  return my_router_ptr;
}

/******************************************************************************/

const int Local_network::get_msg_id() const
{
  return msg_id;
}

/******************************************************************************/

void Local_network::set_msg_id(const int new_id)
{
  msg_id = new_id;
}

/******************************************************************************/

const Local_network *Local_network::get_network_ptr(const Identity address) const
{

  Connections_iter network_obj = connections.find(address);

  return (network_obj == connections.end()) ? nullptr : network_obj->second;

}

/******************************************************************************/

/* Function to generate a random number that is unique to both connecting
   objects and their network */

int Local_network::create_msg_id(const int id_1, const int id_2) 
{

  int max_value = std::numeric_limits<int>::max();

  while(true)
  {

    std::random_device seed;
    std::default_random_engine engine(seed());

    std::uniform_int_distribution<int> range(0, max_value);
    int rand_num = range(engine);

    /* Test that the random number generated is not equal to the current
       'msg_id' of either object before returning it */
    if(rand_num != id_1 && rand_num != id_2)
    {
      return rand_num;
    }

  }
  
}

/******************************************************************************/

bool Local_network::insert_connection(Local_network *object)
{
  return connections.insert({Identity{object->get_id()}, object}).second;
}

/******************************************************************************/

/* Function to add a connection into the 'connections' container of the two
   connecting objects */

bool Local_network::connect_to_network(Local_network *router, 
				       Local_network *connect_obj)
{

  if(router->insert_connection(connect_obj))
  {
    return connect_obj->insert_connection(router);
  }

  return false;

}

/******************************************************************************/

/* Function to remove a connection from the 'connections' container of the two
   connecting objects */

bool Local_network::disconnect_from_network(Local_network *disconnect_obj)
{

  Connections_iter entry = connections.find(disconnect_obj->get_id());
  
  if(entry == connections.end())
  {
    return false;
  }

  connections.erase(entry);

  disconnect_obj->disconnect_from_network(this);

  return true;

}


/******************************************************************************/

/* Function to return an iterator to the first router found in the 
   'connections' container */

Connections_iter Local_network::router_begin()
{

  Connections_iter router = connections.begin();

  while(router != connections.end() && router->first.first != Type::ROUTER)
  {
    router++;
  }

  return router;

}

/******************************************************************************/

Connections_iter Local_network::end()
{
  return connections.end();
}

/******************************************************************************/

void Local_network::print_received_message(const Message &message) const
{
  std::cout << "Host " << message.get_receiver() << " receieved a message "
	    << "from Host " << message.get_sender() << ": " 
	    << message.get_message() << std::endl <<std::endl;
}

/******************************************************************************/
