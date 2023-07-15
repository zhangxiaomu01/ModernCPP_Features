#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace CommandPatternNameSpace {
    //Command Pattern:
    //Command Pattern encapsulates a request as an object thereby letting you 
    //parameterize other objects with different request queue or log request and 
    //support undoable operations. 

    //Define a concrete object which will react to the command (Receiver)
    //For example, table light
    class TableLight {
    public:
        void turnOn() {
            std::cout << "The light is now on!" << std::endl;
        }

        void turnOff() {
            std::cout << "The light is now off!" << std::endl;
        }
    };

    //Command interface: we can execute or revoke the command
    class ICommand {
    public:
        virtual void execute() = 0;
        virtual void unexecute() = 0;
    };
    //Implement of command, can have different concrete implementation
    class tableLightCommand : public ICommand {
    private:
        TableLight* m_tableLight;
    public:
        tableLightCommand(TableLight* L) {
            m_tableLight = L;
        }
        virtual ~tableLightCommand() {
            delete m_tableLight;
        }
        
        void execute() {
            m_tableLight->turnOn();
        }

        void unexecute() {
            m_tableLight->turnOff();
        }
    };

    //Invoker is like a controller, which will trigger the command to execute
    class Invoker {
    private:
        //we can have multiple ICommand here, represents to different input
        ICommand* m_OnOff;
        //ICommand* m_LightDim;
        //ICommand* m_Mode;
    public:
        Invoker(ICommand* C) {
            m_OnOff = C;
        }
        virtual ~Invoker() {
            delete m_OnOff;
        }
        void clickOn() {
            m_OnOff->execute();
        }
        void clickOff() {
            m_OnOff->unexecute();
        }

    };

    void TestDesignPattern() {
        Invoker* testInvoker = new Invoker(new tableLightCommand(new TableLight()));
        testInvoker->clickOn();
        testInvoker->clickOff();

        system("pause");
    }
}