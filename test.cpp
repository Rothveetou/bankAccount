#include "include/CheckingAccount.h"
#include "include/SavingsAccount.h"

#include <type_traits>

static_assert(
    std::is_base_of<Account, CheckingAccount>::value,
    "CheckingAccount is not derived from Account"
);

static_assert(
    std::is_base_of<Account, SavingsAccount>::value,
    "SavingsAccount is not derived from Account"
);

static_assert(
    std::is_convertible<CheckingAccount*, Account*>::value,
    "CheckingAccount does not publicly inherit Account"
);

static_assert(
    std::is_convertible<SavingsAccount*, Account*>::value,
    "SavingsAccount does not publicly inherit Account"
);

int main() {
    return 0;
}