#ifndef IMAGETRACER_H
#define IMAGETRACER_H

#include "itkimagetypes.h"
#include <itkVectorCastImageFilter.h>
#include <itkVectorCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkVectorGradientMagnitudeImageFilter.h>
#include <itkVectorGradientAnisotropicDiffusionImageFilter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkWatershedImageFilter.h>


class ImageTracer
{
  public:
    typedef ColorImageType TImage;
    enum FilterType {
      GaussFilter,
      CADiffusion,
      GADiffusion
    };
    ImageTracer();
    void setInput(TImage::Pointer image) { casterI2V->SetInput(image); }
    TImage::ConstPointer getInput();
    TImage::ConstPointer getSmoothed();
    TImage::ConstPointer getWatershed();
    void setParameters( unsigned iterations, 
			float conductance, 
			float timeStep, 
			float level,
			float threshold,
			float gaussSigma,
			FilterType filter);
    
    
  private:
    typedef itk::Vector<float, 3>          VectorPixelType;
    typedef itk::Image<VectorPixelType, 2> VectorImageType;
    typedef itk::VectorCastImageFilter<TImage, VectorImageType> CastI2VFilterType;
    typedef itk::VectorCurvatureAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> CADiffusionFilterType;
    typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> GADiffusionFilterType;
    typedef itk::VectorGradientMagnitudeImageFilter< VectorImageType, FloatPixelType, FloatImageType > GradientMagnitudeImageFilterType;
    typedef itk::RecursiveGaussianImageFilter< VectorImageType, VectorImageType > GaussianImageFilterType;
    typedef itk::WatershedImageFilter<FloatImageType> WatershedFilterType;
    Labeled2dImageType::Pointer labelImage;
//    std::vector< LabelPixelType > labelVector;
    CastI2VFilterType::Pointer casterI2V;
    CADiffusionFilterType::Pointer cadiffusion;
    GADiffusionFilterType::Pointer gadiffusion;
    GradientMagnitudeImageFilterType::Pointer gradient;
    GaussianImageFilterType::Pointer gaussX;
    GaussianImageFilterType::Pointer gaussY;
    WatershedFilterType::Pointer watershed;
};

#endif // IMAGETRACER_H
