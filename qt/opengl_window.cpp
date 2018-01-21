#include "opengl_window.hpp"


#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

#include <QResizeEvent>

#include "gfx/gfx_factory.hpp"
#include "gfx/gfx.hpp"

opengl_window::opengl_window(QWindow *parent) : QWindow(parent)
{
	g = nullptr;
	
	animating = false;
	context = nullptr;
	device = nullptr;
	setSurfaceType(QWindow::OpenGLSurface);
}

opengl_window::~opengl_window()
{
	if(device)
		delete device;
	g->deinit();
	delete g;
}

void opengl_window::render(QPainter *painter)
{
	Q_UNUSED(painter);
}

void opengl_window::initialize()
{
	gfx_factory gf;
	g = gf.get_gfx();
	g->init(size().width(), size().height());
}

void opengl_window::render()
{
	if(!device)
		device = new QOpenGLPaintDevice;
	
	g->render();
	
	device->setSize(size());
	
	QPainter painter(device);
	render(&painter);
}

void opengl_window::resizeEvent(QResizeEvent *ev)
{
	if(g)
		g->resize(ev->size().width(), ev->size().height());
	
	QWindow::resizeEvent(ev);
}

void opengl_window::renderLater()
{
	requestUpdate();
}

bool opengl_window::event(QEvent *event)
{
	switch(event->type())
	{
		case QEvent::UpdateRequest:
			renderNow();
			return true;
		default:
			return QWindow::event(event);
	}
}

void opengl_window::exposeEvent(QExposeEvent *event)
{
	Q_UNUSED(event);
	
	if(isExposed())
		renderNow();
}

void opengl_window::renderNow()
{
	if(!isExposed())
		return;
	
	bool needsInitialize = false;
	
	if(!context)
	{
		context = new QOpenGLContext(this);
		context->setFormat(requestedFormat());
		context->create();
		
		needsInitialize = true;
	}
	
	context->makeCurrent(this);
	
	if(needsInitialize)
	{
		initializeOpenGLFunctions();
		initialize();
	}
	
	render();
	
	context->swapBuffers(this);
	
	if(animating)
		renderLater();
}

void opengl_window::setAnimating(bool animating)
{
	this->animating = animating;
	
	if(animating)
		renderLater();
}
