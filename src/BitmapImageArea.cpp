#include "BitmapImageArea.h"
#include <itkImageRegionConstIterator.h>


void BitmapImageArea::setBitmap(ColorRGBImageType::Pointer img) { 
	ColorRGBImageType::RegionType reg;
	reg = img->GetBufferedRegion();
	image_surface_ptr_ = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, (int)reg.GetSize(0), (int)reg.GetSize(1));
	unsigned char *line = image_surface_ptr_->get_data();
	unsigned char *pix = line;
	int stride = image_surface_ptr_->get_stride();
	itk::ImageRegionConstIterator<ColorRGBImageType> it(img, reg);
	int width = (int)reg.GetSize(0);
	while(!it.IsAtEnd()) {
		*pix++ = it.Get().GetBlue();
		*pix++ = it.Get().GetGreen();
		*pix++ = it.Get().GetRed();
		*pix++ = 0;
		if (!(--width)) {
			line += stride;
			pix = line; 
			width = (int)reg.GetSize(0);
		}
		++it;
	}
	queue_draw();
}

void BitmapImageArea::drawContent( CairoContextPointer c ) {
  if (image_surface_ptr_) {
    c->set_source(image_surface_ptr_, 0,0);
    c->paint();
  }
}
