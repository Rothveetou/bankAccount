#include "../include/SavingsAccount.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

SavingsAccount::SavingsAccount(
    const std::string& accountHolderName,
    double initialBalance,
    double interestRate
)
    : Account(accountHolderName, initialBalance),
      interestRate(interestRate) {

    if (interestRate < 0) {
        throw std::invalid_argument(
            "Interest rate cannot be negative."
        );
    }
}

void SavingsAccount::applyInterest() {
    double interestAmount =
        balance * interestRate;

    balance += interestAmount;

    transactions.emplace_back(
        TransactionType::Interest,
        interestAmount,
        "Interest payment applied"
    );
}

std::string SavingsAccount::getAccountType() const {
    return "Savings";
}

void SavingsAccount::accountDetail() const {
    std::cout << "\n===== SAVINGS ACCOUNT =====\n";

    Account::accountDetail();

    std::cout
        << "Interest Rate: "
        << std::fixed
        << std::setprecision(2)
        << interestRate * 100
        << "%\n";
}