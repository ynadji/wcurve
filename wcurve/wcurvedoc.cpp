/***************************************************************************
                          wcurvedoc.cpp  -  description
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
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qmsgbox.h>
#include <qfiledialog.h>
#include <qlistview.h>


// application specific includes
#include "wcurvedoc.h"
#include "wcurve.h"
#include "wcurveview.h"


WcurveDoc::WcurveDoc()
{
  pViewList = new QList<WcurveView>;
  pViewList->setAutoDelete(false);

	nucList  = new vector< QChar >;
	
}

WcurveDoc::~WcurveDoc()
{
  delete pViewList;
	nucList->clear();
	delete nucList;
}

void WcurveDoc::addView(WcurveView *view)
{
  pViewList->append(view);
	//changedViewList();
}

void WcurveDoc::removeView(WcurveView *view)
{
	  pViewList->remove(view);
	  if(pViewList->isEmpty())
      //changedViewList();
    //else
    deleteContents();
}

void WcurveDoc::changedViewList(){	
	
	WcurveView *w;
	if((int)pViewList->count() == 1){
  	w=pViewList->first();
  	w->setCaption(m_title);
	}
	else{
		int i;
    for( i=1,w=pViewList->first(); w!=0; i++, w=pViewList->next())
  		w->setCaption(QString(m_title+":%1").arg(i));	
	}
}

bool WcurveDoc::isLastView() {
  return ((int) pViewList->count() == 1);
}


void WcurveDoc::updateAllViews(WcurveView *sender)
{
  WcurveView *w;
  for(w=pViewList->first(); w!=0; w=pViewList->next())
  {
     w->update(sender);
  }
}

void WcurveDoc::refreshAllViews()
{
  WcurveView *w;
  cout << "DOC...refreshAllViews" << endl;
  for(w=pViewList->first(); w!=0; w=pViewList->next())
  {
    w->refresh();
  }
}

void WcurveDoc::setPathName(const QString &name)
{
  m_filename=name;
	m_title=QFileInfo(name).fileName();
}

const QString& WcurveDoc::pathName() const
{
  return m_filename;
}

void WcurveDoc::setTitle(const QString &title)
{
  m_title=title;
}

const QString &WcurveDoc::title() const
{
  return m_title;
}


bool WcurveDoc::closeDocument()
{
  WcurveView *w;
  if(!isLastView())
  {
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
   	 	if(w->isActiveWindow())
			{
				//((Wcurvelistviewitem*)(w->getLvItem()))->setView(0);
				w->close();
 				break;
			}
    }
		return false;
	}
  if(isLastView()) //close the whole document
  {
  	w=pViewList->first();
  	w->close();
		return true;
  }
	
	//shouldn't ever get here
	cout<<"Bad area reached in WcurveDoc::closeDocument()"<<endl;
	return false;
}

bool WcurveDoc::newDocument()
{
  /////////////////////////////////////////////////
  // TODO: Add your document initialization code here
  /////////////////////////////////////////////////
  modified=false;
  return true;
}

bool WcurveDoc::openDocument(const QString &filename, const char *format /*=0*/)
{

	QFile f( filename );
	if ( !f.open( IO_ReadOnly ) )
		return false;
  /////////////////////////////////////////////////
  // TODO: Add your document opening code here
  //determine if this is a .seq or a .wcv
  if(filename.contains(".seq")){
    parseSeqFile(filename);
    m_modelData.m_endNuc = nucCount();
  }
  else if(filename.contains(".fasta")){
    parseFastaFile(filename);
    m_modelData.m_endNuc = nucCount();
  }
  else
    parseWcvFile(filename);

  /////////////////////////////////////////////////
	f.close();
	
  modified=false;
  m_filename=filename;
	m_title=QFileInfo(f).fileName();
  return true;
}

bool WcurveDoc::saveDocument(const QString &filename, WcurveView *v,
                             const char *format /*=0*/)
{
	QFile f( filename );
	if ( !f.open( IO_WriteOnly ) )
		return false;

  /////////////////////////////////////////////////
  // TODO: Add your document saving code here
  /////////////////////////////////////////////////
	//output stream to write to the file
	QTextStream outFile(&f);

	//first we need to write out the view type
	//outFile<<((Wcurvelistviewitem *)(v->getLvItem()))->getID()<<endl;
	//now have the view dump its options
	m_modelData.outputData(&outFile);
	//now output the sequence size
	outFile<<nucList->size()<<endl;

	//output the sequence itself
  vector< QChar >::iterator first = nucList->begin();
  vector< QChar >::iterator last  = nucList->end();
	int count=0;
	while(first != last)
	{
		//output the character
		outFile<<*(first++);

		//increment the base count
    count++;

		//80 bases to a line
		if(!(count%80))
			outFile<<endl;
	}

	outFile<<endl;

  f.close();

  modified=false;
  m_filename=filename;
	m_title=QFileInfo(f).fileName();
  return true;
}

void WcurveDoc::deleteContents()
{
  /////////////////////////////////////////////////
  // TODO: Add implementation to delete the document contents
  /////////////////////////////////////////////////

}

bool WcurveDoc::canCloseFrame(WcurveView* pFrame)
{ /*
	if(!isLastView())
		return true;
		
	bool ret=false;
  if(isModified())
  {
		QString saveName;
  	switch(QMessageBox::information(pFrame, title(), tr("The current file has been modified.\n"
                          "Do you want to save it?"),QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel ))
    {
			case QMessageBox::Yes:
				if(title().contains(tr("Untitled")))
				{
					saveName=QFileDialog::getSaveFileName(0, 0, pFrame);
                                          if(saveName.isEmpty())
          	                                return false;
				}
				else
					saveName=pathName();

				if(!saveDocument(saveName))
				{
 					switch(QMessageBox::critical(pFrame, tr("I/O Error !"), tr("Could not save the current document !\n"
																												"Close anyway ?"),QMessageBox::Yes ,QMessageBox::No))

 					{
 						case QMessageBox::Yes:
 							ret=true;
 						case QMessageBox::No:
 							ret=false;
 					}
				}
				else
					ret=true;
				break;
			case QMessageBox::No:
				ret=true;
				break;
			case QMessageBox::Cancel:
			default:
				ret=false;
				break;
		}
	}
	else*/
		bool ret=true;

	return ret;
}
/** gives access to a constant version of the genome sequence */
vector<QChar> * WcurveDoc::getNucList(){
  return nucList;
}
/** handles the opening of an .seq file */
void WcurveDoc::parseSeqFile(const QString &filename){
	//open the file for text input
	QFile fp(filename);
	fp.open(IO_ReadOnly);

	//create the textstream
	QTextStream infile(&fp);

	QString line;
	//cycle through the file until we find the string "ORIGIN"
	while((line = infile.readLine()).find("ORIGIN") == -1);

	int strlen, i;
	//grab all of the bases
	while(! fp.atEnd())
	{
		//grab the line and get rid of the whitespace
		line = infile.readLine();
		line = line.stripWhiteSpace();
    line.replace( QRegExp(" "), "" );
		strlen = line.length();

		//need to get rid of the numbers at the beginning
		for(i=0;(line[i]).isDigit();i++);

		//now put all of the bases in the list
		for(;i<strlen;i++)
			nucList->push_back(line[i]);
	}

	fp.close();
}
/** parses a *.wcv file */
void WcurveDoc::parseWcvFile(const QString &filename){
  //open the file for text input
  QFile fp(filename);
  fp.open(IO_ReadOnly);

  //create the textstream
  QTextStream infile(&fp);

  infile>>m_modelData.m_k>>m_modelData.m_mod>>m_modelData.m_startNuc
        >>m_modelData.m_endNuc>>m_modelData.m_z>>m_modelData.m_gamma;

  //get the filter
  int boolean;
  for(int i=0;i<FILTER_SIZE;i++){
    infile>>boolean;
    if(boolean)
      m_modelData.m_filter[i] = true;
    else
      m_modelData.m_filter[i] = false;
  }

  //get the number of nucleotides
  int nucCount;
  infile>>nucCount;

  //reserve enough space for the nucleotides
  nucList->reserve(sizeof(QChar)*nucCount);

  //read them in
  QString line;
  int strlen;
  while(! fp.atEnd()) {
    //grab the line and get rid of the whitespace
    line = infile.readLine();
    line = line.stripWhiteSpace();
    strlen = line.length();

    //now put all of the bases in the list
    for(int i=0;i<strlen;i++)
      nucList->push_back(line[i]);
  }

  fp.close();
}
/** parses fasta format files */
void WcurveDoc::parseFastaFile(const QString &filename){
	//open the file for text input
	QFile fp(filename);
	fp.open(IO_ReadOnly);

	//create the textstream
	QTextStream infile(&fp);

	QString line;
	int strlen;
	//get the first line
	line = infile.readLine();

	//grab all of the bases
	while(! fp.atEnd())
	{
		//grab the line and get rid of the whitespace
		line = infile.readLine();
		line = line.stripWhiteSpace();
		strlen = line.length();

		//now put all of the bases in the list
		for(int i=0;i<strlen;i++)
			nucList->push_back(line[i]);
	}

	fp.close();
}
/** returns the number of nucleotides in the sequence */
int WcurveDoc::nucCount(){
	return nucList->size();
}
/** returns the view associated with the doc that matches the passed in key */
WcurveView* WcurveDoc::view(int key){

  const uint count = pViewList->count();

  for(uint i=0;i<count;i++) {
    if(pViewList->at(i)->key() == key) {
      return pViewList->current();
      break;
    }
  }

   //There is no view with that key
   return NULL;
}

/** Part of the GOF Observer Pattern, informs all attached views that the model data has changed */
void WcurveDoc::notify(){
  WcurveView *v = pViewList->first();

  do{
    v->update();
  }while( (v=pViewList->next()) != NULL);
}

/** returns a copy of the model data for this document */
WcurveData WcurveDoc::getModelData(){
  return m_modelData;
}
/** validates the end nucleotide and notifies the wcurve */
bool WcurveDoc::setEndNuc(int end, bool propagate) {

  //if it is the same as the current value no need to upset
  if(m_modelData.m_endNuc == end)
    return true;

  //if a value greater than the sequence size
  //is passed in pin the value at the last base
  if((unsigned int)end > nucList->size()){
    m_modelData.m_endNuc = nucList->size();
    //Notify the views
    if(propagate)
      notify();
    return false;
  }
  else if(end > m_modelData.m_startNuc) {
    m_modelData.m_endNuc = end;
    if(propagate)
      notify();
    return true;
  }
  else
    return false;
}

/** changes the filter and updates the wcurve */
void WcurveDoc::setFilter(bool new_filter[FILTER_SIZE], bool propagate){
  //check to see if any changes have occurred
  bool modified = false;
  for(int i=0;i<FILTER_SIZE;i++)
    if(m_modelData.m_filter[i] != new_filter[i])
      modified = true;

  //if no changes than just exit
  if(!modified)
    return;

  for(int i=0;i<FILTER_SIZE;i++)
    m_modelData.m_filter[i] = new_filter[i];

    if(propagate)
      notify();
}

/** sets the gamma value of the sequenceparser */
void WcurveDoc::setGammaVal(int newG, bool propagate){
  if(newG == m_modelData.m_gamma)
    return;

  m_modelData.m_gamma = newG;

  //Notify the views
    if(propagate)
      notify();
}

/** validates the divisor */
bool WcurveDoc::setKVal(int newKVal, bool propagate){
  //if it is the same as the current value no need to upset
  if(m_modelData.m_k == newKVal)
    return true;

  if(newKVal > 0){
    m_modelData.m_k = newKVal;
    //Notify the views
    if(propagate)
      notify();
    return true;
  }
  else
    return false;
}
/** validates the modulus value and updates the wcurve */
bool WcurveDoc::setModVal(int newModVal, bool propagate){
  //if it is the same as the current value no need to upset
  if(m_modelData.m_mod == newModVal)
    return true;

  if(newModVal > 0){
    m_modelData.m_mod = newModVal;
    //Notify the views
    if(propagate)
      notify();
    return true;
  }
  else
    return false;
}
/** validates the start nucleotide and updates the wcurve */
bool WcurveDoc::setStartNuc(int start, bool propagate){
  //if it is the same as the current value no need to upset
  if(m_modelData.m_startNuc == start)
    return true;

 if(start > 0 && start < m_modelData.m_endNuc){
    m_modelData.m_startNuc = start;
    //Notify the views
    if(propagate)
      notify();
    return true;
  }
  else
    return false;
}
/** changes the z_increment value */
void WcurveDoc::setZVal(int newZ, bool propagate){
  if(newZ == m_modelData.m_z)
    return;

  m_modelData.m_z = newZ;
  //Notify the views
  if(propagate)
      notify();
}

/** trigger to have a WcurveDoc propagate its current state to its associated views */
void WcurveDoc::propagate(){
  notify();
}
/** pSeqView modifier method */
//void WcurveDoc::setSequenceView(SequenceView* seq_view){
//  pSeqView = seq_view;
//}
/** returns k_value (created for easy view access) */
int WcurveDoc::getKVal(){
  return m_modelData.m_k;
}
float WcurveDoc::getGammaVal(){
  return m_modelData.m_gamma;
}
