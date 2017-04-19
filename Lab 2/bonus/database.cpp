#include <iostream>

#include "database.h"
#include "message.h"

/******************************************************************************/

/* Function to compare route tables looking for hosts with the same address. */

bool Database::route_table_match(const Database *router_cmp)
{
  /* Test for an attempt to connect a router to itself */
  if(this == router_cmp)
    return true;
  
  /* iterators for each route table */
  Table_iterator iter_1 = this->route_table.begin();
  Table_iterator iter_1_end = this->route_table.end();

  Table_iterator iter_2 = router_cmp->route_table.begin();
  Table_iterator iter_2_end = router_cmp->route_table.end();
  
  /* Using the sorted ordering of maps, compare each element and increment
     the iterator pointing to the smallest address value */
  while(iter_1 != iter_1_end && iter_2 != iter_2_end)
  {

    if(iter_1->first == iter_2->first)
    {
      /* Perform a look up for each hosts pointer address by sending a 'dummy'
	 message through the network and suppressing the output */
  
      Message dummy{0, iter_1->first, ""};

      const Local_network *host_1;
      host_1 = forward_message(dummy, this, true);
      
      const Local_network *host_2;
      host_2 = router_cmp->forward_message(dummy, router_cmp, true);

      /* Return false to signal that no invalid matches were found */
      return (host_1 == host_2) ? false : true;
    }

    (iter_1->first < iter_2->first) ? iter_1++ : iter_2++;
  }

  return false;
}

/******************************************************************************/

bool Database::add_table_entry(const int new_entry)
{
  return (route_table.insert({new_entry, Hop{this, 0}})).second;
}

/******************************************************************************/

/* Remove the host 'address' from the route table */

void Database::remove_table_entry(const int address)
{

  Table_iterator entry = route_table.begin();
  
  while(entry != route_table.end())
  {
    if(entry->first == address)
    {
      entry = route_table.erase(entry);
    }
    else
    {
      ++entry;
    }
  }

}

/******************************************************************************/

/* Remove all routes from 'router' if the next hop is 'hop_address' */

void Database::remove_router_link(Database *router, const Database *hop_address)
{
  Table_iterator entry = router->route_table.begin();

  while (entry != router->route_table.end())
  {
    if(entry->second.next_hop == hop_address)
    {
      entry = router->route_table.erase(entry);
    }
    else
    {
      ++entry;
    }
  }

}

/******************************************************************************/

/* Create a new table entry for a given table from an existing route */

Table_entry Database::new_entry(Table_entry entry, const Database *router)
{

  entry.second.next_hop = router;

  (entry.second.hops_away)++;

  return entry;

}

/******************************************************************************/

/* Update routes found to the same host */

bool Database::duplicates(Database *router_1, Database *router_2,
			  Table_iterator &iter_1, Table_iterator &iter_2)
{
  
  Table &table_1 = router_1->route_table;
  Table &table_2 = router_2->route_table;
  
  if(iter_1->first == iter_2->first)
  { 
    /* A route is faster if it is two or more hops away */
    const int difference = 2;

    int value = iter_1->second.hops_away - iter_2->second.hops_away;

    /* Test for a faster route or an updated route. Update the hops away
       attribute when a faster route is found */
    if(value <= -difference || iter_2->second.next_hop == router_1)
    {
      table_2[iter_1->first] = Hop{router_1, (iter_1->second.hops_away)+1};
    }
    else if(value >= difference || iter_1->second.next_hop == router_2)
    {
      table_1[iter_2->first] = Hop{router_2, (iter_2->second.hops_away)+1};
    }

    iter_1++;
    iter_2++;

    return true;
	
  }

  return false;

}

/******************************************************************************/

void Database::swap_pointers(Database *&router_1, Database *&router_2,
			     Table_iterator *&iter_1, Table_iterator *&iter_2)
{
    std::swap(router_1, router_2);
    std::swap(iter_1, iter_2); 
}

/******************************************************************************/

bool Database::update_new_entries(Database *&router_1, Database *&router_2,
				  Table_iterator *&iter_1, Table_iterator *&iter_2)
{
  /* Add the entry found in 'router_1' to 'router_2' if the next hop address
     recorded by 'router_1' is not 'router_2' */
  if((*iter_1)->second.next_hop != router_2)
  {
    *iter_2 = (router_2->route_table).insert(new_entry(**iter_1, router_1)).first;

    ++(*iter_1);
    ++(*iter_2);

    return true;
  }
  /* Otherwise remove that entry */
  else
  {
    *iter_1 = (router_1->route_table).erase(*iter_1);
  }

  return false;
}

/******************************************************************************/

/* Add entries found in one route table that the other does not contain */

bool Database::unique_host(Database *router_1, Database *router_2,
		           Table_iterator *iter_1, Table_iterator *iter_2)
{
  /* Swap pointers so 'router_1' and 'iter_1' point to the host with the
     smallest integer address */
  if((*iter_1)->first > (*iter_2)->first)
  {
    swap_pointers(router_1, router_2, iter_1, iter_2);
  }

  return update_new_entries(router_1, router_2, iter_1, iter_2);
  
}

/******************************************************************************/

/* Fill one route table with the entries it does not contain from the other */

bool Database::fill_table(Database *router_1, Database *router_2,
		          Table_iterator *iter_1, Table_iterator *iter_2)
{

  /* Swap pointers so 'router_1' and 'iter_1' correspond to the table which has
     not reached its end */
  if((*iter_1) == router_1->route_table.end())
  {
    swap_pointers(router_1, router_2, iter_1, iter_2);
  }

  for(; *iter_1 != router_1->route_table.end();)
  {  
    return update_new_entries(router_1, router_2, iter_1, iter_2);
  }

  return false;
}
		 
/******************************************************************************/

/* Share information between the route tables of 'router_1' and 'router_2' */

bool Database::share_table(Database *router_1, Database *router_2)
{
  if(router_1 == router_2)
  {
    return false; 
  }


  Table_iterator iter_1 = router_1->route_table.begin();
  Table_iterator iter_2 = router_2->route_table.begin();

  Table &table_1 = router_1->route_table;
  Table &table_2 = router_2->route_table;


  while(iter_1 != table_1.end() && iter_2 != table_2.end())
  {
    if(duplicates(router_1, router_2, iter_1, iter_2) == false)
    {
      unique_host(router_1, router_2, &iter_1, &iter_2);
    }   
  }
 
  return fill_table(router_1, router_2, &iter_1, &iter_2);

}

/******************************************************************************/

/* Find and return the Database pointer of the next hop address for a given
   host ('address') */

const Database *Database::forward_address(const int address) const
{
  Table_iterator entry = route_table.find(address);

  if(entry == route_table.end())
  {
    return nullptr;
  }

  return entry->second.next_hop;

}

/******************************************************************************/
