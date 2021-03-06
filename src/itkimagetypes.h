#ifndef ITKIMAGETYPES_H
#define ITKIMAGETYPES_H

#include "itkImage.h"
#include "itkRGBPixel.h"

typedef unsigned char RGBComponentType;
typedef itk::RGBPixel< RGBComponentType > RGBPixelType;
typedef itk::Image< RGBPixelType, 2 > ColorRGBImageType;
typedef ColorRGBImageType ColorImageType;
typedef unsigned long LabelPixelType;
typedef itk::Image<LabelPixelType, 2>   Labeled2dImageType;
typedef float FloatPixelType;
typedef itk::Image<FloatPixelType, 2>   FloatImageType;

typedef bool BinaryPixelType;
typedef itk::Image<BinaryPixelType, 2>   Binary2dImageType;


#endif