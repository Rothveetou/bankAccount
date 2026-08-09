#ifndef BANK_H
#define BANK_H

#include <memory>
#include <vector>

#include "Account.h"

class Bank {
private:
    std::vector<std::unique_ptr<Account>> accounts;

public:
    Bank() = default;

    void addAccount(std::unique_ptr<Account> account);

    Account* findAccount(int accountNumber);

    const Account* findAccount(int accountNumber) const;

    bool deleteAccount(int accountNumber);

    bool transfer(
        int senderAccountNumber,
        int receiverAccountNumber,
        double amount
    );

    void listAccounts() const;

    std::vector<std::unique_ptr<Account>>& getAccounts();

    const std::vector<std::unique_ptr<Account>>& getAccounts() const;
};

#endif