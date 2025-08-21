#include "savingsAccount.hpp"

SavingsAccount::SavingsAccount(float interest_percent, uint32_t id)
: Account(id), interest_percent(interest_percent){}

void SavingsAccount::updateBalance(int num_of_days)
{
    for (int i = 0; i < num_of_days; i++)
        balance *= 1 + (interest_percent * 0.01);
}