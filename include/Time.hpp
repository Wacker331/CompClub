#ifndef TIME_H
#define TIME_H

class Time
{
    friend std::ostream& operator<< (std::ostream& os, const Time &Out);
protected:
    int Minutes;
    int Hours;

public:
    Time()
    {
        Hours = 0;
        Minutes = 0;
    }

    Time(int InputHours, int InputMinutes)
    {
        if (InputHours >= 0 && InputHours < 24)
            Hours = InputHours;
        else throw std::invalid_argument("Hours belongs only to [0; 24) ");
        if (InputMinutes >= 0 && InputHours < 60)
            Minutes = InputMinutes;
        else
            throw std::invalid_argument("Minutes belongs only to [0; 60) ");
    }

    int operator==(Time& right)
    {
        return this->Hours == right.Hours && this->Minutes == right.Minutes;
    }

    int operator<(Time& right)
    {
        if (this->Hours != right.Hours)
            return this->Hours < right.Hours;
        else
            return this->Minutes < right.Minutes;
    }

    int operator<=(Time& right)
    {
        if (this->Hours != right.Hours)
            return this->Hours <= right.Hours;
        else
            return this->Minutes <= right.Minutes;
    }

    int operator>(Time& right)
    {
        if (this->Hours != right.Hours)
            return this->Hours > right.Hours;
        else
            return this->Minutes > right.Minutes;
    }

    int operator>=(Time& right)
    {
        if (this->Hours != right.Hours)
            return this->Hours >= right.Hours;
        else
            return this->Minutes >= right.Minutes;
    }

    // Comparsions
};

inline std::ostream& operator<< (std::ostream& os, const Time &Out)
{
    char FormatStr[6] = {0};
    snprintf(FormatStr, 6, "%02d:%02d", Out.Hours, Out.Minutes);
    return os << FormatStr;
}
#endif