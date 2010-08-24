#include "ImageTracer.h"
#include "colormapper.h"
//#include <boost/foreach.hpp>

/*
const VectorShapeList &ImageTracer::GetVectorShapeList(void) {
  vectorShapeList.clear();
  LabelSet labSet;
  BOOST_FOREACH(LabelPixelType &label, labelVector) {
    labSet.insert( label );
    traceLabels( labelImage, labSet, vectorShapeList );
  }
  return vectorShapeList;
}
*/

ImageTracer::ImageTracer()
  :casterI2V( CastI2VFilterType::New() ),
  cadiffusion( CADiffusionFilterType::New() ),
  gadiffusion( GADiffusionFilterType::New() ),
  gradient( GradientMagnitudeImageFilterType::New() ),
  gaussX( GaussianImageFilterType::New()),
  gaussY( GaussianImageFilterType::New()),
  watershed( WatershedFilterType::New() ) {
    cadiffusion->SetInput(casterI2V->GetOutput());
    gadiffusion->SetInput(casterI2V->GetOutput());
    gradient->SetInput( casterI2V->GetOutput());
    gradient->SetUsePrincipleComponents( true );
    gaussX->SetDirection(0);
    gaussX->SetInput( casterI2V->GetOutput() );
    gaussY->SetDirection(1);
    gaussY->SetInput( gaussX->GetOutput() );
    watershed->SetInput( gradient->GetOutput());
}

ImageTracer::TImage::ConstPointer ImageTracer::getInput() { 
  TImage::ConstPointer t = casterI2V->GetInput(); 
  return t; 
}


itk::Image< RGBPixelType, 2 >::ConstPointer ImageTracer::getSmoothed() {
  typedef itk::VectorCastImageFilter<VectorImageType, TImage > CastV2IFilterType;
  CastV2IFilterType::Pointer casterV2I = CastV2IFilterType::New();
  casterV2I->SetInput( gradient->GetInput() );
  casterV2I->Update();
  TImage::ConstPointer t =  casterV2I->GetOutput();
  return t;
}

itk::Image< RGBPixelType, 2 >::ConstPointer ImageTracer::getWatershed() {
  watershed->Update();
  Labeled2dImageType::Pointer labelImage = watershed->GetOutput();
  
  typedef unsigned char MonoPixelType;
  typedef itk::Image<MonoPixelType, 2>  MonoImageType;
    typedef itk::UnaryFunctorImageFilter<Labeled2dImageType,
    TImage, ColorMapper> ColorMapFilterType;

  ColorMapper colormapper;
  LabelSetType labelSet;
  colormapper.InitializeFilter( casterI2V->GetInput(), labelImage.GetPointer(), labelSet );
  std::vector< LabelPixelType > labelVector;
  labelVector.clear();
  for(LabelSetType::const_iterator si = labelSet.begin(); si != labelSet.end(); si++) labelVector.push_back(*si);

  ColorMapFilterType::Pointer colormapfilter = ColorMapFilterType::New();
  colormapfilter->SetFunctor( colormapper );
  colormapfilter->SetInput( labelImage );
  colormapfilter->Update();
  TImage::ConstPointer result = colormapfilter->GetOutput();
  return result;
}

void ImageTracer::setParameters( unsigned iterations, 
			float conductance,
			float timeStep,
			float level,
			float threshold,
			float gaussSigma,
			FilterType filter) {
  cadiffusion->SetNumberOfIterations(iterations);
  cadiffusion->SetConductanceParameter(conductance);
  cadiffusion->SetTimeStep(timeStep);

  gadiffusion->SetNumberOfIterations(iterations);
  gadiffusion->SetConductanceParameter(conductance);
  gadiffusion->SetTimeStep(timeStep);
  
  gaussX->SetSigma( gaussSigma );
  gaussY->SetSigma( gaussSigma );
  
  watershed->SetLevel(level);
  watershed->SetThreshold(threshold);
  
  switch(filter) {
    case GaussFilter:
      gradient->SetInput( gaussY->GetOutput() );
      break;
    case CADiffusion:
      gradient->SetInput( cadiffusion->GetOutput() );
      break;
    case GADiffusion:
      gradient->SetInput( gadiffusion->GetOutput() );
      break;
  }
}
