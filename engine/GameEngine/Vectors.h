//
//  Vectors.hpp
//  GameEngine
//
//  Created by David Lively on 2/10/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef VECTORS_H
#define VECTORS_H


template<typename T>
struct TVector2
{
    T X;
    T Y;
    
    
    TVector2(T x, T y)
    {
        X = x;
        Y = y;
    }

    TVector2() : TVector2(0,0)
    {
        
    }
    
};

template<typename T>
struct TVector3 : public TVector2<T>
{
    T Z;
    
    TVector3(T x, T y, T z) : TVector2<T>(x,y)
    {
        Z = z;
        
    }
    
    TVector3() : TVector3<T>(0,0,0)
    {
        
    }
};

template<typename T>
struct TVector4 : public TVector3<T>
{
    T W;
    
    TVector4(T x, T y, T z, T w) : TVector3<T>(x,y,z)
    {
        W = w;
    }
    
    TVector4() : TVector4<T>(0,0,0,1)
    {
        
    }
};

typedef TVector2<float> Vector2;
typedef TVector3<float> Vector3;
typedef TVector4<float> Vector4;


#endif /* Vectors_hpp */
