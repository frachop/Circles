#pragma once
#include "Model.hpp"
#include <QtWidgets>

class CCanvas
:	public QWidget
,	public CCirclesAccess
{
	Q_OBJECT

public:
	CCanvas(QWidget * pParent);

signals:
	void onResize( QSize sz );
	
protected:
    void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	void draw( QPainter & p, CCirclePointer );

};
