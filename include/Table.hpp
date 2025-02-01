#ifndef TABLE_H
#define TABLE_H

#include "Time.hpp"

class Table
{
    // busy flag
    bool BusyFlag;
    // summary hours 
    int PaidHours;
    Time BusyTime;

    Time LastStartTime;
public:
    bool isBusy() const
    {
        return BusyFlag;
    }

    void setBusy(Time& ActionTime, bool Flag)
    {
        if (Flag == true)
        {
            LastStartTime = ActionTime;
        }
        else if (Flag == false)
        {
            Time SessionTime = (ActionTime - LastStartTime);
            BusyTime = BusyTime + SessionTime;
            PaidHours += SessionTime.GetFullHours();
        }
        BusyFlag = Flag;
    }

    void PrintStat(int Index, int HourPrice)
    {
        std::cout << Index + 1 << " " << PaidHours * HourPrice << " " << BusyTime << std::endl;
    }
};

#endif