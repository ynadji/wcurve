/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Thu Feb 21 13:34:41 MST 2002
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

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>

#include "wcurve.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setFont(QFont("helvetica", 12));
  QTranslator tor( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
  tor.load( QString("wcurve.") + QTextCodec::locale(), "." );
  a.installTranslator( &tor );

  WcurveApp *wcurve=new WcurveApp();
  a.setMainWidget(wcurve);

  wcurve->show();

  if(argc>1)
    wcurve->openDocumentFile(argv[1]);
	//else
	  //wcurve->openDocumentFile();
	
  return a.exec();
}
