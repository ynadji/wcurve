/***************************************************************************
                          repeatgapparser.h  -  description
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

#ifndef REPEATGAPPARSER_H
#define REPEATGAPPARSER_H

#include "sequenceparser.h"

/**
  *@author Joseph Ruscio
  */

class RepeatGapParser : public SequenceParser  {
public: 
	RepeatGapParser();
	~RepeatGapParser();
  /** Wcurve algorithm */
  bool convertNucleotide(Vector3D<float> *prevNuc, char nuc,
                                 int divisor, Vector3D<float> *newNuc);
  /** change the gamma value */
  void setGamma(float newG);
  /** returns the string "Repeat" */
  char* idString();

private:
  Vector3D<float> *A;
	Vector3D<float> *G;
  Vector3D<float> *C;
  Vector3D<float> *T;
};

#endif
