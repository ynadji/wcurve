/***************************************************************************
                          globaltypes.h  -  description
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

#ifndef GLOBAL_H
#define GLOBAL_H

#define FILTER_SIZE 12
#define GL_PI 3.141592

#define X_POS_DEFAULT 0.0f
#define Y_POS_DEFAULT 0.0f
#define Z_POS_DEFAULT -4000.0f
//#define Z_POS_DEFAULT 0.0f

#define X_ORI_DEFAULT 0.0f
#define Y_ORI_DEFAULT 90.0f
#define Z_ORI_DEFAULT 0.0f

#define Z_STEP   0.0025

#define FOREVER true

#define ORIGIN_STRING "Origin"
#define NOGAP_STRING  "Gap Stripped"
#define REPEAT_STRING "Repeat"
#define PENTA_STRING  "Pentagon"
#define SEQUENCE_STRING "Sequence"

#define WCURVE_DEBUG true
#define DEBUG_STR_DEFAULT "Default reached in switch statement!!"
enum ViewID{NOGAP, REPEATGAP, ORIGINGAP, PENTAGAP, SEQUENCE};
enum {RED, GREEN, BLUE};

#endif
