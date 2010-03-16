#include "imageArea.h"
#include <cairomm/context.h>
#include <itkImageRegionConstIterator.h>


#include <boost/signals.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

using boost::bind;
using namespace boost;


typedef boost::signal<void (double,double,double,double)> ImageAreaUpdater;
typedef boost::shared_ptr< ImageAreaUpdater > ImageAreaUpdaterPointer;
typedef std::map< int, ImageAreaUpdaterPointer > UpdateMapType;
UpdateMapType UpdateMap;





ImageArea::ImageArea(int updateGroup) :scale(1.0), tx(0), ty(0), updateGroup( updateGroup ) {
	add_events(Gdk::SCROLL_MASK);
	UpdateMapType::iterator umi = UpdateMap.find( updateGroup );
	if (umi == UpdateMap.end()) UpdateMap[ updateGroup ] = ImageAreaUpdaterPointer( new ImageAreaUpdater() );
	umi = UpdateMap.find( updateGroup );
	(umi->second)->connect( bind( &ImageArea::updateArea, this, _1, _2, _3, _4 ) );
}

ImageArea::~ImageArea() {
}

void ImageArea::updateArea( double newWindowScale, double newScale, double newTx, double newTy ) {
	if (windowScale != newWindowScale || scale != newScale 
		|| tx != newTx || ty != newTy ) {
		windowScale = newWindowScale;
		scale = newScale;
		tx = newTx;
		ty = newTy;
		queue_draw();
	}
}

void ImageArea::setImage(ColorRGBImageType::Pointer img) { 
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

bool ImageArea::on_scroll_event (GdkEventScroll* ev) {
	double screenX = (ev->x - get_width()/2.0) / windowScale;
	double screenY = (ev->y - get_height()/2.0) / windowScale;
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
	UpdateMapType::iterator umi = UpdateMap.find( updateGroup );
	if (umi != UpdateMap.end()) (*(umi->second))( windowScale, scale, tx, ty );
	return true;
}



bool ImageArea::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		if (image_surface_ptr_) {
			Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
			const int width = get_width();
			const int height = get_height();
			const double windowAspect = static_cast<double>(width) / height;
			const int image_width = image_surface_ptr_->get_width();
			const int image_height = image_surface_ptr_->get_height();
			const double imageAspect = static_cast<double>(image_width) / image_height;
			if (imageAspect > 1) {
				if (windowAspect>imageAspect) windowScale = height * imageAspect;
				else windowScale = width;
			} else {
				if (windowAspect < imageAspect) windowScale = width / imageAspect;
				else windowScale = height;
			}
			const double wx = width / windowScale;
			const double wy = height / windowScale;
			cr->scale( windowScale, windowScale );
			cr->translate( wx/2, wy/2 );
			cr->scale(scale, scale);
			cr->translate( tx, ty );
			double imgScale = 1.0 / std::max( image_width, image_height );
			cr->scale(imgScale, imgScale);
			cr->translate( -image_width / 2.0,  -image_height / 2.0 );
			cr->set_source(image_surface_ptr_, 0,0);
			cr->paint();
		}
	}
	return true;
}
