#include <windows.h>
#include <iostream>

#include "SimpleThreadPool.h"
#include "StrategyPattern.hpp"
#include "ObserverPattern.hpp"
#include "DecoratorPattern.hpp"

int main() {
    DecoratorPatternNameSpace::testDesignPattern();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
