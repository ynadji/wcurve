/***************************************************************************
                          sequenceparser.h  -  description
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

#ifndef SEQUENCEPARSER_H
#define SEQUENCEPARSER_H

#include <iostream>
using namespace std;
#include "vector3d.cpp"
#include "globaltypes.h"

//#define Z_INCREMENT 0.1
//#define BOX 750.0
/**
  *@author Joseph Ruscio
  */

class SequenceParser {
public: 
	SequenceParser(int zVal = 400, float gammaVal = 750.0f);
	virtual ~SequenceParser();
  /** Virual method that encapsulates the algorithm. */
  virtual bool convertNucleotide(Vector3D<float> *prevNuc, char nuc,
                                 int divisor, Vector3D<float> *newNuc) = 0;
  /** sets the Z value */
  void setZIncrement(int newZ);
  /** returns the current Z increment */
  int getZIncrement();
  /** sets the gamma value */
  virtual void setGamma(float newG);
  /** returns the gamma value */
  float getGamma();
  /** returns a string identifier that tells what type it is */
  virtual char* idString() = 0;

protected: // Protected attributes
  /** the distance between vertices on the z-axis */
  int z_increment;
	float gamma;
  /** holds z increment value used for calculation */
  double z_cal;
};

#endif
