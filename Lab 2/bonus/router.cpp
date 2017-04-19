#include <iostream>

#include "errors.h"
#include "message.h"
#include "network_message.h"
#include "router.h"

using namespace std;

/******************************************************************************/

/* Initialise the base class passing the Type, host address and Router pointer
   as 'this' */

Router::Router(const int _address)
  :Local_network{Type::ROUTER, _address, this}
{

}

/******************************************************************************/

/* Connect a Local_network object to a router */

bool Router::connectTo(Local_network &object)
{

  bool connected = false;

  switch(object.get_type())
  {
    case Type::HOST:

      connected = connect_host(this, &object);

      break;

    case Type::ROUTER:
      
      connected = connect_router(this, &object);

      break;
  }

  /* If a connection was made update the network */
  if(connected == true)
  {
    flood_message(this, &object);

    return true;
  }
  
  return Error{}.connect_failure(*this, object);

}

/******************************************************************************/

/* Disconnect a previously connected Local_network object from a router */

bool Router::disconnect(Local_network &object)
{

  bool disconnected = false;

  /* Disconnect from the Local network */
  if((disconnected = disconnect_from_network(&object)))
  {  
     switch(object.get_type())
     {

       case Type::HOST:

	 /* Remove Database entry */
	 remove_table_entry(object.get_address());

	 /* Update the entire network */
	 flood_message(this, &object, true);

	 return true;

       case Type::ROUTER:

	 /* Remove invalid routes from the route table and update all immediate
	    connections of both routers */
	 disconnect_router(this, &object);

	 /* Update each router's entire network */
	 flood_message(this, &object);
	 flood_message(object.get_router_ptr(), this);

	 return true;

     }
  }
 
  return Error{}.disconnect_failure(*this, object);

 }

/******************************************************************************/

bool Router::disconnectFrom(Router &router)
{
  return disconnect(router);
}

/******************************************************************************/
/*                          INTERNAL HELPER FUNCTIONS                         */
/******************************************************************************/

bool Router::connect_host(Router *router, Local_network *host)
{
  /* Test that a host does not have any connections before connecting to it */
  if(host->router_begin() == host->end() && connect_to_network(router, host))
  {
    /* Add the newly connected host to the router's route table */
    if(router->add_table_entry(host->get_address()))
    {
      return true;
    }
  }

  return false;

}

/******************************************************************************/

bool Router::connect_router(Router *const router_1,
			    Local_network *const router_2)
{
  /* Test for hosts with the same address on the network of each connecting
     router. Test for routers with the same address immediately connected to
     each router */
  if(route_table_match(router_2->get_router_ptr()) == false &&
     unique_router_addresses(router_1, router_2) == true)
  {
    if(connect_to_network(router_1, router_2))
    {
      share_table(router_1, router_2->get_router_ptr());

      return true;
    }
  }
  
  return false;

}

/******************************************************************************/

void Router::disconnect_router(Router *router_1, Local_network *router_2)
{
  /* Remove routes involving the router to be disconnected */
  remove_router_link(router_1, router_2->get_router_ptr());
  remove_router_link(router_2->get_router_ptr(), router_1);

  /* Update immediate neighbours only */
  flood_message(router_1, router_2, false, false);
  flood_message(router_2->get_router_ptr(), router_1, false, false);
}

/******************************************************************************/

void Router::disconnect_host(Router *router_1, Router *router_2, int address)
{
  router_1->remove_table_entry(address);
  router_2->remove_table_entry(address);
}

/******************************************************************************/

void Router::flood_message(Router *router,
			   Local_network *object,
			   bool remove_host,
			   bool network_update)
{

  /* Return a unique number for a new network update */
  int new_id = create_msg_id(router->get_msg_id(), object->get_msg_id());

  /* Create a network message */
  Network_message update = Network_message{router, new_id, object,
					   remove_host, network_update};

  /* Update the network of all changes */
  update_network(router, update, router->router_begin());

}

/******************************************************************************/

/* Recursively update each neighbour and then their neighbours */

void Router::update_network(Router *router, 
			    Network_message &update, 
			    Connections_iter iter)
{   
 
  /* Base case - Test if a router has reached the end of it connections. Test
     if it has previously shared this message and no action it required */
  if(iter == router->end() || update.end(router->get_msg_id()) == true)
  {
    router->set_msg_id(update.get_update_id());
    return;
  }

  /* The current neighbour in which to share information with */
  Router *neighbour = my_router_ptr(iter);
  Connections_iter neighbour_iter = neighbour->router_begin();  
  
  /* Test that this neighbour requires an update and prevent a self update */
  if(update.action(my_network_ptr(iter)) && my_network_ptr(iter) != router)
  {
    /* If a host is being disconnected ask each router to remove its entry */
    if(update.get_remove_host().first == true)
    {
      disconnect_host(router, my_router_ptr(iter),
		      update.get_remove_host().second);
    }
    else
    {
      if(share_table(router, my_router_ptr(iter)) == true)
      {
	update.set_update_id(create_msg_id(0,0));
      }
	
    }
  }
  
  /* Recursively share with all neighbours */
  update_network(router, update, ++iter);
  
  /* Allow each neighbour to recursively share with their own neighbours */
  if(update.share_with_all() == true)
  {
    update_network(neighbour, update.new_informer(this), neighbour_iter++);    
  }

}

/******************************************************************************/

bool Router::send_to(const Message &message, Local_network *network_obj) const
{
  if(network_obj == nullptr)
  {
    return Error{}.message_failure(message.get_sender(), 
					message.get_receiver());
  }

  return forward_message(message, network_obj->get_router_ptr());

}

/******************************************************************************/

const Local_network *Router::forward_message(const Message &message, 
					     const Database *router,
					     bool suppress_output) const
{

  /* Retrieve the next hop address from the routing table for a given host */
  const Database *forward_to = router->forward_address(message.get_receiver());

  if(router == nullptr || forward_to == nullptr)
  {
    Error{}.message_failure(router->router_address());

    return nullptr;
  }
  
  /* Test if you are the destination router and deliver message to host */
  if(forward_to == router)
  {
    return forward_to_host(message, router, suppress_output);
  }

  if(suppress_output == false)
  {
    print_forwarding_message(router, forward_to);
  }

  /* Recursively search for the destination host's router */
  return forward_message(message, forward_to, suppress_output);

}

/******************************************************************************/

const Local_network *Router::forward_to_host(const Message &message, 
			       		     const Database *router,
		       			     bool suppress_output) const
{
  const Local_network *host;

  host = router->look_up_host_id(message.get_receiver());

  if(suppress_output == false)
  {
    host->print_received_message(message);
  }
  
  return host;
}

/******************************************************************************/

void Router::print_forwarding_message(const Database *router_1,
			      const Database *router_2) const
{
  cout << "Router " << router_1->router_address() << " forwarded a message to "
       << "router " << router_2->router_address() << endl << endl;
}

/******************************************************************************/

const Local_network *Router::look_up_host_id(const int host_id) const
{

  return get_network_ptr({Type::HOST, host_id});

}

/******************************************************************************/

const int Router::router_address() const
{
  return this->get_address();
}

/******************************************************************************/

Router *Router::my_router_ptr(Connections_iter &iter) const
{
  return iter->second->get_router_ptr();
}

/******************************************************************************/

Local_network *Router::my_network_ptr(Connections_iter &iter) const
{
  return iter->second;
}

/******************************************************************************/

const int Router::my_address(Connections_iter &iter) const
{
  return iter->second ->get_address();
}

/******************************************************************************/

bool Router::unique_router_addresses(Router *router_1,
				     Local_network *router_2) const
{
  Connections_iter iter_1 = router_1->router_begin();
  Connections_iter iter_2 = router_2->router_begin();

  while(iter_1 != router_1->end() && 
	iter_2 != router_2->end())
  {
    /* Test for routers with the same address */
    if(my_address(iter_1) == my_address(iter_2))
    { 
      /* If a match is found but they are not the same router, return false */ 
      if(my_router_ptr(iter_1) != my_router_ptr(iter_2))
      {
	return false;
      }
    }

    (my_address(iter_1) < my_address(iter_2)) ? iter_1++ : iter_2++;
  } 

  return true;
}

/******************************************************************************/




