#include "BankSystem.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <random>
#include<iomanip>
#include<string.h>
// persistent file management with load account and save account
void BankSystem::loadAccounts() {
    std::ifstream myfile("accounts.txt");
    if (myfile.is_open()) {
        std::string line;
        while (std::getline(myfile, line)) {
            std::stringstream ss(line);
            std::string username, sex, accountType,email;
            int age;
            long  accountnumber;
            double balance;
            char delimiter;

            std::getline(ss, username, ',');
            std::getline(ss, email, ',');
            ss >> age >> delimiter;
            std::getline(ss, sex, ',');
            ss >> delimiter >> accountnumber >> delimiter >> balance >> delimiter;
            std::getline(ss, accountType, ',');
         //logic for account
            if (accountType == "savings") {
                int month;
                std::ifstream savfile("savings.txt");
                if(savfile.is_open()){
                while (savfile >> accountnumber >> month) {
                        if (month>0){
                    accounts.push_back(std::make_shared<SavingAccount>("", balance,email, accountnumber, age, sex,   username,"savings", month, 0.05));
                    break;
                }
                }
                savfile.close();
                }
            } else if (accountType == "checking") {
                double overdraftLimit;
                std::ifstream chkfile("checking.txt");
                if(chkfile.is_open()){
                while (chkfile >> accountnumber >> overdraftLimit) {
                        if (overdraftLimit>=0){
                    accounts.push_back(std::make_shared<CheckingAccount>("", balance, email, accountnumber, age, sex, username,"checking",  overdraftLimit));
                    break;
                }
                }
                chkfile.close();
            }
        }
        myfile.close();
    }
    }
   //password saving operation to check file for saved password
    std::ifstream passfile("password.txt");  // No binary flag, plain text mode

    if (passfile.is_open()) {
        long accNum;
        std::string password;

        while (passfile >> accNum >> password) {
            for (auto& acc : accounts) {
                if (acc->getAccountNumber() == accNum) {
                    acc->setPassword(password);
                    break;
                }
            }
        }

        passfile.close();
    }

}
          //function for saving accounts to an ouput file in txt format
void BankSystem::saveAccounts() {
    std::ofstream outfile("accounts.txt");
    if (outfile.is_open()) {
        for (const auto& acc : accounts) {
            outfile << acc->getUsername() << ',' << acc->getAge() << ',' << acc->getSex() << ','<<acc->getemail()<<','
                    << acc->getAccountNumber() << ','
                    << std::fixed << std::setprecision(2) << acc->getBalance() << ','
                    << acc->getAccountType() << '\n';
        }
        outfile.close();
    }

    std::ofstream savfile("savings.txt");
    std::ofstream chkfile("checking.txt");
    for (const auto& acc : accounts) {
        if (acc->getAccountType() == "savings") {
            if (auto* sav_acc = dynamic_cast<SavingAccount*>(acc.get())) {
                savfile << acc->getAccountNumber() << ' ' << sav_acc->getMonth() << '\n';
            }
        } else if (acc->getAccountType() == "checking") {
            if (auto* chk_acc = dynamic_cast<CheckingAccount*>(acc.get())) {
                chkfile << acc->getAccountNumber() << ' ' << chk_acc->getOverdraftLimit() << '\n';
            }
        }
    }
    savfile.close();
    chkfile.close();

        std::ofstream passfile("password.txt"); // Plain text mode
        if (passfile.is_open()) {
            for (const auto& acc : accounts) {
                passfile << acc->getAccountNumber() << " "
                         << acc->getPassword() << "\n";
            }
            passfile.close();
        }
}

  //function create account that takes in user credentials
//age verification implemented for account verification
bool BankSystem::createAccount(const std::string& username, int age, const std::string& sex,const std::string& email,
                               const std::string& password, const std::string& confirmPassword,
                               const std::string& accountType, int months, double overdraftLimit) {
    if (  username.empty() || email.empty() || sex.empty()) {
        return false;
    }
    // Password validation
    if (password.length() < 6 || password != confirmPassword) {
        return false;
    }

    // Age validation
    if (age < 18) {
        return false;
    }

    // Email validation
    if (email.find("@gmail.com") == std::string::npos &&
        email.find("@Gmail.com") == std::string::npos &&
        email.find("@GMAIL.COM") == std::string::npos) {
        return false;
    }
    // Account type-specific validation
    if (accountType == "savings" && months <= 0) {
        return false; // Invalid duration
    }
    if (accountType== "checking" && overdraftLimit < 0) {
        return false;
    }

        //account number generation randomised function for no redunancy
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    long accountNumber;
    do {
        accountNumber = dis(gen);
    } while (std::any_of(accounts.begin(), accounts.end(),
                         [accountNumber](const auto& acc) { return acc->getAccountNumber() == accountNumber; }));

    if (accountType == "savings"&& months>0) {
        accounts.push_back(std::make_shared<SavingAccount>(password, 0.0,email, accountNumber, age, sex, username, "savings", months, 0.05));
    } else if (accountType == "checking" && overdraftLimit>=0) {
        accounts.push_back(std::make_shared<CheckingAccount>(password, 0.0,email, accountNumber, age, sex, username,"checking" , overdraftLimit));
    } else {
        return false;
    }

    saveAccounts();
    return true;
}
//login function that lops the pointer in search of account based on accountnumber and passord
std::shared_ptr<BankAccount> BankSystem::login(long accountNumber, const std::string& password) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
                           [accountNumber](const auto& acc) { return acc->getAccountNumber() == accountNumber; });
    if (it != accounts.end() && (*it)->getPassword() == password) {
        return *it;
    }
    return nullptr;
}
// bool function delete account that passes condition true o false for option delete
bool BankSystem::deleteAccount(long accountNumber, const std::string& password) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
                           [accountNumber](const auto& acc) { return acc->getAccountNumber() == accountNumber; });
    if (it != accounts.end() && (*it)->getPassword() == password) {
        accounts.erase(it);
        saveAccounts();
        return true;
    }
    return false;
}
//function update account
bool BankSystem::updateAccount(long accountNumber, const std::string& password,const std::string& newemail, const std::string& newUsername,
                                const std::string& newPassword) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
                           [accountNumber](const auto& acc) { return acc->getAccountNumber() == accountNumber; });
    if (it != accounts.end() && (*it)->getPassword() == password) {
        if (!newUsername.empty()) (*it)->setUsername(newUsername);
        if (!newemail.empty()) (*it)->setemail(newemail);
        if (!newPassword.empty()) (*it)->setPassword(newPassword);
        saveAccounts();
        return true;
    }
    return false;
}
//gets the account and returns the available credentials then user can edit his choice
bool BankSystem::accountExists(const std::string& username,const std::string& email){
    for(const auto&account:accounts){
        if(account->getUsername()==username||account->getemail()==email){
            return true;
        }
    }
    return false;
}
