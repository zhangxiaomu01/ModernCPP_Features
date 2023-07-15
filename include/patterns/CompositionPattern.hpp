#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace CompositionPatternNameSpace {
    //Composite Pattern
    //The composite pattern composes objects into tree structures to represent 
    //part-whole hierarchies. Composite let's clients treat individual objects 
    //and compositions of objects uniformly.
    //We define the interface for getHtml method
    class HTMLLists {
    public:
        virtual std::string getHtml() = 0;
    };

    //The Lists class is a composite class which specifically 
    class Lists : public HTMLLists {
    private:
        std::vector<HTMLLists*> m_lists;
    public:
        Lists( std::vector<HTMLLists*> l) {
            m_lists = l;
        }
        std::string getHtml() {
            unsigned int len = m_lists.size();
            std::string html = "<ul>\n";
            for (int i = 0; i < len; ++i) {
                html += "<li>\n";
                html += m_lists[i]->getHtml();
                html += '\n';
                html += "</li>\n";
            }
            html += "</ul>\n";
            return html;
        }
    };

    class Leaf : public HTMLLists {
    private:
        std::string m_text;
    public:
        Leaf(std::string text) {
            m_text = text;
        }
        std::string getHtml() {
            return m_text;
        }
    };

    void TestDesignPattern() {
        std::vector<HTMLLists*> l1 = { new Leaf("TCC"), new Leaf("Sd") };
        std::vector<HTMLLists*> l2 = { new Leaf("printf"), new Lists(l1), new Leaf("It's good!") };
        std::vector<HTMLLists*> OurList = { new Leaf("ACC"), new Lists(l1), new Lists(l2), new Leaf("S1") };

        HTMLLists* lists = new Lists(OurList);

        std::string res = lists->getHtml();
        std::cout << res << std::endl;
        
        system("pause");
    }
}