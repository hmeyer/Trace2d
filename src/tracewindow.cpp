#include "tracewindow.h"

#include <QFileDialog>
#include <QSettings>
#include "itkbasics.h"
#include "ImageTracer.h"
#include "itk2graphicsscene.h"

const QString iterationSettingName("Iterations");
const QString conductanceSettingName("Conductance");
const QString timeStepSettingName("TimeStep");
const QString levelSettingName("Level");
const QString thresholdSettingName("Threshold");
const QString gaussSigmaSettingName("GaussSigma");
const QString filterTypeSettingName("FilterType");

TraceWindow::TraceWindow()
  :QMainWindow(0),tracer(new ImageTracer()) {
  setupUi(this);
  connect(spinIterations,SIGNAL(valueChanged(int)),this,SLOT(onParametersChanged()));
  connect(spinConductance,SIGNAL(valueChanged(double)),this,SLOT(onParametersChanged()));
  connect(spinTimeStep,SIGNAL(valueChanged(double)),this,SLOT(onParametersChanged()));
  connect(spinLevel,SIGNAL(valueChanged(double)),this,SLOT(onParametersChanged()));
  connect(spinThreshold,SIGNAL(valueChanged(double)),this,SLOT(onParametersChanged()));
  connect(spinGaussSigma,SIGNAL(valueChanged(double)),this,SLOT(onParametersChanged()));
  connect(radioGauss,SIGNAL(toggled(bool)),this,SLOT(onParametersChanged()));
  connect(radioGA,SIGNAL(toggled(bool)),this,SLOT(onParametersChanged()));
  connect(radioCA,SIGNAL(toggled(bool)),this,SLOT(onParametersChanged()));
  QSettings settings;
  spinIterations->setValue( settings.value(iterationSettingName, 2).toInt() );
  spinConductance->setValue( settings.value(conductanceSettingName, 0.25).toDouble() );
  spinTimeStep->setValue( settings.value(timeStepSettingName, 1.0).toDouble() );
  spinLevel->setValue( settings.value(levelSettingName, .1).toDouble() );
  spinThreshold->setValue( settings.value(thresholdSettingName, .1).toDouble() );
  spinGaussSigma->setValue( settings.value(gaussSigmaSettingName, 3).toDouble() );
  int filterType = settings.value(filterTypeSettingName, ImageTracer::GaussFilter).toInt();
  switch(filterType) {
    case ImageTracer::GaussFilter: radioGauss->setChecked(true);;
    case ImageTracer::CADiffusion: radioGA->setChecked(true);break;
    case ImageTracer::GADiffusion: radioCA->setChecked(true);break;
  }
}

void TraceWindow::on_actionOpen_triggered() {
  QString fname = QFileDialog::getOpenFileName(this,
			       tr("Open Image"),
			       ".",
			       "Image files (*.png *.jpg)"
			       );
  if (!fname.isEmpty())
    openImage(fname.toStdString());
}

void TraceWindow::openImage(const std::string &fname) {
  tracer->setInput( ImageRead<ColorRGBImageType>( fname ) );
  imageConverter.setInput( tracer->getInput() );
  graphicsView->setScene( imageConverter.getGraphicsScene().get() );
}

void TraceWindow::onParametersChanged() {
  QSettings settings;
  settings.setValue(iterationSettingName, spinIterations->value());
  settings.setValue(conductanceSettingName, spinConductance->value());
  settings.setValue(timeStepSettingName, spinTimeStep->value());
  settings.setValue(levelSettingName, spinLevel->value());
  settings.setValue(thresholdSettingName, spinThreshold->value());
  settings.setValue(gaussSigmaSettingName, spinGaussSigma->value());
  ImageTracer::FilterType filter;
  if (radioGauss->isChecked()) filter = ImageTracer::GaussFilter;
  if (radioGA->isChecked()) filter = ImageTracer::GADiffusion;
  if (radioCA->isChecked()) filter = ImageTracer::CADiffusion;
  settings.setValue(filterTypeSettingName, filter);
  tracer->setParameters( spinIterations->value(),
			 spinConductance->value(),
			 spinTimeStep->value(),
			 spinLevel->value(),
			 spinThreshold->value(), 
			 spinGaussSigma->value(),
			 filter);
}


void TraceWindow::on_buttonApply_clicked() {
  switch(toolBox->currentIndex()) {
    case 0:
      imageConverter.setInput( tracer->getInput() );
      graphicsView->setScene( imageConverter.getGraphicsScene().get() );
      break;
    case 1:
      imageConverter.setInput( tracer->getSmoothed() );
      graphicsView->setScene( imageConverter.getGraphicsScene().get() );
      break;
    case 2:
      imageConverter.setInput( tracer->getWatershed() );
      graphicsView->setScene( imageConverter.getGraphicsScene().get() );
      break;
  }
}


void TraceWindow::on_buttonZoomIn_clicked() {
  graphicsView->scale(1.1, 1.1);
}

void TraceWindow::on_buttonZoomOut_clicked() {
  graphicsView->scale(.9, .9);
  std::cerr << __FUNCTION__ << std::endl;
}
