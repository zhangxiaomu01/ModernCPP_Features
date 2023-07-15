#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace IteratorPatternNameSpace {
    //Iterator Pattern
    //The iterator pattern provides a way to access the element from an aggregate 
    //object sequentially without  exposing the object's underlying representation.
    //Forward definition for PlayerInventory is needed!
    class PlayerInventory;
    struct Item {
        std::string itemID;
        Item(std::string ID) {
            itemID = ID;
        }
    };

    //We will design a simple inventory iterator.
    //Iterator Base class
    class Iterator {
    public:
        virtual bool isDone() = 0;
        virtual int next() = 0;
        virtual Item* currentItem() = 0;
    };
    //Inventory base class
    class Inventory {
    public:
        virtual Iterator* getIterator() = 0;
    };


    //concrete implementation of inventory iterator
    //It has a concrete implementation of player inventory
    //Since compiler won't see the actual implementation of PlayerInventory now
    //We need to give declaration here, and give concrete definition after PlayerInventory 
    //class definition!
    class InventoryIterator : public Iterator {
    private:
        PlayerInventory* m_pInventory;
        int m_currentIndex;
    public:
        InventoryIterator(PlayerInventory* pInv);
        
        bool isDone(); 
        int next(); 
        Item* currentItem(); 
    };

    //Concrete implementation of player inventory.
    //It has a concrete player inventory iterator
    class PlayerInventory : public Inventory {
    public:
        std::vector<Item> m_itemList;
        PlayerInventory(std::vector<Item>& itemList) {
            m_itemList = itemList;
        }
        Iterator* getIterator() {
            return new InventoryIterator(this);
        }
    };

    InventoryIterator::InventoryIterator(PlayerInventory* pInv){
        m_pInventory = pInv;
        m_currentIndex = 0;
    }

    bool InventoryIterator::isDone(){
        return m_currentIndex == m_pInventory->m_itemList.size();
    }

    int InventoryIterator::next() {
        if (m_currentIndex < m_pInventory->m_itemList.size())
            m_currentIndex++;
        return m_currentIndex;
    }

    Item* InventoryIterator::currentItem(){
        if (isDone()) return nullptr;
        return &m_pInventory->m_itemList[m_currentIndex];
    }


    void TestDesignPattern() {
        std::vector<Item> ourItemList = { Item("Knife"), Item("Sword"), Item("Medical Kit") };
        PlayerInventory pInv(ourItemList);
        Iterator* pInvIt = pInv.getIterator();
        while (!pInvIt->isDone()) {
            std::cout << pInvIt->currentItem()->itemID << std::endl;
            pInvIt->next();
        }

        system("pause");
    }
}