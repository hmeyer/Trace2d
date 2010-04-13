#ifndef BITMAPIMAGEAREA_H
#define BITMAPIMAGEAREA_H

#include "imageArea.h"
#include "itkimagetypes.h"


class BitmapImageArea : public ImageArea
{
public:
  void setBitmap(ColorRGBImageType::Pointer img);
protected:
  virtual void drawContent( CairoContextPointer c );
  Cairo::RefPtr< Cairo::ImageSurface > image_surface_ptr_;
};

#endif // BITMAPIMAGEAREA_H
