#pragma once
#include "Model.hpp"
#include <QtWidgets>

// This class is a Circles Database graphical view
// This is where
//	  * Circles are drawn
//    * Circles can be created or selected with mouse click

class CCanvas
:	public QWidget
,	public CCirclesAccess
{
	Q_OBJECT

public:
	CCanvas(QWidget * pParent);

signals:
	// this signal is emitted to inform Circles in order
	// to create random circles in visible area
	void onResize( QSize sz );
	
protected:
    void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	
	// emit onResize signal
	void resizeEvent(QResizeEvent *event) override;

private:
	// Where all circles are drawn
	void draw( QPainter & p, CCirclePointer );
};
