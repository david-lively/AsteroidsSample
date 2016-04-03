//
//  Vectors.hpp
//  GameEngine
//
//  Created by David Lively on 2/10/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef VECTORS_H
#define VECTORS_H

#include <cmath>
#include <ostream>
#include <string>
#include <minmax.h>

struct Vector2
{
    float X;
    float Y;
    
    Vector2() : X(0), Y(0)
    {
    }
    
    Vector2(float x, float y) : X(x), Y(y)
    {
        
    }
    
};


struct Vector3 : public Vector2
{
    float Z;
    
    Vector3() : Vector2(), Z(0)
    {
        
    }
    
    Vector3(float x, float y, float z) : Vector2(x,y), Z(z)
    {
        
    }
    
    Vector3(float all) : Vector3(all,all,all)
    {
        
    }

	std::string toString()
	{
		return std::to_string(X) + " " + std::to_string(Y) + " " + std::to_string(Z);
	}

	
    
    /// return component-wise minimum of the two vectors.
    static Vector3 Min(const Vector3& left, const Vector3& right)
    {
        return Vector3(
                       min(left.X,right.X)
                       ,min(left.Y,right.Y)
                       ,min(left.Z,right.Z)
                       );
    }
    
    /// return component-wise maximum of the two vectors.
    static Vector3 Max(const Vector3& left, const Vector3& right)
    {
        return Vector3(
                       max(left.X,right.X)
                       ,max(left.Y,right.Y)
                       ,max(left.Z,right.Z)
                       );
    }

	friend Vector3 operator*(const float lval, const Vector3& rval)
	{
		return Vector3(lval * rval.X, lval * rval.Y, lval * rval.Z);
	}

	friend Vector3 operator*(const Vector3& lval, const float rval)
	{
		return rval * lval;
	}
    
    template<typename T>
    inline Vector3 operator* (const T& multiplier) const
    {
        return Vector3(X * multiplier, Y * multiplier, Z * multiplier);
    }
    
    template<typename T>
    inline Vector3& operator*=(const T& rval)
    {
        X *= rval;
        Y *= rval;
        Z *= rval;
        
        return *this;
    }
    
    template<typename T>
    inline Vector3& operator/=(const T& rval)
    {
        X /= rval;
        Y /= rval;
        Z /= rval;
        
        return *this;
    }

    template<typename T>
    inline Vector3& operator+=(const T& rval)
    {
        X += rval.X;
        Y += rval.Y;
        Z += rval.Z;
        
        return *this;
    }

	inline Vector3& operator-=(const float rval)
	{
		X -= rval;
		Y -= rval;
		Z -= rval;

		return *this;

	}

    template<typename T>
    inline Vector3& operator-=(const T& rval)
    {
        X -= rval.X;
        Y -= rval.Y;
        Z -= rval.Z;
        
        return *this;
    }
    
    template<typename T>
    inline Vector3 operator+(const T& right) const
    {
        return Vector3(X + right.X, Y + right.Y, Z + right.Z);
    }
    
    template<typename T>
    inline Vector3 operator-(const T& right) const
    {
        return Vector3(X - right.X, Y - right.Y, Z - right.Z);
    }
    
    

    template<typename T>
    inline Vector3& operator=(const T& rval)
    {
        X = rval;
        Y = rval;
        Z = rval;
        
        return *this;
    }
    
    static const float Dot(const Vector3& left, const Vector3& right)
    {
        return left.Dot(right);
    }
    
    const float Dot(const Vector3& right) const
    {
        return X * right.X + Y * right.Y + Z * right.Z;
    }
    
    const float Length() const
    {
        return sqrt(Dot(*this));
        
    }
    
    inline Vector3 Normalized() const
    {
        float oneOverLength = 1.0f / sqrt(Dot(*this));
        
        return *this * oneOverLength;
    }
    
    inline Vector3& Normalize()
    {
        float oneOverLength = 1.0f / sqrt(Dot(*this));
        
        X *= oneOverLength;
        Y *= oneOverLength;
        Z *= oneOverLength;
        
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);

	Vector3 Cross(const Vector3& vec) const
	{
		Vector3 c;

		c.X = Y * vec.Z - Z * vec.Y;
		c.Y = Z * vec.X - X * vec.Z;
		c.Z = X * vec.Y - Y * vec.X;

		return c;
	}

	static Vector3 Cross(const Vector3& left, const Vector3& right)
	{
		return left.Cross(right);
	}

	static const Vector3 Zero;
	static const Vector3 One;
    
    
    
};

struct Vector4 : public Vector3
{
    float W;
    
    Vector4() : Vector3(), W(1)
    {
        
    }
    
    Vector4(float x, float y, float z, float w) : Vector3(x,y,z)
    {
        W = w;
    }
    
    static float Dot(const Vector4& a, const Vector4& b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
    }
    
    template<typename T>
    inline Vector4& operator*=(const T& rval)
    {
        X *= rval;
        Y *= rval;
        Z *= rval;
        W *= rval;
        
        return *this;
    }

    template<typename T>
    inline Vector4& operator/=(const T& rval)
    {
        return (*this) *= 1.f / rval;
    }
    
    inline Vector4 operator*(float rval)
    {
        return Vector4(X * rval, Y * rval, Z * rval, W * rval);
    }
    
    inline Vector4 operator/(float divisor)
    {
        return (*this) * (1.f / divisor);
    }
    
    inline Vector4 operator-(const Vector4& right) const
    {
        return Vector4(X - right.X, Y - right.Y, Z - right.Z, W - right.W);
    }
    
    inline Vector4 operator+(const Vector4& right) const
    {
        return Vector4(X + right.X, Y + right.Y, Z + right.Z, W + right.W);
    }
    
    const float Dot(const Vector4& right)
    {
        return X * right.X + Y * right.Y + Z * right.Z + W * right.W;
    }

	/// normalize XYZ, setting W to 1. 
	inline Vector4 Normalized3()
	{
		float oneOverLength = 1.f / sqrt(X*X + Y*Y + Z*Z);
		return Vector4(X * oneOverLength, Y * oneOverLength, Z * oneOverLength, 1);
	}
    
    inline Vector3& Normalize()
    {
        float oneOverLength = 1.0f / sqrt(Dot(*this));
        
        X *= oneOverLength;
        Y *= oneOverLength;
        Z *= oneOverLength;
        W *= oneOverLength;
        
        return *this;
    }


    
    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);
    
    
};


#endif /* Vectors_hpp */
