#include "ui.hpp"
#include <limits>
#undef max
UI::UI(Bank *bank, Customer **customer) : bank(bank), customer(customer) {}


void UI::clearScreen() 
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

bool UI::checkIfInputFailed()
{
    if (std::cin.fail())
    {
        std::cout << "That's not a valid number! Please try again.\n";
        // Clear the error flag
        std::cin.clear();
        // Remove the bad input from buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        Sleep(2000);
        return true;
    }
    return false;
}

inline void UI::clearNewLineInBuffer()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();
}

void UI::printFirstMenu()
{
    clearScreen();
    std::cout << "-----MENU-----\n\n";
    std::cout << "Sign up -> 1\n";
    std::cout << "Sign in -> 2\n"; 
    std::cout << "Quit -> 3\n";
}

//if true than quit
bool UI::firstMenu()
{
    printFirstMenu();
    clearNewLineInBuffer();

    int answer;
    std::cin >> answer;
    checkIfInputFailed();

    switch(answer)
    {
        case 1:
            signUp();
            break;

        case 2:
            signIn();
            break;

        case 3:
            return true;

        default:
            std::cout << "you need to input between 1 - 3.\n";
            return firstMenu();
    }
    return false;
}

bool UI::signUp()
{
    clearScreen();
    
    clearNewLineInBuffer();
    
    std::cout << "+------Sign Up------+\n";
    std::cout << "What is you full name? ";
    
    std::string full_name;
    std::getline(std::cin, full_name);
    
    std::cout << "\nInsert a new password: ";
    std::string password;
    std::getline(std::cin, password);
    
    uint32_t id = bank->createCustomer(full_name, password);
    
    *customer = bank->getCustomer(id);
    std::cout << "You are now logged into your new account.\n";
    Sleep(2000);
    clearScreen();
    return false;
}

bool UI::signIn()
{
    int try_num = 0;
    do 
    {
        uint32_t id;

        clearNewLineInBuffer();
        
        std::cout << "what is your full name? ";
        
        std::string full_name;
        std::getline(std::cin, full_name);

        std::cout << "what is your password? ";
        std::string password;
        std::getline(std::cin, password);

        *customer = bank->getCustomer(full_name, password);
        try_num++;
    } while (try_num < 3 && *customer == nullptr);
    
    if (*customer == nullptr)
    {
        Sleep(2000);
        return true;
    }
    
    std::cout << "\nYou are now logged into your account.\n";
    Sleep(2000);
    clearScreen();
    return false;
}

void UI::printMainMenu()
{
    std::cout << "-----MENU-----\n";

    std::cout << "\n------ACCOUNT OPERATION------\n";
    std::cout << "Make a new savings account -> 1\n";
    std::cout << "Get a Loan -> 2\n";
    std::cout << "Pay Loan -> 3\n";
    std::cout << "Transfer money from account to a diffrent account -> 4\n";
    std::cout << "Transfer money to someone else -> 5\n";

    std::cout << "\nSign out -> 6\n";

    std::cout << "\n-----DELETE-----\n";
    std::cout << "Delete customer -> 7\n";
    std::cout << "Delete account -> 8\n";

    std::cout << "\n-----TIME SETTINGS-----\n";
    std::cout << "Fast forward (change days) -> 9\n";

    std::cout << "\nQuit -> 10\n";
}

//1 -> quit
//0 -> firstMenu
//-1 -> mainMenu
int UI::mainMenu()
{
    clearScreen();
    if (*customer == nullptr)
    {
        std::cout << "the program tried to run the second menu without any customer connected.\n";
        std::cout << "Quiting...\n";
        Sleep(3000);
        return 1;
    }

    std::cout << "+----your Customer ID: " << (*customer)->getID() << " ----+\n";
    std::cout << "+----Your Accounts/Loans----+\n";
    (*customer)->printAllAccountsInCustomer();
    printMainMenu();

    int answer;

    std::cin >> answer;
    if (checkIfInputFailed())
        return -1;

    // if not returning somthing in the switch it will return later 0
    switch (answer)
    {
    case 1:
    {
        createSavingsAccount();
        break;
    }
    case 2:
    {
        createLoan();
        break;
    }
    case 3:
    {
        payLoan();
        break;
    }
    case 4:
    {
        transferBetweenAccounts();
        break;
    }
    case 5:
    {
        transferBetweenCustomers();
        break;
    }
    case 6:
        std::cout << "signing out...";
        *customer = nullptr;
        return 0;

    case 7:
        deleteCustomer();
        return 0;

    case 8:
    {
        deleteSavingsAccount();
        break;
    }
    case 9:
    {
        changeTime();
        break;
    }
    case 10:
        std::cout << "Quitting...\n";
        return 1;

    default:
        std::cout << "try inputing between 1 - 10.\n";
        Sleep(2000);
    }
    return -1;
}

void UI::createSavingsAccount() 
{
    int id = (*customer)->createSavingsAccount();
    
    if (id == -1)
    {
        std::cout << "there was an error with creating a Savings account.";
        Sleep(3000);
        return;
    }
    std::cout << "created a Savings Account with the ID: "<< id << "\n";
}

void UI::createLoan()
{
    double amount;
    std::cout << "what is the amount of money for the loan? ";
    std::cin >> amount;
    if (checkIfInputFailed())
        return;

    uint32_t loan_answer = (*customer)->createLoan(amount, (*customer)->getMainAccount());
    if (loan_answer == 0)
    {
        std::cout << "Error occured\n";
        Sleep(3000);
    }
    else
        std::cout << "\ncreated loan with the ID: " << loan_answer << "\n";

}

void UI::payLoan()
{
    std::cout << "what is the ID of the loan? ";

    uint32_t loan_id;
    std::cin >> loan_id;

    if (checkIfInputFailed())
        return;

    double amount;
    std::cout << "\nhow much do you want to pay for loan (-1 if you want to pay all the loan)? ";
    std::cin >> amount;

    if (checkIfInputFailed())
        return;

    if (amount == -1)
    {
        double temp = (*customer)->payLoan(loan_id, 0, (*customer)->getLoan(loan_id)->GetDebt());
        std::cout << temp;
        if (temp == 0)
            std::cout << "\nyou finished paying the loan! congrats!!!";
    }
    else
    {
        if ((*customer)->payLoan(loan_id, 0, amount) > 0)
            std::cout << "\nyou still need to pay: " << (*customer)->getLoan(loan_id)->GetDebt() << "$ to finish paying the loan.\n";
    }
    Sleep(2000);
}

void UI::transferBetweenAccounts()
{
    uint32_t withdraw_account_id;
    std::cout << "\nwhat is the ID of the account from which you want to send the money (0 if you want main account)? ";
    std::cin >> withdraw_account_id;

    if (checkIfInputFailed())
        return;

    uint32_t deposit_account_id;
    std::cout << "\nwhat is the ID of the account to which you want the money to be sent (0 if you want main account)? ";
    std::cin >> deposit_account_id;

    if (checkIfInputFailed())
        return;

    double amount;
    std::cout << "\nhow much do you want to transfer? ";
    std::cin >> amount;

    if (checkIfInputFailed())
        return;

    if((*customer)->transferBetweenAccounts(withdraw_account_id, deposit_account_id, amount))
        std::cout << "\ntransfer was succceful.\n";
    else
        std::cout << "\ntransfer failed.\n";
}

void UI::transferBetweenCustomers()
{
    uint32_t deposit_customer_id;
    std::cout << "\nwhat is the ID of the customer that you want to send him money? ";
    std::cin >> deposit_customer_id;

    if (checkIfInputFailed())
        return;

    Customer *deposit_customer = bank->getCustomer(deposit_customer_id);

    if (deposit_customer == nullptr)
        return;

    double amount;
    std::cout << "\nhow much do you want to transfer? ";
    std::cin >> amount;

    if (checkIfInputFailed())
        return;

    if(bank->transfer(**customer, *deposit_customer, amount))
        std::cout << "\ntransfer was succceful.\n";
    else
        std::cout << "\ntransfer failed.\n";
}

void UI::deleteCustomer()
{
    //deleting customer and signing out
    std::cout << "deleting customer...\n";
    bank->deleteCustomer((*customer)->getID());
    std::cout << "signing out...";
    *customer = nullptr;
    Sleep(3000);
}

void UI::deleteSavingsAccount()
{
    uint32_t account_id;
    std::cout << "\nwhat is the ID of the savings account that you want to delete? ";
    std::cin >> account_id;

    if (checkIfInputFailed())
        return;

    Account *account = (*customer)->getSavingsAccount(account_id);
    if (account == nullptr)
        return;

    if (account->getBalance() > 0)
    {
        std::cout << "\nWarning! the account still has " << account->getBalance() << "$ in it.\n";
        // maybe help him put the money in another account?
        (*customer)->getMainAccount()->deposit(account->getBalance());
        std::cout << "The money was added to your main account.\n";
    }
    Sleep(2000);
    //maybe we need to wait for confirmation
    std::cout << "\ndeleting account...\n";
    (*customer)->deleteAccount(account_id);
}

void UI::changeTime()
{
    int amount;
    std::cout << "how many days do you want to fast forward? ";
    std::cin >> amount;

    if (checkIfInputFailed())
        return;

    bank->changeDay(amount);
    Time time;
    std::cout << "\nwe are now at day " << time.day << "\n";
}