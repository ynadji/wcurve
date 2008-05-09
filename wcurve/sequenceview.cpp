/***************************************************************************
                          sequenceview.cpp  -  description
                             -------------------
    begin                : Sat Aug 31 2002
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

#include "sequenceview.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qfont.h>
SequenceView::SequenceView(){
}

SequenceView::~SequenceView(){
}
/** creates a new QSequenceView */
SequenceView::SequenceView(WcurveDoc *pDoc, ViewID key, QWidget *parent)
      :QTextView(parent), WcurveView(pDoc, key)
{
  //QString filename = doc->pathName();
  QFile fp(WcurveView::doc->pathName());
  fp.open(IO_ReadOnly);

  QTextStream infile(&fp);

  while(!fp.atEnd())
    append(infile.readLine() + "\n");

  setTextFormat(PlainText);
  setPaper(white);
  QFont font("Arial", 16, QFont::Bold);
  setFont(font);
  setCaption(WcurveView::doc->title());
}
