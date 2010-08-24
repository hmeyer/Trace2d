#ifndef TRACEWINDOW_H
#define TRACEWINDOW_H

#include "ui_trace2d.h"
#include <boost/shared_ptr.hpp>
#include "itk2graphicsscene.h"

class ImageTracer;
class GraphicsScene;

class TraceWindow: public QMainWindow, private Ui::trace2d
{
  Q_OBJECT
  public:
  TraceWindow();
  
  void openImage(const std::string &fname);
  
  public slots:
  void on_actionOpen_triggered();
  void on_buttonApply_clicked();
  void on_buttonZoomIn_clicked();
  void on_buttonZoomOut_clicked();
  void onParametersChanged();
  
  private:
    boost::shared_ptr< ImageTracer > tracer;
    ITK2GraphicsScene imageConverter;
};

#endif // TRACEWINDOW_H
