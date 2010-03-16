#ifndef WATERSHED_H
#define WATERSHED_H

#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include <itkScalarToRGBPixelFunctor.h>
#include <itkVectorCastImageFilter.h>
#include "itkWatershedImageFilter.h"
#include <itkScalarToRGBPixelFunctor.h>
#include <itkUnaryFunctorImageFilter.h>
#include "itkimagetypes.h"
#include <vector>


class WaterShedSegmentor {
	typedef ColorImageType TImage;
	typedef TImage::Pointer TImagePointer;
	typedef TImage::PixelType TImagePixelType;
	typedef itk::Vector<float, 3>          VectorPixelType;
	typedef itk::Image<VectorPixelType, 2> VectorImageType;
	typedef itk::VectorCastImageFilter<TImage, VectorImageType> CastI2VFilterType;
	typedef itk::VectorCastImageFilter<VectorImageType, TImage > CastV2IFilterType;
	typedef itk::VectorCurvatureAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> DiffusionFilterType;
//	typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> DiffusionFilterType;
	typedef itk::VectorGradientMagnitudeImageFilter< VectorImageType, ScalarPixelType, ScalarImageType > GradientMagnitudeImageFilterType;
	typedef itk::WatershedImageFilter<ScalarImageType> WatershedFilterType;
public:
	WaterShedSegmentor();
	void SetInput( TImagePointer inp);
	TImagePointer GetFiltered();
	void SetNumberOfIterations( unsigned int Iterations );
	void SetConductanceParameter( double Conductance );
	void SetTimeStep( double TimeStep );
	unsigned int getNumLabels() const;
	LabelPixelType getLabel( unsigned int labelIndex ) const;
	Labeled2dImageType::Pointer getWaterShedImage() const;
private:
	TImagePointer input;
	Labeled2dImageType::Pointer labelImage;
	std::vector< LabelPixelType > labelVector;
	CastI2VFilterType::Pointer casterI2V;
	CastV2IFilterType::Pointer casterV2I;
	DiffusionFilterType::Pointer diffusion;
	GradientMagnitudeImageFilterType::Pointer gradient;
	WatershedFilterType::Pointer watershed;
};


#endif
