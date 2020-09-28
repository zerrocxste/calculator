#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

enum OPERATORS
{
    NONE,
    ADDITION,
    SUBSTRACTION,
    MULTIPLICATION,
    DIVISION
};

enum STEPS
{
    WAIT_FOR_FIRST_NUMBER,
    FOUND_SYMBOL,
    WAIT_FOR_SECOND_NUMBER,
    CALC_SECOND_EXPRESSION
};

namespace memory_utils
{
    void StringReplace(char* buf, const char* search, const char* replace)
    {
        char* p = buf;

        size_t l1 = strlen(search);
        size_t l2 = strlen(replace);

        while (p = strstr(p, search))
        {
            memmove(p + l2, p + l1, strlen(p + l1) + 1U);
            memcpy(p, replace, l2);
            p += l2;
        }
    }
}

float calc_ex(std::string expression)
{
    float pflResult = 0;
    std::string ex;
    OPERATORS current_operator;

    ex = expression;

    for (int i = ex.size(); i >= 0; i--)
    {
        std::string number1, number2;
        if (ex[i] == '*' || ex[i] == '/')
        {
            if (ex[i] == '*') {
                current_operator = OPERATORS::MULTIPLICATION;
            }
            else if (ex[i] == '/') {
                current_operator = OPERATORS::DIVISION;
            }
            int b = i - 1;
            while (true)
            {
                if (ex[b] == '+'
                    || ex[b] == '-'
                    || ex[b] == '*'
                    || ex[b] == '/'
                    || ex[b] == '\0'
                    || b < 0)
                    break;
                number1 += ex[b];        
                b--;
            }
            std::reverse(number1.begin(), number1.end());
            int u = i + 1;
            while (true)
            {
                if (ex[u] == '+'
                    || ex[u] == '-'
                    || ex[u] == '*'
                    || ex[u] == '/'
                    || ex[u] == '\0')
                    break;
                number2 += ex[u];
                u++;
            }

            float flNumber1 = atof(number1.c_str());
            float flNumber2 = atof(number2.c_str());

            std::cout << std::endl << "OPERATORS: " << current_operator << std::endl;
            std::cout << "#1 number (string format): " << number1 << std::endl;
            std::cout << "#2 number (string format): " << number2 << std::endl << std::endl;

            switch (current_operator)
            {
            case OPERATORS::MULTIPLICATION:
                pflResult = flNumber1 * flNumber2;
                break;
            case OPERATORS::DIVISION:
                pflResult = flNumber1 / flNumber2;
                break;
            default:
                std::cout << "error #1\n\n";
                break;
            }

            auto replace = [number1, number2, current_operator]() -> std::string {

                std::string p;
                p += number1;
                
                if (current_operator == OPERATORS::MULTIPLICATION)
                    p += std::string("*");
                else if (current_operator == OPERATORS::DIVISION)
                    p += std::string("/");
                
                p += number2;

                return p.c_str();
            };

            //std::cout << replace() << std::endl;

            char res[50];
            strcpy(res, ex.c_str());
            memory_utils::StringReplace(res, replace().c_str(), std::to_string(pflResult).c_str());
            ex = res;
        }
    }

    int step = 0;
    std::string number1, number2;
    bool collect_OK = false;

    for (int j = 0; j < ex.size(); j++)
    {
        if (ex[j] == '+') 
        {
            current_operator = OPERATORS::ADDITION;
            step++;
        }
        else if (ex[j] == '-') 
        {
            current_operator = OPERATORS::SUBSTRACTION;
            step++;
        }
        else
        {
            if (step == STEPS::WAIT_FOR_FIRST_NUMBER) {
                number1 += ex[j];
            }
            else if (step >= STEPS::FOUND_SYMBOL)
            {
                number2 += ex[j];
                if (ex[j + 1] == '+'
                    || ex[j + 1] == '-'
                    || ex[j + 1] == '*'
                    || ex[j + 1] == '/'
                    || ex[j + 1] == '\0')
                {
                    step++;
                    collect_OK = true;
                }
            }

            if (!number2.empty() && collect_OK == true)
            {
                float flNumber1 = atof(number1.c_str());
                float flNumber2 = atof(number2.c_str());

                std::cout << std::endl << "OPERATORS: " << current_operator << std::endl;
                if (step <= WAIT_FOR_SECOND_NUMBER) {
                    std::cout << "#1 number (string format): " << number1 << std::endl;
                }
                else {
                    std::cout << "pflResult (float format): " << pflResult << std::endl;
                }
                std::cout << "#2 number (string format): " << number2 << std::endl << std::endl;

                switch (current_operator)
                {
                case OPERATORS::ADDITION:
                    if (step >= CALC_SECOND_EXPRESSION) {
                        pflResult += flNumber2;
                    }
                    else {
                        pflResult = flNumber1 + flNumber2;
                    }
                    number2.clear();
                    collect_OK = false;
                    break;
                case OPERATORS::SUBSTRACTION:
                    if (step >= CALC_SECOND_EXPRESSION) {
                        pflResult -= flNumber2;
                    }
                    else {
                        pflResult = flNumber1 - flNumber2;
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

    ex = std::to_string(pflResult);

    return static_cast<float>(atof(ex.c_str()));
}

#define DEBUG 1

int main()
{
    std::string expression;
    float fltemp;
    std::vector<char>vtempNum;
    std::string szReplaceData;
    
    std::cout << "Hello\nDo not use space when input expression! e.g 2 + 2\n";
#if DEBUG == 1
    char ex[] = "78+20-(22+94*(16-34+(25+15)))*25-(92-10)";
    expression = ex;
    std::cout << expression << std::endl << std::endl;
    
#else
    std::cout << std::endl << "enter: ";
    std::cin >> expression;
    std::cout << std::endl;
#endif // DEBUG

    for (int i = expression.size(); i >= 0; i--)
    {
        if (expression[i - 1] == '(')
        {
            int c = i;
            while (true)
            {    
                if (expression[c] == ')')
                {
                    break;
                }
                vtempNum.push_back(expression[c]);    
                if (expression[c + 1] == ')')
                {  
                    std::string temp_number;
                    for (auto c : vtempNum)
                    {
                        if (c == '(') break;
                        temp_number += c;
                    }
                    std::cout << temp_number << " " << std::endl;
                    fltemp = calc_ex(temp_number);
                    szReplaceData = std::string("(") + temp_number + std::string(")");
                    vtempNum.clear();
                }
                c++;
            }
            char res[50];
            strcpy(res, expression.c_str());
            memory_utils::StringReplace(res, szReplaceData.c_str(), std::to_string(fltemp).c_str());
            expression = res;
            std::cout << "replace data: " << szReplaceData << " to: " << std::to_string(fltemp).c_str() << std::endl;
            std::cout << "edited expression: " << expression << std::endl;
            szReplaceData.clear();
        }
    }

    std::cout << fltemp << std::endl;

    std::cout << calc_ex(expression) << std::endl;

    system("pause");

    return EXIT_SUCCESS;
}
