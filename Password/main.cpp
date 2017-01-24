#include <SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>

using namespace sf;
using namespace std;

int main()
{
  IpAddress ip = IpAddress::getLocalAddress();
  cout << ip << endl;

  TcpSocket socket;
  bool done = false;

  socket.connect(ip, 2000);
  Packet packet;

  packet << "New user";
  socket.send(packet);
  socket.setBlocking(false);
  
  while (1)
  {
    string a;
    cout << "Register or login" << endl;
    cin >> a;
    if (a == "login")
    {
      string login, password;
      cout << "Enter login: ";
      cin >> login;
      cout << "\nEnter password: ";
      cin >> password;
      packet.clear();
      packet << "log" << login << password;
      socket.send(packet);
      while (true)
      {
        socket.receive(packet);
        string answer;
        packet >> answer;
        if (answer == "true")
        {
          cout << "logined";
          break;
        }
        if (answer == "false")
        {
          cout << "NotRegistered";
          break;
        }
      }
    }
    if (a == "register")
    {
      string login, password;
      cout << "Enter name: ";
      cin >> login;
      cout << "\nEnter password: ";
      cin >> password;
      packet.clear();
      packet << "reg" << login << password;
      socket.send(packet);
      while (true)
      {
        socket.receive(packet);
        string answer;
        packet >> answer;
        if (answer == "true")
        {
          cout << "Registered" << endl;
          break;
        }
        if (answer == "false")
        {
          cout << "Enter another name" << endl;
          break;
        }
      }
    }
   
  }
}
