#pragma once
#include <_main.hpp>

#include <Geode/modify/CCSprite.hpp>
class $modify(CCSpriteExt, CCSprite) {
    static CCSprite* createWithSpriteFrameName(const char* pszSpriteFrameName) {
        auto name = std::string(pszSpriteFrameName);
        if (string::contains(name, "robtoplogo_small.png")) {
            //label
            CCLabelBMFont* label = CCLabelBMFont::create("   user95401's   \noriginal", "chatFont.fnt");
            label->setAlignment(kCCTextAlignmentCenter);
            label->setAnchorPoint(CCPoint());
            //blankSprite
            CCSprite* blankSprite = CCSprite::create();
            blankSprite->addChild(label);
            blankSprite->setContentSize(label->getContentSize());
            //return
            return blankSprite;
        }
        if (string::contains(name, "RobTopLogoBig_001.png")) {
            //label
            CCLabelBMFont* label = CCLabelBMFont::create("user666's original", "gjFont06.fnt");
            label->setAlignment(kCCTextAlignmentCenter);
            label->setAnchorPoint(CCPoint());
            //blankSprite
            CCSprite* blankSprite = CCSprite::create();
            blankSprite->addChild(label);
            blankSprite->setContentSize(label->getContentSize());
            //return
            return blankSprite;
        }
        if (string::contains(name, "GJ_logo_001.png")) {
            srand(time(0));
            CCSprite* GJ_logo_001 = CCSprite::create("GTPS_logo_001.png");
            GJ_logo_001->runAction(CCRepeatForever::create(CCSequence::create(CCEaseSineInOut::create(CCScaleBy::create(1.0, 1.025)), CCEaseSineInOut::create(CCScaleBy::create(1.0, 0.975)), nullptr)));
            CCSprite* GJ_logo_002 = CCSprite::create("GTPS_logo_002.png");
            GJ_logo_002->setColor(GameManager::sharedState()->colorForIdx(rand() % 100));
            GJ_logo_002->setAnchorPoint(CCPointZero);
            GJ_logo_002->setBlendFunc({ GL_ONE, GL_ONE });
            GJ_logo_001->addChild(GJ_logo_002);
            return GJ_logo_001;
        }
        return CCSprite::createWithSpriteFrameName(name.c_str());
    }
};