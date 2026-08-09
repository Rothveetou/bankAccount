#include "../include/Bank.h"

#include <algorithm>
#include <iostream>
#include <utility>

void Bank::addAccount(
    std::unique_ptr<Account> account
) {
    accounts.push_back(
        std::move(account)
    );
}

Account* Bank::findAccount(
    int accountNumber
) {
    for (auto& account : accounts) {
        if (
            account->getAccountNumber()
            == accountNumber
        ) {
            return account.get();
        }
    }

    return nullptr;
}

const Account* Bank::findAccount(
    int accountNumber
) const {
    for (const auto& account : accounts) {
        if (
            account->getAccountNumber()
            == accountNumber
        ) {
            return account.get();
        }
    }

    return nullptr;
}

bool Bank::deleteAccount(
    int accountNumber
) {
    auto newEnd =
        std::remove_if(
            accounts.begin(),
            accounts.end(),

            [accountNumber](
                const std::unique_ptr<Account>& account
            ) {
                return account->getAccountNumber()
                    == accountNumber;
            }
        );

    if (newEnd == accounts.end()) {
        return false;
    }

    accounts.erase(
        newEnd,
        accounts.end()
    );

    return true;
}

bool Bank::transfer(
    int senderAccountNumber,
    int receiverAccountNumber,
    double amount
) {
    Account* sender =
        findAccount(senderAccountNumber);

    Account* receiver =
        findAccount(receiverAccountNumber);

    if (
        sender == nullptr ||
        receiver == nullptr
    ) {
        return false;
    }

    return sender->transfer(
        *receiver,
        amount
    );
}

void Bank::listAccounts() const {
    if (accounts.empty()) {
        std::cout
            << "No accounts found.\n";

        return;
    }

    std::cout
        << "\n=====================================\n";

    std::cout
        << "             ALL ACCOUNTS\n";

    std::cout
        << "=====================================\n";

    for (const auto& account : accounts) {

        std::cout
            << "Account #: "
            << account->getAccountNumber()
            << '\n';

        std::cout
            << "Holder: "
            << account->getAccountHolderName()
            << '\n';

        std::cout
            << "Type: "
            << account->getAccountType()
            << '\n';

        std::cout
            << "Balance: $"
            << account->getBalance()
            << '\n';

        std::cout
            << "-------------------------------------\n";
    }
}

std::vector<std::unique_ptr<Account>>&
Bank::getAccounts() {
    return accounts;
}

const std::vector<std::unique_ptr<Account>>&
Bank::getAccounts() const {
    return accounts;
}