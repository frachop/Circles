#pragma once
#include "Model.hpp"
#include <QtWidgets>

class CControls
:	public QWidget
,	public CCirclesAccess
{
	Q_OBJECT

public:
	CControls(QWidget * pParent);

private slots:
	void onCanvasResize(QSize);
	void onSelectionChanged( );
	void rFillColorSliderMoved( int );
	void gFillColorSliderMoved( int );
	void bFillColorSliderMoved( int );
	void aFillColorSliderMoved( int );
	void radiusSliderMoved( int );
	void enableButtons();
	void createRandoms();

private:
	void onFillColorSliderMoved(int,int,int,int);

private:
	class CUi;
	CUi* _ui;
	QSize _canvasSize;
};
