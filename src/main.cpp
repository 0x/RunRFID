#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <array>

#define BAUD 9600

int main(int argc, char *argv[]) {
  struct sp_port *port;
  int err;

  // need a serial port name
  if (argc < 2) {
    fprintf(stderr, "Usage la port\n");
    return 0;
  }

  // open serial port
  err = sp_get_port_by_name(argv[1], &port);
  if (err == SP_OK) err = sp_open(port, SP_MODE_READ);
  if (err != SP_OK) {
    fprintf(stderr, "Can not open port %s\n", argv[1]);
    return 0;
  }

  // set Baud rate
  sp_set_baudrate(port, BAUD);

  // read data and write to file
  std::fstream otputFile;
  std::array<int, 31> tag;
  int i = 0;
  while (true) {
    int waiting;
    int c = 0;
    do {
      waiting = sp_input_waiting(port);
    } while (waiting <= 0);
    // sort of inefficient -- could read a bunch of bytes at once
    // could even block for all of them at once
    sp_nonblocking_read(port, (void *)&c, 1);
    tag[i] = c;
    if (i % 30 == 0) {
      putchar('\n');
      otputFile.open("RFIDData.txt",
                   std::ios::out | std::ios::binary | std::ios::trunc);
      if (otputFile.is_open()) {
        for (const auto el: tag)
          otputFile << std::hex << el;
      otputFile.close();
      }
      i = 0;
    }
    printf("%02X ", c);

    ++i;
  }

  if (otputFile.is_open()) otputFile.close();
  sp_close(port);
}
