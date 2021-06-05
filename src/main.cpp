#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#define BAUD 9600
// Commands to LA
#define USERCMD_RESET 0
#define USERCMD_RUN 1

int main(int argc, char *argv[]) {
  struct sp_port *port;
  int err;
  int i, cmd;
  int count = 4096;

  // need a serial port name
  if (argc < 2) {
    fprintf(stderr, "Usage la port\n");
    exit(1);
  }

  // Open serial port
  err = sp_get_port_by_name(argv[1], &port);
  if (err == SP_OK) err = sp_open(port, SP_MODE_READ);
  if (err != SP_OK) {
    fprintf(stderr, "Can not open port %s\n", argv[1]);
    exit(2);
  }

  // set Baud rate
  sp_set_baudrate(port, BAUD);

  // read data
  for (i = 0; i < count; i++) {
    int waiting;
    int c = 0;
    do {
      waiting = sp_input_waiting(port);
    } while (waiting <= 0);
    // sort of inefficient -- could read a bunch of bytes at once
    // could even block for all of them at once
    sp_nonblocking_read(port, (void *)&c, 1);
    if (i % 16 == 0) putchar('\n');
    printf("%02X ", c);
    std::cout << std::endl << sizeof(c) << std::endl;
  }
  putchar('\n');
  sp_close(port);
  return 0;
}