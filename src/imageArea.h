#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <gtkmm/drawingarea.h>
#include "itkimagetypes.h"



class ImageArea : public Gtk::DrawingArea
{
public:
  ImageArea(int updateGroup = 0);
  virtual ~ImageArea();
  void setImage(ColorRGBImageType::Pointer img);
protected:
  //Override default signal handler:
  Cairo::RefPtr< Cairo::ImageSurface > image_surface_ptr_;
  double windowScale, scale, tx, ty;
  int updateGroup;
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_scroll_event (GdkEventScroll* ev);
  void updateArea( double newWindowScale, double newScale, double newTx, double newTy );
};

#endif // IMAGEAREA_H
