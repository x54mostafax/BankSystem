#pragma once



#include "CreditCardAccount.h"
#include "Date.h"

// Small collection of checks for the account and due classes.
class Test
{
private:
    // Shared assertion helpers.
    static void Check(bool condition, const char* message)
    {
        if (!condition)
            throw std::runtime_error(message);
    }


    static bool Near(double actual, double expected)
    {
        return std::abs(actual - expected) < 1e-9;
    }



public:
    // Individual test groups.
    static void TestCreditCardAccount()
    {
        constexpr double initialBalance = 1000.95;
        CreditCardAccount account("12", initialBalance);
        Check(Near(account.GetBalance(), initialBalance), "account initial balance");

        account.SetBalance(0.0);
        Check(Near(account.GetBalance(), 0.0), "SetBalance");
        account.SetBalance(initialBalance);

        account.withdraw(500.0);
        Check(Near(account.GetBalance(), initialBalance + 500.0), "withdraw");

        account.deposit(300.0);
        Check(Near(account.GetBalance(), initialBalance + 200.0), "deposit");
        Check(Near(account.AvailableLimit, CreditCardAccount::limitDeposite - 300.0), "available deposit limit");
        Check(Near(account.calculateFee(), CreditCardAccount::fee * account.GetBalance()), "calculateFee");
    }


    static void TestTransaction()
    {
        CreditCardAccount from("20", 500.0);
        CreditCardAccount to("21", 100.0);

        Check(from.transaction(to, 200.0), "transaction return value");
        Check(Near(from.GetBalance(), 300.0), "transaction source balance");
        Check(Near(to.GetBalance(), 300.0), "transaction destination balance");
    }


    static void TestDue()
    {
        CreditCardAccount owner("30", 1000.0);
        Due due(300.0, 3, owner);

        Check(!owner.pay(due, 100.0), "partial due payment result");
        Check(Near(due.amountDue, 200.0), "partial due amount");
        Check(Near(owner.GetBalance(), 900.0), "balance after partial due payment");

        Check(owner.pay(due, 200.0), "full due payment result");
        Check(due.isPaid, "due marked paid");
        Check(Near(due.amountDue, 0.0), "paid due amount");
        Check(Near(owner.GetBalance(), 700.0), "balance after full due payment");

        CreditCardAccount other("31", 100.0);
        Due otherDue(50.0, 1, owner);
        Check(!other.pay(otherDue, 50.0), "another account cannot pay this due");
        Check(owner.pay(otherDue, 50.0), "due can be paid by its owner");
    }


    static void TestLateFeesAndDates()
    {
        CreditCardAccount owner("40", 1000.0);

        Date::day = 1;
        Date::month = 1;
        Date::year = 2000;
        Due daily(100.0, 1, owner);
        Date::PassedDay();
        Check(Near(daily.amountDue, 100.0 * (1.0 + Due::LateFees[0])), "daily late fee");
        Check(Date::day == 2, "advance one day");
        owner.pay(daily, daily.amountDue);

        Date::day = 1;
        Date::month = 1;
        Date::year = 2000;
        Due monthly(100.0, 2, owner);
        Date::PassedMonth();
        Check(Near(monthly.amountDue, 100.0 * (1.0 + Due::LateFees[1])), "monthly late fee");
        Check(Date::month == 2, "advance one month");
        owner.pay(monthly, monthly.amountDue);

        Date::day = 1;
        Date::month = 12;
        Date::year = 2000;
        Due yearly(100.0, 3, owner);
        Date::PassedMonth();
        Check(Near(yearly.amountDue, 100.0 * (1.0 + Due::LateFees[2])), "yearly late fee");
        Check(Date::month == 1 && Date::year == 2001, "advance to next year");
        owner.pay(yearly, yearly.amountDue);

        Date::day = 28;
        Date::month = 2;
        Date::year = 2000;
        Date::PassedDay();
        Check(Date::day == 29 && Date::month == 2, "leap-day rollover");
        Date::PassedDay();
        Check(Date::day == 1 && Date::month == 3, "month rollover after leap day");

        Date::day = 1;
        Date::month = 1;
        Date::year = 2000;
    }


    static void RunAll()
    {
        TestCreditCardAccount();
        TestTransaction();
        TestDue();
        TestLateFeesAndDates();
        std::cout << "All tests passed\n";
    }
};
