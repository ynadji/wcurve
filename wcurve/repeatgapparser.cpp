/***************************************************************************
                          repeatgapparser.cpp  -  description
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

#include "repeatgapparser.h"

RepeatGapParser::RepeatGapParser(){
	A = new Vector3D<float>(-1.0*gamma, -1.0*gamma, 0.0);
  C = new Vector3D<float>(-1.0*gamma, gamma, 0.0);
  G = new Vector3D<float>(gamma, -1.0*gamma, 0.0);
  T = new Vector3D<float>(gamma, gamma, 0.0);
}
RepeatGapParser::~RepeatGapParser(){
	delete A;
  delete C;
  delete G;
  delete T;
}
/** Wcurve algorithm */
bool RepeatGapParser::convertNucleotide(Vector3D<float> *prevNuc,
                              char nuc, int divisor, Vector3D<float> *newNuc){
  Vector3D<float> *baseVec;
  GLclampf r,g,b;
  //Determine which base is given and set the baseVector
	switch(nuc)
  {
    case 'A': case 'a':
				baseVec = A;
				r=1.0;g=0.0;b=0.0;
				break;
    case 'G': case 'g':
				baseVec = G;
				r=0.0;g=1.0;b=0.0;
				break;
    case 'C': case 'c':
				baseVec = C;
				r=0.0;g=0.0;b=1.0;
				break;
    case 'T': case 't':
				baseVec = T;
				r=1.0;g=0.0;b=1.0;
				break;
    case '-':
				//if we get a gap we merely repeat the vector so give back prevVec
				*newNuc = *prevNuc;
        newNuc->Z += z_cal;
        newNuc->rgb[RED]   = 0.5f;
        newNuc->rgb[GREEN] = 0.5f;
        newNuc->rgb[BLUE]  = 0.5f;
				return true;
				break;
		default:
				//cout<<"Junk base value provided: "<<nuc<<endl;
				return false;
				break;
	}

  //save the Z value since we merely increment it
  float tempZ = prevNuc->Z;

	// do the math
	float scalar = 1.0/(float)divisor;
  Vector3D<float> vec1 = (*baseVec-*prevNuc) * scalar;
  Vector3D<float> vec2 = *prevNuc + vec1;
  vec2.Z = tempZ + z_cal;

	//set the color
	vec2.rgb[RED]   = r;
  vec2.rgb[BLUE]  = b;
	vec2.rgb[GREEN] = g;

  //assign the calculated value to the passed in variable
	*newNuc = vec2;
  return true;
}

/** change the gamma value */
void RepeatGapParser::setGamma(float newG){

	gamma = newG;

	//update the base vectors
  delete A;
  delete C;
  delete G;
  delete T;
	
	A = new Vector3D<float>(-1.0*gamma, -1.0*gamma, 0.0);
  C = new Vector3D<float>(-1.0*gamma, gamma, 0.0);
  G = new Vector3D<float>(gamma, -1.0*gamma, 0.0);
  T = new Vector3D<float>(gamma, gamma, 0.0);
}
/** returns the string "Repeat" */
char* RepeatGapParser::idString(){
	return " Repeat";
}
