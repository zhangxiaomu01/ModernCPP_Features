#include <windows.h>
#include <iostream>

#include "SimpleThreadPool.h"
#include "StrategyPattern.hpp"
#include "ObserverPattern.hpp"
#include "DecoratorPattern.hpp"
#include "FactoryPattern.hpp"
#include "SingletonPattern.hpp"

int main() {
    SingletonPatternNameSpace::testDesignPattern();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
