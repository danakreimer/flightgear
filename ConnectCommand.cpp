//
// Created by duni on 22/12/2019.
//

#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <thread>
#include "ConnectCommand.h"
#include "Maps.h"
#include "ExpressionCalculate.h"
#include <algorithm>

void connectClient(int port, string ip) {
  int socketId = 0;
  struct sockaddr_in serverAddress;

  cout << "aaln client" << endl;

  // Create socket
  if ((socketId = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "socket creation error" << endl;
    exit(EXIT_FAILURE);
  }

  cout << "socket created" << endl;

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  const char *ipConst = ip.c_str();

  if (inet_pton(AF_INET, ipConst, &serverAddress.sin_addr) <= 0) {
    cout << "invalid address" << endl;
    exit(EXIT_FAILURE);
  }

  cout << "address converted" << endl;

  while (connect(socketId, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
  }

  cout << "connected" << endl;
  Maps::socketId = socketId;
}

int ConnectCommand::execute(vector<string> params) {
  Interpreter *i = new Interpreter();
  Expression *answer = nullptr;
  string::iterator end_pos = remove(params[2].begin(), params[2].end(), ' ');
  params[2].erase(end_pos, params[2].end());
  end_pos = remove(params[2].begin(), params[2].end(), '\t');
  params[2].erase(end_pos, params[2].end());
  answer = i->interpret(params[2]);
  int port = answer->calculate();
  string ip = params[1];
  ip = ip.substr(1, ip.length() - 2);
  thread threadClient(connectClient, port, ip);
  threadClient.join();
  return 3;
}