#include "VectorMatrix.h"
Matrix Matrix::operator*(Matrix&m) 
{
	Matrix temp;
	int i, j ;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			temp.triMatrix[i][j] = triMatrix[i][0] * m.triMatrix[0][j] + triMatrix[i][1] * m.triMatrix[1][j] + triMatrix[i][2] * m.triMatrix[2][j];
		}
	}
	return temp;
};

Vector3f Matrix::operator*(Vector3f&v) 
{
	Vector3f temp;
		temp.x = triMatrix[0][0] * v.x + triMatrix[0][1] * v.y + triMatrix[0][2] *v.z;
		temp.y = triMatrix[1][0] * v.x + triMatrix[1][1] * v.y + triMatrix[1][2] * v.z;
		temp.z = triMatrix[2][0] * v.x + triMatrix[2][1] * v.y + triMatrix[2][2] * v.z;
	return temp;
};

Matrix Matrix::operator*(float f)
{
	Matrix temp;
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			temp.triMatrix[i][j] = triMatrix[i][j]*f;
		}
	}
	return temp;
};

Matrix Matrix::operator+(Matrix&m)
{
	Matrix temp;
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			temp.triMatrix[i][j] = triMatrix[i][j] + m.triMatrix[i][j];
		}
	}
	return temp;
};
Matrix Matrix::operator-(Matrix&m)
{
	Matrix temp;
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			temp.triMatrix[i][j] = triMatrix[i][j] - m.triMatrix[i][j];
		}
	}
	return temp;
};

float Matrix::determinant()
{
	return triMatrix[0][0] * triMatrix[1][1] * triMatrix[2][2] + triMatrix[1][0] * triMatrix[2][1] * triMatrix[0][2] + triMatrix[2][0] * triMatrix[0][1] * triMatrix[1][2] -
		triMatrix[2][0] * triMatrix[1][1] * triMatrix[0][2] - triMatrix[1][0] * triMatrix[0][1] * triMatrix[2][2] - triMatrix[0][0] * triMatrix[2][1] * triMatrix[1][2];
};

Matrix Matrix::Cofactor()
{
	Matrix temp;
	temp.triMatrix[0][0] = triMatrix[1][1]* triMatrix[2][2]- triMatrix[1][2]* triMatrix[2][1];
	temp.triMatrix[0][1] = -(triMatrix[0][1] * triMatrix[2][2] - triMatrix[2][1] * triMatrix[0][2]);
	temp.triMatrix[0][2] = triMatrix[0][1] * triMatrix[1][2] - triMatrix[1][1] * triMatrix[0][2];
	temp.triMatrix[1][0] = -(triMatrix[1][0] * triMatrix[2][2] - triMatrix[2][0] * triMatrix[1][2]);
	temp.triMatrix[1][1] = triMatrix[0][0] * triMatrix[2][2] - triMatrix[2][0] * triMatrix[0][2];
	temp.triMatrix[1][2] = -(triMatrix[0][0] * triMatrix[1][2] - triMatrix[1][0] * triMatrix[0][2]);
	temp.triMatrix[2][0] = triMatrix[1][0] * triMatrix[2][1] - triMatrix[2][0] * triMatrix[1][1]; 
	temp.triMatrix[2][1] = -(triMatrix[0][0] * triMatrix[2][1] - triMatrix[2][0] * triMatrix[0][1]);
	temp.triMatrix[2][2] = triMatrix[0][0] * triMatrix[1][1] - triMatrix[1][0] * triMatrix[0][1];

	return temp;
};

Matrix Matrix::Inverse()
{
	float deter = determinant();
	return Cofactor()*(1/deter);
};