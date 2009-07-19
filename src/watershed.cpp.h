#ifndef WATERSHED_CPP_H
#define WATERSHED_CPP_H

#include "watershed.h"
#include <iostream>

#include "colormapper.h"



template< class TImage >
WaterShedSegmentor< TImage >::WaterShedSegmentor() {
	casterI2V = CastI2VFilterType::New();
	casterV2I = CastV2IFilterType::New();
	diffusion = DiffusionFilterType::New();
	gradient = GradientMagnitudeImageFilterType::New();
	watershed = WatershedFilterType::New();
	

	watershed->SetInput( gradient->GetOutput());


	gradient->SetInput( diffusion->GetOutput());
	gradient->SetUsePrincipleComponents( true );

	diffusion->SetInput(casterI2V->GetOutput());
	casterV2I->SetInput(diffusion->GetOutput());
}

template< class TImage >
void WaterShedSegmentor< TImage >::SetInput( TImagePointer inp) {
	casterI2V->SetInput( inp );
}

template< class TImage >
typename TImage::Pointer WaterShedSegmentor< TImage >::GetFiltered() {

	gradient->GetOutput();
	


typedef unsigned char  MonoPixelType;
typedef itk::Image<MonoPixelType, 2>  MonoImageType;
	typedef itk::Functor::ScalarToRGBPixelFunctor<LabelPixelType> ColorMapFunctorType;
	typedef itk::UnaryFunctorImageFilter<Labeled2dImageType,
	TImage, ColorMapFunctorType> ColorMapFilterType;
	typename ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
	colormapper->SetInput( watershed->GetOutput());
	colormapper->Update();

	TImagePointer result = colormapper->GetOutput();

	

	return result;
}

template< class TImage >
void WaterShedSegmentor< TImage >::SetNumberOfIterations( unsigned int Iterations ) {
	diffusion->SetNumberOfIterations( Iterations );
}

template< class TImage >
void WaterShedSegmentor< TImage >::SetConductanceParameter( double Conductance ) {
	diffusion->SetConductanceParameter( Conductance );
	watershed->SetLevel( Conductance );
}

template< class TImage >
void WaterShedSegmentor< TImage >::SetTimeStep( double TimeStep ) {
	diffusion->SetTimeStep( TimeStep );
	watershed->SetThreshold( TimeStep );
}

/*
template<class TImagePointer>
TImagePointer ImageDiffusionFilter(TImagePointer input,
	unsigned int Iterations, double Conductance, double TimeStep) {

	typedef typename TImagePointer::ObjectType TImage;
	typedef itk::Vector<float, 3>          VectorPixelType;
	typedef itk::Image<VectorPixelType, 2> VectorImageType;
	typedef itk::VectorCastImageFilter<TImage, VectorImageType> CastI2VFilterType;
	typedef itk::VectorCastImageFilter<VectorImageType, TImage > CastV2IFilterType;
	
	typedef itk::VectorCurvatureAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> DiffusionFilterType;

	CastI2VFilterType::Pointer casterI2V = CastI2VFilterType::New();
	CastV2IFilterType::Pointer casterV2I = CastV2IFilterType::New();

	DiffusionFilterType::Pointer diffusion = DiffusionFilterType::New();
	diffusion->SetNumberOfIterations( Iterations );
	diffusion->SetConductanceParameter( Conductance );
	diffusion->SetTimeStep( TimeStep );

	casterI2V->SetInput( input );
	diffusion->SetInput(casterI2V->GetOutput());
	casterV2I->SetInput(diffusion->GetOutput());
	casterV2I->Update();
	TImagePointer result = casterV2I->GetOutput();
	return result;
}
*/

/*

template<class TInputImagePointerType>
Labeled2dImageType::Pointer ImageWatershed(TInputImagePointerType input, 
	unsigned int DiffusionIterations, double DiffusionConductance,
	bool GradientMagnitudeUsePrincipleComponents,
	double WatershedLevel, double WatershedThreshold) {
	typedef typename TInputImagePointerType::ObjectType TInputImageType;
	typedef itk::Vector<float, 3>          VectorPixelType;
	typedef itk::Image<VectorPixelType, 2> VectorImageType;
	typedef itk::Image<float, 2>           ScalarImageType;
	
	typedef itk::VectorCastImageFilter<TInputImageType, VectorImageType> CastFilterType;
	typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType> DiffusionFilterType;
	typedef itk::VectorGradientMagnitudeImageFilter<VectorImageType> GradientMagnitudeFilterType;
	typedef itk::WatershedImageFilter<ScalarImageType> WatershedFilterType;

	DiffusionFilterType::Pointer diffusion = DiffusionFilterType::New();
	diffusion->SetNumberOfIterations( DiffusionIterations );
	diffusion->SetConductanceParameter( DiffusionConductance );
	diffusion->SetTimeStep(0.125);

	GradientMagnitudeFilterType::Pointer gradient = GradientMagnitudeFilterType::New();
	gradient->SetUsePrincipleComponents( GradientMagnitudeUsePrincipleComponents );

	WatershedFilterType::Pointer watershed = WatershedFilterType::New();
	watershed->SetLevel( WatershedLevel );
	watershed->SetThreshold( WatershedThreshold );
	
	typename CastFilterType::Pointer caster = CastFilterType::New();
	caster->SetInput( input );
	diffusion->SetInput(caster->GetOutput());
	gradient->SetInput(diffusion->GetOutput());
	watershed->SetInput(gradient->GetOutput());
	watershed->Update();

	Labeled2dImageType::Pointer result = watershed->GetOutput();
	return result;
}
*/
ColorRGBImageType::Pointer LabelImage2ColorRGBImage(Labeled2dImageType::Pointer input) {
	typedef itk::Functor::ScalarToRGBPixelFunctor< LabelPixelType > ColorMapFunctorType;
	typedef itk::UnaryFunctorImageFilter< Labeled2dImageType, ColorRGBImageType, ColorMapFunctorType> ColorMapFilterType;
	ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
	colormapper->SetInput( input );
	ColorRGBImageType::Pointer result = colormapper->GetOutput();
	colormapper->Update();
	return result;
}

#endif