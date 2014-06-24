#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    virtual void createScene(void);
    virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    Ogre::HardwarePixelBufferSharedPtr buffer;
    Ogre::TexturePtr CapTex;
    Ogre::MaterialPtr CapMat;
    IplImage *image;
    CvCapture *capture;
    unsigned char *rawdata;
};

#endif // #ifndef __TutorialApplication_h_
