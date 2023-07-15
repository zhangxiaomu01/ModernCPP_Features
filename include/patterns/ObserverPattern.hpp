#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace ObserverPatternNameSpace {
    //02. Observer Pattern
    /*
    We define the observers as the classes who need to observe a frequently changed 
    class Observable class. Whenever the observable makes the change, it will push 
    the information that it has changed to all the observer classes.

    The observer pattern defines an one to many dependency between objects, 
    that when one of the objects changes the states, all of its dependencies are 
    notified and updated automatically.
    */
    //Observer Pattern:
    //Abstract class for observers (different display monitors in our case)
    class Observer {
    public:
        virtual void update() = 0;
        virtual void showInfo() = 0;
    };

    //Class for observee interface:
    //We can add observer reference here
    //We can update any observers if we made the change
    class Observee {
    protected:
        virtual void notifyUpdate() = 0;
    public:
        virtual void addObserver(Observer* ob) = 0;
        //We can also add removeObserver(Observer* ob) method here
    };

    //Concrete implementation below
    //Concrete observer class, wheather station!
    class WheatherInfo : public Observee {
    private:
        //temperature
        float m_temp;
        //moisture
        float m_mois;
        //A set of observers
        std::vector<Observer*> m_observerList;
        //Update Observers
        void notifyUpdate() {
            for (int i = 0; i < m_observerList.size(); ++i) {
                m_observerList[i]->update();
            }
        }
    public:
        WheatherInfo(float temperature, float mois) {
            m_temp = temperature;
            m_mois = mois;
            notifyUpdate();
        }

        void addObserver(Observer* ob) {
            m_observerList.push_back(ob);
            ob->update();
        }

        void setTemp(float temperature) {
            m_temp = temperature;
            notifyUpdate();
        }
        void setMoisture(float moisture) {
            m_mois = moisture;
            notifyUpdate();
        }
        float getTemp() {
            return m_temp;
        }
        float getMoisture() {
            return m_mois;
        }
    };

    //I chose the display mode as the observers. This is not a good example, since the two 
    //classes are pretty the same, we can inheritate them from a display class
    //We may have totally different logic pattern in these two observers
    //Observer 1: Phone display
    class PhoneDisplay : public Observer {
    private:
        //Note here we should define WheatherInfo, the concrete example
        WheatherInfo* m_wheatherInfo;
        void showInfo() {
            std::cout << "====== This is the phone display =======" << std::endl;
            std::cout << "Current temperature is: " << m_wheatherInfo->getTemp() << "." << std::endl;
            std::cout << "Current moinstrue is: " << m_wheatherInfo->getMoisture() << "." << std::endl;
        }
    public:
        PhoneDisplay(WheatherInfo* wIF) {
            m_wheatherInfo = wIF;
            m_wheatherInfo->addObserver(this);
        }
        void update() {
            //Just a simple example for update the info, we could also save temperature etc.
            showInfo();
        }

    };

    //Observer 2: LCD Display
    class LCDDisplay : public Observer {
    private:
        WheatherInfo* m_wheatherInfo;

        void showInfo() {
            std::cout << "====== This is the LCD display =======" << std::endl;
            std::cout << "Temperature: " << m_wheatherInfo->getTemp() << "." << std::endl;
            std::cout << "Moisture: " << m_wheatherInfo->getMoisture() << "." << std::endl;
        }
    public:
        LCDDisplay(WheatherInfo* wIF) {
            m_wheatherInfo = wIF;
            m_wheatherInfo->addObserver(this);
        }
        void update() {
            showInfo();
        }
    };

    void TestDesignPattern() {
        WheatherInfo currentWL(75.0f, 67.0f);
        PhoneDisplay* myPhone = new PhoneDisplay(&currentWL);
        LCDDisplay* myLCD = new LCDDisplay(&currentWL);

        currentWL.setTemp(34.0f);
        currentWL.setMoisture(78.0f);

        system("pause");
    }
}