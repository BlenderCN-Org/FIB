#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ----------------------------------------------------------------------
// standard include files
//
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include "HW.h"

enum {DUMMY, FRR1, FRR2, SRGGE1, SRGGE2, SRGGE3, SRGGE4};


typedef std::map<QString, HW*> hw_type;

class MainWindow : public QMainWindow 
{
	Q_OBJECT

public:
	// constructor
	MainWindow	(QWidget *parent = 0);


public slots:
	void		createWidgets	();
	void		changeHW	(QAction*);
	void		reset		();
	void		quit		();

protected:
	QFrame*		createGroupView	  ();
	void		createActions	  ();
	void		createMenus	  ();
	QHBoxLayout*	createExitButtons ();

private:
	// homework objects
	QStringList	 m_hwName;
	hw_type		 m_hw;

	// widgets
	QStackedWidget	*m_stackWidgetPanels;
	QStackedWidget	*m_stackWidgetViews;

	// menus

    QMenu		*m_menuFRR;
    QMenu		*m_menuSRGGE;

	// actions
    QAction		*m_actionFRR1;
    QAction		*m_actionFRR2;


    QAction		*m_actionSRGGE1;
    QAction		*m_actionSRGGE2;
    QAction     *m_actionSRGGE3;
    QAction     *m_actionSRGGE4;


};

extern MainWindow	*MainWindowP;

#endif // MAINWINDOW_H
