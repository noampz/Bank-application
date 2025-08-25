#pragma once 
#include "account.hpp"
#include <cstdint>

class SavingsAccount : public Account
{
    private:
    float interest_percent;

    public:
    SavingsAccount(float interest_percent, uint32_t id);
    void updateBalance(int num_of_days) override;
    const float getInterestRate() override  {return interest_percent;}
};