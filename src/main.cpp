#include <gtkmm/main.h>
#include "glade/traceUI.hh"
#include <boost/bind.hpp>
#include <itkbasics.h>
#include "imageArea.h"
#include "itkimagetypes.h"
#include "watershed.h"

using boost::bind;
using namespace boost;

int main(int argc, char *argv[]) {


	if( argc < 2 )	{
		std::cerr << "Usage: " << argv[0] << " InputImage" << std::endl;
		return EXIT_FAILURE;
	}

	Gtk::Main kit(&argc, &argv);
	traceUI myTraceUI;

	ImageArea inputImage;
	ImageArea filteredImage;
	ImageArea gradientImage;
	myTraceUI.hboxViewer->add( inputImage );
	myTraceUI.hboxViewer->add( filteredImage );
	myTraceUI.hboxViewer->add( gradientImage );
	inputImage.show();
	filteredImage.show();

	ColorRGBImageType::Pointer input = ImageRead<ColorRGBImageType>( argv[1] );
	typedef WaterShedSegmentor< ColorRGBImageType > WaterShedSegmentorRGB;
	WaterShedSegmentorRGB myWaterShed;
	myWaterShed.SetInput( input );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &WaterShedSegmentorRGB::SetNumberOfIterations, ref(myWaterShed), 
			bind( &Gtk::SpinButton::get_value_as_int, myTraceUI.spinIterations ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &WaterShedSegmentorRGB::SetConductanceParameter, ref(myWaterShed), 
			bind( &Gtk::SpinButton::get_value, myTraceUI.spinConductance ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &WaterShedSegmentorRGB::SetTimeStep, ref(myWaterShed), 
			bind( &Gtk::SpinButton::get_value, myTraceUI.spinTimeStep ) ) );


	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &ImageArea::setImage, &filteredImage, 
			bind( &WaterShedSegmentorRGB::GetFiltered, ref(myWaterShed) ) ) );

	myTraceUI.buttonFilter->signal_clicked().connect( 
		bind( &ImageArea::queue_draw, &filteredImage ) );

	myTraceUI.buttonExit->signal_clicked().connect( 
		bind( &Gtk::Main::quit ) );

	inputImage.setImage( input );
	filteredImage.setImage( input );
	
	kit.run( myTraceUI );
  	return EXIT_SUCCESS;
}
