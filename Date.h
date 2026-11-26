#pragma once

#include <string>
#include "Due.h"

// Tracks the current date and notifies dues when time advances.
class Date
{
public:
    // Current calendar date.
    inline static int day = 1;
    inline static int month = 1;
    inline static int year = 2000;
    inline static const std::string months[12] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };


    // Static date-advancement functions.
    static void PassedDay(int counter = 1)
    {
        for (int i = 0; i < counter; i++) {
            ++day;
            Due::DailyUpdate();

            if (day > daysInMonth()) {
                day = 1;
                PassedMonth();
            }
            if(day==25){
                Due::WarningMonthlyLateMsg();
            }
        }
    }


    static void PassedMonth()
    {
        ++month;
        Due::MontlyUpdate();

        if (month > 12) {
            month = 1;
            PassedYear();
            if(month==10) Due::WarningYearlyLateMsg();
        }
    }


    static void PassedYear()
    {
        ++year;
        Due::YearlyUpdate();
    }



private:
    // Returns the number of days in the current month.
    static int daysInMonth()
    {
        switch (month) {
        case 2:
            return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
        }
    }
};

inline void Due::WarningMsg()
{
    std::string message = "Final Notice: Your account has an overdue balance of " +
                          std::to_string(amountDue) +
                          ". Pay by " + Date::months[Date::month % 12] +
                          " to avoid late fees or suspension.";
    Mail::SendMessage(owner.mail, message);
}
