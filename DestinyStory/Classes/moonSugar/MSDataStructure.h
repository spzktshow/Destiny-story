//
//  MSDataStructure.h
//  DestinyStory
//
//  Created by RockLee on 14-8-19.
//
//

#ifndef __DestinyStory__MSDataStructure__
#define __DestinyStory__MSDataStructure__

#include "moonSugar.h"

NS_MS_BEGIN

/******QueueItem*******/
class QueueItem : public cocos2d::Ref
{
public:
    QueueItem(std::string nameValue){
        name = nameValue;
    };
    ~QueueItem(){};
    
    std::string name;
    
    static QueueItem * create(std::string nameValue)
    {
        QueueItem * item = QueueItem::create(nameValue);
        if (item)
        {
            item->autorelease();
            return item;
        }
        else
        {
            delete item;
            item = nullptr;
            return nullptr;
        }
    };
};

/********Queue**********/
class Queue : public cocos2d::Ref
{
public:
    Queue(){};
    ~Queue(){};
    
    virtual void addItem(moonsugar::QueueItem * itemValue);
    
    virtual int addItemIndex(moonsugar::QueueItem * itemValue, int indexValue);
    
    virtual void removeItem(moonsugar::QueueItem * itemValue);
    
    virtual int checkIndex(moonsugar::QueueItem * itemValue);
    
    virtual void removeIndex(int indexValue);
    
    CREATE_REF_FUNC(Queue);
protected:
    cocos2d::Vector<moonsugar::QueueItem *> items;
};

NS_MS_END;

#endif /* defined(__DestinyStory__MSDataStructure__) */
