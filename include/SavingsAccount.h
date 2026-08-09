#ifndef SAVINGS_ACCOUNT_H
#define SAVINGS_ACCOUNT_H

#include "Account.h"

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(
        const std::string& accountHolderName,
        double initialBalance,
        double interestRate
    );

    void applyInterest();

    std::string getAccountType() const override;

    void accountDetail() const override;
};

#endif