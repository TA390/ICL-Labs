#ifndef DATABASE_H
#define DATABASE_H


#include <map>

/******************************************************************************/

class Database;
class Local_network;
class Message;

/******************************************************************************/

/* struct to hold the contents of the route table. The parameter 'next_hop' is
   the router that a message should be passed to for a given host. The parameter
   'hops_away' is how far the destination host is, using a range of [0 : n] */

struct Hop{

  const Database *next_hop;
  int hops_away;

};

/******************************************************************************/

/* Aliases */
typedef std::map<int, Hop>::const_iterator Table_iterator;
typedef std::pair<int, Hop> Table_entry;
typedef std::map<int, Hop> Table;

/******************************************************************************/


/* class Database stores a routers routing table and performs all table
   synchronisations/updates for all connections and disconnections */

class Database
{

 public:

  /* Function to retrieve the Local_network pointer address of a Host. The
   function returns the given pointer if successful or a nullptr if not */
  virtual const Local_network *look_up_host_id(const int host_id) const = 0;

  /* Function to return the integer address of a router */
  virtual const int router_address() const = 0;

  /* Function to forward a 'message' to the next 'router'. The parameter
     'suppress_output' can be toggled to prevent printing the output. The
     function returns the Local_network pointer address of the Host which
     received the message or a nullptr otherwise */
  virtual const Local_network *forward_message(const Message &message,
					       const Database *router,
					       bool suppress_output) const = 0;

  /* Function to compare the route tables of two routers. The caller compares
     its router table against the parameter 'router_cmp'. The function returns
     true if two different Hosts with the same integer address are found in both
     tables. The function returns false otherwise */
  bool route_table_match(const Database *router_cmp);

  /* Function to add a new entry to the routing table. The parameter 
     'new_entry' is the address of the new Host to be added. The function
     returns true if added successfully and false otherwise */
  bool add_table_entry(const int new_entry);

  /* Function to remove an existing entry from the route table. The parameter
   'address' is the address of the host to be removed */
  void remove_table_entry(const int address);

  /* Function to remove all entries from the route table of the parameter
     'router' where the next hop address is the parameter 'hop_address' */
  void remove_router_link(Database *router, const Database *hop_address);

  /* Function to exchange information between two route tables. The parameters
     correspond to the two routers which information will be shared between.
     The function adds new router, updates the 'hops_away' attribute of existing
     routes and removes dead routes. The parameter remove_host signal whether a
     host has been disconnected (true) and provides the host's address (int) */ 
  bool share_table(Database * router_1, Database *router_2);

  /* Function takes the 'address' of a host and returns the next hop address
     required to reach that host */
  const Database *forward_address(const int address) const;

  

  
  /* 'route_table' is a map containing the integer address of every reachable
     host (the key). The value is a struct 'Hop' with two attributes, the
     'next_hop' and the 'hops_away' */
  std::map<int, Hop> route_table;
 private:
  /* Internal helper function to add a new entry to a routing table where a
     shorter path has been found. The parameter 'entry' is the shorter path
     found and 'router' is the router that knows of this shorter path.
     The function increments the hops_away attribute and assigns 'router' as
     the next hop. The function returns a copy of the newly create entry. */
  Table_entry new_entry(Table_entry entry, const Database *router);

  /* Internal helper function to swap the two 'router_n' pointer parameters and
     the two 'iter_n' pointer parameters. */
  void swap_pointers(Database *&router_1, Database *&router_2,
		     Table_iterator *&iter_1, Table_iterator *&iter_2);

  /* INTERNAL HELPER FUNCTIONS BELOW- for each function below, the 'router_n'
     parameters are the two routers comparing tables. The 'iter_n' parameters
     are the current positions within those tables */

  /* Internal helper function to add an entry found in the route table of
     'router_1' to the route table of 'router_2'. However, if the next hop to
     reach this Host is 'router_2', remove the invalid entry from the route
     table of 'router_1' */
  bool update_new_entries(Database *&router_1, Database *&router_2,
			  Table_iterator *&iter_1, Table_iterator *&iter_2);

  /* Internal helper function to identify entries to the same host in the
     routing tables of the two 'router_n' parameters. The function updates a
     route table if one router has a shorter path to the same destination. The
     function returns true and increments both iterators if a route to the same
     Host was found. The function returns false and leaves the iterators
     unchanged otherwise */
  bool duplicates(Database *router_1, Database *router_2,
		  Table_iterator &iter_1, Table_iterator &iter_2);

  /* Internal helper function to sort the ordering of the parameters passed in
     so that 'router_1' and 'iter_1' correspond to the Host with the smallest
     address. This function calls swap_pointers() and update_new_entries() */
  bool unique_host(Database *router_1, Database *router_2,
		     Table_iterator *iter_1, Table_iterator *iter_2);

  /* Internal helper function to fill the route table of 'router_2', with all
     the entries it does not contain from the route table of 'router_1'. This
     function calls swap_pointers() and update_new_entries() */
  bool fill_table(Database *router_1, Database *router_2,
		  Table_iterator *iter_1, Table_iterator *iter_2);

};


/******************************************************************************/


#endif
