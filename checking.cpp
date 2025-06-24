#include "checking.h"
#include <sstream>
#include <iomanip>
//derived class checkingaccount
//inheritance from bankaccount class
CheckingAccount::CheckingAccount(const std::string& password, double balance,const std::string& email, long accountnumber, int age,
                                 const std::string& sex, const std::string& username ,const std::string& accountType, double limit)
    : BankAccount(password, balance,email, accountnumber, age, sex, username, "checking"),
    overdraftLimit(limit) {}

//withdraw function that implements polymorphism
void CheckingAccount::withdraw(double amount) {
    if (amount > 0 && amount <= (balance + overdraftLimit)) {
        balance -= amount;
    }
}
//inheritance of function getinfo from bankaccount
//function get info that returns users info based on account choice
std::string CheckingAccount::getInfo() const {
    std::stringstream ss;
    ss << BankAccount::getInfo() << "\n"
       << "Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit;
    return ss.str();
}
