#ifndef DUMMY_H
#define DUMMY_H

#include "HW.h"

extern QString GroupBoxStyle;

// ----------------------------------------------------------------------
// standard include files
//

class Dummy : public HW 
{
	Q_OBJECT
public:
	// constructor
	Dummy		(const QGLFormat &glf, QWidget *parent = 0); 
	QGroupBox*	controlPanel	();		// create control panel

public slots:

protected:
	void		initializeGL	();		// init   GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene

private:
	int		m_winW;				// window width
	int		m_winH;				// window height
};

#endif // Dummy_H
