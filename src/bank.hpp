#pragma once
//#include "vector" not needed because the include is in the customer file

#include "customer.hpp"
#include "time.hpp"

class Bank
{
    private:
    std::vector<Customer*> customers;
    float basic_interest;

    uint32_t makeNewCustomerID();
    void updateAllLoans(int amount_of_days);
    void updateAllSavingsAccounts(int amount_of_days);
    bool isPasswordGood(std::string password);
    bool isFullNameGood(std::string full_name);

    public:
    Bank();
    ~Bank();
    float getBasicInterest() {return basic_interest;}
    Customer* getCustomer(uint32_t customer_id);
    Customer* getCustomer(std::string full_name, std::string password);

    // retutns a positive interest rate if loan approved or -1 if loan is not approved
    double approveLoan(Customer *customer, double debt);
    //returns id of customer
    uint32_t createCustomer(std::string full_name, std::string password);
    //true if succeded false if not
    bool deleteCustomer(int customer_id);

    //transfer between both of the main accounts
    bool transfer(Customer &from_customer, Customer &to_customer, double amount);
    
    void changeDay(int amount_to_change);
};

