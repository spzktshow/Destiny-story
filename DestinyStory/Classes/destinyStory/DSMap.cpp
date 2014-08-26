//
//  DSMap.cpp
//  DestinyStory
//
//  Created by RockLee on 14-8-23.
//
//

#include "DSMap.h"
#include "Context.h"

USING_NS_DS;

/*******Map*******/
void Map::initMatrix()
{
    nodeItems.clear();
    //init nodeItems
    for (int i = 0; i < matrix->col; i ++) {
        for (int j = 0; j < matrix->row; j ++) {
            MatrixIndex * matrixIndex = MatrixIndex::create(i, j);
            NodeItem * nodeItem = NodeItem::create(matrixIndex);
            this->nodeItems.pushBack(nodeItem);
        }
    }
}

bool Map::init()
{
    this->initMatrix();
    return true;
}

int Map::getNodeItemIndex(int colIndex, int rowIndex)
{
    int index = MapUtils::calculateIndex(colIndex, rowIndex, this->matrix);
    return index;
}

NodeItem * Map::getNodeItem(int colIndex, int rowIndex)
{
    int index = MapUtils::calculateIndex(colIndex, rowIndex, this->matrix);
    return this->nodeItems.at(index);
}

MapItem * Map::getMapItem(int colIndex, int rowIndex)
{
    long n = this->mapItems.size();
    for (int i = 0; i < n; i ++) {
        MapItem * mapItem = this->mapItems.at(i);
        bool isOver = MapUtils::checkOver(mapItem->matrix, mapItem->matrixIndex, colIndex, rowIndex);
        if (isOver) return mapItem;
    }
    return nullptr;
}

MapItem * Map::getMapItem(int mapId)
{
    long n = this->mapItems.size();
    for (int i = 0; i < n; i ++)
    {
        MapItem * mapItem = this->mapItems.at(i);
        if (mapItem->mapId == mapId) return mapItem;
    }
    return nullptr;
}

int Map::addMapItem(destinyStory::MapItem *mapItem)
{
    long n = this->mapItems.size();
    for (int i = 0; i < n; i ++) {
        destinyStory::MapItem * checkMapItem = this->mapItems.at(i);
        if (MapUtils::checkOver(checkMapItem, mapItem)) return -1;
    }
    this->mapItems.pushBack(mapItem);
    return (int)this->mapItems.size() - 1;
}

int Map::checkMapItem(int mapId)
{
    long n = this->mapItems.size();
    for (int i = 0; i < n; i ++)
    {
        MapItem * mapItem = this->mapItems.at(i);
        if (mapItem->mapId == mapId) return i;
    }
    return -1;
}

void Map::removeMapItem(MapItem * mapItem)
{
    int index = this->checkMapItem(mapItem->mapId);
    if (index != -1) this->mapItems.erase(index);
}

/********MapUtils*******/
int MapUtils::calculateIndex(int colIndex, int rowIndex, Matrix * matrix)
{
    return rowIndex * matrix->col + colIndex;
}

MatrixIndex * MapUtils::calculateMatrixIndex(int index, destinyStory::Matrix *matrix)
{
    int colIndex = index % matrix->col;
    int rowIndex = (int)(index / matrix->col);
    MatrixIndex * matrixIndex = MatrixIndex::create(colIndex, rowIndex);
    return matrixIndex;
}

MatrixIndex * MapUtils::calculateMatrixIndexByMap(int index, destinyStory::Map *map)
{
    Matrix * matrix = map->matrix;
    int colIndex = index % matrix->col;
    int rowIndex = (int)(index / matrix->col);
    return map->getNodeItem(colIndex, rowIndex)->matrixIndex;
}

MatrixIndex * MapUtils::calculateMatrixIndexByMap(int colIndex, int rowIndex, destinyStory::Map *map)
{
    int index = MapUtils::calculateIndex(colIndex, rowIndex, map->matrix);
    return MapUtils::calculateMatrixIndexByMap(index, map);
}

bool MapUtils::checkOver(destinyStory::Matrix *matrix, destinyStory::MatrixIndex *matrixIndex, int colIndex, int rowIndex)
{
    if (colIndex >= matrixIndex->colIndex && colIndex < matrixIndex->colIndex + matrix->col
        && rowIndex >= matrixIndex->rowIndex && rowIndex < matrixIndex->rowIndex + matrix->row)
    {
        return true;
    }
    return false;
}

bool MapUtils::checkOver(MapItem * mapItem, MapItem * checkMapItem)
{
    cocos2d::Point rangeCol(mapItem->matrixIndex->colIndex, mapItem->matrixIndex->colIndex + mapItem->matrix->col - 1);
    cocos2d::Point checkRangeCol(checkMapItem->matrixIndex->colIndex, checkMapItem->matrixIndex->colIndex + checkMapItem->matrix->col - 1);
    if (rangeCol.x > checkRangeCol.y && checkRangeCol.x > rangeCol.y) return false;
    cocos2d::Point rangeRow(mapItem->matrixIndex->rowIndex, mapItem->matrixIndex->rowIndex + mapItem->matrix->row - 1);
    cocos2d::Point checkRangeRow(checkMapItem->matrixIndex->rowIndex, checkMapItem->matrixIndex->rowIndex + checkMapItem->matrix->row - 1);
    if (rangeRow.x > checkRangeRow.y && checkRangeRow.x > rangeRow.y) return false;
    return true;
}

cocos2d::Point MapUtils::calculatePoint(destinyStory::MatrixIndex *matrixIndex, cocos2d::Size size)
{
    float x = matrixIndex->colIndex * size.width;
    float y = matrixIndex->rowIndex * size.height;
    return cocos2d::Point(x, y);
}

cocos2d::Point MapUtils::calculateTopLeft(cocos2d::Point point, cocos2d::Size size)
{
    return point;
}

cocos2d::Point MapUtils::calculateTopRight(cocos2d::Point point, cocos2d::Size size)
{
    point.x = point.x + size.width;
    return point;
}

cocos2d::Point MapUtils::calculateBottomLeft(cocos2d::Point point, cocos2d::Size size)
{
    point.y = point.y + size.height;
    return point;
}

cocos2d::Point MapUtils::calculateBottomRight(cocos2d::Point point, cocos2d::Size size)
{
    point.x = point.x + size.width;
    point.y = point.y + size.height;
    return point;
}
/***********MapView*****************/
void MapView::onEnter()
{
    cocos2d::Layer::onEnter();
    
    this->addViewListeners();
    this->addContextListeners();
}

void MapView::onExit()
{
    this->removeContextListeners();
    this->removeViewListeners();
    
    cocos2d::Layer::onExit();
}

void MapView::addContextListeners()
{
    mapRefreshListener = cocos2d::EventListenerCustom::create(SYSTEM_EVENT_MAP_REFRESH, CC_CALLBACK_1(MapView::onSystemEventMapRefresh, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mapRefreshListener, 1);
}

void MapView::removeContextListeners()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(mapRefreshListener);
}

void MapView::onSystemEventMapRefresh(cocos2d::EventCustom *eventCustom)
{
    this->renderMap();
}

void MapView::renderMap()
{
    
}

void MapView::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(MapView::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    Layer::draw(renderer, transform, flags);
}

void MapView::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    cocos2d::Director * director = cocos2d::Director::getInstance();
    director->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    destinyStory::MatrixIndex * matrixIndexTopLeft = destinyStory::MapUtils::calculateMatrixIndexByMap(0, 0, map);
    cocos2d::Point pointTopLeft = destinyStory::MapUtils::calculatePoint(matrixIndexTopLeft, map->mapRectSize);
    destinyStory::Point3D pointTopLeft3D(pointTopLeft.x, 0, pointTopLeft.y);
    
    destinyStory::MatrixIndex * matrixIndexTopRight = destinyStory::MapUtils::calculateMatrixIndexByMap(map->matrix->col - 1, 0, map);
    cocos2d::Point pointTopRight = destinyStory::MapUtils::calculatePoint(matrixIndexTopRight, map->mapRectSize);
    destinyStory::Point3D pointTopRight3D(pointTopRight.x, 0, pointTopRight.y);
    
    destinyStory::MatrixIndex * matrixIndexBottomLeft = destinyStory::MapUtils::calculateMatrixIndexByMap(0, map->matrix->row - 1, map);
    cocos2d::Point pointBottomLeft = destinyStory::MapUtils::calculatePoint(matrixIndexBottomLeft, map->mapRectSize);
    destinyStory::Point3D pointBottomLeft3D(pointBottomLeft.x, 0, pointBottomLeft.y);
    
    destinyStory::MatrixIndex * matrixIndexBottomRight = destinyStory::MapUtils::calculateMatrixIndexByMap(map->matrix->col - 1, map->matrix->row - 1, map);
    cocos2d::Point pointBottomRight = destinyStory::MapUtils::calculatePoint(matrixIndexBottomRight, map->mapRectSize);
    destinyStory::Point3D pointBottomRight3D(pointBottomRight.x, 0, pointBottomRight.y);
    
    cocos2d::Point screenTopLeft = IsoMapUtils::isoToScreen(pointTopLeft3D);
    screenTopLeft = CoordinateUtils::classicalToCocos(screenTopLeft);
    cocos2d::Point screenTopRight = IsoMapUtils::isoToScreen(pointTopRight3D);
    screenTopRight = CoordinateUtils::classicalToCocos(screenTopRight);
    cocos2d::Point screenBottomLeft = IsoMapUtils::isoToScreen(pointBottomLeft3D);
    screenBottomLeft = CoordinateUtils::classicalToCocos(screenBottomLeft);
    cocos2d::Point screenBottomRight = IsoMapUtils::isoToScreen(pointBottomRight3D);
    screenBottomRight = CoordinateUtils::classicalToCocos(screenBottomRight);
    
    CHECK_GL_ERROR_DEBUG();
    
    glLineWidth(1.0f);
    cocos2d::DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
    cocos2d::DrawPrimitives::drawLine(screenTopLeft, screenTopRight);
    cocos2d::DrawPrimitives::drawLine(screenTopRight, screenBottomRight);
    cocos2d::DrawPrimitives::drawLine(screenBottomRight, screenBottomLeft);
    cocos2d::DrawPrimitives::drawLine(screenBottomLeft, screenTopLeft);
    
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
/******IsoMapUtils**********/
cocos2d::Point IsoMapUtils::isoToScreen(destinyStory::Point3D point3d)
{
    float screenX = point3d.x - point3d.z;
    float screenY = point3d.y * Y_CORRECT + (point3d.x + point3d.z) * .5;
    return cocos2d::Point(screenX, screenY);
}

destinyStory::Point3D IsoMapUtils::screenToIso(cocos2d::Point point)
{
    float x = point.y + point.x * .5;
    float y = 0;
    float z = point.y - point.x * .5;
    return destinyStory::Point3D(x, y, z);
}

float IsoMapUtils::isoToScreenZ(destinyStory::Point3D point3d)
{
    return (point3d.x + point3d.z) * .866 - point3d.y * .707;
}