/***************************************************************************
                          wcurvelistviewitem.cpp  -  description
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

#include "wcurvelistviewitem.h"

Wcurvelistviewitem::Wcurvelistviewitem(QListViewItem* parent, QString name,
  enum ParserID _id, WcurveDoc* pDoc, WcurveView* pView):
  QListViewItem(parent, name)
{
	doc  = pDoc;
	view = pView;
	id   = _id;
}

Wcurvelistviewitem::~Wcurvelistviewitem(){
}
/** returns the id associated with this listviewitem */
enum ParserID Wcurvelistviewitem::getID(){
  return id;
}
/** returns the WcurveView associated with this listviewitem */
WcurveView* Wcurvelistviewitem::getView(){
  return view;
}
/** returns the document associated with this listviewitem */
WcurveDoc* Wcurvelistviewitem::getDoc(){
  return doc;
}
/** Associates a view with this listviewitem */
void Wcurvelistviewitem::setView(WcurveView* pView){
  view = pView;
}
