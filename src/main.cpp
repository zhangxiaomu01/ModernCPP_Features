#include <windows.h>
#include <iostream>

#include "SimpleThreadPool.h"
#include "StrategyPattern.hpp"
#include "ObserverPattern.hpp"
#include "DecoratorPattern.hpp"
#include "FactoryPattern.hpp"
#include "SingletonPattern.hpp"
#include "CommandPattern.hpp"
#include "AdapterPattern.hpp"
#include "ProxyPattern.hpp"

int main() {
    ProxyPatternNameSpace::TestDesignPattern();
    std::cout << "Hello world!" << std::endl;
    return 0;
}