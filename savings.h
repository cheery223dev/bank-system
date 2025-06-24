#ifndef SAVINGACCOUNT_H
#define SAVINGACCOUNT_H

#include "BankAccount.h"

class SavingAccount : public BankAccount {
private:
    int month;
    double interestRate; // Annual interest rate (e.g., 0.05 for 5%)

public:
    SavingAccount(const std::string& password, double balance,const std::string& email, long accountnumber, int age,
                  const std::string& sex, const std::string& username ,const std::string& accountType, int months, double rate);
    void applyInterest();
    void withdraw(double amount) override;//override for polymorphism
    std::string getInfo() const override; // Override for polymorphism
    int getMonth() const { return month; }
};

#endif // SAVINGACCOUNT_H
