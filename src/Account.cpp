#include "../include/Account.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

int Account::accountNumberCounter = 1000000;

Account::Account(
    const std::string& accountHolderName,
    double initialBalance
)
    : accountHolderName(accountHolderName),
      balance(initialBalance) {

    if (accountHolderName.empty()) {
        throw std::invalid_argument(
            "Account holder name cannot be empty."
        );
    }

    if (initialBalance < 0) {
        throw std::invalid_argument(
            "Initial balance cannot be negative."
        );
    }

    accountNumber = ++accountNumberCounter;
}

void Account::deposit(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument(
            "Deposit amount must be greater than zero."
        );
    }

    balance += amount;

    transactions.emplace_back(
        TransactionType::Deposit,
        amount,
        "Deposit completed"
    );
}

bool Account::withdraw(double amount) {
    if (amount <= 0) {
        std::cerr
            << "Withdrawal amount must be greater than zero.\n";

        return false;
    }

    if (amount > balance) {
        std::cerr
            << "Insufficient funds.\n";

        return false;
    }

    balance -= amount;

    transactions.emplace_back(
        TransactionType::Withdrawal,
        amount,
        "Withdrawal completed"
    );

    return true;
}

bool Account::transfer(
    Account& destination,
    double amount
) {
    if (&destination == this) {
        std::cerr
            << "Cannot transfer money to the same account.\n";

        return false;
    }

    if (amount <= 0) {
        std::cerr
            << "Transfer amount must be greater than zero.\n";

        return false;
    }

    if (!withdraw(amount)) {
        return false;
    }

    destination.balance += amount;

    transactions.emplace_back(
        TransactionType::Transfer,
        amount,
        "Transfer sent to account #" +
            std::to_string(destination.accountNumber)
    );

    destination.transactions.emplace_back(
        TransactionType::Transfer,
        amount,
        "Transfer received from account #" +
            std::to_string(accountNumber)
    );

    return true;
}

double Account::getBalance() const {
    return balance;
}

int Account::getAccountNumber() const {
    return accountNumber;
}

std::string Account::getAccountHolderName() const {
    return accountHolderName;
}

void Account::accountDetail() const {
    std::cout << "\n-----------------------------\n";

    std::cout
        << "Account Holder: "
        << accountHolderName
        << '\n';

    std::cout
        << "Account Number: "
        << accountNumber
        << '\n';

    std::cout
        << "Account Type: "
        << getAccountType()
        << '\n';

    std::cout
        << "Balance: $"
        << std::fixed
        << std::setprecision(2)
        << balance
        << '\n';

    std::cout << "-----------------------------\n";
}

void Account::displayTransactionHistory() const {
    std::cout
        << "\nTransaction History for Account #"
        << accountNumber
        << '\n';

    if (transactions.empty()) {
        std::cout
            << "No transactions found.\n";

        return;
    }

    for (const Transaction& transaction : transactions) {
        transaction.display();
    }
}
