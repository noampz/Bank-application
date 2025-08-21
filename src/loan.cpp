#include "loan.hpp"

Loan::Loan(float interest_rate, double debt, uint32_t id) :
interest_rate(interest_rate), debt(debt), id(id) {}

void Loan::addInterest(int num_of_days)
{
    for (int i = 0; i < num_of_days; i++)
        debt *= 1 + (0.01 * interest_rate);
}

double Loan::payLoan(int amount)
{
    if (amount <= 0)
    {
        std::cout << "you can't pay a negative amount.\n";
        return -1;
    }

    if ((debt -= amount) < 0)
    {
        return -2;
    }
    
    return debt;
}