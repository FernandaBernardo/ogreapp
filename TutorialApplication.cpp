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
                    image->width, image->height, 0,
                    Ogre::PF_BYTE_BGRA, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

    updateTexture();

    CapMat = Ogre::MaterialManager::getSingleton().create("CapMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    CapMat->getTechnique(0)->getPass(0)->createTextureUnitState("CapTex");
    CapMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    CapMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    CapMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

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
void TutorialApplication::createFrameListener(void){
	BaseApplication::createFrameListener();
}
//-------------------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt){
    image = cvQueryFrame(capture);
    updateTexture();
	return BaseApplication::frameRenderingQueued(evt);
}

void TutorialApplication::updateTexture(){
    buffer = CapTex->getBuffer();

    buffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
    const Ogre::PixelBox& pixelBox = buffer->getCurrentLock();
    Ogre::uint8* data = (Ogre::uint8*)pixelBox.data;

    int h = image->height;
    int w = image->width;

    for (int i = 0; i < h; i++ ) {
        for(int j=0; j < w; j++ ) {
            data[((j+(i*w))*4)] = image->imageData[(j+(i*w)*3)];//X
            data[((j+(i*w))*4)+1] = image->imageData[(j+(i*w)*3)+1];//
            data[((j+(i*w))*4)+2] = image->imageData[(j+(i*w)*3)+2];
            data[((j+(i*w))*4)+3] = image->imageData[(j+(i*w)*3)+3];
        }
        data += pixelBox.getRowSkip() * Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
    }

    buffer->unlock();

    CapTex->getBuffer()->blitFromMemory(Ogre::PixelBox(CapTex->getWidth(), CapTex->getHeight(),
                            CapTex->getDepth(), CapTex->getFormat()/*PF_A8R8G8B8*/, data));
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
