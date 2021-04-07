#pragma once
#include <QtWidgets>
#include <set>

class CCircles;
class CCircle;

// each circle is managed as shared pointer
// So it is freed as soon as no one needs it
using CCirclePointer = QSharedPointer<CCircle>;

// Circles are stored in a list and selection is returned as a list too
using CCircleList = std::list<CCirclePointer>;

//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

// circle properties ranges and constantes
constexpr qreal k_margins   = 0.1; // 10%
constexpr qreal k_radiusMin = 16.;
constexpr qreal k_radiusMax = 128.;
constexpr float k_alphaMin  = 16;
constexpr float k_alphaMax  = 255;

//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

class CCircle
:	public QObject
{
	Q_OBJECT

public:
	// properties getters
	QPointF center( ) const { return _center; }
	qreal radius() const { return _radius; }
	qreal borderWidth() const { return _borderWidth; }
	QColor borderColor() const { return _borderColor; }
	QColor fillColor() const { return _fillColor; }

signals:
	// changing properties signals
	void centerChanged(QPointF newCenter);
    void radiusChanged(qreal r);
    void borderWidthChanged(qreal b);
    void borderColorChanged(QColor newColor);
    void fillColorChanged(QColor newColor);
    void atLeastOnePropertyChanged();

public slots:
	// changing properties slots
	void setCenter( QPointF c) { _center = c; emit centerChanged( _center ); emit atLeastOnePropertyChanged(); }
	void setRadius( qreal r) { _radius = r; emit radiusChanged( _radius ); emit atLeastOnePropertyChanged(); }
	void setBorderWidth( qreal b) { _borderWidth = b; emit borderWidthChanged( _borderWidth ); emit atLeastOnePropertyChanged(); }
	void setBorderColor( QColor const & c) { _borderColor = c; emit borderColorChanged( _borderColor ); emit atLeastOnePropertyChanged(); }
	void setFillColor( QColor const & c) { _fillColor = c; emit fillColorChanged( _fillColor ); emit atLeastOnePropertyChanged(); }
	void randomize( QSize sz );

public:
	// Helper methods
	// return true if p is inside the circle.
	bool inMe( QPointF p) const { const auto d = ( p - _center ); return sqrt( pow(d.x(),2) + pow(d.y(),2) ) <= _radius; }

protected:
	// Constructors are protected because to prevent
	// anyone except CCircles to create one.
	CCircle() = delete;
	CCircle(const CCircle & ) = delete;
	CCircle(CCircle &&) = delete;
	CCircle(QSize size); // properties randomized in "size" range
	CCircle(QPointF center);

private:
	// propreties
	QPointF _center;
    qreal   _radius;
    qreal   _borderWidth;
    QColor  _borderColor;
    QColor  _fillColor;

private:
	// CCircles class can create CCircle.
	friend class CCircles;
};

//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

class CCircles
:	public QObject
{
	Q_OBJECT

private:
//	singleton instance
	static QScopedPointer<CCircles> _instance;

public:
//	singleton access static method
	static CCircles & getInstance();

private:
	CCircles();
	CCircles(const CCircles &) = delete;
	CCircles(CCircles &&) = delete;

signals:
	void cleared();
	void circleCreated(CCirclePointer newCircle);
	void circleRemoved(CCirclePointer newCircle);
	void selectionChanged();
	void atLeastOnePropertyChanged();

public:
	CCirclePointer create( QPointF center );
	void createRandomCircles(std::size_t count, QSize size);

public:
	std::size_t count() const { return _circles.size(); }
	CCircleList all() const { return _circles; }
	CCircleList selection() const;
	bool isSelected(CCirclePointer c) const { return _selection.find( c.get()) != _selection.end(); }
	std::size_t selectedCount() const { return _selection.size(); }
	void selectSet( CCirclePointer c ) { _selection.clear(); selectAdd(c); }
	void selectAdd( CCirclePointer c );

public slots:
	void clear();
	void selectAll();
	void selClear();
	void selDelete();

public:
	CCirclePointer lastFromPoint(QPoint p) const;

private slots:
	void onAtLeastOnePropertyChanged() { emit atLeastOnePropertyChanged(); }

private:
	CCirclePointer create(CCircle * p);

private:
	CCircleList _circles;
	std::map<CCircle*, CCirclePointer> _selection;
};

//- /////////////////////////////////////////////////////////////////////////////////////////////////////////
//- Helper base class to get access to the Circle database via singleton ////////////////////////////////////

class CCirclesAccess
{
public:
	virtual ~CCirclesAccess() {}
	CCircles & circles() { return CCircles::getInstance(); }
	CCircles const & circles() const { return CCircles::getInstance(); }
};

