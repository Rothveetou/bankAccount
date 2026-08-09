#include "../include/CheckingAccount.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

CheckingAccount::CheckingAccount(
    const std::string& accountHolderName,
    double initialBalance,
    double overdraftLimit
)
    : Account(accountHolderName, initialBalance),
      overdraftLimit(overdraftLimit) {

    if (overdraftLimit < 0) {
        throw std::invalid_argument(
            "Overdraft limit cannot be negative."
        );
    }
}

bool CheckingAccount::withdraw(double amount) {
    if (amount <= 0) {
        std::cerr
            << "Withdrawal amount must be greater than zero.\n";

        return false;
    }

    if (amount > balance + overdraftLimit) {
        std::cerr
            << "Withdrawal exceeds available funds and overdraft limit.\n";

        return false;
    }

    balance -= amount;

    transactions.emplace_back(
        TransactionType::Withdrawal,
        amount,
        "Checking account withdrawal"
    );

    return true;
}

std::string CheckingAccount::getAccountType() const {
    return "Checking";
}

void CheckingAccount::accountDetail() const {
    std::cout << "\n===== CHECKING ACCOUNT =====\n";

    Account::accountDetail();

    std::cout
        << "Overdraft Limit: $"
        << std::fixed
        << std::setprecision(2)
        << overdraftLimit
        << '\n';
}