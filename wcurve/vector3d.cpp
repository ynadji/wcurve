/***************************************************************************
                          vector3d.cpp  -  description
                             -------------------
    begin                : Thu Feb 21 2002
    copyright            : (C) 2002 by Joseph Ruscio
    email                : ruscjos@iit.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "vector3d.h"

template<class T> Vector3D<T>::Vector3D(T x, T y, T z, T w){
  X = x; Y = y; Z = z; W = w;
  rgb[RED] = rgb[GREEN] = rgb[BLUE] = 0.0;
}

template<class T> Vector3D<T>::~Vector3D(){
}
/** vector addition */
template<class T> Vector3D<T> Vector3D<T>::operator+(Vector3D<T> &rhs){
  return Vector3D<T>(X+rhs.X, Y+rhs.Y, Z+rhs.Z);
}
/** vector assignment */
template<class T> void Vector3D<T>::operator=(Vector3D<T> &rhs){
  X = rhs.X; Y = rhs.Y; Z = rhs.Z; W = rhs.W;
  rgb[RED]   = rhs.rgb[RED];
  rgb[GREEN] = rhs.rgb[GREEN];
  rgb[BLUE]  = rhs.rgb[BLUE];
}
/** vector subtraction */
template<class T> Vector3D<T> Vector3D<T>::operator-(Vector3D<T> &rhs){
  return Vector3D<T>(X-rhs.X, Y-rhs.Y, Z-rhs.Z);
}
/** scalar multiplication of a vector */
template<class T> Vector3D<T> Vector3D<T>::operator*(T rhs){
	return Vector3D<T>(X*rhs, Y*rhs, Z*rhs);
}
