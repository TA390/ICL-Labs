#ifndef NETWORK_MESSAGE
#define NETWORK_MESSAGE


/******************************************************************************/

class Local_network;

/******************************************************************************/

/* class Network_message is used to inform the network of a change to the
   topology */

class Network_message{

public:

  /* Initialise all attributes */
  Network_message(const Local_network *_source, int _update_id,
		  Local_network *_informer, bool _update_type,
		  bool share_status);

  /* Function to update the 'informer' attribute and set it equal to the
     parameter 'new_informer' */
  Network_message &new_informer(Local_network *new_informer);

  /* Function to test if the current message has been delivered by a given
     Router. The parameter 'current_message_id' is the 'msg_id' of the router.
     The function returns true if the parameter passed matches the 'update_id'
     attribute, to signal the end of an update. Otherwise it returns false */
  bool end(const int current_message_id);

  /* Function to test whether an update needs to be shared with the current
     'network_obj'. The function returns true if it is necessary to share
     information and false otherwise */
  bool action(const Local_network *network_obj);

  /* Return 'update_id' attribute */
  const int get_update_id() const;

  /* Return 'remove_host' attribute */
  const std::pair<bool, int> get_remove_host() const;

  /* Return 'share_with_network' attribute */
  const bool share_with_all() const;


private:

  /* 'source' is the router starting the update */
  const Local_network *const source;

  /* 'update_id' is the unique number generated for a given message */
  int update_id;

  /* 'informer' is initialised to be the object in which the source has made a 
     change with. It's then updated to reflect the object which told you */
  Local_network *informer;

  /* 'share_with_network' is a flag to signal whether an update should be shared
     with the entire network (true) or with immediate neighbours only (false) */
  bool share_with_network;

  /* 'remove_host' is a flag to signal that a host is being disconnected. The 
   boolean parameter is true if a host is being disconnected and the int 
   parameter is the address of the host to be removed */
  std::pair<bool, int> remove_host;

};

/******************************************************************************/


#endif
