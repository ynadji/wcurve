/***************************************************************************
                          vector3d.h  -  description
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

#ifndef VECTOR3D_H
#define VECTOR3D_H

using namespace std;
#include <GL/gl.h>
#include "globaltypes.h"

/**
  *@author Joseph Ruscio
  */
template<class T>
class Vector3D {
public: 
	Vector3D(T x=(T)(0), T y=(T)(0), T z=(T)(0), T w = (T)(1));
	~Vector3D();
  /** vector adddition */
  Vector3D operator+(Vector3D<T> &rhs);
  /** vector assignment */
  void operator=(Vector3D<T> &rhs);
  /** vector subtraction */
  Vector3D operator-(Vector3D<T> &rhs);
  /** scalar multiplication of a vector */
  Vector3D operator*(T rhs);

  // Public attributes
  /** the x component of the vector. */
  T X;
  /** the y component of the vector */
  T Y;
  /** the z component of the vector */
  T Z;
  /** the w component of the vector */
  T W;
  /** the color of the vector */
	GLclampf rgb[3];
};

#endif
