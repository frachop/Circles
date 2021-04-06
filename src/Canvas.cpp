#include "App.hpp"

CCanvas::CCanvas(QWidget * pParent)
:	QWidget{pParent}
{
	connect( &circles(), SIGNAL(cleared()), this, SLOT(update()) );
	connect( &circles(), SIGNAL(circleCreated(CCirclePointer)), this, SLOT(update()) );
	connect( &circles(), SIGNAL(selectionChanged()), this, SLOT(update()) );
	connect( &circles(), SIGNAL(atLeastOnePropertyChanged()), this, SLOT(update()) );
}

void CCanvas::mousePressEvent(QMouseEvent *event)
{
	auto c = circles().lastFromPoint(event->pos());
	if (c)
	{
		bool const shiftPressed = (QApplication::queryKeyboardModifiers() & Qt::ShiftModifier);
		if (shiftPressed)
		{
			circles().selectAdd(c);
		}
		else
			circles().selectSet(c);
	}
	else
		circles().create(event->pos());
}

void CCanvas::resizeEvent(QResizeEvent *event)
{
	emit onResize(event->size());
}

void CCanvas::draw( QPainter & p, CCirclePointer pCircle)
{
	if (pCircle.get() == nullptr)
		return;

	const CCircle & c = *(pCircle.get());

	p.setPen(QPen(QBrush(c.borderColor()), c.borderWidth()));
	p.setBrush(QBrush(c.fillColor()));
	
	p.drawEllipse(QRectF(c.center().x()-c.radius(), c.center().y()-c.radius(), 2.0 * c.radius(), 2.0 * c.radius()));
	if (circles().isSelected(pCircle))
	{
		QColor color( c.fillColor().red() ^ 133 ,c.fillColor().green() ^ 133 , c.fillColor().blue() ^ 133, 255 );
		p.setPen(QPen(QBrush(color), 3));
		p.drawLine(
			c.center() - QPointF(0.5*c.radius(), 0),
			c.center() + QPointF(0.5*c.radius(), 0)
		);
		p.drawLine(
			c.center() - QPointF(0,0.5*c.radius()),
			c.center() + QPointF(0,0.5*c.radius())
		);
	}
}

void CCanvas::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	
	// in order to use the StyleSheets you need to provide a drawPrimitive to the custom widget :
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	
	painter.resetTransform();
	
	CCircleList const all = circles().all();
	std::for_each(all.begin(), all.end(), [&](CCirclePointer c)
	{
		draw(painter, c);
	});
}
