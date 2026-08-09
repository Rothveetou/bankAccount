#ifndef CHECKING_ACCOUNT_H
#define CHECKING_ACCOUNT_H

#include "Account.h"

class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(
        const std::string& accountHolderName,
        double initialBalance,
        double overdraftLimit = 0.0
    );

    bool withdraw(double amount) override;

    std::string getAccountType() const override;

    void accountDetail() const override;
};

#endif