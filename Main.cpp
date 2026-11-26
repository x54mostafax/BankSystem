#include <iostream>
#include <cstdlib>
#include <map>
#include <string>
#include <utility>
#include <forward_list>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <vector> 
using namespace std;

#include "Mail.h"
#include "Test.h"
#include "Date.h"
#include "CreditCardAccount.h"

int main()
{
    CreditCardAccount C("1", 9000.8);
    CreditCardAccount C2("2", 90);
    CreditCardAccount C3("3", 2000);
    C.freeze();
    C.unfreeze();
    //Test::RunAll();

    Due B(200, 3, C);
    Due MB(100, 2, C);
    
    
    //cout << "Initial due: " << B.amountDue << endl;
    for (int i = 1; i < 30; i++) {
        Date::PassedDay(30);
        //cout << i << " - B: " << B.amountDue << endl;
        //cout << i << " - MB: " << MB.amountDue << endl;
        if (i == 100) C.pay(B, B.amountDue);
        // if (B.isPaid) break;
    }

    //cout << "Before transaction: " << C.GetBalance() << " -> " << C2.GetBalance() << endl;
    C.transaction(C2, 9000.8);
    //cout << "After transaction: " << C.GetBalance() << " -> " << C2.GetBalance() << endl;

    //cout << "Before transaction: " << C3.GetBalance() << " -> " << C2.GetBalance() << endl;
    C3.transaction(C2, 1000);
    //cout << "After transaction: " << C3.GetBalance() << " -> " << C2.GetBalance();
    
    
    C.mail.GetMessages();
        
    cout << "\nWork main";
    return 0;
}
