#include "ufoScence.h"
#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include <string>
USING_NS_CC;

Scene* ufoScence::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = ufoScence::create();
	scene->addChild(layer);
	return scene;
}

void ufoScence::setupPickupPosition()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	pickupPos[0][0] = visibleSize.width / 10 * 3.8;
	pickupPos[0][1] = visibleSize.height / 10 * 2.9;
	pickupPos[1][0] = visibleSize.width / 10 * 5;
	pickupPos[1][1] = visibleSize.height / 10 * 3.5;
	pickupPos[2][0] = visibleSize.width / 10 * 6.2;
	pickupPos[2][1] = visibleSize.height / 30 * 7;
	pickupPos[3][0] = visibleSize.width / 10 * 3.4;
	pickupPos[3][1] = visibleSize.height / 6 * 3;
	pickupPos[4][0] = visibleSize.width / 9 * 3.2;
	pickupPos[4][1] = visibleSize.height / 9 * 6.7;
	pickupPos[5][0] = visibleSize.width / 40 * 17.2;
	pickupPos[5][1] = visibleSize.height / 40 * 19;
	pickupPos[6][0] = visibleSize.width / 7 * 4.6;
	pickupPos[6][1] = visibleSize.height / 14 * 11.2;
}

bool ufoScence::init() {
	if(!Layer::init())
		return false;

	this->scheduleUpdate();

	auto visiblesize = Director::getInstance()->getVisibleSize();

	auto backGround = Sprite::create("background.png");

	float scale = visiblesize.height / float(backGround->getContentSize().height);


	backGround->setScale(scale);
	backGround->setPosition(Vec2((float) (visiblesize.width / 2.0), (float) (visiblesize.height / 2.0)));
	this->addChild(backGround);

	player = Sprite::create("UFO.png");
	player->setTag(UFO);
	player->setPosition(Vec2((float) (visiblesize.width / 2.0), (float) (visiblesize.height / 2.0)));
	player->setScale(scale);
	this->addChild(player);

	/* auto pickup = Sprite::create("Pickup.png");
	pickup->setTag(PICKUP);
	pickup->setPosition(visiblesize.width / 10 * 3, visiblesize.height / 10 * 3);
	pickup->setScale(scale);
	auto pickupBody = PhysicsBody::createCircle(pickup->getContentSize().width / 2.7f, PhysicsMaterial(0.001f, 0.0f, 0.0f));
	pickupBody->setDynamic(false);
	pickupBody->setCategoryBitmask(0x02);
	pickupBody->setCollisionBitmask(0x01);
	pickupBody->setContactTestBitmask(0x02);
	pickup->setPhysicsBody(pickupBody); */

	setupPickupPosition();

	for(int i = 0; i < maxPickups;++i) {
		auto pickup = Sprite::create("Pickup.png");
		pickup->setTag(PICKUP);
		pickup->setPosition(pickupPos[i][0], pickupPos[i][1]);
		pickup->setScale(scale);
		auto pickupBody = PhysicsBody::createCircle(pickup->getContentSize().width / 2.7f, PhysicsMaterial(0.001f, 0.0f, 0.0f));
		pickupBody->setDynamic(false);
		pickupBody->setCategoryBitmask(0x02);
		pickupBody->setCollisionBitmask(0x01);
		pickupBody->setContactTestBitmask(0x02);
		pickup->setPhysicsBody(pickupBody);
		this->addChild(pickup);
	}

	//this->addChild(pickup);


	rb2d = PhysicsBody::createCircle((float) (player->getContentSize().width / 2.5), PhysicsMaterial(0.001f, 0.0f, 0.0f));
	rb2d->setDynamic(true);
	rb2d->setGravityEnable(false);
	rb2d->setCategoryBitmask(0x02);
	rb2d->setCollisionBitmask(0x01);
	rb2d->setContactTestBitmask(0x02);

	rb2d->setRotationEnable(false);
	player->addComponent(rb2d);

	player->getPhysicsBody()->applyForce(Vec2(100, 100));

	auto wallBodyl = PhysicsBody::createEdgeBox(Size(backGround->getContentSize().width, backGround->getContentSize().height) / 20 * 16, PhysicsMaterial(0.0f, 0.0f, 0.0f), 1);
	wallBodyl->setDynamic(false);
	backGround->setPhysicsBody(wallBodyl);
	backGround->setTag(WALL);


	//camera = Camera::getDefaultCamera();
	//camera->setPosition(Vec2(visiblesize.width / 2, visiblesize.height / 2));
	//camera->setPosition(0, 0);
	return true;
}


void ufoScence::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
	//CCLOG("HELLO");
	switch(keyCode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			keyPressed = true;
			force = Vec2(0, 100);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			keyPressed = true;
			force = Vec2(0, -100);
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			keyPressed = true;
			force = Vec2(-100, 0);
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			keyPressed = true;
			force = Vec2(100, 0);
			break;
		default:
			break;
	}
	lastKeyCode = keyCode;
}

void ufoScence::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if(lastKeyCode == keyCode)
		keyPressed = false;
}

void ufoScence::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
	force.x = float(acc->x) * speed;
	force.y = float(acc->y) * speed;

	rb2d->applyImpulse(force);
}

void ufoScence::gameOver(std::string label)
{
	winLabel = Label::createWithTTF(label, "fonts/Marker Felt.ttf", 100);
	winLabel->enableShadow();
	winLabel->setPosition(player->getContentSize().width / 2, player->getContentSize().height + 20);
	player->addChild(winLabel);
}

bool ufoScence::onContact(cocos2d::PhysicsContact &contact)
{
	//CCLOG("Contact Begain");
	auto shapeA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto shapeB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	if(shapeA) {
		if (shapeA->getTag() == PICKUP) {
			this->removeChild(shapeA);
			++grade;
		} else if(shapeB->getTag() == PICKUP) {
			this->removeChild(shapeB);
			++grade;
		}
	}
	if(grade >= maxPickups)
		this->gameOver("You Win");
	return true;
}

void ufoScence::onEnter()
{
	Layer::onEnter();
	Device::setAccelerometerEnabled(true);
	keyboardListener = EventListenerKeyboard::create();
	auto accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(ufoScence::onAcceleration, this));
	keyboardListener->onKeyPressed = CC_CALLBACK_2(ufoScence::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(ufoScence::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);

	auto physicsListener = EventListenerPhysicsContact::create();
	physicsListener->onContactBegin = CC_CALLBACK_1(ufoScence::onContact, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(physicsListener, this);
}

void ufoScence::update(float delta)
{
	if(keyPressed)
		rb2d->applyImpulse(force);

	auto children = this->getChildren();
	for(auto child : children) {
		if(child->getTag() == PICKUP)
			child->setRotation(child->getRotation() + 2);
	}
}