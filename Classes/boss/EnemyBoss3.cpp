#include "EnemyBoss3.h"
#include "manager\AudioManager.h"

EnemyBoss3::EnemyBoss3(string jsonFile, string atlasFile, float scale) :EnemyBoss1(jsonFile, atlasFile, scale)
{
	health = 45;
}

EnemyBoss3 * EnemyBoss3::create(string jsonFile, string atlasFile, float scale)
{
	EnemyBoss3* boss = new EnemyBoss3(jsonFile, atlasFile, scale);
	boss->setAnimation(0, "idle", true);
	boss->update(0.0f);
	boss->normalState = new BossIdling();
	boss->crazyState = nullptr;
	boss->setTag(TAG_BOSS);
	boss->scaleBoss = scale;
	boss->setLevelBoss(3);
	boss->autorelease();
	return boss;
}

void EnemyBoss3::createPool()
{
	slashPool = CCArray::createWithCapacity(3);
	slashPool->retain();
	for (int i = 0; i < 3; i++) {
		auto scale = SCREEN_SIZE.height / 20 / 96;
		auto slash = SlashBoss::create("Animation/Enemy_Boss3/skill-boss-3.json", "Animation/Enemy_Boss3/skill-boss-3.atlas", scale);
		slash->setVisible(false);
		slash->setPosition(this->getPosition());
		//slash->setScale(SCREEN_SIZE.height / 8 / slash->getContentSize().height);
		this->getParent()->addChild(slash, ZORDER_ENEMY);
		auto tmpbody = slash->getB2Body();
		tmpbody = nullptr;
		slashPool->addObject(slash);
	}

	indexSlash = 0;
}

void EnemyBoss3::attack3()
{
	AudioManager::playSound(SOUND_BOSS3SKILL2);
	this->immortal();
	this->clearTracks();
	this->setAnimation(0, "attack3", false);
	this->setTimeScale(0.4f);
}

void EnemyBoss3::fixStupid()
{
	int randnumber = RandomHelper::random_int(0,1);
	if (randnumber) {
		EnemyBoss1::fixStupid();
	}
		
	else {
		EnemyBoss1::fixStupid();
		this->setRealMoveVelocity(Vec2(this->getmoveVelocity().x, this->getmoveVelocity().y*CCRANDOM_0_1()*-1));
	}
}

Vec2 EnemyBoss3::getPosGenSlash()
{
	return this->getBoneLocation("vukhi2");
}

void EnemyBoss3::creatSlash(float angel)
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
	slash->setIsDie(false);
	slash->setVisible(true);
	indexSlash++;
	if (indexSlash >= slashPool->count()) {
		indexSlash = 0;
	}
	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
	}
	auto world = this->getB2Body()->GetWorld();
	slash->initCirclePhysic(world, this->getBoneLocation("vukhi2"));
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngle(angel);
	slash->setRotation(-angel / PI * 180 + 180);
	slash->getB2Body()->SetLinearVelocity(b2Vec2(slash->getB2Body()->GetLinearVelocity().x/1.5f, slash->getB2Body()->GetLinearVelocity().y / 1.5f) +
		this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss3::playSoundAttack1()
{
	AudioManager::playSound(SOUND_BOSS3CHEM);
}

void EnemyBoss3::playSoundAttack2()
{
	AudioManager::playSound(SOUND_BOSS3SKILL1);
}


void EnemyBoss3::playSoundDie()
{
	AudioManager::playSound(SOUND_BOSS3DIE);
}

void EnemyBoss3::doAttack2()
{
	//this->setControlState(0);
	if (this->getPositionY() > SCREEN_SIZE.height / 5) {
		this->attack2();
		auto type1 = CallFunc::create([&]() {
			auto vecBossToHero = this->heroLocation -this->getPosGenSlash();
			this->creatSlash(vecBossToHero.getAngle());

		});

		auto type2 = CallFunc::create([&]() {
			auto vecBossToHero = this->heroLocation -this->getPosGenSlash();
			this->creatSlash(vecBossToHero.getAngle() - PI / 24);
			this->creatSlash(vecBossToHero.getAngle());
			this->creatSlash(vecBossToHero.getAngle() + PI / 24);
		});
		switch (this->getRandAt2())
		{
		case 0: {
			this->runAction(Sequence::create(type1, DelayTime::create(0.3f), type1, DelayTime::create(0.3f), type1, nullptr));
			break;
		}
		case 1: {
			this->runAction(type2);
			break;
		}
		default:
			this->runAction(type2);
			break;
		}
	}
	else {
		this->attack3();
		auto type3 = CallFunc::create([&]() {
			this->creatHidenSlash(PI);
		});
		this->runAction(Sequence::create(DelayTime::create(0.7f),type3,DelayTime::create(0.5f),type3, DelayTime::create(0.5f), type3, nullptr));
	}

}

bool EnemyBoss3::checkStop()
{
	if (this->getRealMoveVelocity().y > 0) {
		if (this->getPositionX() - heroLocation.x > SCREEN_SIZE.width / 1.8f) {
			return true;
		}
	}
	else {
		if (this->getPositionX() - heroLocation.x > SCREEN_SIZE.width / 2.5f) {
			return true;
		}
	}
	return false;
	
}

void EnemyBoss3::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (getCurrent()) {
			if ((strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
				//this->unschedule("bossattack2");
			}
			else if ((strcmp(getCurrent()->animation->name, "attack3") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
			//	this->unschedule("bossattack2");
				this->setTimeScale(1);
			}
			else if ((strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
			}
		}
	});
}
