#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PaceApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void PaceApp::setup()
{
}

void PaceApp::mouseDown( MouseEvent event )
{
}

void PaceApp::update()
{
}

void PaceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 


	
}

CINDER_APP_NATIVE( PaceApp, RendererGl )
