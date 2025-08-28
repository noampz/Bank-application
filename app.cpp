#include "./src/ui.hpp"

int main()
{
    Bank bank = Bank();
    Customer *customer = nullptr;
    UI ui = UI(&bank, &customer);

    while (true)
    {
        if (ui.firstMenu())
            break;
        
        int output;
        do
            output = ui.mainMenu();
        while(output == -1);

        if (output == 0)
            continue;
        
        if (output == 1)
            break;
        
    }
    bank.~Bank();
    return 0;
}