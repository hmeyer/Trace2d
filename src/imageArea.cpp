#include "imageArea.h"
#include <itkImageRegionConstIterator.h>


#include <boost/bind.hpp>

using boost::bind;
using namespace boost;


ImageArea::ImageArea() :scale(1.0), tx(0), ty(0), updatePointer( new ImageAreaUpdater() ) {
  add_events(Gdk::SCROLL_MASK);
  add_events(Gdk::BUTTON1_MOTION_MASK);
  add_events(Gdk::BUTTON_PRESS_MASK);
  myUpdateConnection = updatePointer->connect( bind( &ImageArea::updateArea, this, _1, _2, _3 ) );
}

ImageArea::~ImageArea() {
}

void ImageArea::setUpdateGroup( const ImageArea &other) {
  updatePointer = other.updatePointer;
  myUpdateConnection = updatePointer->connect( bind( &ImageArea::updateArea, this, _1, _2, _3 ) );
}

void ImageArea::updateArea( double newScale, double newTx, double newTy ) {
	if (scale != newScale 
		|| tx != newTx || ty != newTy ) {
		scale = newScale;
		tx = newTx;
		ty = newTy;
		queue_draw();
	}
}

bool ImageArea::on_scroll_event (GdkEventScroll* ev) {
	double screenX = ev->x;
	double screenY = ev->y;
	double imgX = screenX / scale - tx;
	double imgY = screenY / scale - ty;
	switch (ev->direction) {
		case GDK_SCROLL_UP: scale *= 1.1;break;
		case GDK_SCROLL_DOWN: scale /= 1.1;break;
		default:break;
	}
	tx = screenX / scale - imgX;
	ty = screenY / scale - imgY;
	queue_draw();
	(*updatePointer)( scale, tx, ty );
	return true;
}

bool ImageArea::on_motion_notify_event(GdkEventMotion *event) {
  if (event->state & GDK_BUTTON1_MASK) {
    double dx = event->x - lastX;
    double dy = event->y - lastY;
    lastX = event->x;
    lastY = event->y;
    tx += dx / scale;
    ty += dy / scale;
    queue_draw();
    (*updatePointer)( scale, tx, ty );
  }
  return true;
}

bool ImageArea::on_button_press_event(GdkEventButton *event) {
  lastX = event->x;
  lastY = event->y;
  return true;
}



bool ImageArea::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		cr->scale(scale, scale);
		cr->translate( tx, ty );
		drawContent(cr);
	}
	return true;
}
