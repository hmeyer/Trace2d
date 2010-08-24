#include "colormapper.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <iostream>

RGBPixelType 
ColorMapper::operator()( const LabelPixelType & lp) const {
	if (avgColor.find(lp) == avgColor.end()) {
		return RGBPixelType();
	}
	return avgColor.at(lp);
}

void ColorMapper::InitializeFilter( ColorRGBImageType::ConstPointer rgbImage, Labeled2dImageType::ConstPointer labeledImage, LabelSetType &labelSet ) {
	if (rgbImage->GetLargestPossibleRegion() != labeledImage->GetLargestPossibleRegion()) { return; }
	typedef itk::ImageRegionConstIteratorWithIndex< ColorRGBImageType > RGBIteratorType;
	RGBIteratorType rgbIter( rgbImage, rgbImage->GetLargestPossibleRegion() );

	ColorSumMapType colorSumMap;

	labelSet.clear();
	for( rgbIter.GoToBegin(); !rgbIter.IsAtEnd(); ++rgbIter) {
		LabelPixelType label = labeledImage->GetPixel( rgbIter.GetIndex() );
		labelSet.insert( label );
		colorSumMap[ label ].colorSum += rgbIter.Get();
		colorSumMap[ label ].num++;
	}

	for(ColorSumMapType::iterator i=colorSumMap.begin();i!=colorSumMap.end();++i) {
		SumType num = i->second.num;
		RGBSumType sum = i->second.colorSum;
		avgColor[i->first].Set( sum[0]/num, sum[1]/num, sum[2]/num );
	}
}

bool ColorMapper::operator!=( const ColorMapper &other) const {
	return this->avgColor!=other.avgColor;
}


ColorMapper::ColorMapper() {}
