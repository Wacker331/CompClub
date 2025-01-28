#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
    std::string Name;
    int TableId;
    // Status (playing, waiting, out...)

public:
    Client()
    {
        Name = "";
        TableId = -1;
    }

    Client(std::string InputName)
    {
        Name = InputName;
        TableId = -1;
    }

    void SetTable(int InputTable)
    {
        TableId = InputTable;
    }

    int GetTable()
    {
        return TableId;
    }
};

#endif