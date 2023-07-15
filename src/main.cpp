#include <windows.h>
#include <iostream>

#include "SimpleThreadPool.h"
#include "StrategyPattern.hpp"
#include "ObserverPattern.hpp"

int main() {
    ObserverPatternNameSpace::testDesignPattern();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
