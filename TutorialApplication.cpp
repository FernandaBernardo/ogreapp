#include "TutorialApplication.h"

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void){
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void){
}
//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void){
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	mCamera->setPosition(60, 200, 70);
	mCamera->lookAt(0,0,0);

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("PlaneMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture = mat->getTechnique(0)->getPass(0)->createTextureUnitState("grass_1024.jpg");

	mPlane = new Ogre::MovablePlane("Plane");
	mPlane->d = 0;
	mPlane->normal = Ogre::Vector3::UNIT_Y;

	Ogre::MeshManager::getSingleton().createPlane("PlaneMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *mPlane, 120, 120, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
	mPlaneEnt = mSceneMgr->createEntity("PlaneEntity", "PlaneMesh");
	mPlaneEnt->setMaterialName("PlaneMat");

	mPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mPlaneNode->attachObject(mPlaneEnt);

}
//-------------------------------------------------------------------------------------
void TutorialApplication::createFrameListener(void){
	BaseApplication::createFrameListener();

	mTrayMgr->hideLogo();
}
//-------------------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt){
	mPlaneNode->yaw(Ogre::Radian(evt.timeSinceLastFrame));

	cv::VideoCapture cap(0);

    if (!cap.isOpened()) { // if not success, exit program
        return false;
    }

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    cv::Mat frame;

    bool bSuccess = cap.read(frame); // read a new frame from video

    if (!bSuccess) { //if not success, break loop
        return false;
    }

    imshow("MyVideo", frame); //show the frame in "MyVideo" window

    if (cv::waitKey(30) == 27) {
        return false;
    }

	return BaseApplication::frameRenderingQueued(evt);
}

//-------------------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
