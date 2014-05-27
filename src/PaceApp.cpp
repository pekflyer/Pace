#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "Kinect.h"
#include "Resources.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace MsKinect;

class PaceApp : public ci::app::AppBasic  {
  public:
	  void					draw();
	  void					keyDown(ci::app::KeyEvent event);
	  void					prepareSettings(ci::app::AppBasic::Settings* settings);
	  void					resize();
	  void					shutdown();
	  void					setup();
	  void					update();
 private:
	 MsKinect::DeviceRef	mDevice;
	 std::vector<MsKinect::Skeleton>	mSkeletons;
	 ci::gl::TextureRef					mTextureDepth;
	 void								onFrame(MsKinect::Frame frame, const MsKinect::DeviceOptions& deviceOptions);
	 ci::gl::GlslProgRef				mShader;
	 ci::gl::VboMeshRef					mVboMesh;

	 float					mFrameRate;
	 float					mDepth;
	 bool					mFullScreen;
	 bool					mFullScreenPrev;
	 int					mUserCount;

	 ci::CameraPersp						mCamera;
	 ci::Vec3f							mEyePoint;
	 ci::Vec3f							mLookAt;
	 ci::Vec3f							mRotation;

};


void PaceApp::setup()
{
	mDevice = Device::create();
	mDevice->connectEventHandler(&PaceApp::onFrame, this);
	
	mDevice->removeBackground();
	mDevice->enableUserColor(true);
	mDevice->enableBinaryMode(true);
	

	try {
		mDevice->start(DeviceOptions().enableColor(false).enableSkeletonTracking(false).setDepthResolution(ImageResolution::NUI_IMAGE_RESOLUTION_80x60));
	}
	catch (Device::ExcDeviceCreate ex) {
		console() << ex.what() << endl;
	}
	catch (Device::ExcDeviceInit ex) {
		console() << ex.what() << endl;
	}
	catch (Device::ExcDeviceInvalid ex) {
		console() << ex.what() << endl;
	}
	catch (Device::ExcOpenStreamDepth ex) {
		console() << ex.what() << endl;
	}

	mFrameRate  = 0.0f;
	mDepth		= 500.0f;
	mFullScreen = false;
	mFullScreenPrev = mFullScreen;
}
void PaceApp::keyDown(KeyEvent event)
{
}

void PaceApp::prepareSettings(Settings* settings)
{
	settings->setWindowSize(1280, 720);
	settings->setFrameRate(60.0f);
	settings->setFullScreen(false);
}

void PaceApp::onFrame(Frame frame, const DeviceOptions& deviceOptions)
{
	if (mTextureDepth) {
		mTextureDepth->update(Surface32f(frame.getDepthSurface()));
	}
	else {
		mTextureDepth = gl::Texture::create(frame.getDepthSurface());
	}
	mSkeletons = frame.getSkeletons();
}

void PaceApp::update()
{
	mFrameRate = getAverageFps();
	if (mFullScreen != mFullScreenPrev) {
		setFullScreen(mFullScreen);
		mFullScreenPrev = mFullScreen;
	}

	if (!mDevice->isCapturing()) {
		if (getElapsedFrames() % 90 == 0) {
			mDevice->start();
		}
	}
	//console() << mDevice->getUserCount() << endl; 
	//console() << mDevice->getFrameRate() << endl;
}

void PaceApp::draw()
{
	// clear out the window with black
	gl::clear(ColorA(1.0f, 1.0f, 1.0f));


	
}
void PaceApp::shutdown()
{
	mDevice->stop();
}
void PaceApp::resize()
{
	gl::enable(GL_DEPTH_TEST);
	gl::enable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	gl::enable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	gl::enable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	gl::enable(GL_TEXTURE_2D);
	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::enableAlphaBlending();

	mEyePoint = Vec3f(0.0f, 0.0f, 100.0f);
	mLookAt = Vec3f::zero();
	mRotation = Vec3f::zero();
	mCamera.lookAt(mEyePoint, mLookAt);
	mCamera.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 6000.0f);
}


CINDER_APP_BASIC(PaceApp, RendererGl)
