//
//  MSDataStructure.cpp
//  DestinyStory
//
//  Created by RockLee on 14-8-19.
//
//

#include "MSDataStructure.h"

USING_NS_MS;

int Queue::checkIndex(moonsugar::QueueItem * itemValue)
{
    int n = items.size();
    for (int i = 0; i < n; i ++)
    {
        moonsugar::QueueItem * item = items.at(i);
        if (item->name == itemValue->name)
        {
            return i;
        }
    }
    return -1;
}

int Queue::addItemIndex(moonsugar::QueueItem * itemValue, int indexValue)
{
    int index = checkIndex(itemValue);
    if (index != -1) return index;
    items.insert(indexValue, itemValue);
    return indexValue;
}

void Queue::addItem(moonsugar::QueueItem * itemValue)
{
    int index = checkIndex(itemValue);
    if (index == -1)
    {
        items.pushBack(itemValue);
    }
}

void Queue::removeItem(moonsugar::QueueItem *itemValue)
{
    int index = checkIndex(itemValue);
    this->removeIndex(index);
}

void Queue::removeIndex(int indexValue)
{
    items.erase(indexValue);
}