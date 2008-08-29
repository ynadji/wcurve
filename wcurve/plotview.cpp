/***************************************************************************
                          plotview.cpp  -  description
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

#include "plotview.h"
#include "wcurvedoc.h"
#include "displaylist.h"

int PlotView::projection = ORTHOGRAPHIC;
bool PlotView::lineWrap = false;
QColor PlotView::clear_color = BLACK;
bool PlotView::autoRotate = false;
float PlotView::autoZ = 0;

PlotView::PlotView(){
}

PlotView::PlotView(SequenceParser *parser, ViewID key, WcurveDoc* pDoc,
QWidget* parent,          const char *name, int wflags)
         :QGLWidget(parent, name, 0, wflags), WcurveView(pDoc, key), nucParser(parser)
{
  //set all mouseButtons to false
  mousePressed[RIGHT]=mousePressed[LEFT]=mousePressed[MIDDLE]=false;

  //default camera position
  glCam.pos[X] = X_POS_DEFAULT;
  glCam.pos[Y] = Y_POS_DEFAULT;
  glCam.pos[Z] = Z_POS_DEFAULT;

  glCam.orient[X] = X_ORI_DEFAULT;
  glCam.orient[Y] = Y_ORI_DEFAULT;
  glCam.orient[Z] = Z_ORI_DEFAULT;

  //grab all key events
  setFocusPolicy(QWidget::StrongFocus);

  //reserve space for the displaylists
  lists.reserve(DisplayList::numLists(doc->nucCount()));

  last_vec = new Vector3D<float>();
  maxY=0;
  minY=0;
}

PlotView::~PlotView(){
}

/** inherited member of QGLWidget that initializes the OpenGL context */
void PlotView::initializeGL(){

  qglClearColor(clear_color);

  //set shade model to flat
  glShadeModel(GL_FLAT);

  /*print out the GL stuff
  printf("\n%s\n",glGetString(GL_VENDOR));
  printf("%s\n",glGetString(GL_RENDERER));
  printf("OpenGL %s\n",glGetString(GL_VERSION));
  printf("Available Extensions:\n");
  printf("%s\n",glGetString(GL_EXTENSIONS));
  printf("%s\n",gluGetString(GLU_EXTENSIONS));
  */

}
/** inherited member of QGLwidget responsible for window resizing */
void PlotView::resizeGL(int w, int h){


  //prevent divide by 0
  if( h==0)
    h=1;

  //save the values for later use
  height = (int)radius*2;
  width = height*w/h;

  //calculate aspect ratio

  //set the viewport
  glViewport(0, 0, w, h);

  //initialize Projection matrix
  initProjection( width, height );
}

/** */
void PlotView::updateView() {
  nucParser->setGamma(m_data.m_gamma);
  nucParser->setZIncrement(m_data.m_z);

  //crunch the numbers
  renderList();
  updateGL();
}

void PlotView::refreshView() {
  //all we gotta do is refresh the screen
  updateGL();
}

/**
 inherited member of QGLWidget that repaints the window */
void PlotView::paintGL(){
  //clear the buffer
  qglClearColor(clear_color);
  glClear(GL_COLOR_BUFFER_BIT);

  //Setup the projection matrix
  initProjection( width, height );

  //reset the matrix
  glLoadIdentity();

  if(autoRotate){
    //position the camera
    glTranslatef(glCam.pos[X]+centerX, glCam.pos[Y]+centerY, glCam.pos[Z]);

    //orient the camera
    glRotatef(glCam.orient[X], 1.0f, 0.0f, 0.0f);
    glRotatef(glCam.orient[Y], 0.0f, 1.0f, 0.0f);
    glRotatef(autoZ, 0.0f, 0.0f, 1.0f);

    //center the rotation
    glTranslatef(-centerX,-centerY,0.0f);
  }
  else{
    //position the camera
    glTranslatef(glCam.pos[X]+centerX, glCam.pos[Y]+centerY, glCam.pos[Z]);

    //orient the camera
    glRotatef(glCam.orient[X], 1.0f, 0.0f, 0.0f);
    glRotatef(glCam.orient[Y], 0.0f, 1.0f, 0.0f);
    glRotatef(glCam.orient[Z], 0.0f, 0.0f, 1.0f);

    //center the rotation
    glTranslatef(-centerX,-centerY,0.0f);
  }
  plotAxis();
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(centerX, centerY, -10000.0f);
  glVertex3f(centerX, centerY, 10000.0f);
  glEnd();

  //Need to determine how many of the display lists to call
  //update the frustum
  DisplayList::extractFrustum();
  //cycle through lists until we find the first one
  vector< DisplayList * >::iterator listptr = lists.begin();

  //The ordering of the statements in these two while statments
  //is set up precisely.....don't screw with them until
  //you know why, refer to the SLLD
  while(!(*listptr++)->render() && listptr != lists.end() );
  //cycle through to the last one
  while(listptr != lists.end() && (*(listptr++))->render());
}
/** compiles the OpenGL display lists */
void PlotView::renderList()  {
    //"bitmask" matrix approximating planar x-y projection
    bool planeMask[MASK_SUBDIV][MASK_SUBDIV];
    memset(planeMask,0,sizeof(planeMask[0][0])*MASK_SUBDIV*MASK_SUBDIV);
    float gammaValue=doc->getGammaVal();
    //value that, when multiplied by coordinate component,
    //gives # between 0 and 49 for indexing in planeMask.
    float rectifier=(float(MASK_SUBDIV-1)/2.0);
    float normalizer=rectifier/gammaValue;
    int xIndex,yIndex;

  //Ensure that we are the current OpenGL rendering context
  makeCurrent();

  //get the nucleotide sequence
  vector<QChar> *nucList = doc->getNucList();

  //push the origin on and grab an iterator to it
  //increment it to the start nucleotide
  int i=m_data.m_startNuc-1;
  vector< QChar >::iterator nucptr = nucList->begin();
  nucptr += i;

  //convert the nucleotides to Vector3D's
  Vector3D<float> *nextVec = new Vector3D<float>(0.0f, 0.0f, 0.0f);
  Vector3D<float> *prevVec = new Vector3D<float>(0.0f, 0.0f, 0.0f);

  //clear the old displaylists
  vector<DisplayList *>::iterator listptr = lists.begin();
  vector<DisplayList *>::iterator listEnd = lists.end();
  for(;listptr !=listEnd; listptr++)
    if(*listptr != NULL) delete *listptr;
  lists.clear();

  DisplayList *curr_list;
  double vertex_spacing = nucParser->getZIncrement()*Z_STEP;
  do {
    //initialize the display list
    curr_list = new DisplayList();
    curr_list->newList();
    curr_list->setVertexSpacing(vertex_spacing);

    //This is ugly but saves mad CPU cycles. Consult the W-curve SLLD document
    //for an explanation. If you can think of a better way (cause I know there
    //is one just can't think of it now) let us know

    //put the first vertex in the list
    for(;i<m_data.m_endNuc;i++,nucptr++){
      //check to see if we draw this one
      if(!renderNuc(i))
        continue;
      //looks like we're good
      else{
        //if this is good for our 'strategy'
        if( nucParser->convertNucleotide(prevVec,
           *nucptr, m_data.m_k, nextVec)){
          curr_list->firstVertex(*nextVec);
          //put into point mask
          xIndex=int(floor(nextVec->X*normalizer+rectifier));
          yIndex=int(floor(nextVec->Y*normalizer+rectifier));
          planeMask[xIndex][yIndex]=true;
           
          *prevVec = *nextVec;
          break;
        }
        //else
          //print error message to debugging utility here
      }
    }

    //need to account for the last iteration of the for loop
    i++;
    nucptr++;

    //now fill up the rest of the list
    for(;i<m_data.m_endNuc;i++,nucptr++){
      //check to see if we draw this one
      if(!renderNuc(i))
        continue;
      //looks like we're good
      else{
        //if this is good for our 'strategy'
        if( nucParser->convertNucleotide(prevVec,
            *nucptr, m_data.m_k, nextVec)){
          bool list_full = curr_list->addVertex(*nextVec);
          *prevVec = *nextVec;

          //record point in mask
          //put into point mask
          xIndex=int(prevVec->X*normalizer+rectifier);
          yIndex=int(prevVec->Y*normalizer+rectifier);
          planeMask[xIndex][yIndex]=true;
           
          if(prevVec->Y > maxY) maxY=prevVec->Y;
          else if(prevVec->Y < minY) minY=prevVec->Y;
          if(list_full)
            break;
        }
        //else
          //print error message to debugging utility here
      }
    }
    //need to account for the last iteration of the for loop
    i++;
    nucptr++;

    //Add our list to the vector
    lists.push_back(curr_list);

  }while(i< m_data.m_endNuc);

  //we're done, manually finish off the last list
  curr_list->endList();

  //save the last vector
  if(last_vec != NULL)
    delete last_vec;

  last_vec = nextVec;

  //calculate the range
  maxHeight=maxY-minY;

  Point* P = new Point[MASK_SUBDIV*MASK_SUBDIV];
  int index=0;
  for(int j=0; j<MASK_SUBDIV; j++) {
    for(int i=0; i<MASK_SUBDIV; i++) {
      if(planeMask[i][j]==true) {
        P[index]=Point((i-rectifier)/normalizer,(j-rectifier)/normalizer);
        index++;
      }
    }
  }
  Min_circle mc1( P, P+index-1, true);
  Circle circ1=mc1.circle();
  centerX=(circ1.center()).x();
  centerY=(circ1.center()).y();
  radius=sqrt(float(circ1.squared_radius()))+(2*gammaValue)/MASK_SUBDIV;

  delete[] P;


  //clean up the heap
  delete prevVec;
}
/** catches all keyboard events for user input */
void PlotView::keyReleaseEvent(QKeyEvent* e){
  switch(e->key())
  {
    case Key_Up:
      glCam.pos[Y] += TRAN_AMT; break;

    case Key_Down:
      glCam.pos[Y] -= TRAN_AMT; break;

    case Key_Right:
      glCam.pos[X] += TRAN_AMT; break;

    case Key_Left:
      glCam.pos[X] -= TRAN_AMT; break;

    case Key_Prior:
      glCam.pos[Z] += TRAN_AMT; break;

    case Key_Next:
      glCam.pos[Z] -= TRAN_AMT; break;

    case Key_W:
      glCam.orient[X] += ROT_AMT; break;

    case Key_S:
      glCam.orient[X] -= ROT_AMT; break;

    case Key_A:
      glCam.orient[Y] -= ROT_AMT; break;

    case Key_D:
      glCam.orient[Y] += ROT_AMT; break;

    case Key_P:
      cout<<"X"<<glCam.pos[X]<<endl
          <<"Y"<<glCam.pos[Y]<<endl
          <<"Z"<<glCam.pos[Z]<<endl
          <<"XO"<<glCam.orient[X]<<endl
          <<"YO"<<glCam.orient[Y]<<endl
          <<"ZO"<<glCam.orient[Z]<<endl;
      break;

    case Key_F1:
     glCam.pos[X] = -375.0f;
     glCam.pos[Y] = 0.0f;
     glCam.pos[Z] = -1500.0f;

     glCam.orient[X] = 0.0f;
     glCam.orient[Y] = 90.0f;
     glCam.orient[Z] = 0.0f;
     break;

    default:
      e->ignore(); break;
    }

		updateGL();
}

/** draws the axis */
void PlotView::plotAxis(){
  glBegin(GL_LINES);
  //X axis is green
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-1000.0f, 0.0f, 0.0f);
  glVertex3f(1000.0f, 0.0f, 0.0f);
  //Y axis is blue
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, -1000.0f, 0.0f);
  glVertex3f(0.0f, 1000.0f, 0.0f);
  //Z axis is red
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, -10000.0f);
  glVertex3f(0.0f, 0.0f, 10000.0f);
  //Plot the grid in grey
  glColor3f(0.5f, 0.5f, 0.5f);
  for(int i = -1 * GRID_SIZE;i<=GRID_SIZE;i+=(GRID_SIZE/10))
  {
    //draw a line along the x-axis
    glVertex3i(-1 * GRID_SIZE, i, 0);
    glVertex3i(GRID_SIZE, i, 0);

    //draw a line along the y-axis
    glVertex3i(i, -1 * GRID_SIZE, 0);
    glVertex3i(i, GRID_SIZE, 0);
  }
  glEnd();

  //plot the origin
  glBegin(GL_LINE_STRIP);
  //plot the Vector3D origin
  glVertex3f(0.0f, 0.0f, 0.0f);
  glEnd();
}

/** catches all mouse presses */
void PlotView::mousePressEvent(QMouseEvent *e){
//only change mouse for the buttons we are interested in
  switch(e->button()){
    case LeftButton:
      mouseX = e->x();
      mouseY = e->y();
      mousePressed[LEFT] = true;
      break;
    case MidButton:
      mouseZ = e->y();
      mousePressed[MIDDLE] = true;
      break;
    case RightButton:
      mouseX = e->x();
      mouseY = e->y();
      mousePressed[RIGHT] = true;
      break;
    default:
    break;
  }
}
/** catches all mouse release events */
void PlotView::mouseReleaseEvent(QMouseEvent *e){	
	
	switch(e->button())
	{
		case LeftButton:
			//change the camera coordinates according to the mouse movement
			glCam.pos[X] += e->x() - mouseX;
			glCam.pos[Y] += mouseY - e->y();
			mousePressed[LEFT] = false;
			break;
		case MidButton:
			glCam.pos[Z] += mouseZ - e->y();
			mousePressed[MIDDLE] = false;
			break;
		case RightButton:
			//change the camera coordinates according to the mouse movement
			glCam.orient[Y] += e->x() - mouseX;
			glCam.orient[X] += e->y() - mouseY;
			mousePressed[RIGHT] = false;
			break;
		default:
			break;
	}

	//reflect the changes
	updateGL();

}
/** catches all mouse move events that occur while a button is pressed */
void PlotView::mouseMoveEvent(QMouseEvent* e){

  if(mousePressed[LEFT]){
    glCam.pos[X] += e->x() - mouseX;
    glCam.pos[Y] += mouseY - e->y();
    mouseX = e->x();
    mouseY = e->y();
  }
  else if(mousePressed[MIDDLE]){
			glCam.pos[Z] += 5*(mouseZ - e->y());
			mouseZ = e->y();
		}
		else if(mousePressed[RIGHT])
		{
			glCam.orient[Y] += e->x() - mouseX;
			glCam.orient[X] += e->y() - mouseY;
			mouseX = e->x();
			mouseY = e->y();
		}
		//make the changes
		updateGL();
}
/** handles mouse wheel events */
void PlotView::wheelEvent(QWheelEvent *e){

  //if we aren't the active window ignore
  if(!QGLWidget::hasFocus())
    return;

  if(e->delta() > 0)
    glCam.orient[Z] += Z_ROT/2; //two events are generated?
  else
    glCam.orient[Z] -= Z_ROT/2; //two events are generated?

  updateGL();
}
/** changes z rotation when autorotate is on */
void PlotView::rotateIncrement()
{
  autoZ += Z_ROT/2; //two events are generated?
}
/** changes the value of lineWrap */
void PlotView::setLineWrap(bool state){
  lineWrap = state;
}
/** determines if this nucleotide is included in the current rendering pass */
bool PlotView::renderNuc(int idx){
  //check if the nucleotide was filtered
  if(!m_data.m_filter[(idx+1)%FILTER_SIZE])
    return false;
  //check for the modulus removal
  else if((idx+1)%m_data.m_mod)
    return false;
  else
    return true;
}
/** returns whether or not the view is in a linewrapped state */
bool PlotView::isLineWrapped(){
  return lineWrap;
}
/** returns the current projection state */
int PlotView::getProjection(){
  return projection;
}
/** sets the projection to the new value */
void PlotView::setProjection(int proj){
  projection = proj;
}

/** Sets up the GL Projection Matrix (persp or ortho) */
void PlotView::initProjection(int w, int h){
  //switch to the projection matrix
  glMatrixMode(GL_PROJECTION);
  //reset the current matrix
  glLoadIdentity();

  if(projection == PERSPECTIVE){
    //set the perspective
    gluPerspective(22.5, (float)w/(float)h, 4.0, ZFAR);
  }
  else{ // equals ORTHOGRAPHIC
    glOrtho(0.0, w, (centerY-h/2), (centerY+h/2), 4.0, ZFAR);
  }

  //switch back to the modelview matrix
  glMatrixMode(GL_MODELVIEW);
}
/** sets the background color for all views to the passed in color */
void PlotView::setBackgroundColor(int color){

  switch(color) {
    case BLACK: clear_color = black; break;
    case WHITE: clear_color = white; break;
    default: clear_color = black; break;
  }
}

/** sets the autoRotate flag */
void PlotView::setAutoRotate(bool rotating){
  autoRotate=rotating;
  cout << "setAutoRotate " << rotating << endl;
}
