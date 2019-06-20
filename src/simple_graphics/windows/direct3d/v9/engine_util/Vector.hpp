#pragma once
#ifndef SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_UTIL__VECTOR_HPP
#define SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_UTIL__VECTOR_HPP
#include<cmath>

namespace simple_graphics
{
	namespace windows
	{
		namespace direct3d
		{
			namespace v9
			{
				namespace engine_util
				{
					class Vector
					{
					private:
						double x, y, z;
					public:
						//Ĭ�Ϲ��캯��
						inline Vector(
							double x = 0,
							double y = 0,
							double z = 0
						)
						{
							this->x = x;
							this->y = y;
							this->z = z;
						}
						//�������캯��
						inline Vector(const Vector &vector)
						{
							this->x = vector.getX();
							this->y = vector.getY();
							this->z = vector.getZ();
						}

						inline void set(
							const double &x,
							const double &y,
							const double &z
						)
						{
							this->x = x;
							this->y = y;
							this->z = z;
						}
						inline void setX(const double &x)
						{
							this->x = x;
						}
						inline void setY(const double &y)
						{
							this->y = y;
						}
						inline void setZ(const double &z)
						{
							this->z = z;
						}

						inline double getX() const
						{
							return this->x;
						}
						inline double getY() const
						{
							return this->y;
						}
						inline double getZ() const
						{
							return this->z;
						}

						//�����������õ������
						inline Vector operator+(const Vector &vector) const
						{
							return Vector(
								this->x + vector.getX(),
								this->y + vector.getY(),
								this->z + vector.getZ()
							);
						}
						inline Vector operator-(const Vector &vector) const
						{
							return Vector(
								this->x - vector.getX(),
								this->y - vector.getY(),
								this->z - vector.getZ()
							);
						}
						inline Vector operator*(const Vector &vector) const
						{
							return Vector(
								this->x * vector.getX(),
								this->y * vector.getY(),
								this->z * vector.getZ()
							);
						}

						//������ת����
						inline Vector rotate(const Vector &rotation)
						{
							double x, y, z;
							//��x����ת��x�᲻��
							y = this->y*cos(rotation.getX()) - this->z*sin(rotation.getX());
							z = this->y*sin(rotation.getX()) + this->z*cos(rotation.getX());
							this->y = y;
							this->z = z;
							//��y����ת��y�᲻��
							x = this->x*cos(rotation.getY()) + this->z*sin(rotation.getY());
							z = 0 - this->x* sin(rotation.getY()) + this->z*cos(rotation.getY());
							this->x = x;
							this->z = z;
							//��z����ת,z �᲻��
							x = this->x*cos(rotation.getZ()) - this->y*sin(rotation.getZ());
							y = this->x*sin(rotation.getZ()) + this->y*cos(rotation.getZ());
							this->x = x;
							this->y = y;

							return Vector(*this);
						}
					};
				}
			}
		}
	}
}

#endif // !SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_UTIL__VECTOR_HPP