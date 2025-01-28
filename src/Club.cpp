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
        else if (TableNum >= 0 && TableNum < Tables.size())
        {
            std::cout << ActionTime << " " << ActionId << " " << ClientName << " " << TableNum + 1 << std::endl;
            if (!Tables[TableNum].isBusy())
            {
                Client& CurrentClient = ClientsDB[ClientName];
                if (CurrentClient.GetTable() != -1)
                    Tables[CurrentClient.GetTable()].setBusy(false);
                CurrentClient.SetTable(TableNum);
                Tables[TableNum].setBusy(true);
            }
            else
                std::cout << ActionTime << " " << Error << " " << "PlaceIsBusy" << std::endl;
        }
        else
            std::cout << ActionTime << " " << Error << " " 
                << "There is less than " << TableNum << " tables" << std::endl;
    }
    else if (ActionId == Waiting)
    {
        if (ClientsDB.count(ClientName) == 0)
            std::cout << ActionTime << " " << Error << " " << "ClientUnknown" << std::endl;
        else
        {
            std::cout << ActionTime << " " << Waiting << " " << ClientName << std::endl;
            for (int i = 0; i < Tables.size(); i++)
            {
                if (!Tables[i].isBusy())
                {
                    std::cout << ActionTime << " " << Error << " " <<  "ICanWaitNoLonger" << std::endl;
                    break;
                }
            }
            // TODO check clients' queue (re-read rules)
        }
    }
    else if (ActionId == SelfOut)
    {
        Client &CurrentClient = ClientsDB[ClientName];
        if (CurrentClient.GetTable() != -1)
            Tables[CurrentClient.GetTable()].setBusy(false);
        ClientsDB.erase(ClientName);
        std::cout << ActionTime << " " << ActionId << " " << ClientName << std::endl;
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

void Club::Close()
{
    std::cout << CloseTime << std::endl;
}

bool Club::isWorking(Time& Check)
{
    return Check >= OpenTime && Check <= CloseTime;
}