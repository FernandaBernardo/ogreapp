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

	capture = cvCaptureFromCAM(0);
	image = cvQueryFrame(capture);
	rawdata = (unsigned char*)(image->imageData);

	CapTex = Ogre::TextureManager::getSingleton().createManual("CapTex",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    Ogre::TEX_TYPE_2D,
                    640, 480, 0,
                    Ogre::PF_L8/*PF_R3G3B2*/, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

    buffer = CapTex->getBuffer();
    buffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
    Ogre::uint8* data = (Ogre::uint8*)buffer->getCurrentLock().data;

    for (int i = 0; i < image->height; i++ ) {
        for(int j=0; j < image->width; j++ ) {
            data[(j + (i*image->width))] = image->imageData[j+(i*image->width)];
        }
    }

    buffer->unlock();

    CapMat = Ogre::MaterialManager::getSingleton().create("CapMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    CapMat->getTechnique(0)->getPass(0)->createTextureUnitState("CapTex");
    CapMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    CapMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    CapMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    CapTex->getBuffer()->blitFromMemory(Ogre::PixelBox(CapTex->getWidth(), CapTex->getHeight(),
                            CapTex->getDepth(), CapTex->getFormat()/*PF_A8R8G8B8*/, data));

    Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
    rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    rect->setMaterial("CapMat");

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);

    rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

    Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(rect);
}
//-------------------------------------------------------------------------------------
//void TutorialApplication::createFrameListener(void){
//	BaseApplication::createFrameListener();
//}
//-------------------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt){
	//mPlaneNode->yaw(Ogre::Radian(evt.timeSinceLastFrame));

    image = cvQueryFrame(capture);
    buffer = CapTex->getBuffer();

    buffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
    Ogre::uint8* data = (Ogre::uint8*)buffer->getCurrentLock().data;
    buffer->unlock();

    CapTex->getBuffer()->blitFromMemory(Ogre::PixelBox(CapTex->getWidth(), CapTex->getHeight(),
                            CapTex->getDepth(), CapTex->getFormat()/*PF_A8R8G8B8*/, data));

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
