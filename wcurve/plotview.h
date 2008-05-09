/***************************************************************************
                          plotview.h  -  description
                             -------------------
    begin                : Mon Sep 2 2002
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

#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <wcurveview.h>
#include <qwidget.h>
#include <qgl.h>
#include <GL/glu.h>
#include <qlistview.h>
#include <qpoint.h>
#include <qtextstream.h>
#include <qprogressdialog.h>
#include <qprinter.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qlist.h>

#include <vector>
#include <math.h>
#include <string.h>
#include "vector3d.h"
#include "sequenceparser.h"
#include "displaylist.h"
#include <CGAL/compiler_config.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>


/* defines */
#define TRAN_AMT 5.0f
#define ROT_AMT 5.0f
#define ZFAR 20000.0f
#define Z_ROT 10.0f
#define GRID_SIZE 1000

#define X_POS_DEFAULT 0.0f
#define Y_POS_DEFAULT 0.0f
#define Z_POS_DEFAULT -4000.0f
//#define Z_POS_DEFAULT 0.0f

#define X_ORI_DEFAULT 0.0f
#define Y_ORI_DEFAULT 90.0f
#define Z_ORI_DEFAULT 0.0f

#define MASK_SUBDIV 50

enum {X, Y, Z};
enum {LEFT, RIGHT, MIDDLE};
enum {PERSPECTIVE, ORTHOGRAPHIC};
enum {BLACK, WHITE};


//typedefs
typedef  CGAL::Cartesian<float>           K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  K::Point_2                      Point;
typedef  CGAL:: Min_circle_2<Traits>::Circle Circle;


struct Camera {
  GLfloat pos[3];
	GLfloat orient[3];
};

/**class used to display the actual W-curve plots
  *@author Joseph Ruscio
  */

class PlotView :public virtual QGLWidget, public virtual WcurveView  {
public:
  PlotView();
  PlotView(SequenceParser *parser, ViewID key, WcurveDoc* pDoc,
               QWidget* parent, const char *name, int wflags);
  ~PlotView();
  /** changes the value of lineWrap */
  static void setLineWrap(bool state);
  /** returns whether or not the view is in a linewrapped state */
  static bool isLineWrapped();
  /** returns the last vector of the sequence */
  Vector3D<float> getLastVec(){ return *last_vec;}
  /** sets the projection to the new value */
  static void setProjection(int proj);
  /** returns the current projection state */
  static int getProjection();
  /** sets the background color for all views to the passed in Qcolor */
  static void setBackgroundColor(int color);
  /** sets the autoRotate flag for animation */
  static void setAutoRotate(bool rotating);
  /** called from timer in WcurveApp */
  static void rotateIncrement();


  protected:
  /** inherited member of QGLWidget that initializes the OpenGL context */
  void initializeGL();
  /** inherited member of QGLwidget responsible for window resizing */
  void resizeGL(int w, int h);
  /** inherited member of QGLWidget that repaints the window */
  void paintGL();
  /** catches all keyboard events for user input */
  void keyReleaseEvent(QKeyEvent* e);
  /** catches all mouse release events */
  void mouseReleaseEvent(QMouseEvent *e);
  /** catches all mouse presses */
  void mousePressEvent(QMouseEvent *);
  /** catches all mouse move events that occur while a button is pressed */
  void mouseMoveEvent(QMouseEvent* e);
  /** handles mouse wheel events */
  void wheelEvent(QWheelEvent *e);
  /** handles printing functionality */
  void print_contents(QPainter p){};
  void updateView();
  /** performs redraw without recalculating wcurve parameters */
  void refreshView();

  /***************HACK****************************/
  /** HACK to avoid multiple inheritance lattice */
  virtual void setCaption(QString str){QGLWidget::setCaption(str);}
  /** HACK to avoid multiple inheritance lattice */
  virtual bool isActiveWindow(){return QGLWidget::isActiveWindow();}
  /** HACK to avoid multiple inheritance lattice */
  virtual bool close(){return QGLWidget::close();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void showMaximized(){QGLWidget::showMaximized();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void setFocus(){QGLWidget::setFocus();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void show(){QGLWidget::show();}
  /** HACK to avoid multiple inheritance lattice */
  virtual void installEventFilter ( const QObject * obj)
    {QGLWidget::installEventFilter(obj);}
  /** HACK to avoid multiple inheritance lattice */
  virtual void setGeometry( const QRect &rect)
    {QGLWidget::setGeometry(rect);}

private:
  // Private methods
  /** compiles the OpenGL display list referenced to displaylist */
  void renderList();
  /** draws the axis */
  void plotAxis();
  /** determines if this nucleotide is included in the current rendering pass */
  bool renderNuc(int idx);
  /** Sets up the GL Projection Matrix (persp or ortho) */
  void initProjection(int w, int h);

  // Private attributes
  /** the algorithm for creating the Vector3D list (Strategy Pattern) */
  SequenceParser* nucParser;
  /** the OpenGL display lists for precompiling rendering code */
  vector<DisplayList*> lists;
  /** holds the mouse position at the last mousePress event */
  int mouseX; int mouseY; int mouseZ;
  /** keeps track of what mouse buttons are pressed */
	bool mousePressed[3];
  /** used to determine whether we are currently printing */
  bool print_state;
  /** determines whether or not window is set to linewrap */
  static bool lineWrap;
  /** flag to determine if rotate animation is on */
  static bool autoRotate;
  /** keeps track of global rotate angle */
  static float autoZ;
  /** the camera object */
  Camera glCam;
  /** the last vector of the sequence*/
  Vector3D<float> *last_vec;
  /** holds the current projection state */
  static int projection;
  /** holds the width that is given when resizeGL() is called */
  int width;
  /** holds the height that is given when resizeGL() is called */
  int height;
  /** holds the clear color used by OpenGL */
  static QColor clear_color;
  double maxY, minY;
  double maxHeight;
  /** holds the coordinate of the center of the bounding cylinder */
  float centerX, centerY;
  /** holds the radius of the bounding cylinder */
  float radius;

};
#endif
