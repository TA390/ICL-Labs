#ifndef LOCAL_NETWORK_H
#define LOCAL_NETWORK_H


#include <map>

/******************************************************************************/

/* A list of all network objects to be identified by their 'Type' */
enum class Type{ HOST, ROUTER };

/******************************************************************************/

class Message;
class Router;
class Local_network;

/******************************************************************************/

/* Aliases */

/* 'Identity' stores the Type of the object created and its integer address */
typedef std::pair<Type, int> Identity;
typedef std::map<Identity, Local_network *>::const_iterator Connections_iter;

/******************************************************************************/


class Local_network
{

 public:

  /* User defined default constructor to set the type of the network object
     (Host or Router), its int address and the pointer address of any routers */
  Local_network(const Type _object_type, const int _address,
		Router *const _my_router_ptr);

  /* Function to connect a host or router ('object') to a router */
  virtual bool connectTo(Local_network &object) = 0;

  /* Function to disconnect a host or router ('object') from a router */
  virtual bool disconnect(Local_network &object) = 0;

  /* Function to send messages ('message') between hosts and routers
   ('network_obj') directly connected to one another */
  virtual bool send_to(const Message &message,
		       Local_network *network_obj) const = 0;
  
  /* Function to print a 'message' received through the network */
  void print_received_message(const Message &message) const;

  /* Function to add the parameter ('connect_obj') to the caller's std::map
     'connections'. The parameter then adds the caller to its own std::map
     'connections'. The function returns true for a successful connection
     and false otherwise */
  bool connect_to_network(Local_network *router,
			  Local_network *connect_obj);

  /* Function to remove the parameter ('disconnect_obj') from the caller's
     std::map 'connections'. The parameter then removes the caller from its
     own std::map 'connections'. The function returns true for a successful
     disconnect and false otherwise */
  bool disconnect_from_network(Local_network *disconnect_obj);

  /* Functions to retrieve information from private attributes */
  const int get_address() const;
  const Type get_type() const;
  Router *const get_router_ptr() const;
  const int get_msg_id() const;

  /* Function to generate a new message id for a network update. The parameters
     are the current 'msg_id' attributes of the two connecting objects. The
     return value is a random number unique to both connecting objects */
  int create_msg_id(const int id_1, const int id_2);

  /* Function to set the 'msg_id' attribute used for flooding the network
     with an new update. The parameter 'new_id' is the message id of the latest
     update received */
  void set_msg_id(const int new_id);  

  /* Function to return an iterator to the first router held within the
     'connections' container (map) */
  Connections_iter router_begin();

  /* Function to return the end iterator of the connections map */
  Connections_iter end();

  /* Function to return the Local_network pointer of a network object stored
     in the map 'connections'. The parameter 'obj_address' is the Type, integer
     pair of the network object using the alias 'Identity'. The function returns
     a Local_network pointer to the object if found and a nullptr if not */
  const Local_network *get_network_ptr(const Identity obj_address) const;


 private:

  /* map to store the Identity (Type / integer pair) and pointer address of all 
     Local_network connections (Hosts and Routers) */
  std::map<Identity, Local_network *> connections;

  /* 'id' is a std::pair<Type, int> using the alias 'Identity'. This corresponds
     to the object Type (Host or Router) and integer address of that object */
  const Identity id;

  /* The pointer address of a router */
  Router *const my_router_ptr;

  /* 'msg_id' is the number corresponding to the last update message received
     by a Local_network object during a network update */
  int msg_id;

  /* Internal helper function to add a connection to the map 'connections'. The 
     parameter 'object' is a pointer to the object being connected. The function
     returns true if 'object' was connected successfully and false otherwise */
  bool insert_connection(Local_network *object);

  /* Internal helper function to return the attribute 'id' which is the
     Type / integer address pair of a Local_network object */
  const Identity get_id() const;

};


/******************************************************************************/


#endif

