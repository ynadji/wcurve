/***************************************************************************
                          displaylist.h  -  description
                             -------------------
    begin                : Thu Jul 18 2002
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

#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H

#include <qgl.h>
#include <math.h>
#include "vector3d.h"
/**encapsulates the functionality of an OpenGL displayList
  *@author Joseph Ruscio
  */

class DisplayList {
public: 
  DisplayList();
  ~DisplayList();
  /** initializes the display list */
  void newList();
  /** finishes the display list */
  void endList();
  /** calls the display list if it is in the viewing volume */
  bool render();
  /** sets the distance in between vertices, used to calculate the radius */
  void setVertexSpacing(double spacing);
  /** adds the vector to the displaylist, returns false when the display list is full, true otherwise */
  bool addVertex(Vector3D<float> &vec);
  /** adds the vector to the displaylist, and records its Z coordinate for sphere calculations */
  void firstVertex(Vector3D<float> &vec);
  /** Calculates the mathematical boundary of the viewing volume that is used
   to test whether or not a displaylist is rendered. */
  static void extractFrustum();
  /** calculates the number of lists needed for that many vertices */
  static long numLists(long num_verts);

private: // Private attributes
  /** the displaylist id that OpenGL will generate */
  GLuint id;
  /** the radius of the bounding sphere */
  float radius;
  /** the distance in between vertices, used to calculate the radius */
  double vertex_spacing;
  /** the number of vertices the list contains */
  long vertex_count;
  /** holds the six planes that make up the viewing frustum */
  static float frustum[6][4];
  /** The max number of vertices a list can contain. */
  const static long VERTEX_MAX;
  /** the z coordinate of the first vertex in the list */
  float firstZ;
  /** The z coordinate of the shere center */
  float centerZ;

private: // Private methods
  /** returns whether or not the bounding sphere of this displaylist is in the viewing frustum */
  bool sphereInFrustum();
  /** calcuates the center and radius of the sphere */
  void calcSphereCoordinates();
};

#endif
