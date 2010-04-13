#include <gtkmm/main.h>
#include "glade/traceUI.hh"
#include <boost/bind.hpp>
#include <itkbasics.h>
#include "BitmapImageArea.h"
#include "VectorImageArea.h"
#include "itkimagetypes.h"
#include "watershed.h"
#include "trace.h"
#include <boost/foreach.hpp>

using boost::bind;
using boost::ref;
using namespace std;


void tracer(WaterShedSegmentor &wss, VectorImageArea &via) {
cerr << __FILE__ << "[" << __LINE__ << "]:" << __FUNCTION__ << ": begin" << endl;;
  LabelSet labSet;
  ShapeList slist;
  if (wss.getNumLabels() > 0) {
    labSet.insert( wss.getLabel(0) );
cerr << __FILE__ << "[" << __LINE__ << "]:" << __FUNCTION__ << ": tracing..." << endl;;
    traceLabels( wss.getWaterShedImage(), labSet, slist );
cerr << __FILE__ << "[" << __LINE__ << "]:" << __FUNCTION__ << ": tracing finished:" << slist.size() << " curves" << endl;;
    via.clearVectors();
    BOOST_FOREACH( VectorShape &vs, slist ) {
      via.addVector( vs );
    }
    via.queue_draw();
  }
cerr << __FILE__ << "[" << __LINE__ << "]:" << __FUNCTION__ << ": end" << endl;;
}

int main(int argc, char *argv[]) {

	if( argc < 2 )	{
		std::cerr << "Usage: " << argv[0] << " InputImage" << std::endl;
		return EXIT_FAILURE;
	}

	Gtk::Main kit(&argc, &argv);
	traceUI myTraceUI;

	BitmapImageArea inputImage;
	BitmapImageArea filteredImage;
	filteredImage.setUpdateGroup( inputImage );
	VectorImageArea tracedImage;
	tracedImage.setUpdateGroup( filteredImage );
	myTraceUI.hboxViewer->add( inputImage );
	myTraceUI.hboxViewer->add( filteredImage );
	myTraceUI.hboxViewer->add( tracedImage );
	inputImage.show();
	filteredImage.show();
	tracedImage.show();

	ColorRGBImageType::Pointer input = ImageRead<ColorRGBImageType>( argv[1] );
	WaterShedSegmentor myWaterShed;
	myWaterShed.SetInput( input );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &WaterShedSegmentor::SetNumberOfIterations, ref(myWaterShed), 
			bind( &Gtk::SpinButton::get_value_as_int, myTraceUI.spinIterations ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &WaterShedSegmentor::SetConductanceParameter, ref(myWaterShed), 
			bind( &Gtk::SpinButton::get_value, myTraceUI.spinConductance ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &WaterShedSegmentor::SetTimeStep, ref(myWaterShed), 
			bind( &Gtk::SpinButton::get_value, myTraceUI.spinTimeStep ) ) );


	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &BitmapImageArea::setBitmap, &filteredImage, 
			bind( &WaterShedSegmentor::GetFiltered, ref(myWaterShed) ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &tracer, ref(myWaterShed), boost::ref(tracedImage) ) );
			
	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &ImageArea::queue_draw, &filteredImage ) );

	myTraceUI.buttonExit->signal_clicked().connect( 
		bind( &Gtk::Main::quit ) );

	inputImage.setBitmap( input );
	filteredImage.setBitmap( input );
	
	kit.run( myTraceUI );
  	return EXIT_SUCCESS;
}
