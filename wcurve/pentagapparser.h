/***************************************************************************
                          pentagapparser.h  -  description
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

#ifndef PENTAGAPPARSER_H
#define PENTAGAPPARSER_H

#include "sequenceparser.h"

/**
  *@author Joseph Ruscio
  */

class PentaGapParser : public SequenceParser  {
public: 
	PentaGapParser();
	~PentaGapParser();
  /** Wcurve algorithm */
  bool convertNucleotide(Vector3D<float> *prevNuc, char nuc,
                                 int divisor, Vector3D<float> *newNuc);
  /** change the gamma value */
  void setGamma(float newG);
  /** returns the string "Pentagon" */
  char* idString();

private:
  Vector3D<float> *A;
	Vector3D<float> *G;
  Vector3D<float> *C;
  Vector3D<float> *T;
  Vector3D<float> *GAP;
};

#endif
