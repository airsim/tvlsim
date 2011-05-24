// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <iostream>
#include <unistd.h>
// ZeroMQ
#include <zmq.hpp>

// //////////////// M A I N ////////////////////////
int main (int argc, char* argv[]) {

  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  socket.bind ("tcp://*:5555");

  while (true) {
    zmq::message_t request;

    // Wait for next request from client
    socket.recv (&request);
    std::cout << "Received Hello" << std::endl;

    // Do some 'work'
    sleep (1);

    // Send reply back to client
    zmq::message_t reply (5);
    memcpy ((void *) reply.data (), "World", 5);
    socket.send (reply);
  }

  return 0;
}

