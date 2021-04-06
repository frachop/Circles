#include "MainWindow.hpp"
#include <ui_MainWindow.h>

class CMainWindow::CUi
:	public Ui::CMainWindow
{
};

CMainWindow::CMainWindow()
:	QMainWindow()
,	_ui{new CUi}
{
	_ui->setupUi(this);
	
	// Basic signal captures to update status bar text
	connect( &circles(), SIGNAL(cleared()), this, SLOT(updateSB()) );
	connect( &circles(), SIGNAL(circleCreated(CCirclePointer)), this, SLOT(updateSB()) );
	connect( &circles(), SIGNAL(circleRemoved(CCirclePointer)), this, SLOT(updateSB()) );
	connect( &circles(), SIGNAL(selectionChanged()), this, SLOT(updateSB()) );

	// Menu actions
	connect(_ui->_actReset, SIGNAL(triggered()), &circles(), SLOT(clear()) );

	// Controls needs to be aware about the canvas size in order to generate "visibles" random circles
	connect(_ui->_canvas, SIGNAL(onResize(QSize)), _ui->_controls, SLOT(onCanvasResize(QSize)));

	// initial status bar update
	updateSB();
}

CMainWindow::~CMainWindow()
{}

void CMainWindow::updateSB()
{
	QString help("Click to create or select a circle - Hold Shift to select multiple circles");
	auto const circleCount = circles().count();
	if (circleCount)
	{
		help += QString(" - %1 circle%2").arg( circleCount ).arg(circleCount > 1 ? "s" : "");
		auto const selCount = circles().selectedCount();
		if (selCount > 0) {
			help += QString(" - %1 selected").arg( selCount );
		}
	}
	statusBar()->showMessage(help);
}
