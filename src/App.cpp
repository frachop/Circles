#include "App.hpp"


CApplication::CApplication(int & argc, char**argv)
:	QApplication{argc, argv}
,	_pMainWindow{ new CMainWindow }
{
	_pMainWindow->showNormal();
}

CApplication::~CApplication()
{
	
}
