#include "fixtureeditor.h"
#include "ui_fixtureeditor.h"

#include <Box2D/Box2D.h>
#include <QPainter>
#include <QPaintEvent>

#include <iostream>

static QPointF vertex( const b2Vec2 &v )
{
	return QPointF( v.x, v.y );
}
static b2Vec2 vertex( const QPointF &v )
{
	return b2Vec2( v.x(), v.y() );
}

ShapeEditor::ShapeEditor( QWidget *parent ) :
	QWidget(parent),
	m_fixtureDef(0),
	m_selectedVertex(-1)
{
	setPalette( Qt::transparent );
}

void ShapeEditor::setFixtureDef( b2FixtureDef *fixtureDef )
{
	m_fixtureDef = fixtureDef;
}
void ShapeEditor::vertexPressed( int i )
{
	m_selectedVertex = i;
	setMouseTracking( 1 );
}

void ShapeEditor::paintEvent ( QPaintEvent *event )
{
	QPainter p( this );
	if( ! m_fixtureDef ) {
		return ;
	}

	const b2Shape *s = m_fixtureDef->shape;
	if( s->GetType() != b2Shape::e_polygon ) {
		// ui->sprite->setText( "Currently only Polygonal shapes are supported" );
		return ;
	}

	const b2PolygonShape *ps = static_cast<const b2PolygonShape*>( s );
	p.translate( width()/2, height()/2 );
	p.scale( 32, 32 );

	p.setRenderHint( QPainter::Antialiasing );

	// drawing lines
	{
		QPen pen( Qt::SolidLine );
		pen.setColor( Qt::black );
		pen.setWidthF( 0.05 );
		p.setPen( pen );
		b2Vec2 oldV = ps->GetVertex( ps->GetVertexCount()-1 );
		for( int i = 0; i < ps->GetVertexCount(); ++i ) {
			b2Vec2 v = ps->GetVertex( i );
			p.drawLine( vertex(oldV), vertex(v) );
			oldV = v;
		}
	}

	// drawing handles
	{
		QBrush brush( Qt::red, Qt::SolidPattern );
		p.setBrush( brush );
		for( int i = 0; i < ps->GetVertexCount(); ++i ) {
			// QRectF rectangle( -5,-5, 10,10 );
			b2Vec2 v = ps->GetVertex( i );
			p.drawEllipse( vertex(v), .2, .2);
		}
	}
}
void ShapeEditor::mousePressEvent( QMouseEvent *event )
{
	// getting the current shape
	if( ! m_fixtureDef ) {
		return ;
	}
	const b2Shape *s = m_fixtureDef->shape;
	if( ! s || s->GetType() != b2Shape::e_polygon ) {
		// ui->sprite->setText( "Currently only Polygonal shapes are supported" );
		return ;
	}
	const b2PolygonShape *ps = static_cast<const b2PolygonShape*>( s );

	// getting the mouse point in physics coordinates
	QPointF p = event->posF();
	p -= QPointF( width()/2, height()/2 );
	p /= 32;

	// checking if a vertex was pressed
	for( int i = 0; i < ps->GetVertexCount(); ++i ) {
		b2Vec2 v = ps->GetVertex( i );
		QPointF qv( v.x, v.y );
		QLineF l( p, qv );
		if( l.length() < .2 ) {
			vertexPressed( i );
		}
	}
}
void ShapeEditor::mouseReleaseEvent( QMouseEvent *event )
{
	setMouseTracking( 0 );
	m_selectedVertex = -1;
	repaint( );
}
void ShapeEditor::mouseMoveEvent( QMouseEvent *event )
{
	if( m_selectedVertex == -1 ) {
		return ;
	}

	// getting the current shape
	assert( m_fixtureDef );
	const b2Shape *s = m_fixtureDef->shape;
	assert( s && s->GetType() == b2Shape::e_polygon );
	const b2PolygonShape *ps = static_cast<const b2PolygonShape*>( s );

	// getting the mouse point in physics coordinates
	QPointF p = event->posF();
	p -= QPointF( width()/2, height()/2 );
	p /= 32;

	// creating the new shape
	std::vector<b2Vec2> vertices;
	for( int i = 0; i < ps->GetVertexCount(); ++i ) {
		if( i != m_selectedVertex ) {
			vertices.push_back( ps->GetVertex(i) );
		}
		else {
			vertices.push_back( vertex(p) );
		}
	}

	b2PolygonShape *newPs = new b2PolygonShape();
	newPs->Set( &vertices[0], vertices.size() );
	m_fixtureDef->shape = newPs;
	repaint( );

	delete ps;
}

FixtureEditor::FixtureEditor( QWidget *parent ) :
    QWidget(parent),
	ui(new Ui::FixtureEditor)
{
	ui->setupUi(this);

	m_shapeEditor.setParent( this );
}
FixtureEditor::~FixtureEditor()
{
	delete ui;
}

void FixtureEditor::setSprite( const QString &sprite )
{
	ui->sprite->setPixmap( QPixmap( sprite ) );
}
void FixtureEditor::setFixtureDef( b2FixtureDef *fixtureDef )
{
	m_shapeEditor.setFixtureDef( fixtureDef );
	update();
}

void FixtureEditor::resizeEvent ( QResizeEvent *event )
{
	m_shapeEditor.resize( event->size() );
}
