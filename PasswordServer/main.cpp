#include "iostream"
#include <SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>
#include <string>
#include <fstream>

using namespace sf;
using namespace std;

class User
{
public:
  string password;
  string login;
};

vector<User> ReadFromFile(string path)
{
  vector<User> a;
  ifstream in;
  in.open(path);
  int i = 0;
  while (!in.eof())
  {
   a.resize(a.size() + 2);
   in >> a[i].login >> a[i].password;
    i++;
  }
  return a;
}

bool IsNameAvailble(string name)
{
  cout << "Search";
  ifstream in;
  in.open("users.txt");
  while (!in.eof())
  {
    string pass, log;
    in >> log >> pass;
    if (log == name)
    {
      return false;
    }
  }
  return true;
}

bool IsReg(string login, string password)
{
  cout << "Search";
  vector<User> users;
  users = ReadFromFile("users.txt");
  for (size_t i = 0; i < users.size(); i++)
    if (users[i].password == password && users[i].login == login)
      return true;
  return false;
}

void PushToFile(vector<User> users)
{
  ofstream out;
  out.open("users.txt");
  for (size_t i = 0; i < users.size(); i++)
    out << users[i].login << ' ' << users[i].password << '\n';
  out.close();
}

void Register(string name, string password)
{
  vector<User> res = ReadFromFile("users.txt");
  User user;
  user.login = name;
  user.password = password;
  res.push_back(user);
  PushToFile(res);
}

int main()
{
  cout << "Server Running" << endl;
  TcpListener listener;
  SocketSelector selector;
  bool done = false;
  vector<TcpSocket*>clients;
  listener.getLocalPort();
  listener.listen(2000);
  selector.add(listener);
  while (!done)
  {
    if (selector.wait())
    {
      if (selector.isReady(listener))
      {
        TcpSocket *socket = new TcpSocket;

        listener.accept(*socket);
        Packet packet;
        string id;
        string status;
        if (socket->receive(packet) == Socket::Done)
          packet >> id >> status;
        cout << id << endl;
        clients.push_back(socket);
        selector.add(*socket);
      }
      else
      {
        for (int i = 0; i < clients.size(); i++)
        {
          if (selector.isReady(*clients[i]))
          {
            Packet packet, sendpacket;
            if (clients[i]->receive(packet) == Socket::Done)
            {
              string login, password, what;
              packet >> what >>login >> password;
              if (what == "log")
              {
                if (IsReg(login, password))
                  sendpacket << "true";
                else
                  sendpacket << "false";
              }
              if (what == "reg")
              {
                if (IsNameAvailble(login))
                {
                  Register(login, password);
                  sendpacket << "true";
                }
                else
                  sendpacket << "false";
              }
              clients[i]->send(sendpacket);
              //for (int j = 0; j < clients.size(); j++)
              //{
              //  cout << "Packet sended" << endl;
              //  sendpacket << 
              //  clients[j]->send(sendpacket);
              //}
            }
          }
        }
      }
    }
  }

  for (vector<TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
    delete *it;
}