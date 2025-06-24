#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

#include "BankAccount.h"
#include "savings.h"
#include "checking.h"
#include <vector>
#include <memory>
#include <string>

struct PasswordEntry {
    long accountnumber;
    char password[50];
};

class BankSystem {
//declaration of banksystem class as base class

public:
bool accountExists(const std::string& username,const std::string& email);
    std::vector<std::shared_ptr<BankAccount>> accounts;
//smart pointer to handle account storage
    //account declarations
    void loadAccounts();
    void saveAccounts();
    bool createAccount(const std::string& username, int age, const std::string& sex,const std::string& email,
                       const std::string& password, const std::string& confirmPassword,
                       const std::string& accountType, int months, double overdraftLimit);
    std::shared_ptr<BankAccount> login(long accountNumber, const std::string& password);
    bool deleteAccount(long accountNumber, const std::string& password);
    bool updateAccount(long accountNumber, const std::string& password, const std::string& newUsername,
                       const std::string& Newemail, const std::string& newPassword);
};

#endif // BANKSYSTEM_H
