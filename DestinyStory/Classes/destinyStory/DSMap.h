//
//  DSMap.h
//  DestinyStory
//
//  Created by RockLee on 14-8-23.
//
//

#ifndef __DestinyStory__DSMap__
#define __DestinyStory__DSMap__

#include "destinyStory.h"
#include "DSGeometry.h"

NS_DS_BEGIN

class NodeItem : public cocos2d::Ref
{
public:
    NodeItem(MatrixIndex * matrixIndexValue){
        matrixIndex = matrixIndexValue;
        matrixIndex->retain();
    };
    ~NodeItem(){
        matrixIndex->release();
        matrixIndex = nullptr;
    };
    
    MatrixIndex * matrixIndex;
    void setMatrixIndex(MatrixIndex * matrixIndexValue)
    {
        if (matrixIndex != nullptr)
        {
            matrixIndex->release();
        }
        matrixIndex = matrixIndexValue;
        matrixIndex->retain();
    };
    
    /****该节点的消费****/
    int cost;
    
    static NodeItem * create(MatrixIndex * matrixIndexValue)
    {
        NodeItem * nodeItem = new NodeItem(matrixIndexValue);
        if (nodeItem)
        {
            nodeItem->autorelease();
            return nodeItem;
        }
        else
        {
            delete nodeItem;
            nodeItem = nullptr;
            return nullptr;
        }
    };
};

class MapItem : public cocos2d::Ref
{
public:
    MapItem(MatrixIndex * matrixIndexValue, Matrix * matrixValue):mapId(-1){
        matrixIndex = matrixIndexValue;
        matrixIndex->retain();
        matrix = matrixValue;
        matrix->retain();
    };
    ~MapItem(){
        matrixIndex->release();
        matrixIndex = nullptr;
        matrix->release();
        matrix = nullptr;
    };
    
    MatrixIndex * matrixIndex;
    void setMatrixIndex(MatrixIndex * matrixIndexValue)
    {
        if (matrixIndex)
        {
            matrixIndex->release();
        }
        matrixIndex = matrixIndexValue;
        matrixIndex->retain();
    };
    
    Matrix * matrix;
    void setMatrix(Matrix * matrixValue)
    {
        if (matrix)
        {
            matrix->release();
        }
        matrix = matrixValue;
        matrix->retain();
    };
    
    int mapId;
    
    static MapItem * create(MatrixIndex * matrixIndexValue, Matrix * matrixValue)
    {
        MapItem * mapItem = new MapItem(matrixIndexValue, matrixValue);
        if (mapItem)
        {
            mapItem->autorelease();
            return mapItem;
        }
        else
        {
            delete mapItem;
            mapItem = nullptr;
            return nullptr;
        }
    };
    
};

class Map : public cocos2d::Ref
{
public:
    Map(Matrix * matrixValue, cocos2d::Size mapRectSizeValue){
        matrix = matrixValue;
        matrix->retain();
        
        mapRectSize = mapRectSizeValue;
    };
    ~Map(){
        matrix->release();
        matrix = nullptr;
    };
    
    Matrix * matrix;
    void setMatrix(Matrix * matrixValue)
    {
        if (matrix)
        {
            matrix->release();
        }
        matrix = matrixValue;
        matrix->retain();
        
        this->initMatrix();
    };
    /*****地图块Size*****/
    cocos2d::Size mapRectSize;
    
    cocos2d::Vector<NodeItem *> nodeItems;
    
    cocos2d::Vector<MapItem *> mapItems;
    
    static Map * create(Matrix * matrixValue, cocos2d::Size mapRectSizeValue)
    {
        Map * map = new Map(matrixValue, mapRectSizeValue);
        if (map && map->init())
        {
            map->autorelease();
            return map;
        }
        else
        {
            delete map;
            map = nullptr;
            return nullptr;
        }
    };
    
    /******获取NodeItem的索引Index****/
    int getNodeItemIndex(int colIndex, int rowIndex);
    /******获取NodeItem******/
    NodeItem * getNodeItem(int colIndex, int rowIndex);
    
    /******根据colIndex, rowIndex返回相应的MapItem****/
    MapItem * getMapItem(int colIndex, int rowIndex);
    /*****根据MapId获取相应的MapItem*****/
    MapItem * getMapItem(int mapId);
    
    /*****添加MapItem,如果添加成功返回mapItem所在的列表索引值，否则返回-1***/
    int addMapItem(MapItem * mapItem);
    
    void removeMapItem(MapItem * mapItem);
    
    virtual bool init();
protected:
    void initMatrix();
    
    int checkMapItem(int mapId);
};

class MapUtils : public cocos2d::Ref
{
public:
    /****计算nodeItem的index***/
    static int calculateIndex(int colIndex, int rowIndex, Matrix * matrix);
    /****计算MatrixIndex,会new出新MatrixIndex,不推荐使用****/
    static MatrixIndex * calculateMatrixIndex(int index, Matrix * matrix);
    /******这个计算返回Map保存的NodeItem的MatrixIndex*******/
    static MatrixIndex * calculateMatrixIndexByMap(int index, Map * map);
    static MatrixIndex * calculateMatrixIndexByMap(int colIndex, int rowIndex, Map * map);
    /******检测MatrixIndex为焦点，matix为返回的矩阵是否覆盖colIndex, rowIndex****/
    static bool checkOver(Matrix * matrix, MatrixIndex * matrixIndex, int colIndex, int rowIndex);
    /*****检测两个MapItem是否相交*********/
    static bool checkOver(MapItem * mapItem, MapItem * checkMapItem);
    /******计算矩阵索引在地图中的坐标******/
    static cocos2d::Point calculatePoint(MatrixIndex * matrixIndex, cocos2d::Size size);
    
    static cocos2d::Point calculateTopLeft(cocos2d::Point point, cocos2d::Size size);
    static cocos2d::Point calculateTopRight(cocos2d::Point point, cocos2d::Size size);
    static cocos2d::Point calculateBottomLeft(cocos2d::Point point, cocos2d::Size size);
    static cocos2d::Point calculateBottomRight(cocos2d::Point point, cocos2d::Size size);
};

#define SYSTEM_EVENT_MAP_REFRESH            "systemEventMapRefresh"
class MapView : public cocos2d::Layer
{
public:
    MapView(Map * mapValue){
        map = mapValue;
        map->retain();
    };
    ~MapView(){
        map->release();
        map = nullptr;
    };
    
    Map * map;
    void setMap(Map * mapValue)
    {
        if (map) map->release();
        map = mapValue;
        map->retain();
        
        this->renderMap();
    };
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void addViewListeners(){};
    virtual void removeViewListeners(){};
    
    virtual void addContextListeners();
    virtual void removeContextListeners();
    
    void onSystemEventMapRefresh(cocos2d::EventCustom * eventCustom);
    cocos2d::EventListenerCustom * mapRefreshListener;
    
    static MapView * create(Map * mapValue)
    {
        MapView * mapView = new MapView(mapValue);
        if (mapView && mapView->init() != false)
        {
            mapView->autorelease();
            return mapView;
        }
        else
        {
            delete mapView;
            mapView = nullptr;
            return nullptr;
        }
    };
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
protected:
    void renderMap();
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    cocos2d::CustomCommand _customCommand;
};

static const float Y_CORRECT = 1.2247;//

class IsoMapUtils : public cocos2d::Ref
{
public:
    /******iso to 2d******/
    static cocos2d::Point isoToScreen(destinyStory::Point3D point3d);
    /******iso to 2d, zOrder*****/
    static float isoToScreenZ(destinyStory::Point3D point3d);
    /*****2d to iso******/
    static destinyStory::Point3D screenToIso(cocos2d::Point point);
};
NS_DS_END;

#endif /* defined(__DestinyStory__DSMap__) */
