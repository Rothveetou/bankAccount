#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

#include "Transaction.h"

class Account {
protected:
    std::string accountHolderName;
    int accountNumber;
    double balance;

    static int accountNumberCounter;

    std::vector<Transaction> transactions;

public:
    Account(
        const std::string& accountHolderName,
        double initialBalance
    );

    virtual ~Account() = default;

    virtual void deposit(double amount);

    virtual bool withdraw(double amount);

    bool transfer(
        Account& destination,
        double amount
    );

    double getBalance() const;

    int getAccountNumber() const;

    std::string getAccountHolderName() const;

    virtual std::string getAccountType() const = 0;

    virtual void accountDetail() const;

    void displayTransactionHistory() const;
};

#endif