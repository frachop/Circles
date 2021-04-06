#pragma once
#include "Canvas.hpp"
#include "Controls.hpp"


class CMainWindow
:	public QMainWindow
,	public CCirclesAccess
{
	Q_OBJECT

public:
	CMainWindow();
	~CMainWindow();

private slots:
	// updating status bar text
	void updateSB();

private:
	class CUi;
	// QScopedPointer or std::unique_ptr
	QScopedPointer<CUi> _ui;
};
