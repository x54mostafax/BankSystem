#pragma once


#include "Account.h"

// Transfers an amount between two accounts and records the transaction.
class Transaction
{
private:
    // Accounts involved in the transfer.
    const std::string ID;
    Account& from;
    Account& to;
    double amount;
public:
    // Transaction registry and identifier counter.
    inline static std::map<std::string, Transaction*> AllTransactions;
    inline static std::size_t nextID = 1;

    // Transaction details.
    



    // Static transaction lookup functions.
    static std::map<std::string, Transaction*>& GetAllTransactions()
    {
        return AllTransactions;
    }


    static Transaction* FindByID(const std::string& id)
    {
        auto it = AllTransactions.find(id);
        return it == AllTransactions.end() ? nullptr : it->second;
    }



    // Construction and lifetime methods.
    Transaction(Account& from, Account& to, double amount)
        : from(from),
          to(to),
          ID(std::to_string(nextID++)),
          amount(amount)
    {
        std::cout << "#Trans#";
        if (from.GetBalance() < amount || amount <= 0) {
            std::cout << "Transaction is rejected. ";
            std::exit(1);
        }

        from.SetBalance(from.GetBalance() - amount);
        to.SetBalance(to.GetBalance() + amount);
        AllTransactions[ID] = this;
    }


    Transaction(const Transaction&) = delete;


    Transaction& operator=(const Transaction&) = delete;


    ~Transaction()
    {
        AllTransactions.erase(ID);
    }
};
