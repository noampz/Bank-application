#include "account.hpp"

Account::Account(uint32_t id) : balance (1000.0), id(id) {}

double Account::deposit(double amount)
{
    if (amount < 0)
    {
        std::cout << "you cannot deposit negative number.\n";
        return -1;
    }
    balance += amount;
    return balance;
}

double Account::withdraw(double amount)
{
    if (balance - amount < 0)
    {
        std::cout << "Insufficient funds.\n";
        return -1;
    }

    if (amount < 0)
    {
        std::cout << "you cannot withdraw negative number.\n";
        return -1;
    }

    balance -= amount;
    return balance;
}