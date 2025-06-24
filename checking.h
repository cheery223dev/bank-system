#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H

#include "BankAccount.h"
//derived class from bank system base class
class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;
//inheritance
public:
    CheckingAccount(const std::string& password, double balance,const std::string& email, long accountnumber, int age,
                    const std::string& sex, const std::string& username ,const std::string& accountType, double limit);
    //override of virtual function for polymorphism
    void withdraw(double amount) override;
    std::string getInfo() const override; // Override for polymorphism
    double getOverdraftLimit() const { return overdraftLimit; }
};

#endif // CHECKINGACCOUNT_H
