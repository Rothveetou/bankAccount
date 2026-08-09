#include "../include/CheckingAccount.h"
#include "../include/SavingsAccount.h"

#include <iostream>
#include <memory>
#include <string>
#include <limits>

int main() {
    std::cout << "=====================================\n";
    std::cout << "       WELCOME TO V2 BANK\n";
    std::cout << "=====================================\n\n";

    std::string name;
    int accountChoice;
    double initialBalance;

    std::cout << "Enter account holder name: ";
    std::getline(std::cin, name);

    std::cout << "\nChoose account type:\n";
    std::cout << "1. Checking Account\n";
    std::cout << "2. Savings Account\n";
    std::cout << "Enter choice: ";
    std::cin >> accountChoice;

    std::cout << "Enter initial deposit: $";
    std::cin >> initialBalance;

    try {
        std::unique_ptr<Account> account;

        if (accountChoice == 1) {
            double overdraftLimit;

            std::cout << "Enter overdraft limit: $";
            std::cin >> overdraftLimit;

            account = std::make_unique<CheckingAccount>(
                name,
                initialBalance,
                overdraftLimit
            );
        }
        else if (accountChoice == 2) {
            double interestRate;

            std::cout << "Enter annual interest rate (%): ";
            std::cin >> interestRate;

            interestRate /= 100.0;

            account = std::make_unique<SavingsAccount>(
                name,
                initialBalance,
                interestRate
            );
        }
        else {
            std::cout << "Invalid account type.\n";
            return 1;
        }

        std::cout << "\nAccount created successfully!\n";

        account->accountDetail();

        int choice = 0;

        while (choice != 6) {
            std::cout << "\n=====================================\n";
            std::cout << "           BANKING MENU\n";
            std::cout << "=====================================\n";

            std::cout << "1. View Account Details\n";
            std::cout << "2. Deposit Money\n";
            std::cout << "3. Withdraw Money\n";
            std::cout << "4. View Balance\n";
            std::cout << "5. Transaction History\n";
            std::cout << "6. Exit\n";

            std::cout << "\nEnter choice: ";
            std::cin >> choice;

            switch (choice) {

                case 1: {
                    account->accountDetail();
                    break;
                }

                case 2: {
                    double amount;

                    std::cout << "Enter deposit amount: $";
                    std::cin >> amount;

                    try {
                        account->deposit(amount);

                        std::cout
                            << "Deposit successful.\n";
                    }
                    catch (const std::exception& error) {
                        std::cerr
                            << "Error: "
                            << error.what()
                            << '\n';
                    }

                    break;
                }

                case 3: {
                    double amount;

                    std::cout << "Enter withdrawal amount: $";
                    std::cin >> amount;

                    if (account->withdraw(amount)) {
                        std::cout
                            << "Withdrawal successful.\n";
                    }
                    else {
                        std::cout
                            << "Withdrawal failed.\n";
                    }

                    break;
                }

                case 4: {
                    std::cout
                        << "Current Balance: $"
                        << account->getBalance()
                        << '\n';

                    break;
                }

                case 5: {
                    account->displayTransactionHistory();
                    break;
                }

                case 6: {
                    std::cout
                        << "\nThank you for using V2 Bank!\n";

                    break;
                }

                default: {
                    std::cout
                        << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
    catch (const std::exception& error) {
        std::cerr
            << "Error creating account: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}