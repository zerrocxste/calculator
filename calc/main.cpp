#include "main.h"

#include "calculator.h"

int main()
{
    std::unique_ptr<CCalculator>g_Calculator = std::make_unique<CCalculator>();

    std::cout << "Hello\nDo not use space when input expression! e.g 2 + 2\n";

    std::string szExpression;

    std::cout << std::endl << "enter: ";
    std::cin >> szExpression;
    std::cout << std::endl << std::endl;

    g_Calculator->setup(szExpression);
    g_Calculator->compute();
    std::cout << "Result: "<< g_Calculator->getResult() << std::endl;

    system("pause");

    return EXIT_SUCCESS;
}