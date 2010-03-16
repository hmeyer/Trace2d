#ifndef TRACE_H
#define TRACE_H

#include "itkimagetypes.h"
#include <set>

#include <boost/scoped_array.hpp>

void callpotrace(void);

void traceLabels( Labeled2dImageType::Pointer labelImage, const std::set< LabelPixelType > &labels );




#endif
