#ifndef __KawazCatch__MainScene__
#define __KawazCatch__MainScene__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
protected:
	MainScene();
	virtual ~MainScene();
	bool init() override;
public:
	static cocos2d::Scene* createScene();
	void update(float dt);
	CREATE_FUNC(MainScene);
	//_player�ϐ���,getPlayer()���\�b�h,setPlayer(Sprite*)���\�b�h�������I�Ɏ��������
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player);
	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _fruits, Fruits);
	CC_SYNTHESIZE(int, _score, Score);
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _scoreLabel, ScoreLabel);
private:
	enum class FruitType
	{
		//�����S�A�Ԃǂ��A�݂���A�΂ȂȁA�T�N�����{�A�ő�l
		APPLE,GRAPE,ORANGE,BANANA,CHERRY,COUNT
	};
	cocos2d::Sprite* addFruit();
	void release(FruitType);
	bool removeFruit(cocos2d::Sprite* fruit);
	void catchFruit(cocos2d::Sprite* fruit);//�t���[�c���L���b�`
};
#endif // !__KawazCatch__MainScene__
