#ifndef __COIN_BAG_H__
#define __COIN_BAG_H__

#include "B2Skeleton.h"



class CoinBag : public B2Skeleton
{

public:
	CoinBag(string jsonFile, string atlasFile,float scale);
	static CoinBag* create(string jsonFile, string atlasFile, float scale);
	SkeletonAnimation *effect;
	void updateMe(float dt);
	void die();
	//void listener();
	void onExit();
	
};

#endif // __B2_SPRITE_H__