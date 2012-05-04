#ifndef FIXTUREEDITOR_H
#define FIXTUREEDITOR_H

#include <QWidget>

class b2FixtureDef;

namespace Ui {
class FixtureEditor;
}

class ShapeEditor : public QWidget
{
private:
	b2FixtureDef *m_fixtureDef;
	int m_selectedVertex;

public:
	ShapeEditor( QWidget *parent = 0 );

	void setFixtureDef( b2FixtureDef *fixtureDef );
	void vertexPressed( int i );

protected:
	void paintEvent( QPaintEvent *event );
	void mousePressEvent( QMouseEvent *event );
	void mouseReleaseEvent( QMouseEvent *event );
	void mouseMoveEvent( QMouseEvent *event );

};

class FixtureEditor : public QWidget
{
	Q_OBJECT

private:
	Ui::FixtureEditor *ui;
	ShapeEditor m_shapeEditor;
	
public:
	explicit FixtureEditor( QWidget *parent = 0 );
	~FixtureEditor();

	void setSprite( const QString &sprite );
	void setFixtureDef( b2FixtureDef *fixtureDef );

public:
	void resizeEvent ( QResizeEvent *event );

};

#endif // FIXTUREEDITOR_H
