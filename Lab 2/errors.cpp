#include <iostream>
#include "errors.h"
#include "local_network.h"

using namespace std;

/******************************************************************************/

bool Error::connect_failure(const Local_network &object_1,
			    const Local_network &object_2) const
{

  cerr <<"ERROR: failed to connect "<< object_1 <<"' & "<< object_2 <<"'"
       << endl << endl;

  return false;

}

/******************************************************************************/

bool Error::disconnect_failure(const Local_network &object_1,
			       const Local_network &object_2) const
{

  cerr <<"ERROR: failed to disconnect "<< object_1 <<"' & "<< object_2
       << endl << endl;
         
  return false;

}

/******************************************************************************/

bool Error::message_failure(const int router) const
{

  cerr <<"Routing of message failed at Router " << router << endl << endl;
         
  return false;

}

/******************************************************************************/

bool Error::message_failure(const int host_1, const int host_2) const
{

  cerr <<"ERROR: failed to send a message from Host " << host_1 << " to Host "
       << host_2 << endl << endl;
         
  return false;

}

/******************************************************************************/
/*                          INTERNAL HELPER FUNCTIONS                         */
/******************************************************************************/

const char *type_to_str(const Type object_type)
{

  switch(object_type)
  {
    case Type::HOST:
      return "host";

    case Type::ROUTER:
      return "router";

    default:
      return "unknown type";
  }

}

/******************************************************************************/

std::ostream& operator<<(std::ostream &out_stream, const Local_network &object)
{
  out_stream << type_to_str(object.get_type()) <<" '"<< object.get_address();
  return out_stream;
}

/******************************************************************************/
