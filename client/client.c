#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// provides access to an address struct
// that is used to define where you want to connect to
#include <netinet/in.h>

int main(int argc, char *argv[]) {
  // create a network socket
  int network_socket;
  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  // declare address for socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  // INADDR_ANY = 0.0.0.0
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(19001);
  int connection_status =
      connect(network_socket, (struct sockaddr *)&server_address,
              sizeof(server_address));
  if (connection_status == -1) {
    printf("Something went wrong!\n");
  }
  /*
  Let's try to send something to the server here.
  */
  if (send(network_socket, argv[1], sizeof(argv[1]), 0) == -1) {
    printf("ERROR\n");
  }
  // receive data back from server
  char server_response[256];
  recv(network_socket, &server_response, sizeof(server_response), 0);
  printf("%s\n", server_response);
  close(network_socket);
  return 0;
}