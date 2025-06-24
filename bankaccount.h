#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
//access specifiers or encapsulation of data
class BankAccount {
protected:
    std::string username;
    int age;
    std::string sex;
    std::string email;
    long phonenumber;
    std::string password;
    long accountnumber;
    double balance;
    std::string accountType;
//public constructor for data initialisation
public:
    BankAccount();
    BankAccount(const std::string& password, double balance,const std::string& email, long accountnumber, int age,
                const std::string& sex, const std::string& username, const std::string& accountType);

    virtual ~BankAccount() = default;
    // Virtual destructor for polymorphism

    // Getters
    std::string getUsername() const { return username; }
    std::string getemail() const { return email; }
    int getAge() const { return age; }
    std::string getSex() const { return sex; }
     std::string getPassword() const { return password; }
    long getAccountNumber() const { return accountnumber; }
    double getBalance() const { return balance; }
    std::string getAccountType() const { return accountType; }

    // Setters
    void setUsername(const std::string& name) { username = name; }
    void setPassword(const std::string& pass) { password = pass; }
    void setemail(const std::string& mail) { email = mail; }

    // Virtual functions for polymorphism
    virtual void deposit(double amount);
    virtual void withdraw(double amount) =0; // Pure virtual for polymorphism
    virtual std::string getInfo() const; // Virtual for account-specific info
};

#endif // BANKACCOUNT_H
