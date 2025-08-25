#pragma once
#include <iostream>
#include <cstdint>

class Loan
{
    private:
    float interest_rate;
    double debt;
    uint32_t id;

    public:
    Loan(float interest_rate, double debt, uint32_t id);
    uint32_t getID() const{return id;}
    
    // returns the amount of debt in loan
    double GetDebt() const {return debt;}
    float getInterestRate() const {return interest_rate;}
    
    //add interest amount to the debt
    void addInterest(int num_of_days);
    
    //gets the amount to pay the loan
    //returns the amount that is still in the loan
    //or -1 if amount is negative
    //or -2 if paid to much
    double payLoan(int amount);
};