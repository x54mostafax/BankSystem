#pragma once




#include "Account.h"

// Represents an amount owed by an account, with time-based late fees.
class Due
{
public:
    // Static due registry, IDs, late fees, and update groups.
    inline static double LateFees[3] = {0.001, 0.05, 0.75};
    inline static std::map<std::string, Due*> AllDues;
    inline static std::size_t nextID = 1;
    inline static std::forward_list<Due*> DailyDue;
    inline static std::forward_list<Due*> MonthlyDue;
    inline static std::forward_list<Due*> YearlyDue;

    // Due-specific attributes.
    const std::string ID;
    double amountDue;
    int dueDate;
    Account& owner;
    bool isPaid;



    // Static registry and update functions.
    static std::map<std::string, Due*>& GetAllDues()
    {
        return AllDues;
    }


    static Due* FindByID(const std::string& id)
    {
        auto it = AllDues.find(id);
        return it == AllDues.end() ? nullptr : it->second;
    }


    static void DailyUpdate()
    {
        for (Due* due : DailyDue)
            if (due != nullptr)
                due->ImplementLateFee();
    }

    static void MontlyUpdate()
    {
        for (Due* due : MonthlyDue)
            if (due != nullptr)
                due->ImplementLateFee();
    }

    static void WarningMonthlyLateMsg()
    {
        for (Due* due : MonthlyDue)
            if (due != nullptr)
                due->WarningMsg();
    }


    static void YearlyUpdate()
    {
        for (Due* due : YearlyDue)
            if (due != nullptr)
                due->ImplementLateFee();
    }
    
    static void WarningYearlyLateMsg()
    {
        for (Due* due : MonthlyDue)
            if (due != nullptr)
                due->WarningMsg();
    }



    // Construction, payment, and late-fee methods.
    Due(double amountDue, int dueDate, Account& owner)
        : ID(std::to_string(nextID++)),
          amountDue(amountDue),
          dueDate(dueDate),
          owner(owner),
          isPaid(false)
    {
        AllDues[ID] = this;

        switch (dueDate) {
        case 1:
            DailyDue.push_front(this);
            break;
        case 2:
            MonthlyDue.push_front(this);
            break;
        default:
            this->dueDate = 3;
            YearlyDue.push_front(this);
        }
    }


    Due(const Due&) = delete;


    Due& operator=(const Due&) = delete;


    virtual ~Due()
    {
        AllDues.erase(ID);
        DailyDue.remove(this);
        MonthlyDue.remove(this);
        YearlyDue.remove(this);
    }


    virtual bool pay(double amount)
    {
        if (amount <= 0.0)
            std::cout << "Payment amount must be positive";

        if (isPaid)
            return false;

        const double payment = amount < amountDue ? amount : amountDue;
        amountDue -= payment;
        owner.SetBalance(owner.GetBalance() - payment);

        if (amountDue <= 0.0) {
            amountDue = 0.0;
            isPaid = true;

            switch (dueDate) {
            case 1:
                DailyDue.remove(this);
                break;
            case 2:
                MonthlyDue.remove(this);
                break;
            case 3:
                YearlyDue.remove(this);
                break;
            }
            return true;
        }

        return false;
    }


    virtual void ImplementLateFee()
    {
        if (!isPaid)
            amountDue *= 1.0 + LateFees[dueDate - 1];
    }
    virtual void WarningMsg();
    
    
};
