#include "trace.h"
extern "C" {
#include "potracelib.h"
}

#include "labelSelectorPixelAccessor.h"
#include "itkImageAdaptor.h"
#include "itkImageRegionConstIterator.h"
#include <boost/scoped_array.hpp>
#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "VectorShape.h"


using boost::scoped_array;
using boost::scoped_ptr;
using boost::shared_ptr;

class TraceBitmap {
public:
  typedef boost::signal<void (float)> ProgressSignalType;
  TraceBitmap();
  template< class ImageConstPointerType >
  void setImage( ImageConstPointerType binaryImage );
  boost::signals::connection doOnProgress( const ProgressSignalType::slot_type &slot);
  void printPath(std::ostream &out) const;
  void trace(void);
  void pathToShapeList( ShapeList &sl ) const;
protected:
  static void internalPrintPath(std::ostream &out, const potrace_path_t *p, const std::string &intend = std::string());
  static void internalPathToShapeList(ShapeList &sl, const potrace_path_t *p);
  static void progress_callback( double progress, void *data );
  potrace_param_t getParams(void) const;

  ProgressSignalType m_progress;
  scoped_array< potrace_word > m_bitmap;
  shared_ptr< potrace_param_t > m_potrace_param; //needed shared_ptr for custom deleters
  shared_ptr< potrace_state_t > m_potrace_state; //needed shared_ptr for custom deleters
  scoped_ptr< potrace_bitmap_t > m_potrace_bitmap;
};

void TraceBitmap::printPath(std::ostream &out) const {
  if (m_potrace_state) {
    if (m_potrace_state->status == POTRACE_STATUS_OK)
      internalPrintPath(out, m_potrace_state->plist);
    else
      out << "unsuccessful tracing" << std::endl;
  }
}

std::ostream& operator<< ( std::ostream& out, const potrace_dpoint_t &p ) {
    out << "[" << p.x << "," << p.y << "]";
    return out;
}

std::ostream& operator<< ( std::ostream& out, const potrace_curve_t &cu ) {
    out <<  cu.c[ cu.n -1 ][2];
    for(int i = 0; i< cu.n; i++) {
      if (cu.tag[i] == POTRACE_CORNER) {
	out << "->" << cu.c[i][1] << "->" << cu.c[i][2];
      } else if (cu.tag[i] == POTRACE_CURVETO) {
	out << "~" << cu.c[i][0] << "~" << cu.c[i][1] << "~" << cu.c[i][2];
      } else {
	out << "[unknown segment]";
      }
    }
    return out;
}

void TraceBitmap::internalPrintPath(std::ostream &out, const potrace_path_t *p, const std::string &intend) {
  if (p) {
    out << intend << (char)p->sign << "curve[" << p->area <<"]: " << p->curve << std::endl;
    if (p->childlist) internalPrintPath( out, p->childlist, intend+"  ");
    if (p->sibling) internalPrintPath( out, p->sibling, intend);
  } else out << "NULL" << std::endl;
}


TraceBitmap::TraceBitmap() {
  typedef boost::function< void (potrace_param_t *)> potrace_param_DeleterType;
  m_potrace_param.reset( potrace_param_default(), potrace_param_DeleterType( &potrace_param_free ) );
  m_potrace_param->progress.callback = &TraceBitmap::progress_callback;
  m_potrace_param->progress.data = this;
}

void TraceBitmap::progress_callback( double progress, void *data ) {
  TraceBitmap *tb = static_cast<TraceBitmap*>(data);
  tb->m_progress( progress );
}

boost::signals::connection TraceBitmap::doOnProgress( const ProgressSignalType::slot_type &slot) {
  return m_progress.connect( slot );
}

template< class ImageConstPointerType >
void TraceBitmap::setImage( ImageConstPointerType binaryImage ) {
	typedef typename ImageConstPointerType::ObjectType ImageType;
	typedef typename ImageType::RegionType RegionType;
	typedef typename itk::ImageRegionConstIterator< ImageType > IteratorType;

	RegionType imageRegion = binaryImage->GetLargestPossibleRegion();
	m_potrace_bitmap.reset( new potrace_bitmap_t );
	int &h = m_potrace_bitmap->h;
	int &w = m_potrace_bitmap->w;
	int &dy = m_potrace_bitmap->dy;
	w = imageRegion.GetSize(0);
	h = imageRegion.GetSize(1);
	
	dy = w / ( sizeof( potrace_word ) * 8 ) 
		+ (( w % ( sizeof( potrace_word ) * 8 )) ? 1:0);
	int dy_bits = dy * sizeof( potrace_word ) * 8;
	
	m_bitmap.reset( new potrace_word[ dy * h ]);
	m_potrace_bitmap->map = m_bitmap.get();
	
	IteratorType it( binaryImage, imageRegion ); it.GoToBegin();
	potrace_word pw;
	unsigned int pw_mask = (sizeof( potrace_word)*8)-1;
	unsigned int pIdx = 0;
	for(int y = 0; y < h; y++) {
	  pw = 0;
	  int x;
	  for(x = 0; x < w; x++) {
	    if (it.Get()) pw|=1;
	    if ((x & pw_mask) == pw_mask) {
	      m_bitmap[pIdx++] = pw;
	      pw = 0;
	    } else {
	      pw <<= 1;
	    }
	    ++it;
	  }
	  for(; x < dy_bits; x++) {
	    if ((x & pw_mask) == pw_mask) {
	      m_bitmap[pIdx++] = pw;
	      pw = 0;
	    } else {
	      pw <<= 1;
	    }
	  }
	}
}

void TraceBitmap::trace(void) {
  typedef boost::function< void (potrace_state_t *)> potrace_state_DeleterType;
  if (m_potrace_bitmap)
    m_potrace_state.reset( potrace_trace( m_potrace_param.get(), m_potrace_bitmap.get()), potrace_state_DeleterType( &potrace_state_free ) );
  else throw std::runtime_error("did not set any image");
}

void TraceBitmap::pathToShapeList( ShapeList &sl ) const {
  if (m_potrace_state) {
    if (m_potrace_state->status == POTRACE_STATUS_OK)
      internalPathToShapeList(sl, m_potrace_state->plist);
    else
      throw std::runtime_error("unsuccessful tracing");
  }
}

void TraceBitmap::internalPathToShapeList(ShapeList &sl, const potrace_path_t *p) {
  if (p) {
    sl.push_back( VectorShape() );
    VectorShape &vs = sl.back();
    const potrace_curve_t &cu = p->curve;
    for(int i = 0; i< cu.n; i++) {
      vs.push_back( Segment() );
      Segment &seg = vs.back();
      if (cu.tag[i] == POTRACE_CORNER) {
	seg.type = CornerSegment;
	seg.w.x = cu.c[i][1].x;
	seg.w.y = cu.c[i][1].y;
	seg.endPoint.x = cu.c[i][2].x;
	seg.endPoint.y = cu.c[i][2].y;
      } else if (cu.tag[i] == POTRACE_CURVETO) {
	seg.type = BezierSegment;
	seg.u.x = cu.c[i][0].x;
	seg.u.y = cu.c[i][0].y;
	seg.w.x = cu.c[i][1].x;
	seg.w.y = cu.c[i][1].y;
	seg.endPoint.x = cu.c[i][2].x;
	seg.endPoint.y = cu.c[i][2].y;
      } else {
	throw std::runtime_error("unknown segment type");
      }
    }
    if (p->childlist) internalPathToShapeList( sl, p->childlist);
    if (p->sibling) internalPathToShapeList( sl, p->sibling);
  }
}


void writeProgress( float progress ) {
  std::cerr << "tracing: " << progress*100 << "%" << std::endl;
}

void traceLabels( Labeled2dImageType::Pointer labelImage, const std::set< LabelPixelType > &labels, ShapeList &shapelist ) {
  typedef itk::ImageAdaptor<  Labeled2dImageType, LabelSelectorPixelAccessor > ImageAdaptorType;
  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
  LabelSelectorPixelAccessor accessor;
  accessor.SetLabels( labels );
  adaptor->SetPixelAccessor( accessor );
  adaptor->SetImage( labelImage );
  TraceBitmap tb;
  tb.setImage( adaptor );
  tb.doOnProgress( boost::function< void (float) >( &writeProgress ));
  tb.trace();
  tb.pathToShapeList( shapelist );
  tb.printPath( std::cerr);
}

