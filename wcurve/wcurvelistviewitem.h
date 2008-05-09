/***************************************************************************
                          wcurvelistviewitem.h  -  description
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

#ifndef WCURVELISTVIEWITEM_H
#define WCURVELISTVIEWITEM_H

using namespace std;

#include <qlistview.h>
#include "wcurveview.h"
#include "wcurvedoc.h"
#include "globaltypes.h"

/**
  *@author Joseph Ruscio
  */

class Wcurvelistviewitem : public QListViewItem  {
public: 
	Wcurvelistviewitem(QListViewItem* parent, QString name, enum ParserID _id,
                     WcurveDoc* pDoc, WcurveView* pView = NULL);

  virtual ~Wcurvelistviewitem();

  /** returns the WcurveView associated with this listviewitem */
  WcurveView* getView();
  /** Associates a view with this listviewitem */
  void setView(WcurveView* pView);
  /** returns the id associated with this listviewitem */
  enum ParserID getID();
  /** returns the document associated with this listviewitem */
  WcurveDoc* getDoc();

private: // Private attributes
  WcurveDoc*    doc;
  WcurveView*   view;
  enum ParserID id;
};

#endif
