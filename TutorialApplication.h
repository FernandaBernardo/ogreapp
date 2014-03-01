#ifndef __BasicTutorial2_h_
#define __BasicTutorial2_h_

#include "BaseApplication.h"

class TutorialApplication : public BaseApplication
{
public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);

protected:
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
};

#endif // #ifndef __BasicTutorial2_h_
