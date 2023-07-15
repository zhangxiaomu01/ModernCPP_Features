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
#include "BridgePattern.hpp"
#include "TemplateMethodPattern.hpp"
#include "CompositionPattern.hpp"
#include "IteratorPattern.hpp"

int main() {
    IteratorPatternNameSpace::TestDesignPattern();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
