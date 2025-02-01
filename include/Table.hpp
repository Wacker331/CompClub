#ifndef TABLE_H
#define TABLE_H

#include "Time.hpp"

class Table
{
    // busy flag
    bool BusyFlag;
    // Full hours that was paid
    int PaidHours;
    // summary hours 
    Time BusyTime;

    Time LastStartTime;
public:
    bool isBusy() const
    {
        return BusyFlag;
    }

    // Sets busy flag of the table and count time that table was in use
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

    // Print stats when club closes using index of the table and price in hour
    void PrintStat(int Index, int HourPrice)
    {
        std::cout << Index + 1 << " " << PaidHours * HourPrice << " " << BusyTime << std::endl;
    }
};

#endif