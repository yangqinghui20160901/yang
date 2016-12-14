/*
*game:flappy bird
*author:tashaxing
*time:2014/10/26
*/
#include "GameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
Scene* GameScene::createScene()
{
	
	//������Ϸ����
	auto gameScene = Scene::createWithPhysics(); //�����������ʼ������
	gameScene->getPhysicsWorld()->setGravity(Vec2(0, -900)); //����������,�������ٶȿ��Ը����ָи�С��

	//�������Ϸ��
	auto gameLayer = GameScene::create();
	gameLayer->setPhysicWorld(gameScene->getPhysicsWorld()); //����������
	gameScene->addChild(gameLayer);
	return gameScene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point visibleOrigin = Director::getInstance()->getVisibleOrigin();

	//��ʼ����Ϸ״̬
	gameStatus = GAME_READY;
	score = 0;

	

	//�����Ϸ����
	Sprite *backGround = Sprite::createWithSpriteFrameName("bg.png");
	backGround->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2);
	this->addChild(backGround);
	//logo
	auto gameLogo = Sprite::createWithSpriteFrameName("bird_logo.png");
	gameLogo->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2+100);
	gameLogo->setName("logo");
	this->addChild(gameLogo);

	//��ӹ���
	createPipes();

	//С��
	birdSprite = Sprite::create();
	birdSprite->setPosition(visibleOrigin.x + visibleSize.width / 3, visibleOrigin.y + visibleSize.height / 2);
	this->addChild(birdSprite);
	auto birdAnim = Animate::create(AnimationCache::getInstance()->animationByName("birdAnimation"));
	birdSprite->runAction(RepeatForever::create(birdAnim));  //�ӳᶯ��
	auto up = MoveBy::create(0.4f, Point(0, 8));
	auto upBack = up->reverse();
	if (gameStatus == GAME_READY)
	{
		swingAction = RepeatForever::create(Sequence::create(up, upBack, NULL));
		birdSprite->runAction(swingAction); //���»ζ�����
	}
    //С��󶨸���
	auto birdBody = PhysicsBody::createCircle(BIRD_RADIUS); //��С�񵱳�һ��Բ������Ū��ȷ����������
	birdBody->setDynamic(true);   //����Ϊ���Ա����������ö�����
	birdBody->setContactTestBitmask(1); //������������Ϊ1���ܼ�⵽��ͬ��������ײ
	birdBody->setGravityEnable(false);   //�����Ƿ�����Ӱ��,׼�������в�������Ӱ��
	birdSprite->setPhysicsBody(birdBody); //ΪС�����ø���

	//�������land
	land1 = Sprite::createWithSpriteFrameName("land.png");
	land1->setAnchorPoint(Point::ZERO); 
	land1->setPosition(Point::ZERO); 
	this->addChild(land1, 10);  //�������
	land2 = Sprite::createWithSpriteFrameName("land.png");
	land2->setAnchorPoint(Point::ZERO);
	land2->setPosition(Point::ZERO);
	this->addChild(land2, 10);
	

	//���õذ����
	Node *groundNode = Node::create();
	auto groundBody = PhysicsBody::createBox(Size(visibleSize.width, land1->getContentSize().height));
	groundBody->setDynamic(false);
	groundBody->setContactTestBitmask(1);
	groundNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE); //���������еĸ���ֻ������ê������Ϊ����
	groundNode->setPhysicsBody(groundBody);
	groundNode->setPosition(visibleOrigin.x+visibleSize.width/2,land1->getContentSize().height/2);
	this->addChild(groundNode);

	//�Ƿ���,������㣬��������ŪͼƬ������^_^
	scoreLabel = LabelTTF::create("0", "Felt", 35);
	scoreLabel->setPosition(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2 + 200);
	scoreLabel->setVisible(false); //һ��ʼ����
	this->addChild(scoreLabel);

	//�����ײ���
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//��Ӵ�������
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}
//���ò����������
void GameScene::setPhysicWorld(PhysicsWorld *world)
{
	m_physicWorld = world;
}
//��Ϸ��ʼ
void GameScene::gameStart()
{
	gameStatus = GAME_START;
	score = 0;//���÷���
	scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
	this->getChildByName("logo")->setVisible(false); //logo��ʧ
	scoreLabel->setVisible(true); //�Ʒֿ�ʼ
	this->scheduleUpdate();//����Ĭ�ϸ���
	this->schedule(schedule_selector(GameScene::scrollLand), 0.01f); //�������Ӻ͵ذ����
	birdSprite->stopAction(swingAction); //��Ϸ��ʼ��ֹͣ���¸���
	birdSprite->getPhysicsBody()->setGravityEnable(true); //��ʼ����������
}
//��Ϸ����
void GameScene::gameOver()
{
	gameStatus = GAME_OVER;
	//��ȡ��ʷ����
	bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");
	if (score > bestScore)
	{
		bestScore = score;  //������÷���
		UserDefault::getInstance()->setIntegerForKey("BEST", bestScore);
	}
		
	
	SimpleAudioEngine::getInstance()->playEffect("hit.mp3");
	//��Ϸ������ֹͣ�ذ�͹ܵ��Ĺ���
	this->unschedule(schedule_selector(GameScene::scrollLand));

	
}
//��ײ���
bool GameScene::onContactBegin(const PhysicsContact& contact)
{
	if (gameStatus == GAME_OVER)  //����Ϸ�������ټ����ײ
		return false;
	
	gameOver();
	return true;
}
//��������
bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	switch (gameStatus)
	{
	case GAME_OVER:
		break;
	case GAME_READY:
	{
		gameStart();
		birdSprite->getPhysicsBody()->setVelocity(Vec2(0, 250)); //��һ�����ϵĳ��ٶ�
		SimpleAudioEngine::getInstance()->playEffect("wing.mp3");
		//����ҲҪд����Ȼtouchx��ֵ��δֵ֪�����������bug
		touchX = touch->getLocation().x;
	}
		break;
	case GAME_START:
	{
		auto curVelocity = birdSprite->getPhysicsBody()->getVelocity();
		birdSprite->getPhysicsBody()->setVelocity(Vec2(0, 250>(curVelocity.y + 500) ? (curVelocity.y + 500):250)); //���ϵ��ٶ����½�Ӱ��
		SimpleAudioEngine::getInstance()->playEffect("wing.mp3");
		//���ϵ��ӽǣ��������ţ��ٺ�
		touchX = touch->getLocation().x;
	}
		break;
	default:
		break;
	}

	return true;
}
void GameScene::onTouchEnded(Touch *touch, Event *event)
{
	//�������㻬������100������˲����100
	if (touch->getLocation().x - touchX > 100)
	{
		score += 100;
		scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
		SimpleAudioEngine::getInstance()->playEffect("point.mp3");
	}
		
}
//�ذ�����Զ����ʱ���ص�
void GameScene::scrollLand(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����ͼƬѭ���ƶ�
	land1->setPositionX(land1->getPositionX() - 1.0f);
	land2->setPositionX(land1->getPositionX() + land1->getContentSize().width - 2.0f);
	if (land2->getPositionX() <= 0)
		land1->setPosition(Point::ZERO);

	//���ӹ���
	for (auto &singlePipe : pipes)
	{
		singlePipe->setPositionX(singlePipe->getPositionX() - 1.0f);
		if (singlePipe->getPositionX() < -PIPE_WIDTH/2)
		{
			singlePipe->setPositionX(visibleSize.width+PIPE_WIDTH/2);
			singlePipe->setPositionY(getRandomHeight());
			singlePipe->setName("newPipe");  //ÿ������һ�����ӣ���Ϊnew
		}
	}
}
//��ȡ����߶ȣ����ڹܵ�
int GameScene::getRandomHeight()
{
	auto size = Director::getInstance()->getVisibleSize();
	//ʹ�õ�������������������Ļ���ĵ�-(40~270)�м��������,������Լ��Գ�����
	return size.height / 2 - 40 - CCRANDOM_0_1() * (270 - 40);
}
//�����ܵ� 
void GameScene::createPipes()
{
	//ͬ��Ļ���ֵ�ֻ���������ӣ��ŵ��������棬���°�Ϊһ��
	for (int i = 0; i < 2; i++)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Sprite *pipeUp = Sprite::createWithSpriteFrameName("pipe_up.png");
		Sprite *pipeDown = Sprite::createWithSpriteFrameName("pipe_down.png");
		Node *singlePipe = Node::create();
		//���Ϲܰ󶨸���
		auto pipeUpBody = PhysicsBody::createBox(pipeUp->getContentSize());
		pipeUpBody->setDynamic(false);
		pipeUpBody->setContactTestBitmask(1);
		pipeUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pipeUp->setPhysicsBody(pipeUpBody);
		//���������ӷֿ����ø��壬���������м�Ŀ�϶ʹ��С��ͨ��
		//���¹ܰ󶨸���
		auto pipeDownBody = PhysicsBody::createBox(pipeDown->getContentSize());
		pipeDownBody->setDynamic(false);
		pipeDownBody->setContactTestBitmask(1);
		pipeDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pipeDown->setPhysicsBody(pipeDownBody);

		pipeUp->setPosition(0, PIPE_HEIGHT + PIPE_SPACE);
		singlePipe->addChild(pipeUp);
		singlePipe->addChild(pipeDown);  //pipeDownĬ�ϼӵ���0,0�������ºϲ�����ʱsinglePipe������Ĺ�������Ϊê��
		singlePipe->setPosition(i*PIPE_INTERVAL + WAIT_DISTANCE, getRandomHeight() ); //���ó�ʼ�߶�
		singlePipe->setName("newPipe");
		this->addChild(singlePipe);  //���������Ӷ����뵽��
		pipes.pushBack(singlePipe);  //���������Ⱥ���ӵ�����
	}

}
//Ĭ�ϵĸ��º���
void GameScene::update(float dt)
{
	//����Ϸ��ʼʱ���жϵ÷֣������ʵҲ����д�������ط���������ӹ����ĸ��º���������ߴ����������
	if (gameStatus == GAME_START)
	{
		for (auto &pipe : pipes)
		{
			if (pipe->getName() == "newPipe") //����һ�����Ӿ��ж�
			{
				if (pipe->getPositionX() < birdSprite->getPositionX())
				{
					score++;
					scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
					SimpleAudioEngine::getInstance()->playEffect("point.mp3");
					pipe->setName("passed"); //����Ѿ������Ĺ���
				} 
			}
		}
	}
	//С�����ת
	auto curVelocity = birdSprite->getPhysicsBody()->getVelocity();
	birdSprite->setRotation(-curVelocity.y*0.1 - 20);  //������ֱ������ٶ������ת�Ƕȣ���ʱ��Ϊ��
	
	if (birdSprite->getPositionY() <= land1->getContentSize().height+BIRD_RADIUS)
	{
		birdSprite->stopAllActions(); //С����˾Ͳ������ȳ���
		birdSprite->setRotation(70);  //����Ϊ�쳯�£�˳ʱ��70����ת
		birdSprite->getPhysicsBody()->setDynamic(false);  //����Ϊ������
		SimpleAudioEngine::getInstance()->playEffect("die.mp3");
		this->unscheduleUpdate();  //��С�������������ֹͣĬ�ϸ���
		gamePanelAppear();  //�����Ƿ���
	}
	
}
//����Ƿְ������˵�
void GameScene::gamePanelAppear()
{
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��node��gameoverlogo�ͼǷְ����һ��
	Node *gameOverPanelNode = Node::create();
	auto gameOverLabel = Sprite::createWithSpriteFrameName("gameover.png");
	gameOverPanelNode->addChild(gameOverLabel);
	auto panel = Sprite::createWithSpriteFrameName("board.PNG");//ע�������Ǵ�дPNG��ԭͼƬ��ʲô��׺�������ʲô�����ִ�Сд
	gameOverLabel->setPositionY(panel->getContentSize().height); //����һ������
	gameOverPanelNode->addChild(panel);
	//�Ƿְ��������������
	auto curScoreTTF = LabelTTF::create(String::createWithFormat("%d", score)->getCString(), "Arial", 20);
	curScoreTTF->setPosition(panel->getContentSize().width-40, panel->getContentSize().height-45);
	curScoreTTF->setColor(Color3B(255, 0, 0));
	panel->addChild(curScoreTTF);
	auto bestScoreTTF = LabelTTF::create(String::createWithFormat("%d", bestScore)->getCString(), "Arial", 20);
	bestScoreTTF->setPosition(panel->getContentSize().width - 40, panel->getContentSize().height - 90);
	bestScoreTTF->setColor(Color3B(0, 255, 0));
	panel->addChild(bestScoreTTF);
	this->addChild(gameOverPanelNode);
	gameOverPanelNode->setPosition(origin.x + size.width / 2, origin.y + size.height );
	//���붯��
	gameOverPanelNode->runAction(MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y + size.height / 2)));
	SimpleAudioEngine::getInstance()->playEffect("swooshing.mp3");
	//��Ӳ˵�
	MenuItemImage *restartItem = MenuItemImage::create("start_btn.png", "start_btn_pressed.png", this,menu_selector(GameScene::gameRetart));
	auto menu = CCMenu::createWithItem(restartItem);
	menu->setPosition(origin.x + size.width / 2, 150);
	this->addChild(menu);
}
//��Ϸ���¿�ʼ
void GameScene::gameRetart(Ref *sender)
{
	//���»ص���ʼ����
	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene); //�������ü���Ч�ˣ�ֱ��ת��
}