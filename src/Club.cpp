#include "Club.hpp"

Club::Club(std::ifstream& InputFile)
{
    std::string Line;

    // 1st line of input file
    std::getline(InputFile, Line);
    int TablesNum = std::stoi(Line);
    for (int i = 0; i < TablesNum; i++)
        Tables.push_back(Table());

    // 2nd line of input file
    std::getline(InputFile, Line);
    const std::regex StartEndTimeRegex("(\\d{2}):(\\d{2}) +(\\d{2}):(\\d{2})");
    std::smatch StartEndTimeMatch;
    if (regex_search(Line, StartEndTimeMatch, StartEndTimeRegex))
    {
        if (StartEndTimeMatch.size() == 5)
        {
            // 0 index is the whole regex group
            int StartHour = std::stoi(StartEndTimeMatch[1].str());
            int StartMinute = std::stoi(StartEndTimeMatch[2].str());
            int EndHour = std::stoi(StartEndTimeMatch[3].str());
            int EndMinute = std::stoi(StartEndTimeMatch[4].str());
            OpenTime = Time(StartHour, StartMinute);
            CloseTime = Time(EndHour, EndMinute);
            if (CloseTime < OpenTime)
            {
                std::cout << "Club closes earlier than opens..." << std::endl;
                exit(-1);
            }
            std::cout << OpenTime << std::endl;
        }
        else
            throw std::invalid_argument("Wrong time format of working hours");
    }
    
    // 3rd line of input file
    std::getline(InputFile, Line);
    HourPrice = std::stoi(Line);
}

void Club::PerformAction(const std::smatch &Match)
{
    int Hours = std::stoi(Match[2].str());
    int Minutes = std::stoi(Match[3].str());
    Time ActionTime(Hours, Minutes);
    int ActionId = std::stoi(Match[4].str());
    std::string ClientName = Match[5].str();
    if (ActionId == ComeIn)
    {
        std::cout << ActionTime << " " << ActionId << " " << ClientName << std::endl;
        if (!isWorking(ActionTime))
            std::cout << ActionTime << " " << Error << " " << "NotOpenYet" << std::endl;
        else if (ClientsDB.count(ClientName) > 0)
            std::cout << ActionTime << " " << Error << " " << "YouShallNotPass" << std::endl;
        else if (ClientsDB.count(ClientName) == 0)
        {
            ClientsDB[ClientName] = Client(ClientName);
        }
    }
    else if (ActionId == GetTableSelf)
    {
        int TableNum = std::stoi(Match[6].str()) - 1;
        if (ClientsDB.count(ClientName) == 0)
            std::cout << ActionTime << " " << Error << " " << "ClientUnknown" << std::endl;
        else
            TakeTable(ActionTime, TableNum, ActionId, ClientsDB[ClientName]);
    }
    else if (ActionId == Waiting)
    {
        if (ClientsDB.count(ClientName) == 0)
            std::cout << ActionTime << " " << Error << " " << "ClientUnknown" << std::endl;
        else
        {
            StartWaiting(ActionTime, ClientsDB[ClientName]);
        }
    }
    else if (ActionId == SelfOut)
    {
        Client &CurrentClient = ClientsDB[ClientName];
        ExitClub(ActionTime, ActionId, CurrentClient);
        ClientsDB.erase(ClientName);
    }
    else
        std::cout << "Unknown ActionId: " << ActionId << std::endl;
}

void Club::ParseActionLine(std::string Line)
{
    const std::regex InputRegex("((\\d{2}):(\\d{2}) +([1-4]) +([\\w-]+)(?:(?: )+(\\d+))?)");
    std::smatch Match;
    int Hour, Minute, EventId;
    std::string ClientName;
    if (regex_search(Line, Match, InputRegex))
    {
        if (Match.size() == 7)
        {
            PerformAction(Match);
        }
        else 
            throw std::invalid_argument("Wrong input");
    }
    else
    {
        std::cout << "Fail on line - " << Line << std::endl;
        std::cout << "Exiting..." << std::endl;
        exit(-1);
    }
}

void Club::TakeTable(Time& ActionTime, int TableNum, int ActionId, Client& CurrentClient)
{
    if (!isWorking(ActionTime))
        return;
    
    if (TableNum >= 0 && TableNum < Tables.size())
    {
        std::cout << ActionTime << " " << ActionId << " " << CurrentClient.GetName() << " " << TableNum + 1 << std::endl;
        if (!Tables[TableNum].isBusy())
        {
            if (CurrentClient.GetTable() != -1)
                Tables[CurrentClient.GetTable()].setBusy(ActionTime, false);
            CurrentClient.SetTable(TableNum);
            Tables[TableNum].setBusy(ActionTime, true);
        }
        else
            std::cout << ActionTime << " " << Error << " " << "PlaceIsBusy" << std::endl;
    }
    else
    {
        std::cout << ActionTime << " " << Error << " " 
                    << "There is less than " << TableNum << " tables" << std::endl;
    }
}

void Club::StartWaiting(Time& ActionTime, Client& CurrentClient)
{
    std::cout << ActionTime << " " << Waiting << " " << CurrentClient.GetName() << std::endl;
    for (int i = 0; i < Tables.size(); i++)
    {
        if (!Tables[i].isBusy())
        {
            std::cout << ActionTime << " " << Error << " " <<  "ICanWaitNoLonger" << std::endl;
            return;
        }
    }
    if (ClientsQueue.size() > Tables.size())
    {
        ExitClub(ActionTime, ComeOut, CurrentClient);
        ClientsDB.erase(CurrentClient.GetName());
    }
    else
    {
        ClientsQueue.push(&CurrentClient);
    }
}

void Club::ExitClub(Time& ActionTime, int ActionId, const Client& CurrentClient)
{
    int TableNumber = CurrentClient.GetTable();
    if (TableNumber != -1)
        Tables[TableNumber].setBusy(ActionTime, false);
    std::cout << ActionTime << " " << ActionId << " " << CurrentClient.GetName() << std::endl;
    if (ClientsQueue.size() > 0)
    {
        Client *ReplaceClient = ClientsQueue.front();
        TakeTable(ActionTime, TableNumber, GetTable, *ReplaceClient);
        ClientsQueue.pop();
    }
}

void Club::Close()
{
    for (const auto& Client : ClientsDB)
    {
        ExitClub(CloseTime, ComeOut, Client.second);
    }
    ClientsDB.clear();
    std::cout << CloseTime << std::endl;
    for (int i = 0; i < Tables.size(); i++)
    {
        Tables[i].PrintStat(i, HourPrice);
    }
}

bool Club::isWorking(Time& Check)
{
    return Check >= OpenTime && Check <= CloseTime;
}