#include "../include/Transaction.h"

#include <iomanip>
#include <iostream>

Transaction::Transaction(
    TransactionType type,
    double amount,
    const std::string& description
)
    : type(type),
      amount(amount),
      description(description) {
}

void Transaction::display() const {
    std::string typeName;

    switch (type) {
        case TransactionType::Deposit:
            typeName = "Deposit";
            break;

        case TransactionType::Withdrawal:
            typeName = "Withdrawal";
            break;

        case TransactionType::Transfer:
            typeName = "Transfer";
            break;

        case TransactionType::Interest:
            typeName = "Interest";
            break;
    }

    std::cout << typeName
              << " | $"
              << std::fixed
              << std::setprecision(2)
              << amount
              << " | "
              << description
              << '\n';
}