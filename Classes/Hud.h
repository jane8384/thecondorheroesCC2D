#ifndef __HUD_H__
#define __HUD_H__

#include "Button.h"


class Hud : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();	
	CREATE_FUNC(Hud);

protected:
	CC_SYNTHESIZE(Button*, btnAttack, BtnAttack);
	CC_SYNTHESIZE(Button*, btnSKill_1, BtnSkil_1);


private:
	void addButton();

};

#endif // __HUD_H__