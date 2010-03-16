#include "labelSelectorPixelAccessor.h"



LabelSelectorPixelAccessor::ExternalType LabelSelectorPixelAccessor::Get( const InternalType & input ) const {
	if (m_labels.find( input ) != m_labels.end() ) return true;
	else return false;
}
	
void LabelSelectorPixelAccessor::SetLabels( const LabelSelectorPixelAccessor::LabelSet &lset ) {
	m_labels = lset;
}
	
void LabelSelectorPixelAccessor::operator=( const LabelSelectorPixelAccessor &vpa ) {
	m_labels = vpa.m_labels;
}
