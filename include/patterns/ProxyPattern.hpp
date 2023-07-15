#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace ProxyPatternNameSpace {
    //Proxy Pattern
    //Proxy Pattern  provides surrogate or placeholder for another object to control access to it. 
    //Some intentions for using proxy: remote proxy, virtual proxy or protection proxy.
    enum AccountType {
        Administrator,
        NormalClient
    };

    //Interface for base class, we will have a delete operation in this class.
    class IRequestDelete {
    public:
        virtual void DeleteFile(std::string* myFile) = 0;
    };

    //Concrete implementation of Request Delete class
    class CRequestDelete : public IRequestDelete {
    public:
        //Do the actual deletion here
        void DeleteFile(std::string* myFile) {
            std::cout << *myFile << " has been deleted!" << std::endl;
            delete myFile;
        }
    };

    //Proxy of request delete class. Client should always interact with proxy instead of concrete
    //implementation of request delete class.
    class proxyRequestDelete : public IRequestDelete {
    private:
        AccountType m_Acc;
        IRequestDelete* m_cRD;
    public:
        proxyRequestDelete(AccountType Acc, IRequestDelete* iRD) {
            m_Acc = Acc;
            m_cRD = iRD;
        }
        ~proxyRequestDelete() {
            delete m_cRD;
        }
        void DeleteFile(std::string* myFile) {
            if (m_Acc == AccountType::NormalClient)
                std::cout << "You do not have permission to do delete operation!" << std::endl;
            else
                m_cRD->DeleteFile(myFile);
        }
    };

    void TestDesignPattern() {
        std::string* testString = new std::string("SecretFile.text");
        IRequestDelete* iRD = new proxyRequestDelete(AccountType::NormalClient, new CRequestDelete());
        iRD->DeleteFile(testString);
        IRequestDelete* iRD1 = new proxyRequestDelete(AccountType::Administrator, new CRequestDelete());
        iRD1->DeleteFile(testString);

        system("pause");
    }
}