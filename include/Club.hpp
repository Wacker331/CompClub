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
    // List of allowed actions in club
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
    // Tables
    std::vector<Table> Tables;
    // Clients database by name
    std::map<std::string, Client> ClientsDB;
    // Working hours
    Time OpenTime, CloseTime;
    // Price per hour
    int HourPrice;
    // Clients' queue
    std::queue<Client*> ClientsQueue;

    // Receives action command and perform it
    void PerformAction(const std::smatch &Match);

    // Register the <TableNum> table for <CurrentClient>
    void TakeTable(Time& ActionTime, int TableNum, int ActionId, Client& CurrentClient);

    // Register <CurrentClient> to the waiting queue
    void StartWaiting(Time& ActionTime, Client& CurrentClient);

    // <CurrentClient> exit the club
    void ExitClub(Time& ActionTime, int ActionId, Client& CurrentClient);

public:
    // Parses first line of the <InputFile> to create a club
    Club(std::ifstream& InputFile);
    // Parses the input line
    void ParseActionLine(std::string Line);
    // Close club
    void Close();
    // Check is club working at the time
    bool isWorking(Time& Check);
};

#endif