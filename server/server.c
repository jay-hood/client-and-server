#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// provides access to an address struct
// that is used to define where you want to connect to
#include <netinet/in.h>

int main() {

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(19001);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bindval = bind(server_socket, (struct sockaddr *)&server_address,
                     sizeof(server_address));

  // connection and backlog (total number of connections waiting on the socket
  // at any point in time)
  // I think this blocks until a connection is encountered, which is its primary
  // purpose
  char *last_str;
  int total_size;
  while (1) {

    listen(server_socket, 1);

    int client_socket = accept(server_socket, NULL, NULL);
    char buffer[4096];
    int recv_size = recv(client_socket, &buffer, sizeof(buffer), 0);
    if (recv_size != -1) {
      /*
      Increment total size by the amount of the file received
      if total size exceeds the buffer (or we got everything), allocate memory
      to an array and concatenate the buffer to that array. Clear the buffer, go
      to next iteration.

      if recv_size is -1, we are done receiving data (maybe include a timeout
      here) and we can be confident we're received all the data (maybe include a
      header and tail to the file so we KNOW when we're done - look up packet
      design) From here, if there's anything in the buffer or recv_size is > 0,
      then we can append the rest of that to the array and do something else
      with our completed data object.

      */
      char server_message[256] = "Hello from server! You sent me the word: ";

      printf("Value of n: %i\n", recv_size);
      printf("%s\n", buffer);
      strncat(server_message, buffer, sizeof(buffer));
      send(client_socket, server_message, sizeof(server_message), 0);
      free(last_str);
      last_str = (char *)malloc(recv_size);
      last_str = buffer;
    }
  }
  close(server_socket);

  return 0;
}