#pragma once



#include "Account.h"
#include "Transaction.h"
#include "Due.h"


// Account implementation with credit and deposit limits.
class CreditCardAccount : public Account
{
public:
    // Credit-card limits and available deposit amount.
    static constexpr double fee = 0.0;
    static constexpr double limitDeposite = 20000.0;
    static constexpr double limitBalance = 50000.0;
    double AvailableLimit = limitDeposite;



    // Account operations.
    CreditCardAccount(std::string ID, double balance = 0.0)
        : Account(std::move(ID), balance)
    {
        if (GetBalance() < 0.0 || GetBalance() > limitBalance)
            std::cout << "Initial balance must be within the credit limit";
    }


    void withdraw(double amount) override
    {
        if (amount <= 0.0)
            std::cout << "Withdrawal amount must be positive";
        if (GetBalance() + amount > limitBalance)
            std::cout << "Withdrawal exceeds the credit limit";

        SetBalance(GetBalance() + amount);
    }


    void deposit(double amount) override
    {
        if (amount <= 0.0)
            std::cout << "Deposit amount must be positive";
        if (amount > AvailableLimit)
            std::cout << "Withdrawal exceeds the credit limit";

        AvailableLimit -= amount;
        SetBalance(GetBalance() - amount);
    }


    double calculateFee() const override
    {
        return fee * GetBalance();
    }


    bool transaction(Account& to, double amount) override
    {
        Transaction T(*this, to, amount);
        return true;
    }


    bool pay(Due& due, double amount) override
    {
        if (this != &due.owner)
            return false;
        return due.pay(amount);
    }
};
