#pragma once
#include "cocos2d.h"
//һЩȫ�ֳ���
const int BIRD_RADIUS = 15; //С��뾶
const int PIPE_HEIGHT = 320;//����ܵ�����
const int PIPE_WIDTH = 52; //�ܵ����
const int PIPE_SPACE = 100; //���¹�֮��ķ�϶
const int PIPE_INTERVAL = 170;//������������֮��ļ��,288/2+52/2
const int WAIT_DISTANCE = 380;//�ȴ�����
enum GAME_STATUS  //��Ϸ״̬,׼������ʼ������
{
	GAME_READY,
	GAME_START,
	GAME_OVER
};
class GameScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float dt);   
	void scrollLand(float dt); 
	void setPhysicWorld(cocos2d::PhysicsWorld *world); 
	virtual bool onContactBegin(const cocos2d::PhysicsContact& contact);  
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event); 
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	void createPipes(); 
	int getRandomHeight();
	void gameStart();
	void gameOver();
	void gameRetart(Ref *sender);
	void gamePanelAppear();
private:
	cocos2d::Sprite *birdSprite; //С��
	cocos2d::RepeatForever *swingAction; //С��Ļζ�����
	cocos2d::LabelTTF *scoreLabel; //�Ʒ�
	cocos2d::Sprite *land1, *land2;  //�ذ�
	cocos2d::Vector<cocos2d::Node*> pipes; //�ܵ���������װ����
	cocos2d::PhysicsWorld *m_physicWorld; //��Ϸ����������
	GAME_STATUS gameStatus; //��Ϸ״̬����
	int score,bestScore;   //��Ϸ��ǰ��������÷���
	int touchX;  //�����������
};