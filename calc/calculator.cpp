#include "calculator.h"

bool CCalculator::findBrackets()
{
    bool bResult = false;
    for (int i = 0; i < _szExpression.size(); i++)
    {
        if (_szExpression[i] == '(')
        {
            bResult = true;
            break;
        }      
    }
    return bResult;
}

float CCalculator::computeExpression(std::string pszExpression)
{
    float pflResult = 0;
    std::string ex;
    OPERATORS current_operator;

    ex = pszExpression;

    const char symbols[] = { '/', '*' };
    for (auto s : symbols)
    {
        for (int i = ex.size(); i >= 0; i--)
        {
            std::string number1, number2;
            if (ex[i] == s)
            {
                current_operator = this->reintepretSymbol(s);
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

                #if DEBUG == 1
                std::cout << std::endl << "OPERATORS: " << current_operator << std::endl;
                std::cout << "#1 number (string format): " << number1 << std::endl;
                std::cout << "#2 number (string format): " << number2 << std::endl << std::endl;
                #endif // DEBUG

                switch (current_operator)
                {
                case OPERATORS::MULTIPLICATION:
                    pflResult = flNumber1 * flNumber2;
                    break;
                case OPERATORS::DIVISION:
                    pflResult = flNumber1 / flNumber2;
                    break;
                default:
                #if DEBUG == 1
                    std::cout << "error #1\n\n";
                #endif // DEBUG
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

                    return p;
                };

                char res[256/*?*/];
                strcpy(res, ex.c_str());
                utils.StringReplace(res, replace().c_str(), std::to_string(pflResult).c_str());
                ex = res;
            }
        }
    }

    int step = 0;
    std::string number1, number2;
    bool collect_OK = false;

    for (int j = 0; j < ex.size(); j++)
    {
        if (ex[j] == '+' || ex[j] == '-') 
        {
            current_operator = this->reintepretSymbol(ex[j]);
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

                #if DEBUG == 1
                std::cout << std::endl << "OPERATORS: " << current_operator << std::endl;
                if (step <= WAIT_FOR_SECOND_NUMBER) {
                    std::cout << "#1 number (string format): " << number1 << std::endl;
                }
                else {
                    std::cout << "pflResult (float format): " << pflResult << std::endl;
                }
                std::cout << "#2 number (string format): " << number2 << std::endl << std::endl;
                #endif // DEBUG

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
                    #if DEBUG == 1
                    std::cout << "error #1\n\n";
                    #endif // DEBUG
                    break;
                }
            }
        }
    }

    ex = std::to_string(pflResult);

    return static_cast<float>(atof(ex.c_str()));
}

void CCalculator::solveBrackets()
{
    float flTemp = 0;
    for (int i = _szExpression.size(); i >= 0; i--)
    {
        if (_szExpression[i - 1] == '(')
        {
            std::string szReplaceData;
            int c = i;
            while (true)
            {
                if (_szExpression[c] == ')')
                {
                    flTemp = this->computeExpression(szReplaceData);
                    szReplaceData = std::string("(") + szReplaceData + std::string(")");
                    break;
                }
                szReplaceData += _szExpression[c];
                c++;
            }
            char res[256/*?*/];
            strcpy(res, _szExpression.c_str());
            utils.StringReplace(res, szReplaceData.c_str(), std::to_string(flTemp).c_str());
            _szExpression = res;
            #if DEBUG == 1
            std::cout << "replace data: " << szReplaceData << " to: " << std::to_string(flTemp).c_str() << std::endl;
            std::cout << "edited expression: " << _szExpression << std::endl;
            #endif //DEBUG
        }
    }
}

void CCalculator::compute()
{
    if (this->findBrackets())
        this->solveBrackets();

    _flResult = this->computeExpression(_szExpression);
}
