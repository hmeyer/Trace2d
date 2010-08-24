#ifndef ITK2GRAPHICSSCENE_H
#define ITK2GRAPHICSSCENE_H

#include "itkimagetypes.h"
#include <boost/shared_ptr.hpp>

class QGraphicsScene;
class QPixmap;
class QImage;

class ITK2GraphicsScene {
  public:
  ITK2GraphicsScene() {}
  void setInput( ColorRGBImageType::ConstPointer input ) { itkimage = input;mtime=0; }
  typedef boost::shared_ptr< QGraphicsScene > QGraphicsScenePtr;
  QGraphicsScenePtr getGraphicsScene();
  private:
    ColorRGBImageType::ConstPointer itkimage;
    unsigned long mtime;
    QGraphicsScenePtr result;
    
    boost::shared_ptr<QPixmap> pixmap;
    boost::shared_ptr<QImage> image;
};

#endif // ITK2GRAPHICSSCENE_H
