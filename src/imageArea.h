#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>


#include <boost/signals.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::signal<void (double,double,double)> ImageAreaUpdater;
typedef boost::shared_ptr< ImageAreaUpdater > ImageAreaUpdaterPointer;
typedef Cairo::RefPtr<Cairo::Context> CairoContextPointer;

class ImageArea : public Gtk::DrawingArea
{
public:
  ImageArea();
  virtual ~ImageArea();
  void setUpdateGroup( const ImageArea &other );
protected:
  //Override default signal handler:
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_scroll_event (GdkEventScroll* ev);
  virtual bool on_button_press_event(GdkEventButton *event);
  virtual bool on_motion_notify_event(GdkEventMotion *event);
  virtual void drawContent( CairoContextPointer c ) = 0;
  void updateArea(double newScale, double newTx, double newTy );

  double scale, tx, ty;
  double lastX, lastY;
  ImageAreaUpdaterPointer updatePointer;
  boost::signals::scoped_connection myUpdateConnection;
};

#endif // IMAGEAREA_H
