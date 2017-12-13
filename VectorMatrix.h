/* 
 * File:   VectorMatrix.h
 * Author: dracula
 * Comments:
 * Revision history: EDITION 0.1
 */

// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_HEADER_VECTOR_MATRIX_H
#define	XC_HEADER_VECTOR_MATRIX_H

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

//#pragma once
#include<stdio.h>
#include<math.h>

class Vector3f
{	
public:
	float x, y, z;
	Vector3f() :x(0), y(0), z(0) {};
	Vector3f(float x, float y, float z) :x(x), y(y), z(z) {};
	Vector3f operator+(Vector3f&v);
	Vector3f operator-(Vector3f&v);
	float operator*(Vector3f&v);
	Vector3f operator*(float s);
	Vector3f operator^(Vector3f&v);
	Vector3f operator=(Vector3f&v);
	float Length();
	void Normalize();
};

class Matrix 
{
public:
	float triMatrix[3][3];
	Matrix()
	{
		int i, j;
		for (i = 0; i<3; i++)
		{
			for (j = 0; j<3; j++)
			{
				triMatrix[i][j] = 0;
			}
		}
	}
	Matrix(float x11, float  x12, float  x13, float  x21, float  x22, float  x23, float  x31, float  x32, float  x33) 
	{
		triMatrix[0][0] = x11;
		triMatrix[0][1] = x12;
		triMatrix[0][2] = x13;
		triMatrix[1][0] = x21;
		triMatrix[1][1] = x22;
		triMatrix[1][2] = x23;
		triMatrix[2][0] = x31;
		triMatrix[2][1] = x32;
		triMatrix[2][2] = x33;
	};
	Matrix(float m[3][3])
	{
		int i, j;
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				triMatrix[i][j] = m[i][j];
			}
		}
	}
	Matrix operator+(Matrix&m);
	Matrix operator-(Matrix&m);
	Matrix operator*(Matrix&m);
	Matrix operator*(float f);
	Vector3f operator*(Vector3f&v);
	float determinant();
	Matrix Inverse();
	Matrix Cofactor();
	//void operator=(Matrix&m);

};

Vector3f OMRS_controller(Vector3f qd, Vector3f dqd, Vector3f ddqd, Vector3f q, Vector3f dq);
Vector3f OMRS_model(Vector3f u, Vector3f q, Vector3f dq);

class Parameter
{
public:
	const float m;
	const float Iv;
	const float r;
	const float Din;
	const float Dout;
	const float La;
	const float I0;
	const float kt;
	const float kb;
	const float n;
	const float b0;
	const float Ra;
	const float beta0;
	const float beta1;
	const float beta2;
	static const float Kp[3][3];
	static const float Kd[3][3];
	Parameter() :m(35), Iv(1.35), r(0.06), Din(0.147), Dout(0.236), La((Din + Dout) / 2), I0(3.15 * pow(10, -5)), kt(0.0292), kb(1 / 34.34), n(186), b0(1.5 * pow(10, -4)), Ra(0.61), beta0(pow(n, 2) * I0 / pow(r, 2)), beta1(pow(n, 2) * (b0 + kt*kb / Ra) / pow(r, 2)), beta2(n*kt / r / Ra){};
};

#endif
