#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

enum class TransactionType {
    Deposit,
    Withdrawal,
    Transfer,
    Interest
};

class Transaction {
private:
    TransactionType type;
    double amount;
    std::string description;

public:
    Transaction(
        TransactionType type,
        double amount,
        const std::string& description
    );

    void display() const;
};

#endif