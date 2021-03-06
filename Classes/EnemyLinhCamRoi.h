#ifndef __ENEMY_LINH_CAM_ROI_H__
#define __ENEMY_LINH_CAM_ROI_H__

#include "EnemyLinhCamGiao.h"

#define SOUND_ROIAT		"sound/roiat.mp3"
#define SOUND_ROIDIE	"sound/roidie.mp3"
class EnemyLinhCamRoi : public EnemyLinhCamGiao
{
public:
	EnemyLinhCamRoi(spSkeletonData*data);
	static EnemyLinhCamRoi* create(string filename, float scale);

	void initBoxPhysic(b2World * world, Point pos);
	virtual void playsoundAt();
	virtual void playsoundDie();
	
};

#endif // __ENEMY_H__
