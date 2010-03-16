#include <gtkmm/main.h>
#include "glade/traceUI.hh"
#include <boost/bind.hpp>
#include <itkbasics.h>
#include "imageArea.h"
#include "itkimagetypes.h"
#include "watershed.h"
#include "trace.h"

using boost::bind;
using boost::ref;
using namespace std;


void tracer(WaterShedSegmentor &wss) {
cerr << __FILE__ << "[" << __LINE__ << "]:" << __FUNCTION__ << ": begin" << endl;;
  set< LabelPixelType > labSet;
  if (wss.getNumLabels() > 0) {
    labSet.insert( wss.getLabel(0) );
cerr << __FILE__ << "[" << __LINE__ << "]:" << __FUNCTION__ << ": tracing..." << endl;;
    traceLabels( wss.getWaterShedImage(), labSet );
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

	ImageArea inputImage;
	ImageArea filteredImage;
	myTraceUI.hboxViewer->add( inputImage );
	myTraceUI.hboxViewer->add( filteredImage );
	inputImage.show();
	filteredImage.show();

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
		bind( &ImageArea::setImage, &filteredImage, 
			bind( &WaterShedSegmentor::GetFiltered, ref(myWaterShed) ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &tracer, ref(myWaterShed) ) );
			
	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &ImageArea::queue_draw, &filteredImage ) );

	myTraceUI.buttonExit->signal_clicked().connect( 
		bind( &Gtk::Main::quit ) );

	inputImage.setImage( input );
	filteredImage.setImage( input );
	
	kit.run( myTraceUI );
  	return EXIT_SUCCESS;
}
