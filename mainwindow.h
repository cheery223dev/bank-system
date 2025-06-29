#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "bankSystem.h" // Assuming BankSystem is defined in bankSystem.h

// Forward declaration of the UI class generated by Qt Designer
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_login_loginButton_clicked();
    void on_createAccount_createButton_clicked();
    void on_depositButton_clicked();
    void on_withdrawButton_clicked();
    void on_refreshInfoButton_clicked();
    void on_deleteAccountButton_clicked();
    void on_updateButton_clicked(); // For updating account details
    void on_applyInterestButton_clicked();
    void on_mainMenu_exitButton_clicked();


    //new slots for return button only
    void on_welcome_createAccountButton_clicked();
    void on_welcome_loginButton_clicked();
    void on_mainMenu_depositButton_clicked();
    void on_mainMenu_withdrawButton_clicked();
    void on_mainMenu_accountInfoButton_clicked();
    void on_mainMenu_deleteButton_clicked();
    void on_mainMenu_updateButton_clicked();
    void on_returntoMainMenuButton_clicked();
    void on_returntoMainMenuButton_2_clicked();
    void on_returntoMainMenuButton_3_clicked();
    void on_returntoMainMenuButton_4_clicked();
    void on_returntoMainMenuButton_5_clicked();
    void on_createAccount_returnButton_clicked();
    void on_login_returnButton_clicked();
private:
    Ui::MainWindow *ui; // Pointer to the UI object
    BankSystem bankSystem; // BankSystem object to manage accounts
    std::shared_ptr<BankAccount> currentAccount; // Current logged-in account
};

#endif // MAINWINDOW_H
