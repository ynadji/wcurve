/***************************************************************************
                          sequenceview.h  -  description
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

#ifndef SEQUENCEVIEW_H
#define SEQUENCEVIEW_H

#include <qtextview.h>
#include <qpainter.h>
#include "wcurvedoc.h"
#include "wcurveview.h"

/**The text display of a sequence.
  *@author Joseph Ruscio
  */

class SequenceView : public QTextView, public WcurveView{
public: 
	SequenceView();
	~SequenceView();
  /** creates a new QSequenceView */
   SequenceView(WcurveDoc *pDoc, ViewID key, QWidget *parent);
  /** handles printing functionality */
  void print_contents(QPainter p){};
  void updateView(){};
  void refreshView(){};

  /***************HACK****************************/
  /** HACK to avoid multiple inheritance lattice */
  virtual void setCaption(QString str){QTextView::setCaption(str);}
  /** HACK to avoid multiple inheritance lattice */
  virtual bool isActiveWindow(){return QTextView::isActiveWindow();}
  /** HACK to avoid multiple inheritance lattice */
  virtual bool close(){return QTextView::close();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void showMaximized(){QTextView::showMaximized();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void setFocus(){QTextView::setFocus();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void show(){QTextView::show();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void installEventFilter ( const QObject * obj)
    {QTextView::installEventFilter(obj);}
  /** HACK to avoid multiple inheritance lattice */
  virtual void setGeometry( const QRect &rect)
    {QTextView::setGeometry(rect);}

};

#endif
