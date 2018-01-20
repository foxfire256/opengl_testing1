#include "opengl_window.hpp"


#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

opengl_window::opengl_window(QWindow *parent) : QWindow(parent)
{
	animating = false;
	context = nullptr;
	device = nullptr;
	setSurfaceType(QWindow::OpenGLSurface);
}

opengl_window::~opengl_window()
{
	if(device)
		delete device;
}

void opengl_window::render(QPainter *painter)
{
	Q_UNUSED(painter);
}

void opengl_window::initialize()
{
}

void opengl_window::render()
{
	if(!device)
		device = new QOpenGLPaintDevice;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	device->setSize(size());
	
	QPainter painter(device);
	render(&painter);
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
