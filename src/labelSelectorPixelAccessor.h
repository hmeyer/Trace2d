#ifndef LABELSELECTORPIXELACCESSOR_H
#define LABELSELECTORPIXELACCESSOR_H

#include "itkimagetypes.h"
#include <set>


class LabelSelectorPixelAccessor {
public:
	typedef LabelPixelType InternalType;
	typedef BinaryPixelType ExternalType;
	typedef std::set< InternalType > LabelSet;
  	ExternalType Get( const InternalType & input ) const;
	void SetLabels( const LabelSet &lset );
	void operator=( const LabelSelectorPixelAccessor &vpa );
private:
	LabelSet m_labels;
};


#endif
