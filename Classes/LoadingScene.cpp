#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
using namespace cocos2d;
using namespace CocosDenshion;


bool LoadingScene::init()
{
	if (Scene::init())
		return true;
	else
		return false;
}

void LoadingScene::onEnter()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//û����Ӳ㣬ֱ���ڳ���������Ӿ���
	//�����������
	auto splashBg = Sprite::create("splash.png");
	splashBg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(splashBg);
	
	auto logo = LabelTTF::create("by Tashaxing", "Arial", 20);
	logo->setColor(Color3B(0, 255, 0));
	logo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50);
	this->addChild(logo);
	//��Ӽ��ػص����������첽��������
	Director::getInstance()->getTextureCache()->addImageAsync("game.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	
}

void LoadingScene::loadingCallBack(Texture2D *texture)
{
	//Ԥ����֡��������
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game.plist", texture);
	//Ԥ����֡����
	auto birdAnimation = Animation::create();
	birdAnimation->setDelayPerUnit(0.2f);
	birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1.png"));
	birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2.png"));
	birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird3.png"));
	AnimationCache::getInstance()->addAnimation(birdAnimation, "birdAnimation"); //��С�񶯻���ӵ���������
	//Ԥ������Ч
	SimpleAudioEngine::getInstance()->preloadEffect("die.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("hit.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("point.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("swooshing.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("wing.mp3");

	//���������ת����Ϸ����
	auto gameScene = GameScene::createScene();
	TransitionScene *transition = TransitionFade::create(0.5f, gameScene);
	Director::getInstance()->replaceScene(transition);
}