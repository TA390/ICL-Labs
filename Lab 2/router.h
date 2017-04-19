#ifndef ROUTER_H
#define ROUTER_H


/******************************************************************************/

#include "database.h"
#include "local_network.h"

/******************************************************************************/

class Network_message;

/******************************************************************************/

class Router : public Local_network, public Database
{

 public:

  /* Initialise the base class Local_network with the address passed as an
     argument */
  explicit Router(const int _address);

  /* Overridden (Local_network) function to connect a Router to any
     Local_network object. The parameter 'object' corresponds to the Host or
     Router to be connected. The function returns true if successful and false
     otherwise */
  bool connectTo(Local_network &object);

  /* Overridden (Local_network) function to disconnect a Router from any
     Local_network object in which it is connected to. The parameter 'object'
     corresponds to the Host or Router to be disconnected. The function returns
     true if successful and false otherwise */
  bool disconnect(Local_network &object);

  /* Function to disconnect a Router from another Router. The parameter 'router'
     corresponds to the Router to be disconnected. The function returns true if
     successful and false otherwise */
  bool disconnectFrom(Router &router);


 private:

  /* Internal helper function to connect a router ('router') to a host
     ('host'). The function returns true is successfully connected and false
     otherwise */
  bool connect_host(Router *router, Local_network *host);

  /* Internal helper function to disconnect a router ('router_1') from another
     router ('router_2'). The function returns true is successfully disconnected
     and false otherwise */
  bool connect_router(Router *router_1, Local_network *router_2);

  /* Internal helper function to remove routes where the next hop address is
     that of the router being disconnected and to update their neighbours. The
     parameters 'router_n' are the two routers being disconnected */
  void disconnect_router(Router *router_1, Local_network *router_2);


  /* Internal helper function to remove a host 'address' from the route
     tables of both router parameters ('router_1' and 'router_2') */
  void disconnect_host(Router *router_1, Router *router_2, int address);

  /* Internal helper function to create a Network_message to be used to flood
     the network with an update. The parameter 'router' is the router to update
     the network. The parameter 'object' corresponds to the Host or Router in
     which a change has occurred between. The parameter 'remove_host' is a flag
     to signal that a host is being disconnected. The parameter 'network_update'
     is set true by default indicating that an update is for the entire network.
     If false an update is shared with a router's immediate neighbours only */
  void flood_message(Router *router,
		     Local_network *object,
		     bool remove_host = false,
		     bool network_update = true);

  /* Internal helper function to update the network of a change to the topology.
     The parameter 'router' is the router delivering the update. The parameter
     'update' is a message containing the information of the update. The 'iter'
     parameter is an iterator to the router's neighbours */
  void update_network(Router *router,
		      Network_message &update,
		      Connections_iter iter);

  /* Overridden (Local_network) function to send a messages to a connection.
     The parameter 'network_obj' corresponds to the router in which to send the
     message to. The function returns true if a message reaches its destination
     successfully or false otherwise */
  bool send_to(const Message &message, Local_network *network_obj) const;

  /* Overridden (Database) function to forward a message through the network
     to its receiver (a Host). The parameter 'message' contains the message and
     'router' is the next hop address for the message. The parameter
     'suppress_output' is set to false by default. Setting this to true will
     prevent the message being printed (used for look ups). The function returns
     the Local_network pointer address of the host which received the message or
     nullptr otherwise */
  const Local_network *forward_message(const Message &message,
				       const Database *router,
				       bool suppress_output = false) const; 

  /* Internal helper function to forward the 'message' to the destination host.
     The parameter 'router' is the router to which the destination host is
     connected. The parameter 'suppress_output' specifies whether the message
     should be output to the screen */
  const Local_network *forward_to_host(const Message &message, 
					       const Database *router,
					       bool suppress_output) const;
  
  /* Overridden (Database) function to allow a Database object to look up the
     Local_network pointer address of a host. The parameter 'host_id' is the
     integer address of the host. The function returns its Local_network
     pointer address if found or nullptr otherwise */
  const Local_network *look_up_host_id(const int host_id) const;

  /* Overridden (Database) function to return a router's integer address */
  const int router_address() const;

  /* Internal helper function to return the pointer address of a router when
     given an  iterator 'iter' as an argument */
  Router *my_router_ptr(Connections_iter &iter) const;

  /* Internal helper function to return the Local_network pointer address of a
     router when given an iterator 'iter' as an argument*/
  Local_network *my_network_ptr(Connections_iter &iter) const;

  /* Internal helper function to return the integer address of a Local_network
     object when given an iterator 'iter' as an argument */
  const int my_address(Connections_iter &iter) const;

  /* Internal helper function to prevent routers connecting if any of their
     immediately connected routers have the same integer address. The
     parameters 'router_n' are the two routers attempting to connect */
  bool unique_router_addresses(Router *router_1, Local_network *router_2) const;

  /* prevent copying */
  Router(Router &);
  Router &operator=(Router &);
  
};

/******************************************************************************/


#endif
