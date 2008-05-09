/***************************************************************************
                          sequenceparser.cpp  -  description
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

#include "sequenceparser.h"

SequenceParser::SequenceParser(int zVal, float gammaVal){
	z_increment = zVal;
}
SequenceParser::~SequenceParser(){
}
/** sets the Z value */
void SequenceParser::setZIncrement(int newZ){
	z_increment = newZ;
  z_cal = z_increment * Z_STEP;
}
/** returns the current Z increment */
int SequenceParser::getZIncrement(){
	return z_increment;
}
/** returns the gamma value */
float SequenceParser::getGamma(){
	return gamma;
}
/** sets the gamma value */
void SequenceParser::setGamma(float newG){
	gamma = newG;
}
