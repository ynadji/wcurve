/***************************************************************************
                          wcurveview.cpp  -  description
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "wcurveview.h"
#include "wcurvedoc.h"

WcurveView::WcurveView(WcurveDoc* pDoc, ViewID key)
    :doc(pDoc), m_key(key)
{
  //setCaption(doc->title() + key);
}

WcurveView::WcurveView(){
}

WcurveView::~WcurveView(){
}

WcurveDoc *WcurveView::getDocument() const {
  return doc;
}

void WcurveView::update(WcurveView* pSender){
  if(pSender != this)
    update();
}

/** Called when the model data in the document has changed */
void WcurveView::update(){
  //get the new model data
  m_data = doc->getModelData();

  //call template method that concrete classes implement
  updateView();
}

/** Called when each view needs to be animated */
void WcurveView::refresh(){
  //call template methoid that concrete classes implement
  refreshView();
}

void WcurveView::print(QPrinter *pPrinter)
{
  //if (pPrinter->setup(this))
  //{
  //  QPainter p;
  //  p.begin(pPrinter);

    //call template method that concrete classes implement
  //  print_contents(p);

  //  p.end();
  //}
}

void WcurveView::closeEvent(QCloseEvent*)
{
  // LEAVE THIS EMPTY: THE EVENT FILTER IN THE WcurveApp CLASS TAKES CARE FOR CLOSING
  // QWidget closeEvent must be prevented.
}

