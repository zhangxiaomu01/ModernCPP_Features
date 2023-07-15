#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace BridgePatternNameSpace {
    //Bridge Pattern
    //Bridge pattern: The intent of the bridge pattern is to decouple an abstraction
    //from its implementation so that the two can vary independently.
    //The actual content class
    class ArtistInfo {
    private:
        std::string m_name;
        int m_age;
        std::string m_Intro;

    public:
        ArtistInfo(std::string name, int age, std::string intro) {
            m_name = name;
            m_age = age;
            m_Intro = intro;
        }
        std::string getName() {
            return m_name;
        }
        std::string Summary() {
            return m_Intro;
        }
        int getAge() {
            return m_age;
        }

    };

    class BookInfo {
    private:
        std::string m_name;
        int m_publishYear;
        std::string m_Author;
    public:
        BookInfo(std::string name, int year, std::string author) {
            m_name = name;
            m_publishYear = year;
            m_Author = author;
        }
        std::string getName() {
            return m_name;
            
        }
        std::string AuthorInfo() {
            return m_Author;
        }
        int getYear() {
            return m_publishYear;
        }
    };

    //Define the resource interface. The actual implementation can vary based on the same 
    //interface
    class IResources {
    public:
        virtual void Title() = 0;
        virtual void chronoInfo() = 0;
        virtual void Intro() = 0;
    };

    //Define the display interface to display information of Resources
    class IDisplay {
    protected:
        IResources* m_resource;
    public:
        IDisplay(IResources* res) {
            m_resource = res;
        }
        virtual ~IDisplay() {
            delete m_resource;
        }
        virtual void ShowInfo() = 0;
    };

    //Concrete implementation, note we can change ShowInfo content here!
    class LongForm : public IDisplay {
    public:
        LongForm(IResources* res) : IDisplay(res){}
        void ShowInfo() {
            std::cout << "This is the long form view!" << std::endl;
            std::cout << "+++++++++++++++++++++++++++" << std::endl;
            std::cout << "Title: ";
            m_resource->Title();
            std::cout << "The images is below: " << std::endl;
            std::cout << "===========================" << std::endl;
            m_resource->Intro();
            m_resource->chronoInfo();
            std::cout << "Close the Page!" << std::endl;
            std::cout << std::endl;
        }
    };

    class ShortForm : public IDisplay {
    public:
        ShortForm(IResources* res) : IDisplay(res) {}
        void ShowInfo() {
            std::cout << "This is the short form view!" << std::endl;
            std::cout << "............................" << std::endl;
            std::cout << "Title: ";
            m_resource->Title();
            std::cout << "The images is below: " << std::endl;
            std::cout << "*****------*****" << std::endl;
            m_resource->chronoInfo();
            m_resource->Intro();
            std::cout << std::endl;
        }
    };

    class ArtistResource : public IResources {
    private:
        ArtistInfo* m_Artist;
    public:
        ArtistResource(ArtistInfo* a) {
            m_Artist = a;
        }
        void Title() {
            std::cout << m_Artist->getName() << std::endl; 
        }
        void Intro() {
            std::cout << m_Artist->Summary() << std::endl;
        }
        void chronoInfo() {
            std::cout << "This artist is " << m_Artist->getAge() << " years old." << std::endl;
        }
        ~ArtistResource() {
            delete m_Artist;
        }
    };

    class BookResource : public IResources {
    private:
        BookInfo* m_Book;
    public:
        BookResource(BookInfo* b) {
            m_Book = b;
        }
        void Title() {
            std::cout << m_Book->getName() << std::endl;
        }
        void Intro() {
            std::cout << "This book is written by ";
            std::cout << m_Book->AuthorInfo() << std::endl;
        }
        void chronoInfo() {
            std::cout << "This book is published in year " << m_Book-> getYear() << std::endl;
        }
        ~BookResource() {
            delete m_Book;
        }
    };

    /*
    Note we now have artist resource (A) and book resource (B), long term view (L) and short 
    term view (S). We can easily combine them together. For example, (A-S), (A-L), (B-L), (B-S).
    This is what this meant to solve. The combination problem within different classes.
    */
    void TestDesignPattern() {
        ArtistInfo aArtist("Machael Jackson", 39, "He is a gigant in music which affect thousands of people!");
        BookInfo aBook("Justice", 1987, "Eric Meier");
        IDisplay* longFormInst = new LongForm(new ArtistResource(&aArtist));
        IDisplay* shortFormInst = new ShortForm(new BookResource(&aBook));
        IDisplay* longFormInst_02 = new LongForm(new BookResource(&aBook));
        longFormInst->ShowInfo();
        longFormInst_02->ShowInfo();
        shortFormInst->ShowInfo();
        delete longFormInst;
        delete shortFormInst;
        delete longFormInst_02;
        
        system("pause");
    }
}