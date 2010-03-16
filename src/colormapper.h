#ifndef COLORMAPPER_H
#define COLORMAPPER_H


#include "itkimagetypes.h"
#include <map>
#include <set>

typedef std::set< LabelPixelType > LabelSetType;


class ITK_EXPORT ColorMapper
{
public:
  ColorMapper();
  ~ColorMapper() {};

  RGBPixelType operator()( const LabelPixelType &) const;
  bool operator!=( const ColorMapper &) const;
  void InitializeFilter( const ColorRGBImageType::Pointer rgbImage, const Labeled2dImageType::Pointer labeledImage, LabelSetType &labelSet );
private:
	typedef unsigned long SumType;
	typedef itk::RGBPixel< SumType > RGBSumType;
	typedef struct { RGBSumType colorSum; SumType num; } ColorSumType;
	typedef std::map< LabelPixelType, ColorSumType > ColorSumMapType;

	typedef std::map< LabelPixelType, RGBPixelType > LabelColorMap;
	LabelColorMap avgColor;
};



#endif
