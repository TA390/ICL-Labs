#ifndef MESSAGE_H
#define MESSAGE_H


/******************************************************************************/


/* class Message is used to send messages between hosts */

class Message{

public:

  /* Initialise all attributes, when creating a Message */
  Message(const int source, const int destination, const char *msg);

  /* Return the 'source_address' attribute */
  const int get_sender() const;

  /* Return the 'destination_address' attribute */
  const int get_receiver() const;

  /* Return the 'message' attribute */
  const char *get_message() const;

private:

  /* 'source_address' is the address of the host sending the message */
  const int source_address;

  /* 'destination_address' is the address of the host receiving the message */
  const int destination_address;

  /* 'message' is the message being sent */
  const char *message;

};


/******************************************************************************/


#endif
