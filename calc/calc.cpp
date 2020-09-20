#include <iostream>
#include <Windows.h>

enum OPERATORS
{
    NONE,
    ADDITION,
    SUBSTRACTION,
    MULTIPLICATION,
    DIVISION
};

enum
{
    WAIT_FOR_FIRST_NUMBER,
    FOUND_SYMBOL,
    WAIT_FOR_SECOND_NUMBER,
    CALC_SECOND_EXPRESSION
};

int main()
{   
    SetConsoleTitle("calculator");
    
    std::cout << "Hello\nDo not use space when input expression! e.g 2 + 2\n";

    float pflResult = 0;
    static std::string expression, number1, number2;
    OPERATORS current_operator;
    bool collect_OK = false;
    static int step = 0;

    std::cout << std::endl << "enter: ";
    std::cin >> expression;
    std::cout << std::endl;

    for (int j = 0; j < expression.size(); j++)
    {      
        if (expression[j] == '+')
        {
            current_operator = ADDITION;
            step++;
        }
        else if (expression[j] == '-')
        {
            current_operator = SUBSTRACTION;
            step++;
        }
        else if (expression[j] == '*')
        {
            current_operator = MULTIPLICATION;
            step++;
        }
        else if (expression[j] == '/')
        {
            current_operator = DIVISION;
            step++;
        }
        else
        {
            if (step == WAIT_FOR_FIRST_NUMBER)
            {
                number1 += expression[j];
            }
            else if (step >= FOUND_SYMBOL)
            {
                number2 += expression[j];
                if (expression[j + 1] == '+'
                    || expression[j + 1] == '-'
                    || expression[j + 1] == '*'
                    || expression[j + 1] == '/'
                    || expression[j + 1] == '\0')
                {
                    step++;
                    collect_OK = true;
                }
            }

            if (!number2.empty() && collect_OK == true)
            {
                float flNumber1 = atof(number1.c_str());
                float flNumber2 = atof(number2.c_str());

                std::cout << "OPERATORS: " << current_operator << std::endl;
                if (step <= WAIT_FOR_SECOND_NUMBER)
                {
                    std::cout << "#1 number (string format): " << number1 << std::endl;
                }
                else
                {
                    std::cout << "pflResult (float format): " << pflResult << std::endl;
                }
                std::cout << "#2 number (string format): " << number2 << std::endl << std::endl;

                switch (current_operator)
                {
                case ADDITION:
                    if (step >= CALC_SECOND_EXPRESSION)
                    {
                        pflResult += flNumber2;                  
                    }
                    else
                    {
                        pflResult = flNumber1 + flNumber2;
                    }
                    number2.clear();
                    collect_OK = false;
                    break;
                case SUBSTRACTION:
                    if (step >= CALC_SECOND_EXPRESSION)
                    {
                        pflResult -= flNumber2;
                    }
                    else
                    {
                        pflResult = flNumber1 - flNumber2;
                    }
                    number2.clear();
                    collect_OK = false;
                    break;
                case MULTIPLICATION:
                    if (step >= CALC_SECOND_EXPRESSION)
                    {
                        pflResult *= flNumber2;
                    }
                    else
                    {
                        pflResult = flNumber1 * flNumber2;
                    }
                    number2.clear();
                    collect_OK = false;
                    break;
                case DIVISION:
                    if (step >= CALC_SECOND_EXPRESSION)
                    {
                        pflResult /= flNumber2;
                    }
                    else
                    {
                        pflResult = flNumber1 / flNumber2;
                    }
                    number2.clear();
                    collect_OK = false;
                    break;
                default:
                    std::cout << "error #1\n\n";
                    break;
                }
            }
        }    
    } 
    
    std::cout << "result: " << pflResult << std::endl;

    system("pause");

    return EXIT_SUCCESS;
}