#include "App.hpp"
#include <ui_Controls.h>

class CControls::CUi
:	public Ui::CControls
{
};

CControls::CControls(QWidget * pParent)
:	QWidget{pParent}
,	_ui(new CUi)
,	_canvasSize{300,400}
{
	_ui->setupUi(this);
	connect( &circles(), SIGNAL(cleared()), this, SLOT(enableButtons()) );
	connect( &circles(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()) );
	connect( &circles(), SIGNAL(circleCreated(CCirclePointer )), this, SLOT(enableButtons()) );
	connect( &circles(), SIGNAL(circleRemoved(CCirclePointer)), this, SLOT(enableButtons()) );

	connect( _ui->_rFillColorSlider, SIGNAL(sliderMoved(int)), this, SLOT(rFillColorSliderMoved(int)));
	connect( _ui->_gFillColorSlider, SIGNAL(sliderMoved(int)), this, SLOT(gFillColorSliderMoved(int)));
	connect( _ui->_bFillColorSlider, SIGNAL(sliderMoved(int)), this, SLOT(bFillColorSliderMoved(int)));
	connect( _ui->_aFillColorSlider, SIGNAL(sliderMoved(int)), this, SLOT(aFillColorSliderMoved(int)));
	connect( _ui->_rFillColorSlider, SIGNAL(valueChanged(int)), this, SLOT(rFillColorSliderMoved(int)));
	connect( _ui->_gFillColorSlider, SIGNAL(valueChanged(int)), this, SLOT(gFillColorSliderMoved(int)));
	connect( _ui->_bFillColorSlider, SIGNAL(valueChanged(int)), this, SLOT(bFillColorSliderMoved(int)));
	connect( _ui->_aFillColorSlider, SIGNAL(valueChanged(int)), this, SLOT(aFillColorSliderMoved(int)));

	_ui->_radiusSlider->setRange(k_radiusMin, k_radiusMax);
	connect( _ui->_radiusSlider, SIGNAL(sliderMoved(int)), this, SLOT(radiusSliderMoved(int)));
	connect( _ui->_radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(radiusSliderMoved(int)));

	connect( _ui->_btnReset, SIGNAL(pressed()), &circles(), SLOT(clear()));
	connect( _ui->_btnClearSelection, SIGNAL(pressed()), &circles(), SLOT(selClear()));
	connect( _ui->_btnRemoveSelected, SIGNAL(pressed()), &circles(), SLOT(selDelete()));
	connect( _ui->_btnCreate10Circles, SIGNAL(pressed()), this, SLOT(createRandoms()));
	connect( _ui->_btnSelectAll, SIGNAL(pressed()), &circles(), SLOT(selectAll()));

	onSelectionChanged();
}

void CControls::onCanvasResize(QSize s)
{
	_canvasSize = s;
}

void CControls::enableButtons( )
{
	const auto & cs = circles();
	_ui->_btnReset->setEnabled( cs.count() > 0);
	_ui->_btnClearSelection->setEnabled(cs.selectedCount() > 0);
	_ui->_btnRemoveSelected->setEnabled(cs.selectedCount() > 0);
	_ui->_btnSelectAll->setEnabled(	(cs.count() > 0) && (cs.selectedCount() != cs.count()) );
}

static void setSliderValue( QSlider * s, int v) {
	s->blockSignals(true);
	s->setValue(v);
	s->blockSignals(false);
}

void CControls::onSelectionChanged( )
{
	const bool enabled = circles().selectedCount() > 0;
	_ui->_rFillColorSlider->setEnabled(enabled);
	_ui->_gFillColorSlider->setEnabled(enabled);
	_ui->_bFillColorSlider->setEnabled(enabled);
	_ui->_aFillColorSlider->setEnabled(enabled);
	_ui->_radiusSlider->setEnabled(enabled);
	if (enabled) {
		auto sel = circles().selection();
		int r{0},g{0},b{0},a{0}, radius{0};
		std::for_each(sel.begin(), sel.end(), [&](CCirclePointer c){
			r += c->fillColor().red();
			g += c->fillColor().green();
			b += c->fillColor().blue();
			a += c->fillColor().alpha();
			
			radius += c->radius();
		});
				
		const auto selCount = sel.size();
		setSliderValue(_ui->_rFillColorSlider, r/selCount);
		setSliderValue(_ui->_gFillColorSlider, g/selCount);
		setSliderValue(_ui->_bFillColorSlider, b/selCount);
		setSliderValue(_ui->_aFillColorSlider, a/selCount);
		setSliderValue(_ui->_radiusSlider, static_cast<int>( radius / selCount) );
	}
	
	enableButtons();
}

void CControls::onFillColorSliderMoved(int r,int g,int b, int a)
{
	auto sel = circles().selection();
	std::for_each(sel.begin(),sel.end(),[r,g,b,a](CCirclePointer c) {
		c->setFillColor(QColor(r,g,b,a));
	});
}

void CControls::rFillColorSliderMoved( int r)
{
	onFillColorSliderMoved(r, _ui->_gFillColorSlider->value(), _ui->_bFillColorSlider->value(), _ui->_aFillColorSlider->value());
}
void CControls::gFillColorSliderMoved( int g)
{
	onFillColorSliderMoved(_ui->_rFillColorSlider->value(), g, _ui->_bFillColorSlider->value(), _ui->_aFillColorSlider->value());
}
void CControls::bFillColorSliderMoved( int b)
{
	onFillColorSliderMoved(_ui->_rFillColorSlider->value(), _ui->_gFillColorSlider->value(), b, _ui->_aFillColorSlider->value());
}
void CControls::aFillColorSliderMoved( int a)
{
	onFillColorSliderMoved(_ui->_rFillColorSlider->value(), _ui->_gFillColorSlider->value(), _ui->_bFillColorSlider->value(), a);
}

void CControls::radiusSliderMoved( int r)
{
	auto const sel = circles().selection();
	std::for_each(sel.begin(),sel.end(),[r](CCirclePointer c) { c->setRadius(r); });
}


void CControls::createRandoms( )
{
	circles().createRandomCircles(10, _canvasSize);
}

