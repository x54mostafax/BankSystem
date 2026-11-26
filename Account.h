#pragma once




class Due;
class Mail;
// Abstract base class for accounts.
class Account
{
protected:
    // Account identity and current balance.
    std::string ID;
    double balance;
    bool isfrozen=false;


public:
     Mail mail;
    // Registry of active accounts.
    inline static std::map<std::string, Account*> AllAccounts;



    // Static account lookup functions.
    static std::map<std::string, Account*>& GetAllAccounts()
    {
        return AllAccounts;
    }


    static Account* FindByID(const std::string& id)
    {
        auto it = AllAccounts.find(id);
        return it == AllAccounts.end() ? nullptr : it->second;
    }



    // Account operations.
    Account(std::string ID, double balance)
        : ID(std::move(ID)), balance(balance),mail(*this)
    {
        AllAccounts[this->ID] = this;
    }


    virtual ~Account()
    {
        auto it = AllAccounts.find(ID);
        if (it != AllAccounts.end() && it->second == this)
            AllAccounts.erase(it);
    }


    virtual void withdraw(double amount) = 0;
    virtual void deposit(double amount) = 0;
    virtual double calculateFee() const = 0;
    virtual bool pay(Due& due, double amount) = 0;
    virtual bool transaction(Account& to, double amount) = 0;
    void SetBalance(double b) { 
        if(isfrozen){
            std::cout <<"this Account is isfrozen."  << std::endl;
            exit(1);
        }
        balance = b; 
        
    }
    double GetBalance() const { return balance; }
    void freeze(){isfrozen=true;}
    void unfreeze(){isfrozen=false;}
    
    
    
    friend class Mail;
    friend class Due;
    friend class Transaction;
    
    
    
    
};
