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
  void printPath(std::ostream &out);
  void trace(void);
protected:
  void internalPrintPath(std::ostream &out, const potrace_path_t *p, const std::string &intend = std::string());
  static void progress_callback( double progress, void *data );
  potrace_param_t getParams(void) const;

  ProgressSignalType m_progress;
  scoped_array< potrace_word > m_bitmap;
  shared_ptr< potrace_param_t > m_potrace_param;
  shared_ptr< potrace_state_t > m_potrace_state;
  scoped_ptr< potrace_bitmap_t > m_potrace_bitmap;
};

void TraceBitmap::printPath(std::ostream &out) {
  if (m_potrace_state) {
    if (m_potrace_state->status == POTRACE_STATUS_OK)
      internalPrintPath(out, m_potrace_state->plist);
    else
      out << "unsuccessful tracing" << std::endl;
  }
}

void printPoint( std::ostream &out, const potrace_dpoint_t &p ) {
    out << "[" << p.x << "," << p.y << "]";
}

void TraceBitmap::internalPrintPath(std::ostream &out, const potrace_path_t *p, const std::string &intend) {
  if (p) {
    out << intend << "curve[" << (char)p->sign <<"]: ";
    const potrace_curve_t &cu = p->curve;
    printPoint( out, cu.c[ cu.n -1 ][2] );
    for(int i = 0; i< cu.n; i++) {
      if (cu.tag[i] == POTRACE_CORNER) {
	out << "->"; printPoint( out, cu.c[i][1] );
	out << "->"; printPoint( out, cu.c[i][2] );
      } else if (cu.tag[i] == POTRACE_CURVETO) {
	out << "~"; printPoint( out, cu.c[i][0] );
	out << "~"; printPoint( out, cu.c[i][1] );
	out << "~"; printPoint( out, cu.c[i][2] );
      } else {
	out << "[unknown segment]";
      }
    }
    out << std::endl;
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
	
	m_bitmap.reset( new potrace_word[ dy * h ]);
	m_potrace_bitmap->map = m_bitmap.get();
	
	IteratorType it( binaryImage, imageRegion ); it.GoToBegin();
	potrace_word pw;
	unsigned int pw_mask = (2 << sizeof( potrace_word))-1;
	unsigned int pidx = 0;
	for(int y = 0; y < h; y++) {
	  pw = 0;
	  int x;
	  for(x = 0; x < w; x++) {
	    if (it.Get()) pw|=1;
	    if ((x & pw_mask) == pw_mask) {
	      m_bitmap[pidx++] = pw;
	      pw = 0;
	    } else {
	      pw <<= 1;
	    }
	    ++it;
	  }
	  for(; x < dy; x++) {
	    if ((x & pw_mask) == pw_mask) {
	      m_bitmap[pidx++] = pw;
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

void writeProgress( float progress ) {
  std::cerr << "tracing: " << progress*100 << "%" << std::endl;
}

void traceLabels( Labeled2dImageType::Pointer labelImage, const std::set< LabelPixelType > &labels ) {
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
  tb.printPath( std::cerr );
}

