#include "VectorImageArea.h"

#include <exception>
#include <boost/foreach.hpp>
#include <iostream>

void VectorImageArea::drawContent(CairoContextPointer c) {
  BOOST_FOREACH( VectorShape &vect, m_vectors) {
    c->set_line_width(0.2);
    c->begin_new_path();
    c->move_to( vect.back().endPoint.x, vect.back().endPoint.y );
    BOOST_FOREACH( Segment &seg, vect ) {
      switch( seg.type ) {
	case CornerSegment:
	  c->line_to( seg.w.x, seg.w.y );
	  c->line_to( seg.endPoint.x, seg.endPoint.y );
	  break;
	case BezierSegment:
	  c->curve_to( seg.u.x, seg.u.y,
		       seg.w.x, seg.w.y,
		       seg.endPoint.x, seg.endPoint.y );
	  break;
	default:
	  throw std::runtime_error("unknown SegmentType");
      }
    }
    c->stroke();
  }
}

void VectorImageArea::addVector( const VectorShape &vect ) {
  m_vectors.push_back( vect );
}
