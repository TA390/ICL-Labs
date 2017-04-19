#include <iostream>

#include "network_message.h"
#include "local_network.h"

/******************************************************************************/

Network_message::Network_message(const Local_network *_source, int _update_id,
				 Local_network *_informer, bool _update_type,
				 bool share_status)
  :source{_source}, update_id{_update_id}, informer{_informer}
{
  share_with_network = share_status;

  /* Set 'remove_host' to signal whether a host is being disconnected */
  remove_host.first = _update_type;
  remove_host.second = _informer->get_address();

}

/******************************************************************************/

Network_message &Network_message::new_informer(Local_network *new_informer)
{
  informer = new_informer;
  return *this;
}

/******************************************************************************/

bool Network_message::end(const int current_message_id)
{

  if(current_message_id == update_id)
  {
    return true;
  }

  return false;
}

/******************************************************************************/

bool Network_message::action(const Local_network *network_obj)
{
  return (network_obj != source) &&  (network_obj != informer);
}

/******************************************************************************/

const int Network_message::get_update_id() const
{
  return update_id;
}

/******************************************************************************/

const std::pair<bool, int> Network_message::get_remove_host() const
{
  return remove_host;
}

/******************************************************************************/

const bool Network_message::share_with_all() const
{
  return share_with_network;
}

/******************************************************************************/
