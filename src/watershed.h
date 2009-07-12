#ifndef WATERSHED_H
#define WATERSHED_H

#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include <itkScalarToRGBPixelFunctor.h>
#include <itkVectorCastImageFilter.h>
#include "itkWatershedImageFilter.h"
#include "itkimagetypes.h"

typedef unsigned long LabelPixelType;
typedef itk::Image<LabelPixelType, 2>   LabeledImageType;
typedef float LabelPixelType;
typedef itk::Image<ScalarPixelType, 2>   ScalarImageType;

template <class TImage>
class WaterShedSegmentor {
	typedef typename TImage::Pointer TImagePointer;
	typedef itk::Vector<float, 3>          VectorPixelType;
	typedef itk::Image<VectorPixelType, 2> VectorImageType;
	typedef itk::VectorCastImageFilter<TImage, VectorImageType> CastI2VFilterType;
	typedef itk::VectorCastImageFilter<VectorImageType, TImage > CastV2IFilterType;
	typedef itk::VectorCurvatureAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> DiffusionFilterType;
//	typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> DiffusionFilterType;
	typedef itk::VectorGradientMagnitudeImageFilter< VectorImageType > GradientMagnitudeImageFilterType;
//	typedef itk::WatershedImageFilter<ScalarImageType> WatershedFilterType;
public:
	WaterShedSegmentor();
	void SetInput( TImagePointer inp);
	TImagePointer GetFiltered();
	void SetNumberOfIterations( unsigned int Iterations );
	void SetConductanceParameter( double Conductance );
	void SetTimeStep( double TimeStep );
private:
	typename CastI2VFilterType::Pointer casterI2V;
	typename CastV2IFilterType::Pointer casterV2I;
	typename DiffusionFilterType::Pointer diffusion;
	typename GradientMagnitudeFilterType::Pointer gradient;
};

#include "watershed.cpp.h"

#endif
