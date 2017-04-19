#include <iostream>

#include "message.h"

/******************************************************************************/

Message::Message(const int source, const int destination, const char *msg)
  :source_address{source}, destination_address{destination}, message{msg}
{
}

/******************************************************************************/

const int Message::get_sender() const
{
  return source_address;
}

/******************************************************************************/

const int Message::get_receiver() const
{
  return destination_address;
}

/******************************************************************************/

const char *Message::get_message() const
{
  return message;
}

/******************************************************************************/

