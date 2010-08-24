#ifndef TRACE_H
#define TRACE_H

#include "itkimagetypes.h"
#include <set>
#include <boost/scoped_array.hpp>
#include "VectorShape.h"
#include <list>


typedef std::set< LabelPixelType > LabelSet;

void traceLabels( Labeled2dImageType::Pointer labelImage, const LabelSet &labels, VectorShapeList &shapelist );




#endif
