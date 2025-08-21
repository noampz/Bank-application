#pragma once
//#include "vector" not needed because the include is in the customer file

#include "customer.hpp"
#include "time.hpp"

class Bank
{
    private:
    std::vector<Customer*> customers;
    float basic_interest;
    bool seeded;

    uint32_t makeNewCustomerID();
    void updateAllLoans(int amount_of_days);
    void updateAllSavingsAccounts(int amount_of_days);

    public:
    Bank();
    ~Bank();
    float getBasicInterest() {return basic_interest;}
    Customer* getCustomer(uint32_t customer_id);

    // retutns a positive interest rate if loan approved or -1 if loan is not approved
    double approveLoan(Customer *customer, double debt);
    //returns id of customer
    uint32_t createCustomer();
    //true if succeded false if not
    bool deleteCustomer(int customer_id);

    bool transfer(Customer &from_customer, uint32_t from_id_of_account, Customer &to_customer, uint32_t to_id_of_account, double amount);
    
    void changeDay(int amount_to_change);
};

