#ifndef TABLE_H
#define TABLE_H

class Table
{
    // busy flag
    bool BusyFlag;
    // summary hours 
    int FullHours;
public:
    bool isBusy()
    {
        return BusyFlag;
    }
    void setBusy(bool Flag)
    {
        BusyFlag = Flag;
    }
};

#endif