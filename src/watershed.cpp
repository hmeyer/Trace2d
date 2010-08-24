#ifndef WATERSHED_CPP_H
#define WATERSHED_CPP_H

#include "watershed.h"
#include <iostream>
#include <set>
#include "colormapper.h"



WaterShedSegmentor::WaterShedSegmentor() {
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

void WaterShedSegmentor::SetInput( TImagePointer inp) {
	input = inp;
	casterI2V->SetInput( input );
}

WaterShedSegmentor::TImagePointer WaterShedSegmentor::GetFiltered() {
	gradient->GetOutput();
typedef unsigned char MonoPixelType;
typedef itk::Image<MonoPixelType, 2>  MonoImageType;
	typedef itk::UnaryFunctorImageFilter<Labeled2dImageType,
	TImage, ColorMapper> ColorMapFilterType;
	
	ColorMapper colormapper;
	watershed->Update();
	labelImage = watershed->GetOutput();
	LabelSetType labelSet;
//	colormapper.InitializeFilter( input, labelImage, labelSet );
//	.clear();
	for(LabelSetType::const_iterator si = labelSet.begin(); si != labelSet.end(); si++) labelVector.push_back(*si);

	ColorMapFilterType::Pointer colormapfilter = ColorMapFilterType::New();
	colormapfilter->SetFunctor( colormapper );
	colormapfilter->SetInput( labelImage );
	colormapfilter->Update();
	TImagePointer result = colormapfilter->GetOutput();
	return result;
}

unsigned int WaterShedSegmentor::getNumLabels() const {
  return labelVector.size();
}

Labeled2dImageType::Pointer WaterShedSegmentor::getWaterShedImage() const {
  return labelImage;
}


LabelPixelType WaterShedSegmentor::getLabel( unsigned int labelIndex ) const {
  return labelVector.at( labelIndex );
}


void WaterShedSegmentor::SetNumberOfIterations( unsigned int Iterations ) {
	diffusion->SetNumberOfIterations( Iterations );
}

void WaterShedSegmentor::SetConductanceParameter( double Conductance ) {
	diffusion->SetConductanceParameter( Conductance );
	watershed->SetLevel( Conductance );
}

void WaterShedSegmentor::SetTimeStep( double TimeStep ) {
	diffusion->SetTimeStep( TimeStep );
	watershed->SetThreshold( TimeStep );
}

#endif