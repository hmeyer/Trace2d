#include "itk2graphicsscene.h"

#include <QGraphicsScene>
#include <QImage>
#include <QColor>
#include <boost/make_shared.hpp>
#include <itkImageRegionConstIterator.h>

ITK2GraphicsScene::QGraphicsScenePtr ITK2GraphicsScene::getGraphicsScene() {
  if (result && itkimage.IsNotNull() && mtime == itkimage->GetMTime()) return result;
  result = boost::make_shared<QGraphicsScene>();
  result->setSceneRect(QRectF());
  if (itkimage.IsNotNull()) {
    ColorRGBImageType::RegionType reg( itkimage->GetBufferedRegion() );
    image = boost::make_shared<QImage>(reg.GetSize(0),reg.GetSize(1),QImage::Format_RGB32);
    unsigned height = reg.GetSize(1);
    reg.SetSize(1,1);
    for(unsigned y = 0; y < height; ++y) {
      QRgb *scanline = (QRgb*)( image->scanLine(y) );
      reg.SetIndex(1,y);
      itk::ImageRegionConstIterator<ColorRGBImageType> it(itkimage, reg);
      for(it.GoToBegin(); !it.IsAtEnd(); ++it) {
	const RGBPixelType &itkPix = it.Value();
	*scanline = qRgb( itkPix.GetRed(), itkPix.GetGreen(), itkPix.GetBlue() );
	++scanline;
      }
    }
    pixmap = boost::make_shared<QPixmap>(QPixmap::fromImage(*image));
    result->addPixmap( *pixmap );
    mtime = itkimage->GetMTime();
  }
  return result;
}
