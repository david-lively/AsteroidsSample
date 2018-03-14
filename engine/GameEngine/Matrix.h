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

#include <vector>


class Matrix
{
public:
//    const Matrix Identity;


    /// X basis vector
    float m00=1, m01=0, m02=0, m03=0;
    /// y basis vector
    float m10=0, m11=1, m12=0, m13=0;
    /// z basis vector
    float m20=0, m21=0, m22=1, m23=0;
    /// translation
    float m30=0, m31=0, m32=0, m33=1;
    
    Matrix() :
    m00(1), m01(0), m02(0), m03(0)
    ,m10(0), m11(1), m12(0), m13(0)
    ,m20(0), m21(0), m22(1), m23(0)
    ,m30(0), m31(0), m32(0), m33(1)
    {
        
    }

    Vector4 Transform(const Vector4& v) const;
    Vector3 Transform(const Vector3& v) const
    {
        return (Vector3)Transform(Vector4(v.X, v.Y, v.Z, 1));
    }

    
    /// transformed y axis
    Vector3 Up() { return Vector3(m10, m11, m12); }
    Vector3 Translation() { return Vector3(m30, m31, m32); }
    
    Vector4 GetRow(int index)
    {
        Vector4 result;
        
        void* ptr = this + (index * 4);

        memcpy(&result,ptr, sizeof(Vector4));
        
        return result;
    }
    
    std::vector<Vector4> GetAllRows();
    std::vector<Vector4> GetAllCols();
    
    
    static void CreateRotationX(Matrix& m, float radians);
    static void CreateRotationY(Matrix& m, float radians);
    static void CreateRotationZ(Matrix& m, float radians);
    
    
    static Matrix CreateRotationX(float radians);
    static Matrix CreateRotationY(float radians);
    static Matrix CreateRotationZ(float radians);
    
    static Matrix CreateRotation(float x, float y, float z);
    static Matrix CreateRotation(const Vector3& rotate);
    static Matrix CreateTranslation(float x, float y, float z);
    static Matrix CreateTranslation(const Vector3& position);
    
    static Matrix CreateScale(float x, float y, float z);
    static Matrix CreateScale(float uniformScale);
    static Matrix CreateScale(const Vector3& scale);
    
    static Matrix CreatePerspective(float fov, float aspect, float zNear, float zFar);

	Vector3 GetScaleVector()
	{
		return Vector3(m00, m11, m22) * (1.f / m33) ;
	}

    static void Multiply(const Matrix& left, const Matrix& right, Matrix& result);
    
    static Matrix Multiply(const Matrix& left, const Matrix& right)
    {
        Matrix result;
        
        Multiply(left, right, result);
        
        return result;
    }
    
    Matrix operator*(const Matrix& right)
    {
        return Multiply(*this, right);
    }

	friend Matrix operator*(const Matrix& left, const Matrix& right)
	{
		return Multiply(left, right);
	}
    
    
private:
};

#endif /* Matrix_hpp */








