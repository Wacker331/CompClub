#ifndef CLUB_H
#define CLUB_H

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>

#include "Table.hpp"
#include "Time.hpp"
#include "Client.hpp"

class Club
{
    enum Action
    {
        ComeIn = 1,
        GetTableSelf = 2,
        Waiting = 3,
        SelfOut = 4,
        ComeOut = 11,
        GetTable = 12,
        Error = 13
    };
    // const int ErrorId = 13;
    // Tables
    std::vector<Table> Tables;
    // Clients database by name
    std::map<std::string, Client> ClientsDB;
    // Working hours
    Time OpenTime, CloseTime;
    // Price
    int HourPrice;
    // Clients' queue
    std::queue<Client*> ClientsQueue;

    void PerformAction(const std::smatch &Match);
    void TakeTable(Time& ActionTime, int TableNum, int ActionId, Client& CurrentClient);
    void StartWaiting(Time& ActionTime, Client& CurrentClient);
    void ExitClub(Time& ActionTime, int ActionId, const Client& CurrentClient);

public:
    Club(std::ifstream& InputFile);
    // Parses the input line
    void ParseActionLine(std::string Line);
    // Close club
    void Close();
    // Check is club working at the time
    bool isWorking(Time& Check);
};

#endif