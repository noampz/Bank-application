#pragma once
#include <iostream>
#include <cstdint>

class Account
{
    protected:
    double balance;
    //this will be an ID for a spesifc account in a customer and not a global ID
    uint32_t id;

    public:
    Account(uint32_t id);
    // returns current amount of money in account after the added deposit or -1 if failed
    virtual double deposit(double amount);
    // returns current amount of money in account after the withdraw or -1 if failed
    virtual double withdraw(double amount);

    virtual void updateBalance(int num_of_days) = 0;
    
    double getBalance() const { return balance; }
    virtual const float getInterestRate() = 0;
    //savings accounts are % 2 == 0
    // checking accounts are % 2 == 1;
    uint32_t getID() const { return id; }
};