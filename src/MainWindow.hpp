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
	virtual ~CMainWindow();

private slots:
	void updateSB();

private:
	class CUi;
	CUi* _ui;
};
