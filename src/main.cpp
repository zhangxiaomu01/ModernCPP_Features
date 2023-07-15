#include <windows.h>
#include <iostream>

#include "SimpleThreadPool.h"
#include "StrategyPattern.hpp"
#include "ObserverPattern.hpp"
#include "DecoratorPattern.hpp"
#include "FactoryPattern.hpp"

int main() {
    FactoryPatternNameSpace::testDesignPattern();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
