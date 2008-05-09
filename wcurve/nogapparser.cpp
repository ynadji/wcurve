/***************************************************************************
                          nogapparser.cpp  -  description
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

#include "nogapparser.h"

NoGapParser::NoGapParser(){
	A = new Vector3D<float>(-1.0*gamma, -1.0*gamma, 0.0);
  C = new Vector3D<float>(-1.0*gamma, gamma, 0.0);
  G = new Vector3D<float>(gamma, -1.0*gamma, 0.0);
  T = new Vector3D<float>(gamma, gamma, 0.0);
}
NoGapParser::~NoGapParser(){
	delete A;
  delete C;
  delete G;
  delete T;
}
/** Wcurve algorithm */
bool NoGapParser::convertNucleotide(Vector3D<float> *prevNuc,
                              char nuc, int divisor, Vector3D<float> *newNuc){
  Vector3D<float> *baseVec;
	GLclampf r,g,b;

  /*Determine which base is given and set the baseVector*/
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
				return false;
		default:
				//cout<<"Junk base value provided: "<<nuc<<endl;
				return false;
				break;
	}

  //save the Z value since we merely increment it
  //float tempZ = prevNuc.Z;

	float preX  = prevNuc->X;
	float preY  = prevNuc->Y;
  //float preZ  = prevNuc.Z;

  float baseX  = baseVec->X;
  float baseY  = baseVec->Y;
  //float baseZ  = baseVec.Z;

	// do the math
	float scalar = 1.0/(float)divisor;
	/*
  baseX -= preX;
  baseY -= preY;

  baseX *= scalar;
  baseY *= scalar;

  baseX += preX;
  baseY += preY;
  */
  //Vector3D<float> vec1 = ((*baseVec-prevNuc) * scalar);
  //Vector3D<float> vec2 = prevNuc + vec1;
  //vec2.Z = tempZ + Z_INCREMENT;
	
	//set the color
	newNuc->rgb[RED]   = r;
  newNuc->rgb[BLUE]  = b;
	newNuc->rgb[GREEN] = g;

	//assign the calculated value to the passed in variable
	newNuc->X = preX + ((baseX-preX) * scalar);
  newNuc->Y = preY + ((baseY-preY) * scalar);
  newNuc->Z = prevNuc->Z + z_cal;

  return true;
		
}
/** change the gamma value */
void NoGapParser::setGamma(float newG){
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
/** returns the string "Gap Stripped" */
char* NoGapParser::idString(){
	return " Gap Stripped";
}
