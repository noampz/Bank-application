#pragma once 
#include "account.hpp"

class CheckingAccount : public Account 
{
    public:
    CheckingAccount(uint32_t id);

    //does nothing
    void updateBalance(int num_of_days) override {}
    const float getInterestRate() override  {return 0;}
};