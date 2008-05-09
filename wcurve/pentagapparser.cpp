/***************************************************************************
                          pentagapparser.cpp  -  description
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

#include "pentagapparser.h"
#include <math.h>

#define C1 cos((2*GL_PI)/5)
#define C2 cos(GL_PI/5)
#define S1 sin((2*GL_PI)/5)
#define S2 sin((4*GL_PI)/5)

PentaGapParser::PentaGapParser(){
	A = new Vector3D<float>(-1.0*gamma*S2, -1.0*gamma*C2, 0.0);
  C = new Vector3D<float>(-1.0*gamma*S1, gamma*C1, 0.0);
  G = new Vector3D<float>(gamma*S2, -1.0*gamma*C2, 0.0);
  T = new Vector3D<float>(gamma*S1, gamma*C1, 0.0);
  GAP = new Vector3D<float>(0.0, gamma, 0.0);
}
PentaGapParser::~PentaGapParser(){
	delete A;
  delete C;
  delete G;
  delete T;
	delete GAP;
}
/** Wcurve algorithm */
bool PentaGapParser::convertNucleotide(Vector3D<float> *prevNuc, char nuc,
                                     int divisor, Vector3D<float> *newNuc){
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
				baseVec = GAP;
				r=0.5;g=0.5;b=0.5;
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
void PentaGapParser::setGamma(float newG){
	gamma = newG;
	
	//update the base vectors
  delete A;
  delete C;
  delete G;
  delete T;
	delete GAP;
	
	A = new Vector3D<float>(-1.0*gamma*S2, -1.0*gamma*C2, 0.0);
  C = new Vector3D<float>(-1.0*gamma*S1, gamma*C1, 0.0);
  G = new Vector3D<float>(gamma*S2, -1.0*gamma*C2, 0.0);
  T = new Vector3D<float>(gamma*S1, gamma*C1, 0.0);
  GAP = new Vector3D<float>(0.0, gamma, 0.0);
}
/** returns the string "Pentagon" */
char* PentaGapParser::idString(){
	return " Pentagon";
}
