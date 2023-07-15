#pragma once

#include<iostream>
#include<vector>
#include<string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

namespace TemplateMethodPatternNameSpace {
    //Template Method Pattern
    //Template Method Pattern defines a skeleton of the algorithm in an operation, 
    //deferring some steps to subclasses. Template class let subclasses redefine 
    //certain steps of the algorithm without changing the algorithm's structure. 
    class Data {
    private:
        bool isValid;
    public:
        Data(bool v) { isValid = v; }
        bool checkValid() { return isValid; }
    };

    //Record class is our template!
    //In our record class, we define the structure of save() method. The only thing missing is 
    //ValidateData() and ValidateDataSave(). We can implement in the concrete class!
    class Record {
    protected:
        Data* m_data;
        virtual bool ValidateData() = 0;
        virtual bool ValidateDataSave() = 0;
    public:
        Record(Data* data) { m_data = data; }
        void Save() {
            std::cout << "Initializing the data base!" << std::endl;
            std::cout << "Now validate data to be saved!" << std::endl;
            if (ValidateData()) {
                std::cout << "Data is OK! Now saving..." << std::endl;
            }
            else {
                std::cout << "Data type not supported. Saving denied!" << std::endl;
                return;
            }
            
            if (ValidateDataSave()) {
                std::cout << "Data successfully saved!" << std::endl;
            }
            else {
                std::cout << "Data save failed! " << std::endl;
            }
        }
        virtual ~Record() { delete m_data; }
        
    };

    class userRecord : public Record {
    public:
        userRecord(Data* d) : Record(d){}
        bool ValidateData() {
            return m_data->checkValid();
        }
        //We simulate the validate process here using rand() - 85% chance success
        bool ValidateDataSave() {
            std::cout << "Now validate data base save!" << std::endl;
            srand(time(NULL));
            int x = rand() % 100;
            return x < 85;
        }
    };


    void TestDesignPattern() {
        Record* aRecord = new userRecord(new Data(false));
        aRecord->Save();
        std::cout << std::endl;
        Record* sRecord = new userRecord(new Data(true));
        sRecord->Save();

        delete aRecord;
        delete sRecord;
        
        system("pause");
    }
}