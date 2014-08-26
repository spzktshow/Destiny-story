//
//  DSGeometry.h
//  DestinyStory
//
//  Created by RockLee on 14-8-23.
//
//

#ifndef __DestinyStory__DSGeometry__
#define __DestinyStory__DSGeometry__

#include "destinyStory.h"

NS_DS_BEGIN

class MatrixIndex : public cocos2d::Ref
{
public:
    MatrixIndex(int colIndexValue, int rowIndexValue){
        colIndex = colIndexValue;
        rowIndex = rowIndexValue;
    };
    ~MatrixIndex(){};
    
    int colIndex;
    int rowIndex;
    
    static MatrixIndex * create(int colIndexValue, int rowIndexValue)
    {
        MatrixIndex * matrixIndex = new MatrixIndex(colIndexValue, rowIndexValue);
        if (matrixIndex != nullptr)
        {
            matrixIndex->autorelease();
            return matrixIndex;
        }
        else
        {
            delete matrixIndex;
            matrixIndex = nullptr;
            return nullptr;
        }
    };
};

class Matrix : public cocos2d::Ref
{
public:
    Matrix(int colValue, int rowValue){
        col = colValue;
        row = rowValue;
    };
    ~Matrix(){};
    
    int col;
    int row;
    
    static Matrix * create(int colValue, int rowValue)
    {
        Matrix * matrix = new Matrix(colValue, rowValue);
        if (matrix)
        {
            matrix->autorelease();
            return matrix;
        }
        else
        {
            delete matrix;
            matrix = nullptr;
            return nullptr;
        };
    };
};

class Point3D
{
public:
    Point3D(float xValue, float yValue, float zValue){
        x = xValue;
        y = yValue;
        z = zValue;
    };
    ~Point3D(){};
    
    float x;
    float y;
    float z;
};

NS_DS_END;

#endif /* defined(__DestinyStory__DSGeometry__) */
