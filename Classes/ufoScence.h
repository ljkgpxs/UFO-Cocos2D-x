#pragma once

#include <cocos2d.h>
#include <string>

enum {
    UFO,
    PICKUP,
    WALL
};

class ufoScence : public cocos2d::Layer {
public:
    CREATE_FUNC(ufoScence);
    static cocos2d::Scene* createScene();
    virtual bool init();

    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    virtual void onEnter();
    virtual void update(float delta);

private:
    cocos2d::Sprite* player;
    cocos2d::Camera* camera;
    cocos2d::PhysicsBody* rb2d;
    cocos2d::EventListenerKeyboard *keyboardListener;
    bool keyPressed;
    cocos2d::Vec2 force;
    cocos2d::EventKeyboard::KeyCode lastKeyCode;
    float speed = 50;
    int grade = 0;
    cocos2d::Label* winLabel;
    const int maxPickups = 7;
    float pickupPos[7][2];

    void onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event);
    bool onContact(cocos2d::PhysicsContact& contact);
    void gameOver(std::string);
    void setupPickupPosition();
};

