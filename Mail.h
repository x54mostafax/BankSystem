#pragma once

using Entry=pair<long long,string>;
class Account;
class Mail{
    inline static int intialTime=0;
    private:
    Account& owner;
    
    vector<Entry>Messages ;

    public:
    Mail( Account& owner):owner(owner){};
    static void SendMessage(Mail& ownerMail,const string& Text){
        ownerMail.Messages.emplace_back(Entry{intialTime++,Text});
    }
    void GetMessages(){
        for (auto msg : Messages) {
            std::cout << "\nTime " <<msg.first<<"\nText :"<<msg.second<<" \n";
            
        }
    }
    
};