#include "BankAccount.h"
#include <sstream>
#include <iomanip>


//default and parametalized constructors to set values and parameters for bank account
BankAccount::BankAccount() : username(""), age(0), sex(""),email(""), password(""),
    accountnumber(0), balance(0.0), accountType("regular") {}

BankAccount::BankAccount(const std::string& password, double balance,const std::string& email, long accountnumber, int age,
                         const std::string& sex, const std::string& username, const std::string& accountType)
    : username(username), age(age), sex(sex),email(email), password(password),
    accountnumber(accountnumber), balance(balance), accountType(accountType) {}

//function deposit for depositing into account
void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}
//string getinfo set to const so to get info based on account type
std::string BankAccount::getInfo() const {
    std::stringstream ss;
    ss << "Account Number: " << accountnumber << "\n"
       << "Username: " << username << "\n"
       << "Balance: $" << std::fixed << std::setprecision(2) << balance << "\n"
       << "Account Type: " << accountType;
    return ss.str();
}
