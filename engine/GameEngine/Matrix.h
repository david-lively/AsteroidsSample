//
//  Matrix.hpp
//  GameEngine
//
//  Created by David Lively on 2/10/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "Vectors.h"

class Matrix
{
public:
    /// X basis vector
    float m00, m01, m02, m03;
    /// y basis vector
    float m10, m11, m12, m13;
    /// z basis vector
    float m20, m21, m22, m23;
    /// translation
    float m30, m31, m32, m33;
    
    const float* Data() const { return &m00; }
    
    
    Matrix() :
    m00(1), m01(0), m02(0),m03(0)
    ,
    m10(0), m11(1), m12(0),m13(0)
    ,
    m20(0), m21(0), m22(1),m23(0)
    ,
    m30(0), m31(0), m32(0),m33(1)
    {
        
    }
    
    
    static Matrix Identity();
    
    static Matrix CreateRotationZ(float theta);
    static Matrix CreateRotationY(float theta);
    
    static Matrix CreateRotation(float x, float y, float z);
    static Matrix CreateTranslation(float x, float y, float z);
    static Matrix CreateScale(float uniformScale);
    static Matrix CreateScale(float x, float y, float z);
    
    static Matrix CreatePerspective();
    
    

    
    static void Multiply(const Matrix& left, const Matrix& right, Matrix& result);
    
    static Matrix Multiply(const Matrix& left, const Matrix& right)
    {
        Matrix result;
        
        Multiply(left, right, result);
        
        return result;
    }
    
    Matrix operator*(const Matrix& right) const
    {
        return Multiply(*this, right);
    }
    
    
private:
};

#endif /* Matrix_hpp */
