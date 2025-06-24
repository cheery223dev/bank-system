#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    bankSystem.loadAccounts();

    ui->stackedWidget->setCurrentIndex(0);
    // Existing gender exclusivity logic
    connect(ui->createAccount_createSexEdit1, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->createAccount_createSexEdit2->setChecked(false);
            ui->createAccount_sexStatusLabel->setText("Valid");
        } else if (!ui->createAccount_createSexEdit2->isChecked()) {
            ui->createAccount_sexStatusLabel->setText("Invalid");
        }
    });
    connect(ui->createAccount_createSexEdit2, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->createAccount_createSexEdit1->setChecked(false);
            ui->createAccount_sexStatusLabel->setText("Valid");
        } else if (!ui->createAccount_createSexEdit1->isChecked()) {
            ui->createAccount_sexStatusLabel->setText("Invalid");
        }
    });

    // Connect signals for real-time validation
    connect(ui->createAccount_createPassEdit, &QLineEdit::textChanged, this, [this]() {
        QString password = ui->createAccount_createPassEdit->text();
        if (password.length() >= 6) {
            ui->createAccount_passwordStatusLabel->setText("Valid");
            ui->createAccount_createPassEdit->setStyleSheet("");
        } else {
            ui->createAccount_passwordStatusLabel->setText("Invalid");
            ui->createAccount_createPassEdit->setStyleSheet("border: 1px solid red");
        }
    });

    connect(ui->createAccount_createmailEdit, &QLineEdit::textChanged, this, [this]() {
        QString email = ui->createAccount_createmailEdit->text();
        QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

        if (regex.match(email).hasMatch()) {
            ui->createAccount_emailStatusLabel->setText("Valid");
            ui->createAccount_createmailEdit->setStyleSheet("");
        } else {
            ui->createAccount_emailStatusLabel->setText("Invalid");
            ui->createAccount_createmailEdit->setStyleSheet("border: 1px solid red");
        }
    });




    connect(ui->login_loginButton, &QPushButton::clicked, this, &MainWindow::on_login_loginButton_clicked);
    connect(ui->createAccount_createButton, &QPushButton::clicked, this, &MainWindow::on_createAccount_createButton_clicked);
    connect(ui->welcome_createAccountButton, &QPushButton::clicked, this, &MainWindow::on_welcome_createAccountButton_clicked);
    connect(ui->welcome_loginButton, &QPushButton::clicked, this, &MainWindow::on_welcome_loginButton_clicked);
    connect(ui->mainMenu_depositButton, &QPushButton::clicked, this, &MainWindow::on_mainMenu_depositButton_clicked);
    connect(ui->mainMenu_withdrawButton, &QPushButton::clicked, this, &MainWindow::on_mainMenu_withdrawButton_clicked);
    connect(ui->mainMenu_accountInfoButton, &QPushButton::clicked, this, &MainWindow::on_mainMenu_accountInfoButton_clicked);
    connect(ui->mainMenu_deleteButton, &QPushButton::clicked, this, &MainWindow::on_mainMenu_deleteButton_clicked);
    connect(ui->mainMenu_updateButton, &QPushButton::clicked, this, &MainWindow::on_mainMenu_updateButton_clicked);
    connect(ui->mainMenu_exitButton, &QPushButton::clicked, this, &MainWindow::on_mainMenu_exitButton_clicked);
    connect(ui->returntoMainMenuButton, &QPushButton::clicked, this, &MainWindow::on_returntoMainMenuButton_clicked);
    connect(ui->returntoMainMenuButton_2, &QPushButton::clicked, this, &MainWindow::on_returntoMainMenuButton_2_clicked);
    connect(ui->returntoMainMenuButton_3, &QPushButton::clicked, this, &MainWindow::on_returntoMainMenuButton_3_clicked);
    connect(ui->returntoMainMenuButton_4, &QPushButton::clicked, this, &MainWindow::on_returntoMainMenuButton_4_clicked);
    connect(ui->returntoMainMenuButton_5, &QPushButton::clicked, this, &MainWindow::on_returntoMainMenuButton_5_clicked);
    connect(ui->login_returnButton, &QPushButton::clicked, this, &MainWindow::on_login_returnButton_clicked);
    connect(ui->createAccount_returnButton, &QPushButton::clicked, this, &MainWindow::on_createAccount_returnButton_clicked);


    // Enable/disable months and overdraft based on account type
    connect(ui->createAccount_createAccountTypeCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        ui->createAccount_createMonthsEdit->setEnabled(text == "savings");
        ui->createAccount_createOverdraftEdit->setEnabled(text == "checking");
    });

}

MainWindow::~MainWindow() {
    delete ui;
}

// Login and Create Account implementations
void MainWindow::on_login_loginButton_clicked() {
    bool ok;

    long accountNumber = ui->login_accountNumberEdit->text().toLong(&ok);
    std::string password = ui->login_passwordEdit->text().toStdString();
    currentAccount = bankSystem.login(accountNumber, password);

    if (ok &&currentAccount) {
        QMessageBox::information(this, "Success", "Logged in successfully!");
         ui->stackedWidget->setCurrentIndex(3);
    } else {
        QMessageBox::warning(this, "Error", "Invalid account number or password");
    }
}
//abstraction at the level of the user interface that just calls the account and it works
void MainWindow::on_createAccount_createButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->createAccountPage); // Ensure we're on create account page


        // Collect inputs
        std::string username = ui->createAccount_createNameEdit->text().toStdString();
        std::string email = ui->createAccount_createmailEdit->text().toStdString();
        int age = ui->createAccount_createAgeEdit->value();
        std::string password = ui->createAccount_createPassEdit->text().toStdString();
        std::string confirmPassword = ui->createAccount_createConfirmPassEdit->text().toStdString();
        std::string accountType = ui->createAccount_createAccountTypeCombo->currentText().toStdString();
        int months = ui->createAccount_createMonthsEdit->value();
        double overdraftLimit = ui->createAccount_createOverdraftEdit->value();

        std::string sex;
        if (ui->createAccount_createSexEdit1->isChecked()) sex = "male";
        else if (ui->createAccount_createSexEdit2->isChecked()) sex = "female";

        // Reset all styles and labels
        bool ok = true;

        // Email validation
        QRegularExpression emailRegex("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,10}$",
                                      QRegularExpression::CaseInsensitiveOption);
        if (!emailRegex.match(QString::fromStdString(email)).hasMatch()) {
            ok = false;
            ui->createAccount_createmailEdit->setStyleSheet("border: 1px solid red");
            ui->createAccount_emailStatusLabel->setText("Invalid email");
        }

        // Password validation
        if (password.length() < 6) {
            ok = false;
            ui->createAccount_createPassEdit->setStyleSheet("border: 1px solid red");
            ui->createAccount_passwordStatusLabel->setText("Password too short");
        } else if (password != confirmPassword) {
            ok = false;
            ui->createAccount_createConfirmPassEdit->setStyleSheet("border: 1px solid red");
            ui->createAccount_passwordStatusLabel->setText("Passwords do not match");
        }

        // Age validation
        if (age < 18) {
            ok = false;
            ui->createAccount_createAgeEdit->setStyleSheet("border: 1px solid red");
        }

        // Sex validation
        if (sex.empty()) {
            ok = false;
            ui->createAccount_createSexEdit1->setStyleSheet("border: 1px solid red");
            ui->createAccount_createSexEdit2->setStyleSheet("border: 1px solid red");
            ui->createAccount_sexStatusLabel->setText("Please select a gender");
        }

        // Account type validation
        if (accountType == "savings" && months <= 0) {
            ok = false;
            ui->createAccount_createMonthsEdit->setStyleSheet("border: 1px solid red");
        } else if (accountType == "checking" && overdraftLimit < 0) {
            ok = false;
            ui->createAccount_createOverdraftEdit->setStyleSheet("border: 1px solid red");
        }

        // Duplicate check
        if (bankSystem.accountExists(username, email)) {
            ui->createAccount_accountNumberLabel->setText("Account Number: N/A");
            ui->createAccount_statusLabel->setText("Status: Account already exists. Please log in.");
            ui->stackedWidget->setCurrentIndex(2);
            return;
        }

        // Final validation and creation

         if (ok) {
                bankSystem.createAccount(username, age, sex, email, password, confirmPassword,
                                         accountType, months, overdraftLimit);

                long accountNumber = bankSystem.accounts.back()->getAccountNumber();
                bankSystem.saveAccounts();

                for (int i = 1; i < ui->stackedWidget->count(); ++i) {
                    ui->stackedWidget->setEnabled(true);
                }
                // Proceed to next page (main menu or dashboard)
                ui->createAccount_accountNumberLabel->setText(QString("Account Number: %1").arg(accountNumber));
                QString accountnumberstr=QString::number(accountNumber);
                ui->createAccount_statusLabel->setText("Status: account created successfully");
                QMessageBox::information(this, "Success", "Account created successfully!"+accountnumberstr);
                ui->stackedWidget->setCurrentIndex(3);
                return;

            // One and only error box for invalid input

         }else{
                 QMessageBox::warning(this, "Invalid Credentials", "Account creation failed. Check your details and try again.");
         ui->createAccount_accountNumberLabel->setText("Account Number: N/A");
         ui->createAccount_statusLabel->setText("Status: Failed to create account");
         return;
             }


}
void MainWindow::on_depositButton_clicked() {

    if (!currentAccount) return;
    double amount = ui->depositAmountSpin->value();
    if (amount <= 0) {
        QMessageBox::warning(this, "Error", "Invalid deposit amount");
        return;
    }
    currentAccount->deposit(amount);
    bankSystem.saveAccounts();
    ui->infoDisplay->setText(QString::fromStdString(currentAccount->getInfo()));
    QMessageBox::information(this, "Success", "Deposit successful!");
    ui->depositAmountSpin->setValue(0.0);
}

void MainWindow::on_withdrawButton_clicked() {

    if (!currentAccount) return;
    double amount = ui->withdrawAmountSpin->value();
    if (amount <= 0) {
        QMessageBox::warning(this, "Error", "Invalid withdrawal amount");
        return;
    }
    double previousBalance = currentAccount->getBalance();
    currentAccount->withdraw(amount);
    if (currentAccount->getBalance() != previousBalance) {
        bankSystem.saveAccounts();
        ui->infoDisplay->setText(QString::fromStdString(currentAccount->getInfo()));
        QMessageBox::information(this, "Success", "Withdrawal successful!");
        ui->withdrawAmountSpin->setValue(0.0);
    } else {
        QMessageBox::warning(this, "Error", "Withdrawal failed. Check amount or limits.");
    }
}

void MainWindow::on_mainMenu_accountInfoButton_clicked() {
       ui->stackedWidget->setCurrentWidget(ui->infoTab);
    if (!currentAccount) return;
    ui->infoDisplay->setText(QString::fromStdString(currentAccount->getInfo()));
}

void MainWindow::on_deleteAccountButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->deleteaccountpage);
    if (!currentAccount) return;
    bool ok;
    QString password = QInputDialog::getText(this, "Password", "Confirm Password:", QLineEdit::Password, "", &ok);
    if (!ok) return;
    if (bankSystem.deleteAccount(currentAccount->getAccountNumber(), password.toStdString())) {
        currentAccount = nullptr;
        ui->infoDisplay->clear();
        for (int i = 1; i < ui->stackedWidget->count(); ++i) {
            ui->stackedWidget->setEnabled(false);
        }
        ui->stackedWidget->setCurrentIndex(0); // Go to Welcome page
        QMessageBox::information(this, "Success", "Account deleted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete account.");
    }
}

void MainWindow::on_updateButton_clicked() {
    if (!currentAccount) return;
    bool ok;
    QString password = QInputDialog::getText(this, "Password", "Enter current password:", QLineEdit::Password, "", &ok);
    if (!ok) return;
    std::string newUsername = ui->NewNameEdit->text().toStdString();
    std::string newemail = ui->NewemailEdit->text().toStdString();
    std::string newPassword = ui->NewPasswordEdit->text().toStdString();
    if (bankSystem.updateAccount(currentAccount->getAccountNumber(), password.toStdString(), newUsername,newemail, newPassword)) {
        ui->infoDisplay->setText(QString::fromStdString(currentAccount->getInfo()));
        QMessageBox::information(this, "Success", "Account updated successfully!");
        ui->NewNameEdit->clear();
        ui->NewemailEdit->clear();
        ui->NewPasswordEdit->clear();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update account.");
    }
}

void MainWindow::on_applyInterestButton_clicked() {
    if (!currentAccount) return;
    if (auto* sav_acc = dynamic_cast<SavingAccount*>(currentAccount.get())) {
        sav_acc->applyInterest();
        bankSystem.saveAccounts();
        ui->infoDisplay->setText(QString::fromStdString(currentAccount->getInfo()));
        QMessageBox::information(this, "Success", "Interest applied successfully!");
    } else {
        QMessageBox::warning(this, "Error", "This feature is only available for savings accounts.");
    }
}

// Navigation implementations
void MainWindow::on_welcome_createAccountButton_clicked() {
       ui->stackedWidget->setCurrentWidget(ui->createAccountPage);    // Go to Main Menu page after login
    ui->createAccount_createNameEdit->setEnabled(true);
    ui->createAccount_createmailEdit->setEnabled(true);
    ui->createAccount_createPassEdit-> setEnabled(true);
    ui->createAccount_createConfirmPassEdit-> setEnabled(true);
    ui->createAccount_createAgeEdit-> setEnabled(true);
    ui->createAccount_createSexEdit1-> setEnabled(true);
    ui->createAccount_createSexEdit2-> setEnabled(true);
    ui->createAccount_createMonthsEdit->setEnabled(true);
    ui->createAccount_createOverdraftEdit-> setEnabled(true);
}

void MainWindow::on_welcome_loginButton_clicked() {
       ui->stackedWidget->setCurrentWidget(ui->loginPage); // Go to Main Menu page after login
    ui->login_accountNumberEdit->setEnabled(true);
    ui->login_passwordEdit->setEnabled(true);
}

void MainWindow::on_mainMenu_depositButton_clicked() {
      ui->stackedWidget->setCurrentWidget(ui->depositpage);
}

void MainWindow::on_mainMenu_withdrawButton_clicked() {
     ui->stackedWidget->setCurrentWidget(ui->withdrawpage);
}

void MainWindow::on_refreshInfoButton_clicked() {
    if (currentAccount) {
        ui->infoDisplay->setText(QString::fromStdString(currentAccount->getInfo()));
    }
}

void MainWindow::on_mainMenu_updateButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->updateaccountpage);
}

void MainWindow::on_mainMenu_deleteButton_clicked() {
      ui->stackedWidget->setCurrentWidget(ui->deleteaccountpage);
}

void MainWindow::on_mainMenu_exitButton_clicked() {
    QApplication::quit();
}

// Return to Main Menu implementations
void MainWindow::on_returntoMainMenuButton_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Return to mainMenuPage
}

void MainWindow::on_returntoMainMenuButton_2_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Return to mainMenuPage
}

void MainWindow::on_returntoMainMenuButton_3_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Return to mainMenuPage
}

void MainWindow::on_returntoMainMenuButton_4_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Return to mainMenuPage
}

void MainWindow::on_returntoMainMenuButton_5_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Return to mainMenuPage
}
void MainWindow::on_login_returnButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0); // Return to mainMenuPage
}
void MainWindow::on_createAccount_returnButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0); // Return to mainMenuPage
}
