/***************************************************************************
                          wcurvedata.h  -  description
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

#ifndef WCURVEDATA_H
#define WCURVEDATA_H

#include "globaltypes.h"
#include <qtextstream.h>
/**contains all of the configuration data for a wcurve
  *@author Joseph Ruscio
  */

class WcurveData {
public: 
  WcurveData();
  WcurveData(int nucCount);
  ~WcurveData();
  /** writes all of the data options to the specified stream */
  void outputData(QTextStream * outStream);
  /** the k value (divisor) for the sequence parser */
  int m_k;
  /** the modulus value of this view */
  int m_mod;
  /** the "moving window" filter for the genome sequence */
  bool m_filter[FILTER_SIZE];
  /** the starting nucleotide position */
  int m_startNuc;
  /** the ending nucleotide position */
  int m_endNuc;
  /** the gamma value */
  float m_gamma;
  /** the z increment value */
  int m_z;
};

#endif
