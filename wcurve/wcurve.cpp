/***************************************************************************
                          wcurve.cpp  -  description
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

// Qt includes
#include <qvbox.h>
#include <qaccel.h>

// application specific includes
#include "wcurveview.h"
#include "sequenceview.h"
#include "wcurvedoc.h"
#include "wcurve.h"
#include "plotview.h"
#include "sequenceview.h"

//#include "filenew.xpm"
#include "fileopen.xpm"
#include "filesave.xpm"
#include "buttongroup.xpm"

WcurveApp::WcurveApp()
{
  setCaption(tr("Wcurve " VERSION ) );

	convert = new QString();

  printer = new QPrinter;
  untitledCount=0;
  pDocList = new QList<WcurveDoc>();
  pDocList->setAutoDelete(true);

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initView();
  initActions();
  initMenuBar();
  initToolBar();
  initStatusBar();
  //resize( 450, 400 );
  showMaximized();
  viewToolBar->setOn(true);
  viewStatusBar->setOn(true);

  //we are not in realtime mode initially
  m_realtime = false;

  //we are not in autorotate mode initially either
  m_autoRotate = false;

}

WcurveApp::~WcurveApp()
{
  delete printer;
}

void WcurveApp::initActions()
{
  QPixmap openIcon, saveIcon, newIcon, wizIcon;
  //newIcon = QPixmap(filenew);
  openIcon = QPixmap(fileopen);
  saveIcon = QPixmap(filesave);

  //Your not gonna believe this....
  //The include file used for this icon (buttongroup.xpm) assigns its char array a different name
  //"magick"....might want to think about changing it. I also had to hack the file to make it const
  //vim ~/wcurve/wcurve/buttongroup.xmp to see what I mean
  wizIcon  = QPixmap(magick);


  //fileNew = new QAction(tr("New File"), newIcon, tr("&New"), QAccel::stringToKey(tr("Ctrl+N")), this);
  //fileNew->setStatusTip(tr("Creates a new document"));
  //fileNew->setWhatsThis(tr("New File\n\nCreates a new document"));
  //connect(fileNew, SIGNAL(activated()), this, SLOT(slotFileNew()));

  fileOpen = new QAction(tr("Open File"), openIcon, tr("&Open..."), 0, this);
  fileOpen->setStatusTip(tr("Opens an existing document"));
  fileOpen->setWhatsThis(tr("Open File\n\nOpens an existing document"));
  connect(fileOpen, SIGNAL(activated()), this, SLOT(slotFileOpen()));

  fileSave = new QAction(tr("Save File"), saveIcon, tr("&Save"), QAccel::stringToKey(tr("Ctrl+S")), this);
  fileSave->setStatusTip(tr("Saves the actual document"));
  fileSave->setWhatsThis(tr("Save File.\n\nSaves the actual document"));
  connect(fileSave, SIGNAL(activated()), this, SLOT(slotFileSave()));

  fileSaveAs = new QAction(tr("Save File As"), tr("Save &as..."), 0, this);
  fileSaveAs->setStatusTip(tr("Saves the actual document under a new filename"));
  fileSaveAs->setWhatsThis(tr("Save As\n\nSaves the actual document under a new filename"));
  connect(fileSaveAs, SIGNAL(activated()), this, SLOT(slotFileSaveAs()));

  fileClose = new QAction(tr("Close File"), tr("&Close"), QAccel::stringToKey(tr("Ctrl+W")), this);
  fileClose->setStatusTip(tr("Closes the actual document"));
  fileClose->setWhatsThis(tr("Close File\n\nCloses the actual document"));
  connect(fileClose, SIGNAL(activated()), this, SLOT(slotFileClose()));

  filePrint = new QAction(tr("Print File"), tr("&Print"), QAccel::stringToKey(tr("Ctrl+P")), this);
  filePrint->setStatusTip(tr("Prints out the actual document"));
  filePrint->setWhatsThis(tr("Print File\n\nPrints out the actual document"));
  connect(filePrint, SIGNAL(activated()), this, SLOT(slotFilePrint()));

  fileQuit = new QAction(tr("Exit"), tr("E&xit"), QAccel::stringToKey(tr("Ctrl+Q")), this);
  fileQuit->setStatusTip(tr("Quits the application"));
  fileQuit->setWhatsThis(tr("Exit\n\nQuits the application"));
  connect(fileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

  /*  We currently have no use for Edit Menu functionality
  editCut = new QAction(tr("Cut"), tr("Cu&t"), QAccel::stringToKey(tr("Ctrl+X")), this);
  editCut->setStatusTip(tr("Cuts the selected section and puts it to the clipboard"));
  editCut->setWhatsThis(tr("Cut\n\nCuts the selected section and puts it to the clipboard"));
  connect(editCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

  editCopy = new QAction(tr("Copy"), tr("&Copy"), QAccel::stringToKey(tr("Ctrl+C")), this);
  editCopy->setStatusTip(tr("Copies the selected section to the clipboard"));
  editCopy->setWhatsThis(tr("Copy\n\nCopies the selected section to the clipboard"));
  connect(editCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

  editUndo = new QAction(tr("Undo"), tr("&Undo"), QAccel::stringToKey(tr("Ctrl+Z")), this);
  editUndo->setStatusTip(tr("Reverts the last editing action"));
  editUndo->setWhatsThis(tr("Undo\n\nReverts the last editing action"));
  connect(editUndo, SIGNAL(activated()), this, SLOT(slotEditUndo()));

  editPaste = new QAction(tr("Paste"), tr("&Paste"), QAccel::stringToKey(tr("Ctrl+V")), this);
  editPaste->setStatusTip(tr("Pastes the clipboard contents to actual position"));
  editPaste->setWhatsThis(tr("Paste\n\nPastes the clipboard contents to actual position"));
  connect(editPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));
  */

  viewLineWrap = new QAction(tr("LineWrap"), tr("&Linewrap"), 0,
          this, 0, true);
  viewLineWrap->setOn(false);
  viewLineWrap->setStatusTip(tr("Enables/disables the toolbar"));
  viewLineWrap->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
  connect(viewLineWrap, SIGNAL(toggled(bool)), this, SLOT(slotViewLineWrap(bool)));

  viewPerspective = new QAction(tr("Perspective"), tr("Perspective"), 0, this,
          0, true);
  viewPerspective->setOn(false);
  viewPerspective->setStatusTip(tr("Sets the current projection to Perspective"));
  viewPerspective->setWhatsThis(tr("Perspective\n\n Changes the projection") +
      tr(" to perspective."));
  connect(viewPerspective, SIGNAL(toggled(bool)), this,
          SLOT(slotViewPerspective(bool)));

  viewOrthogonal = new QAction(tr("Orthogonal"), tr("Orthogonal"), 0, this,
      0, true);
  viewOrthogonal->setOn(true);
  viewOrthogonal->setStatusTip(
      tr("Sets the current projection to Orthogonal"));
  viewOrthogonal->setWhatsThis(tr("Orthogonal\n\n Changes the projection") +
      tr(" to orthogonal."));
  connect(viewOrthogonal, SIGNAL(toggled(bool)), this,
          SLOT(slotViewOrthogonal(bool)));

  viewRealTime = new QAction(tr("RealTime"), tr("&RealTime"), 0, this, 0,true);
  viewRealTime->setOn(false);
  viewRealTime->setStatusTip(tr("Enables/disables realtime updates"));
  viewRealTime->setWhatsThis(
          tr("Realtime\n\nEnables/disables realtime updates"));
  connect(viewRealTime, SIGNAL(toggled(bool)), this,
          SLOT(slotViewRealTime(bool)));

  viewAutoRotate = new QAction(tr("AutoRotate"), tr("&AutoRotate"), 0, this, 0,true);
  viewAutoRotate->setOn(false);
  viewAutoRotate->setStatusTip(tr("Enables/disables automatic rotation"));
  viewAutoRotate->setWhatsThis(
          tr("Auto Rotate\n\nEnables/disables automatic rotation"));
  connect(viewAutoRotate, SIGNAL(toggled(bool)), this,
          SLOT(slotViewAutoRotate(bool)));          

  viewWhite = new QAction(tr("White"), tr("White"), 0, this, 0, true);
  viewWhite->setOn(false);
  viewWhite->setStatusTip(tr("Sets the background color to white"));
  viewWhite->setWhatsThis(tr("White\n\n Changes the background to white."));
  connect(viewWhite, SIGNAL(toggled(bool)), this, SLOT(slotViewWhite(bool)));

  viewBlack = new QAction(tr("Black"), tr("Black"), 0, this, 0,true);
  viewBlack->setOn(true);
  viewBlack->setStatusTip(tr("Sets the background to black."));
  viewBlack->setWhatsThis(tr("Black\n\nChanges the background to black."));
  connect(viewBlack, SIGNAL(toggled(bool)), this, SLOT(slotViewBlack(bool)));

  viewToolBar = new QAction(tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
  viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
  viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
  connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

  viewStatusBar = new QAction(tr("Statusbar"), tr("&Statusbar"), 0, this, 0, true);
  viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
  viewStatusBar->setWhatsThis(tr("Statusbar\n\nEnables/disables the statusbar"));
  connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

  //windowNewWindow = new QAction(tr("New Window"), tr("&New Window"), 0, this);
  //windowNewWindow->setStatusTip(tr("Opens a new view for the current document"));
  //windowNewWindow->setWhatsThis(tr("New Window\n\nOpens a new view for the current document"));
  //connect(windowNewWindow, SIGNAL(activated()), this, SLOT(slotWindowNewWindow()));

  windowCascade = new QAction(tr("Cascade"), tr("&Cascade"), 0, this);
  windowCascade->setStatusTip(tr("Cascades all windows"));
  windowCascade->setWhatsThis(tr("Cascade\n\nCascades all windows"));
  connect(windowCascade, SIGNAL(activated()), pWorkspace, SLOT(cascade()));

  windowTile = new QAction(tr("Tile"), tr("&Tile"), 0, this);
  windowTile->setStatusTip(tr("Tiles all windows"));
  windowTile->setWhatsThis(tr("Tile\n\nTiles all windows"));
  connect(windowTile, SIGNAL(activated()), pWorkspace, SLOT(tile()));


  windowAction = new QActionGroup(this, 0, false);
  //windowAction->insert(windowNewWindow);
  windowAction->insert(windowCascade);
  windowAction->insert(windowTile);

  helpAboutApp = new QAction(tr("About"), tr("&About..."), 0, this);
  helpAboutApp->setStatusTip(tr("About the application"));
  helpAboutApp->setWhatsThis(tr("About\n\nAbout the application"));
  connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

  updateShow = new QAction(tr("Update W-curves"), wizIcon, tr("&Update...."), 0, this);
  updateShow->setStatusTip(tr("Updates the selected W-curves"));
  updateShow->setWhatsThis(tr("Update\n\nUpdates the selected W-curves"));
  connect(updateShow, SIGNAL(activated()), this, SLOT(slotUpdateButtonClicked()));

}

void WcurveApp::initMenuBar()
{
  ///////////////////////////////////////////////////////////////////
  // MENUBAR

  ///////////////////////////////////////////////////////////////////
  // menuBar entry pFileMenu
  pFileMenu=new QPopupMenu();
  //fileNew->addTo(pFileMenu);
  fileOpen->addTo(pFileMenu);
  fileClose->addTo(pFileMenu);
  pFileMenu->insertSeparator();
  fileSave->addTo(pFileMenu);
  fileSaveAs->addTo(pFileMenu);
  pFileMenu->insertSeparator();
  filePrint->addTo(pFileMenu);
  pFileMenu->insertSeparator();
  fileQuit->addTo(pFileMenu);

  ///////////////////////////////////////////////////////////////////
  /* menuBar entry editMenu
  pEditMenu=new QPopupMenu();
  editUndo->addTo(pEditMenu);
  pEditMenu->insertSeparator();
  editCut->addTo(pEditMenu);
  editCopy->addTo(pEditMenu);
  editPaste->addTo(pEditMenu);*/

  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  pViewMenu=new QPopupMenu();
  pViewMenu->setCheckable(true);
  viewToolBar->addTo(pViewMenu);
  viewStatusBar->addTo(pViewMenu);
  viewLineWrap->addTo(pViewMenu);
  viewRealTime->addTo(pViewMenu);
  viewAutoRotate->addTo(pViewMenu);

  //projection submenu
  QPopupMenu *pPerspMenu=new QPopupMenu();
  pPerspMenu->setCheckable(true);
  pViewMenu->insertItem("Projection",pPerspMenu);
  QActionGroup *pPerspGrp = new QActionGroup(this, 0, true);
  pPerspGrp->insert(viewOrthogonal);
  pPerspGrp->insert(viewPerspective);
  pPerspGrp->addTo(pPerspMenu);

  //background color submenu
  QPopupMenu *pBackMenu=new QPopupMenu();
  pBackMenu->setCheckable(true);
  pViewMenu->insertItem("Background",pBackMenu);
  QActionGroup *pBackGrp = new QActionGroup(this, 0, true);
  pBackGrp->insert(viewWhite);
  pBackGrp->insert(viewBlack);
  pBackGrp->addTo(pBackMenu);
  ///////////////////////////////////////////////////////////////////
  // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE

  ///////////////////////////////////////////////////////////////////
  // menuBar entry windowMenu
  pWindowMenu = new QPopupMenu(this);
  pWindowMenu->setCheckable(true);
  connect(pWindowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry helpMenu
  pHelpMenu=new QPopupMenu();
  helpAboutApp->addTo(pHelpMenu);
  pHelpMenu->insertSeparator();
  pHelpMenu->insertItem(tr("What's &This"), this, SLOT(whatsThis()), SHIFT+Key_F1);


  menuBar()->insertItem(tr("&File"), pFileMenu);
  //menuBar()->insertItem(tr("&Edit"), pEditMenu);
  menuBar()->insertItem(tr("&View"), pViewMenu);
  menuBar()->insertItem(tr("&Window"), pWindowMenu);
  menuBar()->insertItem(tr("&Help"), pHelpMenu);

}

void WcurveApp::initToolBar()
{
  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
  fileToolbar = new QToolBar(this, "file operations");
  //fileNew->addTo(fileToolbar);
  fileOpen->addTo(fileToolbar);
  fileSave->addTo(fileToolbar);
  updateShow->addTo(fileToolbar);
  fileToolbar->addSeparator();

  //add k value label
  QLabel *label = new QLabel(QString("K Value"), fileToolbar);
  //Add kVal spin box
  pKSlider = new QSlider(1, 600, 1, 100, Horizontal, fileToolbar);
  //create the kval display label
	kLabel = new QLabel(convert->sprintf("%d", pKSlider->value()), fileToolbar);
  connect(pKSlider, SIGNAL(valueChanged(int)), this,
          SLOT(slotKValChanged(int)));

  //add mod value label
  fileToolbar->addSeparator();
  label = new QLabel(QString("Modulus"), fileToolbar);
  //Add modVal spin box
  pModSpinBox = new QSpinBox(1, 3, 1, fileToolbar);
  connect(pModSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(slotModChanged(int)));

  //add gamma value label
  fileToolbar->addSeparator();
  label = new QLabel(QString("Gamma"), fileToolbar);
  //Add gammaVal spin box
  pGammaSpinBox = new QSpinBox(GAMMA_MIN, GAMMA_MAX, GAMMA_STEP, fileToolbar);
  connect(pGammaSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(slotGammaChanged(int)));

	//add start nucleotide  label
  fileToolbar->addSeparator();
  label = new QLabel(QString("Start"), fileToolbar);
	//create the start nucleotide slider
	pStartSlider = new QSlider(1, 100000, 1, 1, Horizontal, fileToolbar);
	//create the start nucleotide display label
	startLabel = new QLabel(convert->sprintf("%d", pStartSlider->value()), fileToolbar);
	connect(pStartSlider, SIGNAL(valueChanged(int)), this, SLOT(slotStartNucChanged(int)));

	
  //add start nucleotide  label
  fileToolbar->addSeparator();
  label = new QLabel(QString("End"), fileToolbar);
	//create the start nucleotide slider
	pEndSlider = new QSlider(1, 10000000, 1, 10000000, Horizontal, fileToolbar);
	//create the start nucleotide display label
	endLabel = new QLabel(convert->sprintf("%d", pEndSlider->value()), fileToolbar);
	connect(pEndSlider, SIGNAL(valueChanged(int)), this, SLOT(slotEndNucChanged(int)));

  //add Z-increment  label
  fileToolbar->addSeparator();
  label = new QLabel(QString("Z Increment"), fileToolbar);
	//create the start nucleotide slider
	pZSlider = new QSlider(1, 400, 1, 40, Horizontal, fileToolbar);
	//create the start nucleotide display label
	zLabel = new QLabel(convert->sprintf("%f", (double)(pZSlider->value()) * Z_STEP),
												fileToolbar);
	connect(pZSlider, SIGNAL(valueChanged(int)), this, SLOT(slotZValChanged(int)));

  //QWhatsThis::whatsThisButton(fileToolbar);
}

void WcurveApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  //STATUSBAR
  statusBar()->message(tr("Ready."));
}

void WcurveApp::initView()
{

  QSplitter* view_back = new QSplitter( this );
  view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

  /***/
  QVBox* controls = new QVBox(view_back);
  controls->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  //view_back->setStretchFactor(controls, 1);

  /* Create the File list view */
  pFileView = new QListView(controls);
  //Make the down arrow visible
  pFileView->setRootIsDecorated(true);
  //set multi-selection mode to false
  pFileView->setSelectionMode(QListView::Single);

  pFileView->addColumn("File");
  connect(pFileView, SIGNAL( currentChanged(QListViewItem *) ), this,
          SLOT(slotViewListSelection(QListViewItem *)));

  //Create the filter checkbox group
  QString *pFilterLabel = new QString();
  QVButtonGroup *pGroup = new QVButtonGroup("Filter", controls);
  for(int i=0; i<FILTER_SIZE;i++)
  {
    pFilter[i] = new QCheckBox(pFilterLabel->sprintf("Nucleotide %d",i+1), pGroup);
    pGroup->insert(pFilter[i]);
  }
  connect(pGroup, SIGNAL(clicked(int)), this, SLOT(slotFilterModified()));

  /* Create the Workspace and set it as the centralwidget */
  pWorkspace = new QWorkspace( view_back );
  setCentralWidget(view_back);
  //any time a window gains focus we need to change the controls
  connect(pWorkspace, SIGNAL(windowActivated(QWidget*)), this,
    SLOT(slotUpdateControls(QWidget*)));
}


void WcurveApp::createClient(WcurveDoc* doc, ViewID key)
{
  WcurveView* w = createView(key, doc);
  QRect viewRect=pWorkspace->rect();
  viewRect.setBottom(viewRect.bottom()/4);
  w->setGeometry(viewRect);

  w->installEventFilter(this);
  w->setCaption(doc->title() + viewKeyString(key));
  doc->addView(w);
  
      /* DON'T WANT TO DO THIS ANYMORE
  // show the very first window in maximized mode
  if ( pWorkspace->windowList().isEmpty() )
    w->showMaximized();
  else
    w->show();          */
    
    w->update();
    w->show();

}

void WcurveApp::openDocumentFile(const char* file)
{
  statusBar()->message(tr("Opening file..."));
  WcurveDoc* doc;
	// check, if document already open. If yes, set the focus to the first view
  for(doc=pDocList->first(); doc > 0; doc=pDocList->next())
  {
    if(doc->pathName()==file)
    {
      WcurveView* view=doc->firstView();	
      view->setFocus();
      return;
     }
  }
  doc = new WcurveDoc();
  pDocList->append(doc);
  doc->newDocument();
  // Creates an untitled window if file is 0
	if(!file)
  {
    untitledCount+=1;
    QString fileName=QString(tr("Untitled%1")).arg(untitledCount);
    doc->setPathName(fileName);
    doc->setTitle(fileName);
  }
  // Open the file
  else
  {
    if(!doc->openDocument(file))
    {
      QMessageBox::critical(this, tr("Error !"),tr("Could not open document !"));
      delete doc;
      return;
    }
  }
  // create the window
  QListViewItem *lvItem = new QListViewItem(pFileView, doc->title());
  QListViewItem *lv[4];
  lv[ORIGINGAP] = new QListViewItem(lvItem, ORIGIN_STRING);
  lv[REPEATGAP] = new QListViewItem(lvItem, REPEAT_STRING);
  lv[PENTAGAP]  = new QListViewItem(lvItem, PENTA_STRING);
  lv[NOGAP]     = new QListViewItem(lvItem, NOGAP_STRING);

  //default to No gap
  createClient(doc);

  //add the textview
  //doc->setSequenceView(new SequenceView(doc, pWorkspace, file));

  statusBar()->message(tr("Ready."));
}

bool WcurveApp::queryExit()
{
  int exit=QMessageBox::information(this, tr("Quit..."),
                                    tr("Do your really want to quit?"),
                                    QMessageBox::Ok, QMessageBox::Cancel);

  if (exit==1)
  {

  }
  else
  {

  };

  return (exit==1);
}

bool WcurveApp::eventFilter(QObject* object, QEvent* event)
{
  if((event->type() == QEvent::Close)&&((WcurveApp*)object!=this))
  {
    QCloseEvent* e=(QCloseEvent*)event;
    WcurveView* pView = resolveViewType((QWidget*)object);
    WcurveDoc* pDoc=pView->getDocument();
    if(pDoc->canCloseFrame(pView))
    {
      pDoc->removeView(pView);
      if(!pDoc->firstView())
			{
        pDocList->remove(pDoc);
			}

      e->accept();
    }
    else
      e->ignore();
  }
  return QWidget::eventFilter( object, event );    // standard event processing
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


/*void WcurveApp::slotFileNew()
{
  statusBar()->message(tr("Creating new file..."));

  openDocumentFile();		

  statusBar()->message(tr("Ready."));
}
*/

void WcurveApp::slotFileOpen()
{
  statusBar()->message(tr("Opening file..."));

  QString fileName = QFileDialog::
    getOpenFileName("/home/genebank","Sequence Files *.seq;;Wcurve Files *.wcv;;Fasta Files (*.fasta *.fna)",this);
  if (!fileName.isEmpty())
  {
     openDocumentFile(fileName);		
  }
  statusBar()->message(tr("Ready."));
}


void WcurveApp::slotFileSave()
{
	//can't save if there is no document
	if(pDocList->isEmpty())
	{
		QMessageBox::critical (this, tr("I/O Error !"), tr("There is no document to save !"));
  	return;
	}

  statusBar()->message(tr("Saving file..."));
	
  WcurveView* m = resolveViewType(pWorkspace->activeWindow());
  if( m )
  {
    WcurveDoc* doc = m->getDocument();
    if(!doc->title().contains(tr(".wcv"))){
      slotFileSaveAs();
    }
    else
      if(!doc->saveDocument(doc->pathName(), m))
        QMessageBox::critical (this, tr("I/O Error !"), tr("Could not save the current document !"));
  }

  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotFileSaveAs()
{
	//can't save if there is no document
	if(pDocList->isEmpty())
	{	
	  QMessageBox::critical (this, tr("I/O Error !"), tr("There is no document to save !"));
    return;
	}

  statusBar()->message(tr("Saving file under new filename..."));
  QString fn = QFileDialog::getSaveFileName("/home","Wcurve Files *.wcv", this);
  //filename must have *.wcv name
	if(fn.find(".wcv") != (fn.length() - 4))
		fn.append(".wcv");

	if (!fn.isEmpty())
  {
    WcurveView* m = resolveViewType(pWorkspace->activeWindow());
    if( m )
    {
      WcurveDoc* doc = m->getDocument();
      if(!doc->saveDocument(fn, m))
      {
         QMessageBox::critical (this, tr("I/O Error !"), tr("Could not save the current document !"));
         return;
      }
      doc->changedViewList();
    }
  }
  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotFileClose()
{
  statusBar()->message(tr("Closing file..."));
	
  WcurveView* m = resolveViewType(pWorkspace->activeWindow());
  if( m )
  {
    WcurveDoc* doc=m->getDocument();
    if(doc->closeDocument()) // is the whole thing going down or a view
		{	
			pDocList->remove(doc);
			delete doc;
		}
  }

  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotFilePrint()
{
  statusBar()->message(tr("Printing..."));
	
  WcurveView* m = resolveViewType(pWorkspace->activeWindow());
  if ( m )
    m->print( printer );

  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotFileQuit()
{ 
  statusBar()->message(tr("Exiting application..."));
  ///////////////////////////////////////////////////////////////////
  // exits the Application
//  if(doc->isModified())
//  {
//    if(queryExit())
//    {
//      qApp->quit();
//    }
//    else
//    {
//
//    };
//  }
//  else
//  {
    qApp->quit();
//  };

  statusBar()->message(tr("Ready."));
}

/*
void WcurveApp::slotEditUndo()
{
  statusBar()->message(tr("Reverting last action..."));
	
  WcurveView* m = (WcurveView*) pWorkspace->activeWindow();
  if ( m )
//   m->undo();

  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotEditCut()
{
  statusBar()->message(tr("Cutting selection..."));
	
  WcurveView* m = (WcurveView*) pWorkspace->activeWindow();
  if ( m )
//  m->cut();	

  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotEditCopy()
{
  statusBar()->message(tr("Copying selection to clipboard..."));

  WcurveView* m = (WcurveView*) pWorkspace->activeWindow();
  if ( m )
//  m->copy();

  statusBar()->message(tr("Ready."));
}

void WcurveApp::slotEditPaste()
{
  statusBar()->message(tr("Inserting clipboard contents..."));
	
  WcurveView* m = (WcurveView*) pWorkspace->activeWindow();
  if ( m )
//   m->paste();

  statusBar()->message(tr("Ready."));
}
*/

void WcurveApp::slotViewToolBar(bool toggle)
{
  statusBar()->message(tr("Toggle toolbar..."));
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off
   if (toggle== false)
  {
    fileToolbar->hide();
  }
  else
  {
    fileToolbar->show();
  };

 statusBar()->message(tr("Ready."));
}

void WcurveApp::slotViewStatusBar(bool toggle)
{
  statusBar()->message(tr("Toggle statusbar..."));
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off
  
  if (toggle == false)
  {
    statusBar()->hide();
  }
  else
  {
    statusBar()->show();
  }

  statusBar()->message(tr("Ready."));
}
/*
void WcurveApp::slotWindowNewWindow()
{
  statusBar()->message(tr("Opening new document view..."));
	
  WcurveView* m = (WcurveView*) pWorkspace->activeWindow();
  if ( m ){
    WcurveDoc* doc = m->getDocument();
    createClient(doc);
  }

  statusBar()->message(tr("Ready."));
}
*/
void WcurveApp::slotHelpAbout()
{
  QMessageBox::about(this,tr("About..."),
                     tr("Wcurve\nVersion " VERSION
                     "\n(c) 2002 by Joseph Ruscio"));
}

void WcurveApp::slotStatusHelpMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar()->message(text, 2000);
}

void WcurveApp::windowMenuAboutToShow()
{
  pWindowMenu->clear();	
  //windowNewWindow->addTo(pWindowMenu);
  windowCascade->addTo(pWindowMenu);
  windowTile->addTo(pWindowMenu);

  if ( pWorkspace->windowList().isEmpty() )
  {
    windowAction->setEnabled(false);
  }
  else
  {
    windowAction->setEnabled(true);
  }

  pWindowMenu->insertSeparator();

  QWidgetList windows = pWorkspace->windowList();
  for ( int i = 0; i < int(windows.count()); ++i )
  {
    int id = pWindowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT( windowMenuActivated( int ) ) );
    pWindowMenu->setItemParameter( id, i );
    pWindowMenu->setItemChecked( id, pWorkspace->activeWindow() == windows.at(i) );
  }
}

void WcurveApp::windowMenuActivated( int id )
{
  QWidget* w = pWorkspace->windowList().at( id );
  if ( w )
    w->setFocus();
}

/** changes all control settings to newly selected view */
void WcurveApp::slotUpdateControls(QWidget *w){

  //do nothing if the window that gained focus isn't a good view
  WcurveView *view = resolveViewType(w);
  if(!view)
    return;

  //change the end vector in the statusbar
  if(PlotView *plot = dynamic_cast<PlotView*>(view)) {
    Vector3D<float> last = plot->getLastVec();
    statusBar()->message(
      convert->sprintf("X=%.2f, Y=%.2f, Z=%.2f", last.X,
                        last.Y, last.Z));
  }

  //if the newly selected view has the same parent doc as the last one
  //we need only update the FileView
  if(view->getDocument() == m_activeDoc){
    setListViewSelection(m_activeDoc->title(), viewKeyString(view->key()));
    return;
  }

  //get the new active doc
  m_activeDoc = view->getDocument();
  //select the correct item in the list
  setListViewSelection(m_activeDoc->title(), viewKeyString(view->key()));

  //Get the data for this doc
  WcurveData data = m_activeDoc->getModelData();

  //cout<<"In updateControls ";
  //v->printID();

  //update the spin boxes
  pKSlider->setValue(data.m_k);
  pModSpinBox->setValue(data.m_mod);

  //update the filter
  for(int i=0; i<FILTER_SIZE;i++)
    pFilter[i]->setChecked(data.m_filter[i]);

  //update the start and end nucs
  pStartSlider->setMaxValue(m_activeDoc->nucCount());
  pStartSlider->setValue(data.m_startNuc);
  startLabel->setText(convert->sprintf("%d",data.m_startNuc));

  pEndSlider->setMaxValue(m_activeDoc->nucCount());
  pEndSlider->setValue(data.m_endNuc);
  endLabel->setText(convert->sprintf("%d", data.m_endNuc));

  //update the z-increment slider
  pZSlider->setValue(data.m_z);
  zLabel->setText(convert->sprintf("%f",data.m_z*Z_STEP));

  //update the gamma spin box
  pGammaSpinBox->setValue((int)data.m_gamma);

}

/** change the focus window by selecting a view from the list */
void WcurveApp::slotViewListSelection(QListViewItem* item){

 //First things first set the selected item to be selected
 pFileView->setCurrentItem(item);

 //Get the parent of the selected listview item to determine
 //which document the view is associated with
 QListViewItem *parent = item->parent();

 //if parent is null then the selected view was a document title
 //this is where we will display the QTextview
  WcurveDoc *doc;
  ViewID key;
  if(parent == NULL){
    //ADD CODE TO DISPLAY SEQUENCE STRING
    doc = findDoc(item->text(0));
    key = SEQUENCE;
  }
  else{
    doc = findDoc(parent->text(0));
    key = viewKey(item->text(0));
  }

  WcurveView *view = doc->view(key);

  if(view == NULL){
    createClient(doc, key);
  }
  else
    view->setFocus();
}
/** changes the filter value by clicking on the checkboxes */
void WcurveApp::slotFilterModified(){
  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  //get the new filter values
  bool filterVal[FILTER_SIZE];
  for(int i=0;i<FILTER_SIZE;i++)
  filterVal[i] = pFilter[i]->isChecked();

  //pass the new filter to the parent document of the active view
  m_activeDoc->setFilter(filterVal, m_realtime);
}
/** change the starting nucleotide */
void WcurveApp::slotStartNucChanged(int newVal){
  //reflect the change
  startLabel->setText(convert->sprintf("%d",newVal));

  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  //if the new value fails revert to the old ones
  if(!m_activeDoc->setStartNuc(newVal, m_realtime)) {
    WcurveData data = m_activeDoc->getModelData();
    pStartSlider->setValue(data.m_startNuc);
    startLabel->setText(convert->sprintf("%d",data.m_startNuc));
  }
}
/** change the ending nucleotide */
void WcurveApp::slotEndNucChanged(int newVal){
  //reflect the change
  endLabel->setText(convert->sprintf("%d",newVal));

  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  //if the new value fails revert to the old ones
  if(!m_activeDoc->setEndNuc(newVal, m_realtime)){
    WcurveData data = m_activeDoc->getModelData();
    pEndSlider->setValue(data.m_endNuc);
    endLabel->setText(convert->sprintf("%d",data.m_endNuc));
  }
}

/** changes the modulus value of the algorithm */
void WcurveApp::slotModChanged(int newVal){
  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  m_activeDoc->setModVal(newVal, m_realtime);
}

/** changes the k value (divisor) of an algorithm */
void WcurveApp::slotKValChanged(int newVal){
  kLabel->setText(convert->sprintf("%d", newVal));

  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  m_activeDoc->setKVal(newVal, m_realtime);
}

/** method to produce different types of SequenceParser's (factory method pattern) */
WcurveView* WcurveApp::createView(enum ViewID key, WcurveDoc *doc){

  switch(key) {
    case NOGAP:
      return ((WcurveView *)
        new PlotView(new NoGapParser(), key, doc, pWorkspace,0,
                     WDestructiveClose));

    case REPEATGAP:
      return ((WcurveView *)
        new PlotView(new RepeatGapParser(), key, doc, pWorkspace,0,
                     WDestructiveClose));

    case ORIGINGAP:
      return ((WcurveView *)
        new PlotView(new OriginGapParser(), key, doc,pWorkspace,0,
                     WDestructiveClose));

    case PENTAGAP:
      return ((WcurveView*)
        new PlotView(new PentaGapParser(), key, doc, pWorkspace,0,
                     WDestructiveClose));

    case SEQUENCE:
      return ((WcurveView*)
        new SequenceView(doc, key, pWorkspace));

    default:
      if(WCURVE_DEBUG)
        QMessageBox::critical(this, tr("WcurveApp::createView"),tr(DEBUG_STR_DEFAULT));
      return ((WcurveView *)new PlotView(new NoGapParser(), key, doc, pWorkspace,0,
                     WDestructiveClose));
  }
}

/** handles a change in the Z-increment */
void WcurveApp::slotZValChanged(int val){
  zLabel->setText(convert->sprintf("%f", (double) val * Z_STEP));

  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  m_activeDoc->setZVal(val, m_realtime);
}

/** handles a modification to the pGamma spin box */
void WcurveApp::slotGammaChanged(int newVal){
  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

  //Need to avoid that nasty divide by zero exception
  if(newVal == 0)
    newVal = 1;

  m_activeDoc->setGammaVal(newVal, m_realtime);
}

/** handles the wizard button click event */
void WcurveApp::slotUpdateButtonClicked(){

  //if there are no docs, nobody cares
  if(pDocList->isEmpty())
    return;

   m_activeDoc->propagate();
}


/** changes the Line wrap status of the active window */
void WcurveApp::slotViewLineWrap(bool toggle){
  //if there are no docs, nobody cares
  if(pDocList->isEmpty()){
    viewLineWrap->setOn(false);
    return;
  }
  if(viewLineWrap->isOn()){
    PlotView::setProjection(ORTHOGRAPHIC);
    PlotView::setLineWrap(true);
  }
  else{
    PlotView::setLineWrap(false);
  }
  updateAllViews();
}

/** Returns a numeric key based upon the string */
ViewID WcurveApp::viewKey(QString type){

  if(type == ORIGIN_STRING)
    return ORIGINGAP;
  else if(type == PENTA_STRING)
    return PENTAGAP;
  else if(type == NOGAP_STRING)
    return NOGAP;
  else if(type == REPEAT_STRING)
    return REPEATGAP;
  else if(type == SEQUENCE_STRING)
    return SEQUENCE;
  else
    if(WCURVE_DEBUG)
        QMessageBox::critical(this, tr("WcurveApp::viewKey"),tr(DEBUG_STR_DEFAULT));

  //execution should never reach this point!!
  return NOGAP;
}
/** converts a view key to its string representation */
QString WcurveApp::viewKeyString(ViewID key){
  switch(key) {
    case ORIGINGAP:return QString(ORIGIN_STRING);
    case PENTAGAP :return QString(PENTA_STRING);
    case REPEATGAP:return QString(REPEAT_STRING);
    case NOGAP    :return QString(NOGAP_STRING);
    case SEQUENCE :return QString(SEQUENCE_STRING);
    default:
      if(WCURVE_DEBUG)
        QMessageBox::critical(this, tr("WcurveApp::viewKeyString"),tr(DEBUG_STR_DEFAULT));
      return QString(NOGAP_STRING);
  }
}
/** matches the parameters to the viewlistitem that they uniquely determine and selects it */
void WcurveApp::setListViewSelection(QString parent_text, QString child_text){
  QListViewItem *item = pFileView->firstChild();

  //handle error case
  if(item == NULL) {
    if(WCURVE_DEBUG)
      QMessageBox::critical(this, tr("WcurveApp::setListViewSelection"),
                            tr("Empty listview!!"));
    return;
  }

  //find the parent item
  do{
    if(item->text(0) == parent_text)
      break;
  }while((item = item->nextSibling()) != NULL);

  //handle error case
  if(item == NULL) {
    if(WCURVE_DEBUG)
      QMessageBox::critical(this, tr("WcurveApp::setListViewSelection"),
                            tr("Invalid Parent Name"));
    return;
  }

  if(child_text != SEQUENCE_STRING) {
    //isolate the child that matches the text
    item = item->firstChild();
    do{
      if(item->text(0) == child_text)
        break;
    }while((item = item->nextSibling()) != NULL);

    //handle error case
    if(item == NULL) {
      if(WCURVE_DEBUG)
        QMessageBox::critical(this, tr("WcurveApp::setListViewSelection"),
                              tr("Invalid Child Name"));
      return;
    }
  }

  //Whew!! we have the item and can set it to be the current item
  pFileView->setCurrentItem(item);
}
/** handles the realtime menu option */
void WcurveApp::slotViewRealTime(bool toggled){
  m_realtime = toggled;
}
/** handles the auto rotate menu option */
void WcurveApp::slotViewAutoRotate(bool toggled){
  m_autoRotate = toggled;
  PlotView::setAutoRotate(toggled);
  if(toggled) startTimer(50);
  else killTimers();
}    
/** called when the perspective menu option is clicked */
void WcurveApp::slotViewPerspective(bool toggled){
  if(!toggled)
    return;

  PlotView::setProjection(PERSPECTIVE);
  updateAllViews();
}
/** called when orthogonal is selected */
void WcurveApp::slotViewOrthogonal(bool toggled){
  if(!toggled)
    return;

  PlotView::setProjection(ORTHOGRAPHIC);
  updateAllViews();

}
/** Executes rotation animation in each view */
void WcurveApp::timerEvent(QTimerEvent *){
  cout << "TIMER" << endl;
  PlotView::rotateIncrement();
  refreshAllViews();
}

/** Calls updateViews on every document in the list. */
void WcurveApp::updateAllViews(){

  if(pDocList->isEmpty())
    return;

   pDocList->first();

   do{
      pDocList->current()->updateAllViews();
   }while( (pDocList->next()) != NULL);
}

/** Calls refreshAllViews on every document in the list. */
void WcurveApp::refreshAllViews(){
  if(pDocList->isEmpty())
    return;

  pDocList->first();

  do{
    pDocList->current()->refreshAllViews();
  } while ((pDocList->next()) != NULL);
}
/** changes the background color to white */
void WcurveApp::slotViewWhite(bool toggled){
  if(!toggled)
    return;

  //Change all the views to white
  PlotView::setBackgroundColor(WHITE);
  updateAllViews();
}
/** Changes the background color to black */
void WcurveApp::slotViewBlack(bool toggled){
    if(!toggled)
    return;

  //Change all the views to black
  PlotView::setBackgroundColor(BLACK);
  updateAllViews();
}
/** takes a Qwidget and performs the necessary hocus-pocus to turn it into a WcurveView */
WcurveView* WcurveApp::resolveViewType(QWidget *view){

  //do nothing if the window that gained focus isn't a curve view
  if(PlotView *plot = dynamic_cast<PlotView*>(view))
    return (WcurveView *) plot;
  else if(SequenceView *seq = dynamic_cast<SequenceView*>(view))
    return (WcurveView *) seq;
  else
    return NULL;

}
/** returns the WcurveDoc whose title matches the passed in string */
WcurveDoc * WcurveApp::findDoc(QString title){
  WcurveDoc *doc = pDocList->first();
  do{
    if(doc->title() == title)
      return doc;
  }while((doc = pDocList->next()) != NULL);

  //ERROR CONDITION....NEED AN ERROR PRINTOUT
  return pDocList->first();
}
