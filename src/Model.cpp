#include "Model.hpp"

//- Internal constants ///////////////////////////////////////////////////////////////////////////////////////////

constexpr qreal k_defaultBorderWidth{3.f};
static const QColor k_defaultBorderColor{"#073642"};
static const std::vector<QString> k_colorSet {
	"#b58900",
	"#cb4b16",
	"#dc322f",
	"#d33682",
	"#6c71c4",
	"#268bd2",
	"#2aa198",
	"#859900"
};

//- Static function tools ///////////////////////////////////////////////////////////////////////////////////////////

// random number between 0 and 1
template<typename T>
T rand01() { return static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX) + static_cast<T>(1)); }

// generate a random center point in "size" range 
static QPoint getRandomCenter(QSize size) {

	const QSizeF koef{ QSizeF(size) * 0.8f };
	const QPointF margins{ 0.1 * size.width(), 0.1 * size.height() };
		
	QSizeF p{
		margins.x() + (rand01<float>() * koef.width()),
		margins.y() + (rand01<float>() * koef.height())
	};
	return {
		static_cast<int>(p.width()),
		static_cast<int>(p.height())
	};
}

// generate a random radius value
static qreal getRandomRadius() {
	return k_radiusMin + (rand01<float>() * (k_radiusMax - k_radiusMin));
}

// generate a random fill color
static QColor getRandomFillColor() {
	QColor color(k_colorSet[std::rand() % k_colorSet.size()]);
	color.setAlpha( k_alphaMin + static_cast<int>(rand01<float>() * (k_alphaMax-k_alphaMin)) );
	return color;
}

//- Circle methods ///////////////////////////////////////////////////////////////////////////////////////////

CCircle::CCircle(QSize sz)
:	_center{getRandomCenter(sz)}
,	_radius{getRandomRadius()}
,	_borderWidth{k_defaultBorderWidth}
,	_borderColor{k_defaultBorderColor}
,	_fillColor{getRandomFillColor()}
{
}

CCircle::CCircle(QPointF center)
:	_center{center}
,	_radius{getRandomRadius()}
,	_borderWidth{k_defaultBorderWidth}
,	_borderColor{k_defaultBorderColor}
,	_fillColor{getRandomFillColor()}
{
}

void CCircle::randomize(QSize size)
{
	_center = getRandomCenter(size);
	_radius = getRandomRadius();
	setFillColor(getRandomFillColor());
}

//- Circles Singleton management ///////////////////////////////////////////////////////////////////////////////

QScopedPointer<CCircles> CCircles::_instance{nullptr};
CCircles & CCircles::getInstance()
{
	if (_instance.get() == nullptr) {
		_instance.reset( new CCircles );
	}
	return *_instance;
}

//- Circles methods ////////////////////////////////////////////////////////////////////////////////////////////

CCircles::CCircles()
:	_circles{}
,	_selection{}
{
}

void CCircles::clear()
{
	selectAll();
	selDelete();
	emit cleared();
}

void CCircles::selClear() {
	_selection.clear();
	emit selectionChanged();
}

void CCircles::selectAll()
{
	const auto previousCount = selectedCount();
	std::for_each(_circles.begin(), _circles.end(), [&](CCirclePointer p) {
		_selection[p.get()] = p;
	});
	
	// emit only if selection actually changed
	if (previousCount != selectedCount())
		emit selectionChanged();
}

void CCircles::selDelete()
{
	if (_selection.empty())
		return;

	// First save selection so we can Clear it
	auto sel = _selection;
	selClear();

	// For each circle that was selected
	std::for_each(sel.begin(),sel.end(), [&](std::pair<CCircle *, CCirclePointer> pair) {
	
		auto it = std::find_if(_circles.begin(), _circles.end(), [&pair](CCirclePointer c) {
			return c.get() == pair.second;
		});
		if (it != _circles.end())
		{
			// disconnect
			disconnect( pair.second.get(), SIGNAL(atLeastOnePropertyChanged()), this, SLOT(onAtLeastOnePropertyChanged()));

			// remove from database
			_circles.erase( it );
			
			// Finally emit circleRemoved signal
			emit circleRemoved(pair.second);
		}
	});
}

QSharedPointer<CCircle> CCircles::create( CCircle * c)
{
	Q_ASSERT(c);
	QSharedPointer<CCircle> newCircle( c );
	_circles.push_back(newCircle);
	
	connect( newCircle.get(), SIGNAL(atLeastOnePropertyChanged()), this, SLOT(onAtLeastOnePropertyChanged()));
	
	emit circleCreated(newCircle);
	return newCircle;
}

QSharedPointer<CCircle> CCircles::create( QPointF center )
{
	return create( new CCircle(center) );
}
	
void CCircles::createRandomCircles(std::size_t count, QSize size)
{
	selClear();
	for (std::size_t i=0; i<count; ++i)
		create( new CCircle(size) );
}
	
CCirclePointer CCircles::lastFromPoint(QPoint p) const
{
	auto found = std::find_if(_circles.rbegin(), _circles.rend(), [&p](const typename CCircleList::value_type & c) {
		return c->inMe(p);
	});
	
	return (found == _circles.rend()) ? nullptr : *found;
}

CCircleList CCircles::selection() const
{
	CCircleList result;
	std::for_each(_selection.begin(),_selection.end(), [&](std::pair<CCircle *, CCirclePointer> i) {
		result.push_back(i.second);
	});
	return result;
}

void CCircles::selectAdd( CCirclePointer c)
{
	if (c.get() == nullptr)
		return;
		
	if (_selection.find(c.get()) != _selection.end())
		return;

	_selection[c.get()] = c;
	emit selectionChanged();
}
