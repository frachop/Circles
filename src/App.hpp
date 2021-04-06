#pragma once
#include "MainWindow.hpp"
#include <QtWidgets/qapplication.h>

class CApplication
:	public QApplication
{
	Q_OBJECT

public:
	CApplication(int & argc, char**argv);

private:
	QScopedPointer<CMainWindow> _pMainWindow;
};
