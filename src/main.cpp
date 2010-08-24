#include <QApplication>
#include <QCoreApplication>
#include "tracewindow.h"

int main(int argc, char *argv[]) {
//  inputImage.setBitmap( input );
//  filteredImage.setBitmap( input );
  
/*
	BitmapImageArea inputImage;
	BitmapImageArea filteredImage;
	filteredImage.setUpdateGroup( inputImage );
	VectorImageArea tracedImage;
	tracedImage.setUpdateGroup( filteredImage );
*/

  QCoreApplication::setOrganizationName("hmeyer");
  QCoreApplication::setOrganizationDomain("hmeyer@github.com");
  QCoreApplication::setApplicationName("Trace2d");
     
  QApplication app(argc, argv);
  TraceWindow mainWindow;
  if (app.arguments().size() > 1) {
    mainWindow.openImage(app.arguments()[1].toStdString());
  }
  mainWindow.show();
  return app.exec();
}
