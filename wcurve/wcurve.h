/***************************************************************************
                           wcurve.h  -  description
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

#ifndef WCURVE_H
#define WCURVE_H

using namespace std;

#include <iostream>

// include files for QT
#include <qapp.h>
#include <qmainwindow.h>
#include <qaction.h>
#include <qworkspace.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qwhatsthis.h>
#include <qpopupmenu.h>
#include <qaccel.h>
#include <qtoolbutton.h>
#include <qmsgbox.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qsplitter.h>
#include <qvbuttongroup.h>
#include <qcheckbox.h>
#include <qslider.h>

//local includes
#include "globaltypes.h"
#include "sequenceparser.h"
#include "nogapparser.h"
#include "repeatgapparser.h"
#include "origingapparser.h"
#include "pentagapparser.h"
#include "wcurvedata.h"

#define GAMMA_MIN  0
#define GAMMA_MAX  2000
#define GAMMA_STEP 50

// forward declaration of the Wcurve classes
class WcurveDoc;
class WcurveView;

/**
  * This Class is the base class for your application. It sets up the main
  * window and providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class WcurveView is
  * created which creates your view.
  * 	
  * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
  * @version KDevelop version 1.3 code generation
  */

class WcurveApp : public QMainWindow
{
  Q_OBJECT
  
  public:
    /** construtor */
    WcurveApp();
    /** destructor */
    ~WcurveApp();

    /** opens a file specified by commandline option */
    void openDocumentFile(const char *file=0);
  /** matches the parameters to the viewlistitem that they uniquely determine and selects it */
  void setListViewSelection(QString parent_text, QString child_text);


  protected:
    /** overloaded for Message box on last window exit */
    bool queryExit();
    /** event filter to catch close events for MDI child windows and is installed in createClient() on every child window.
      * Closing a window calls the eventFilter first which removes the view from the connected documents' view list. If the
      * last view is going to be closed, the eventFilter() tests if the document is modified; if yes, it asks the user to
      * save the document. If the document title contains "Untitled", slotFileSaveAs() gets called to get a save name and path.
      */
    virtual bool eventFilter(QObject* object, QEvent* event);
    /** creates a new child window. The document that will be connected to it
     * has to be created before and the instances filled, with e.g. openDocument().
     * Then call createClient() to get a new MDI child window.
     * @see WcurveDoc#addView
     * @see WcurveDoc#openDocument
     * @param doc pointer to the document instance that the view will
     * be connected to.
     */
    void createClient(WcurveDoc* doc, ViewID key=NOGAP);

  private slots:

    /** generate a new document in the actual view */
    //void slotFileNew();
    /** open a document */
    void slotFileOpen();
    /** save a document */
    void slotFileSave();
    /** save a document under a different filename*/
    void slotFileSaveAs();
    /** close the actual file */
    void slotFileClose();
    /** print the actual file */
    void slotFilePrint();
    /** exits the application */
    void slotFileQuit();

    /** reverts the last editing action */
    //void slotEditUndo();
    /** put the marked text/object into the clipboard and remove
     * it from the document */
    //void slotEditCut();
    /** put the marked text/object into the clipboard*/
    //void slotEditCopy();
    /** paste the clipboard into the document*/
    //void slotEditPaste();

    /** toggle the toolbar*/
    void slotViewToolBar(bool toggle);
    /** toggle the statusbar*/
    void slotViewStatusBar(bool toggle);
    /** creates a new view for the current document */
    //void slotWindowNewWindow();
    /** shows an about dlg*/
    void slotHelpAbout();
    /** change the status message of the whole statusbar temporary */
    void slotStatusHelpMsg(const QString &text);
    /** gets called when the window menu is activated; recreates the window menu with all opened window titles. */
    void windowMenuAboutToShow();
    /** activates the MDI child widget when it gets selected from the window menu. */
    void windowMenuActivated( int id );
  /** changes all control settings to newly selected view */
  void slotUpdateControls(QWidget *w);
  /** change the ending nucleotide */
  void slotEndNucChanged(int newVal);
  /** change the starting nucleotide */
  void slotStartNucChanged(int newVal);
  /** changes the filter value by clicking on the checkboxes */
  void slotFilterModified();
  /** change the focus window by selecting a view from the list */
  void slotViewListSelection(QListViewItem* item);
  /** changes the modulus value of the algorithm */
  void slotModChanged(int newVal);
  /** changes the k value (divisor) of an algorithm */
  void slotKValChanged(int newVal);
  /** handles a change in the Z-increment */
  void slotZValChanged(int val);
  /** handles a modification to the pGamma spin box */
  void slotGammaChanged(int newVal);
  /** handles the wizard button click event */
  void slotUpdateButtonClicked();
  /** changes the Line wrap status of the active window */
  void slotViewLineWrap(bool toggle);
  /** handles the realtime menu option */
  void slotViewRealTime(bool toggled);
  /** handles the auto rotate menu option */
  void slotViewAutoRotate(bool toggled);
  /** called when orthogonal is selected */
  void slotViewOrthogonal(bool toggled);
  /** called when the perspective menu option is clicked */
  void slotViewPerspective(bool toggled);
  /** Changes the background color to black */
  void slotViewBlack(bool toggled);
  /** changes the background color to white */
  void slotViewWhite(bool toggled);

  private:

    /** initializes all QActions of the application */
    void initActions();
    /** initMenuBar creates the menu_bar and inserts the menuitems */
    void initMenuBar();
    /** this creates the toolbars. Change the toobar look and add new toolbars in this
     * function */
    void initToolBar();
    /** setup the statusbar */
    void initStatusBar();
    /** setup the mainview*/
    void initView();
    /** method to produce different types of WcurveView's (factory method
        pattern) */
    WcurveView* createView(enum ViewID key, WcurveDoc *doc);
    /** Returns a numeric key based upon the string */
    ViewID viewKey(QString type);
    /** converts a view key to its string representation */
    QString viewKeyString(ViewID key);
    /** Calls updateViews on every document in the list. */
    void updateAllViews();
    /** Calls refreshViews on every document in the list. */
    void refreshAllViews();
    /** Executes rotation animation in each view */
    void timerEvent(QTimerEvent *);

    /** takes a Qwidget and performs the necessary hocus-pocus to turn it into
    a WcurveView */
    WcurveView* resolveViewType(QWidget *view);
    /** returns the WcurveDoc whose title matches the passed in string */
    WcurveDoc * findDoc(QString title);

    /** file_menu contains all items of the menubar entry "File" */
    QPopupMenu *pFileMenu;
    /** edit_menu contains all items of the menubar entry "Edit" */
    //QPopupMenu *pEditMenu;
    /** view_menu contains all items of the menubar entry "View" */
    QPopupMenu *pViewMenu;
    /** view_menu contains all items of the menubar entry "Help" */
    QPopupMenu *pHelpMenu;
    /** the window menu */
    QPopupMenu *pWindowMenu;

    QToolBar *fileToolbar;

    /** pWorkspace is the MDI frame widget that handles MDI child widgets. Inititalized in
      * initView()
      */
    QWorkspace *pWorkspace;
    /** the printer instance */
    QPrinter *printer;
    /** a counter that gets increased each time the user creates a new document with "File"->"New" */
    int untitledCount;
    /** a list of all open documents. If the last window of a document gets closed, the installed eventFilter
     * removes this document from the list. The document list is checked for modified documents when the user
     * is about to close the application. */
    QList<WcurveDoc> *pDocList;

    //QAction *fileNew;
    QAction *fileOpen;
    QAction *fileSave;
    QAction *fileSaveAs;
    QAction *fileClose;
    QAction *filePrint;
    QAction *fileQuit;

    QAction *updateShow;

    //QAction *editUndo;
    //QAction *editCut;
    //QAction *editCopy;
    //QAction *editPaste;

    QAction *viewToolBar;
    QAction *viewStatusBar;
    QAction *viewRealTime;
    QAction *viewAutoRotate;
    QAction *viewLineWrap;
    QAction *viewOrthogonal;
    QAction *viewPerspective;
    QAction *viewWhite;
    QAction *viewBlack;

    QActionGroup *windowAction;
    //QAction *windowNewWindow;
    QAction *windowTile;
    QAction *windowCascade;
    QAction *helpAboutApp;
  	/** the main file view that holds the list of currently open files */
  	QListView* pFileView;
  	/** used to select the k value (divisor) */
  	QSlider* pKSlider;
  	/** used to select the starting nucleotide position */
  	QSlider*  pStartSlider;
  	/**  used to select the modulus value */
  	QSpinBox* pModSpinBox;
  	/** used to select the ending nucleotide position */
  	QSlider*  pEndSlider;
  	/** used to set the "sliding window" filter */
  	QCheckBox* pFilter[FILTER_SIZE];
    /** Displays the value of the starting nucleotide slider */
    QLabel *startLabel;
    /** Displays the value of the ending nucleotide slider  */
    QLabel *endLabel;
  	/** Used to access the QString::sprintf() */
  	QString *convert;
    /** displays the current divisor */
    QLabel* kLabel;
    /** allows the user to set the Z-increment between vertices */
    QSlider* pZSlider;
    /** displays the current z increment */
		QLabel* zLabel;
    /** allows the user to vary the gamma value or 'box size' */
    QSpinBox* pGammaSpinBox;
		/** wizard to change all options at once */
    //InfoWizard* pWizard;
  /** button to show the wizard */
  QButton* pUpdateButton;
  /** determines whether control updates are procerssed immediately */
  bool m_realtime;
  /** indicates whether curve should automatically rotate or not */
  bool m_autoRotate;
  /** The parent document of the view that currently has focus */
  WcurveDoc* m_activeDoc;
};
#endif

