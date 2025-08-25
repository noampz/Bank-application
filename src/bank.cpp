#include "bank.hpp"
#include <cstdlib>
#include <functional>

uint32_t Bank::next_customer_id = 1;

Bank::Bank() : basic_interest(3.0) {}

Bank::~Bank()
{
    for (Customer* customer : customers)
    {
        delete(customer);
    }
}

double Bank::approveLoan(Customer *customer, double debt)
{
    double total_balance = customer->totalBalance();
    if (total_balance / debt <= 0.75)
    {
        std::cout << "loan is not approved\n";
        return -1;
    }
    
    //iterest calculation
    return (10 * debt / (total_balance == 0 ? 1 : total_balance)) + basic_interest;
}

//TODO:
// change this thingy
bool Bank::isPasswordGood(std::string password)
{
    return true;
}

bool Bank::isFullNameGood(std::string full_name)
{
    if (full_name.length() < 3)
    {
        std::cout << "your full name cannot be less than 3 characters\n";
        return false;
    }
    return true;
}

uint32_t Bank::createCustomer(std::string full_name, std::string password)
{
    if (!isPasswordGood(password) || !isFullNameGood(full_name))
        return -1;

    uint32_t id = makeNewCustomerID();

    customers.push_back(new Customer(id, full_name, password,*this));
    return id;
    
}

uint32_t Bank::makeNewCustomerID()
{
    return next_customer_id++;
}

bool Bank::deleteCustomer(int customer_id)
{
    int length = customers.size();

    bool found = false;

    std::vector<Customer*> customers_copy;
    //reserve the size of the vector needed
    customers_copy.reserve(length - 1);

    for (int i = length - 1; i >= 0; i--)
    {
        if (customers.at(i)->getID() == customer_id)
        {
            delete customers.at(i);
            found = true;
        }
        else
        {
            customers_copy.push_back(customers.back());
        }
        customers.pop_back();
    }

    if (found == false)
        std::cout << "Customer with the ID: " << customer_id << " was not found";

    customers = customers_copy;
    return found;
}

bool Bank::transfer(Customer &from_customer, Customer &to_customer, double amount)
{
    Account *from_account = from_customer.getMainAccount();
    if (from_account == nullptr)
       return false;

    Account *to_account = to_customer.getMainAccount();
    if (to_account == nullptr)
        return false;

    if(from_account->withdraw(amount) >= 0)
    {
        to_account->deposit(amount);
        return true;
    }
        
    return false;
}

void Bank::updateAllLoans(int amount_of_days)
{
    int length = customers.size();
    for (int i = 0; i < length; i++)
    {
        customers.at(i)->updateLoans(amount_of_days);
    }
}

void Bank::updateAllSavingsAccounts(int amount_of_days)
{
    int length = customers.size();
    for (int i = 0; i < length; i++)
    {
        customers.at(i)->updateSavingsAccounts(amount_of_days);
    }
}

void Bank::changeDay(int amount_to_change)
{
    if (amount_to_change >= 0)
    {
        Time::day += amount_to_change;
        updateAllLoans(amount_to_change);
        updateAllSavingsAccounts(amount_to_change);
        return;
    }
    std::cout << "you cant reverse the time.\n";
}

Customer* Bank::getCustomer(uint32_t customer_id)
{
    for (Customer* customer : customers)
    {
        if (customer->getID() == customer_id)
            return customer;
    }

    std::cout << "no customer found with this ID: " << customer_id << "\n";
    return nullptr;
}

Customer* Bank::getCustomer(std::string full_name, std::string password)
{
    std::hash<std::string> hasher;
    size_t hashed_password = hasher(password);

    bool name_found = false;

    for (Customer* customer : customers)
    {
        if (customer->getFullName() == full_name)
        {
            if (customer->getHashedPassword() == hashed_password)
            {
                return customer;
            }
            name_found = true;
        }
            
    }
    if (!name_found)
        std::cout << "No customer found with the name: " << full_name << "\n";
    else
        std::cout << "Either the password or the full name is incorrect.\n";
    return nullptr;
}
   