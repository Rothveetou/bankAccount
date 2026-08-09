#include "../include/Bank.h"
#include "../include/CheckingAccount.h"
#include "../include/SavingsAccount.h"

#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <iomanip>

void clearInput() {
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

void createAccount(Bank& bank) {
    std::string name;
    int accountType;
    double initialBalance;

    clearInput();

    std::cout << "\n========== CREATE ACCOUNT ==========\n";

    std::cout << "Enter account holder name: ";
    std::getline(std::cin, name);

    if (name.empty()) {
        std::cout << "Account holder name cannot be empty.\n";
        return;
    }

    std::cout << "\nChoose Account Type\n";
    std::cout << "1. Checking Account\n";
    std::cout << "2. Savings Account\n";
    std::cout << "Choice: ";

    if (!(std::cin >> accountType)) {
        std::cout << "Invalid account type.\n";
        clearInput();
        return;
    }

    std::cout << "Initial Deposit: $";

    if (!(std::cin >> initialBalance)) {
        std::cout << "Invalid deposit amount.\n";
        clearInput();
        return;
    }

    if (initialBalance < 0) {
        std::cout << "Initial balance cannot be negative.\n";
        return;
    }

    if (accountType == 1) {
        double overdraftLimit;

        std::cout << "Overdraft Limit: $";

        if (!(std::cin >> overdraftLimit)) {
            std::cout << "Invalid overdraft limit.\n";
            clearInput();
            return;
        }

        auto account =
            std::make_unique<CheckingAccount>(
                name,
                initialBalance,
                overdraftLimit
            );

        // Keep temporary pointer before moving ownership to Bank
        Account* createdAccount = account.get();

        bank.addAccount(
            std::move(account)
        );

        std::cout << "\n=====================================\n";
        std::cout << "      ACCOUNT CREATED SUCCESSFULLY\n";
        std::cout << "=====================================\n";

        createdAccount->accountDetail();
    }

    else if (accountType == 2) {
        double interestRate;

        std::cout << "Annual Interest Rate (%): ";

        if (!(std::cin >> interestRate)) {
            std::cout << "Invalid interest rate.\n";
            clearInput();
            return;
        }

        if (interestRate < 0) {
            std::cout << "Interest rate cannot be negative.\n";
            return;
        }

        interestRate /= 100.0;

        auto account =
            std::make_unique<SavingsAccount>(
                name,
                initialBalance,
                interestRate
            );

        Account* createdAccount = account.get();

        bank.addAccount(
            std::move(account)
        );

        std::cout << "\n=====================================\n";
        std::cout << "      ACCOUNT CREATED SUCCESSFULLY\n";
        std::cout << "=====================================\n";

        createdAccount->accountDetail();
    }

    else {
        std::cout << "Invalid account type. Choose 1 or 2.\n";
    }
}

void viewAccount(Bank& bank) {
    int accountNumber;

    while (true) {
        std::cout
            << "\nEnter account number "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Please enter a numeric account number.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout << "Search cancelled.\n";
            return;
        }

        Account* account =
            bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " does not exist. Try again.\n";

            continue;
        }

        std::cout << "\n=====================================\n";
        std::cout << "            ACCOUNT DETAILS\n";
        std::cout << "=====================================\n";

        account->accountDetail();

        return;
    }
}

void depositMoney(Bank& bank) {
    int accountNumber;
    double amount;

    Account* account = nullptr;

    // Keep asking until valid account is found
    while (account == nullptr) {
        std::cout
            << "\nEnter account number "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout
                << "Deposit cancelled.\n";

            return;
        }

        account =
            bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " was not found. Try again.\n";
        }
    }

    // Keep asking until deposit amount is valid
    while (true) {
        std::cout
            << "Enter deposit amount "
            << "(0 to cancel): $";

        if (!(std::cin >> amount)) {
            std::cout
                << "Invalid input. "
                << "Please enter a number.\n";

            clearInput();
            continue;
        }

        if (amount == 0) {
            std::cout
                << "Deposit cancelled.\n";

            return;
        }

        if (amount < 0) {
            std::cout
                << "Deposit must be greater than $0.\n";

            continue;
        }

        try {
            account->deposit(amount);
        std::cout << "=====================================\n";
            std::cout
                << "\n=====================================\n"
                << "\nDeposit successful!\n"
                << "\n=====================================\n"
                << "Account: #"
                << account->getAccountNumber()
                << '\n'
                << "Holder: "
                << account->getAccountHolderName()
                << '\n'
                << "Deposited: $"
                << amount
                << '\n'
                << "New Balance: $"
                << account->getBalance()
                << '\n';

            break;
        }
        catch (const std::exception& error) {
            std::cout
                << "Deposit failed: "
                << error.what()
                << '\n';
        }
    }
}

void withdrawMoney(Bank& bank) {
    Account* account = nullptr;
    int accountNumber;
    double amount;

    while (account == nullptr) {
        std::cout
            << "\nEnter account number "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout << "Withdrawal cancelled.\n";
            return;
        }

        account = bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " not found. Try again.\n";
        }
    }

    while (true) {
        std::cout
            << "Enter withdrawal amount "
            << "(0 to cancel): $";

        if (!(std::cin >> amount)) {
            std::cout
                << "Invalid input. "
                << "Please enter a numeric amount.\n";

            clearInput();
            continue;
        }

        if (amount == 0) {
            std::cout << "Withdrawal cancelled.\n";
            return;
        }

        if (amount < 0) {
            std::cout
                << "Withdrawal amount must be greater than $0.\n";

            continue;
        }

        double oldBalance = account->getBalance();

        if (!account->withdraw(amount)) {
            std::cout
                << "Withdrawal failed. "
                << "Please enter another amount.\n";

            continue;
        }

        std::cout << "\n=====================================\n";
        std::cout << "       WITHDRAWAL SUCCESSFUL\n";
        std::cout << "=====================================\n";

        std::cout
            << "Account #: "
            << account->getAccountNumber()
            << '\n';

        std::cout
            << "Holder: "
            << account->getAccountHolderName()
            << '\n';

        std::cout
            << "Previous Balance: $"
            << std::fixed
            << std::setprecision(2)
            << oldBalance
            << '\n';

        std::cout
            << "Withdrawn: $"
            << amount
            << '\n';

        std::cout
            << "New Balance: $"
            << account->getBalance()
            << '\n';

        return;
    }
}
void transferMoney(Bank& bank) {
    Account* sender = nullptr;
    Account* receiver = nullptr;

    int senderNumber;
    int receiverNumber;
    double amount;

    while (sender == nullptr) {
        std::cout
            << "\nEnter sender account number "
            << "(0 to cancel): ";

        if (!(std::cin >> senderNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (senderNumber == 0) {
            std::cout << "Transfer cancelled.\n";
            return;
        }

        sender = bank.findAccount(senderNumber);

        if (sender == nullptr) {
            std::cout
                << "Sender account #"
                << senderNumber
                << " not found. Try again.\n";
        }
    }

    while (receiver == nullptr) {
        std::cout
            << "Enter receiver account number "
            << "(0 to cancel): ";

        if (!(std::cin >> receiverNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (receiverNumber == 0) {
            std::cout << "Transfer cancelled.\n";
            return;
        }

        if (receiverNumber == senderNumber) {
            std::cout
                << "You cannot transfer money "
                << "to the same account.\n";

            continue;
        }

        receiver = bank.findAccount(receiverNumber);

        if (receiver == nullptr) {
            std::cout
                << "Receiver account #"
                << receiverNumber
                << " not found. Try again.\n";
        }
    }

    while (true) {
        std::cout
            << "Enter transfer amount "
            << "(0 to cancel): $";

        if (!(std::cin >> amount)) {
            std::cout
                << "Invalid input. "
                << "Please enter a numeric amount.\n";

            clearInput();
            continue;
        }

        if (amount == 0) {
            std::cout << "Transfer cancelled.\n";
            return;
        }

        if (amount < 0) {
            std::cout
                << "Transfer amount must be greater than $0.\n";

            continue;
        }

        double senderOldBalance =
            sender->getBalance();

        double receiverOldBalance =
            receiver->getBalance();

        if (
            !bank.transfer(
                senderNumber,
                receiverNumber,
                amount
            )
        ) {
            std::cout
                << "Transfer failed. "
                << "Check available funds and try again.\n";

            continue;
        }

        std::cout << "\n=====================================\n";
        std::cout << "         TRANSFER SUCCESSFUL\n";
        std::cout << "=====================================\n";

        std::cout
            << "From: "
            << sender->getAccountHolderName()
            << " (#"
            << sender->getAccountNumber()
            << ")\n";

        std::cout
            << "To: "
            << receiver->getAccountHolderName()
            << " (#"
            << receiver->getAccountNumber()
            << ")\n";

        std::cout
            << "Amount: $"
            << std::fixed
            << std::setprecision(2)
            << amount
            << '\n';

        std::cout
            << "\nSender Balance\n"
            << "Before: $"
            << senderOldBalance
            << '\n'
            << "After:  $"
            << sender->getBalance()
            << '\n';

        std::cout
            << "\nReceiver Balance\n"
            << "Before: $"
            << receiverOldBalance
            << '\n'
            << "After:  $"
            << receiver->getBalance()
            << '\n';

        return;
    }
}

void viewBalance(Bank& bank) {
    int accountNumber;

    while (true) {
        std::cout
            << "\nEnter account number "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout << "Balance inquiry cancelled.\n";
            return;
        }

        Account* account =
            bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " not found. Try again.\n";

            continue;
        }

        std::cout << "\n=====================================\n";
        std::cout << "             BALANCE\n";
        std::cout << "=====================================\n";

        std::cout
            << "Account Holder: "
            << account->getAccountHolderName()
            << '\n';

        std::cout
            << "Account Number: "
            << account->getAccountNumber()
            << '\n';

        std::cout
            << "Account Type: "
            << account->getAccountType()
            << '\n';

        std::cout
            << "Current Balance: $"
            << std::fixed
            << std::setprecision(2)
            << account->getBalance()
            << '\n';

        return;
    }
}

void viewTransactions(Bank& bank) {
    int accountNumber;

    while (true) {
        std::cout
            << "\nEnter account number "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout
                << "Transaction history cancelled.\n";

            return;
        }

        Account* account =
            bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " not found. Try again.\n";

            continue;
        }

        std::cout << "\n=====================================\n";
        std::cout << "        TRANSACTION HISTORY\n";
        std::cout << "=====================================\n";

        std::cout
            << "Holder: "
            << account->getAccountHolderName()
            << '\n';

        std::cout
            << "Account #: "
            << account->getAccountNumber()
            << '\n';

        account->displayTransactionHistory();

        return;
    }
}

void applySavingsInterest(Bank& bank) {
    int accountNumber;

    while (true) {
        std::cout
            << "\nEnter savings account number "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout
                << "Interest operation cancelled.\n";

            return;
        }

        Account* account =
            bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " not found. Try again.\n";

            continue;
        }

        SavingsAccount* savings =
            dynamic_cast<SavingsAccount*>(account);

        if (savings == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " is a "
                << account->getAccountType()
                << " account.\n";

            std::cout
                << "Interest can only be applied "
                << "to a Savings account.\n";

            continue;
        }

        double oldBalance =
            savings->getBalance();

        savings->applyInterest();

        std::cout << "\n=====================================\n";
        std::cout << "       INTEREST APPLIED\n";
        std::cout << "=====================================\n";

        std::cout
            << "Holder: "
            << savings->getAccountHolderName()
            << '\n';

        std::cout
            << "Account #: "
            << savings->getAccountNumber()
            << '\n';

        std::cout
            << "Previous Balance: $"
            << std::fixed
            << std::setprecision(2)
            << oldBalance
            << '\n';

        std::cout
            << "Interest Earned: $"
            << savings->getBalance() - oldBalance
            << '\n';

        std::cout
            << "New Balance: $"
            << savings->getBalance()
            << '\n';

        return;
    }
}

void deleteAccount(Bank& bank) {
    int accountNumber;

    while (true) {
        std::cout
            << "\nEnter account number to delete "
            << "(0 to cancel): ";

        if (!(std::cin >> accountNumber)) {
            std::cout
                << "Invalid input. "
                << "Account number must be numeric.\n";

            clearInput();
            continue;
        }

        if (accountNumber == 0) {
            std::cout
                << "Account deletion cancelled.\n";

            return;
        }

        Account* account =
            bank.findAccount(accountNumber);

        if (account == nullptr) {
            std::cout
                << "Account #"
                << accountNumber
                << " not found. Try again.\n";

            continue;
        }

        std::cout << "\n=====================================\n";
        std::cout << "          DELETE ACCOUNT\n";
        std::cout << "=====================================\n";

        std::cout
            << "Holder: "
            << account->getAccountHolderName()
            << '\n';

        std::cout
            << "Account #: "
            << account->getAccountNumber()
            << '\n';

        std::cout
            << "Type: "
            << account->getAccountType()
            << '\n';

        std::cout
            << "Balance: $"
            << std::fixed
            << std::setprecision(2)
            << account->getBalance()
            << '\n';

        char confirmation;

        std::cout
            << "\nAre you sure you want to delete "
            << "this account? (y/n): ";

        std::cin >> confirmation;

        if (
            confirmation != 'y' &&
            confirmation != 'Y'
        ) {
            std::cout
                << "Account deletion cancelled.\n";

            return;
        }

        if (bank.deleteAccount(accountNumber)) {
            std::cout << "\n=====================================\n";
            std::cout << "       ACCOUNT DELETED\n";
            std::cout << "=====================================\n";

            std::cout
                << "Account #"
                << accountNumber
                << " was deleted successfully.\n";
        }
        else {
            std::cout
                << "Unable to delete account.\n";
        }

        return;
    }
}

int main() {
    Bank bank;

    int choice = 0;

    while (choice != 12) {
        std::cout
            << "\n=====================================\n"
            << "           V2 BANK SYSTEM\n"
            << "=====================================\n"
            << "1. Create Account\n"
            << "2. View Account Details\n"
            << "3. Deposit Money\n"
            << "4. Withdraw Money\n"
            << "5. Transfer Money\n"
            << "6. View Balance\n"
            << "7. Transaction History\n"
            << "8. Apply Savings Interest\n"
            << "9. Search Account\n"
            << "10. List All Accounts\n"
            << "11. Delete Account\n"
            << "12. Exit\n"
            << "\nEnter choice: ";

        if (!(std::cin >> choice)) {
            std::cout
                << "Invalid input. Enter a number from 1-12.\n";

            clearInput();
            choice = 0;

            continue;
        }

        try {
            switch (choice) {
                case 1:
                    createAccount(bank);
                    break;

                case 2:
                    viewAccount(bank);
                    break;

                case 3:
                    depositMoney(bank);
                    break;

                case 4:
                    withdrawMoney(bank);
                    break;

                case 5:
                    transferMoney(bank);
                    break;

                case 6:
                    viewBalance(bank);
                    break;

                case 7:
                    viewTransactions(bank);
                    break;

                case 8:
                    applySavingsInterest(bank);
                    break;

                case 9:
                    viewAccount(bank);
                    break;

                case 10:
                    bank.listAccounts();
                    break;

                case 11:
                    deleteAccount(bank);
                    break;

                case 12:
                    std::cout
                        << "\nThank you for using V2 Bank!\n";
                    break;

                default:
                    std::cout
                        << "Invalid option. Choose 1-12.\n";
            }
        }
        catch (const std::exception& error) {
            std::cerr
                << "Error: "
                << error.what()
                << '\n';
        }
    }

    return 0;
}