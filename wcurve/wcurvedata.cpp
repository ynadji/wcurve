/***************************************************************************
                          wcurvedata.cpp  -  description
                             -------------------
    begin                : Sun Jul 14 2002
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

#include "wcurvedata.h"

WcurveData::WcurveData(){
  //set up the filter as allowing all positions
  for(int i=0; i<FILTER_SIZE; i++)
    m_filter[i] = true;

  m_startNuc = 1;
  m_endNuc   = 2; //We don't know how large a sequence it is
  m_mod   = 1;
  m_k     = 300;
  m_z     = 40;
  m_gamma    = 750.0;

}
WcurveData::WcurveData(int nucCount){
  //set up the filter as allowing all positions
  for(int i=0; i<FILTER_SIZE; i++)
    m_filter[i] = true;

  m_startNuc = 1;
  m_endNuc   = nucCount; //We don't know how large a sequence it is
  m_mod   = 1;
  m_k     = 300;
  m_z     = 40;
  m_gamma    = 750.0;

}
WcurveData::~WcurveData(){
}
/** writes all of the data options to the specified stream */
void WcurveData::outputData(QTextStream * outStream){
  //write the kval, zval, modval, start nuc, and end nuc
  *outStream<<m_k<<" "<<m_mod<<" "<<m_startNuc<<" "<<m_endNuc<<" "
  <<m_z<<" "<<m_gamma<<endl;
  //write the filter
  for(int i = 0; i < FILTER_SIZE; i++)
  {
    //write 12 to a line
    if(!((i+1)%12)){
      *outStream<<endl;
    }
    *outStream<<m_filter[i]<<" ";
  }

  *outStream<<endl;
}
