#include "savings.h"
#include <sstream>
#include <iomanip>
//derived class checkingaccount
//inheritance no need to redefine the data again and place under access specifiers
SavingAccount::SavingAccount(const std::string& password, double balance,const std::string& email, long accountnumber, int age,
                             const std::string& sex, const std::string& username ,const std::string& accountType, int months,double rate)
    : BankAccount(password, balance, email, accountnumber, age, sex, username, "savings"),
    month(months), interestRate(rate) {}

void SavingAccount::applyInterest() {
    double interest = balance * interestRate * (month / 12.0);
    balance += interest;
}
//withdraw function that implements polymorphism
void SavingAccount::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
    }
}
//function get info that returns users info based on account choice
std::string SavingAccount::getInfo() const {
    std::stringstream ss;
    ss << BankAccount::getInfo() << "\n"
       << "Months: " << month << "\n"
       << "Interest Rate: " << (interestRate * 100) << "%";
    return ss.str();
}
