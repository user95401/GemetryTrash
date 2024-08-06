#pragma once
#include <_main.hpp>

#include <Geode/modify/CCSprite.hpp>
class $modify(SpecialSprites, CCSprite) {
    void pulseOpacitySch(float) {
        if (auto sprite = typeinfo_cast<CCSprite*>(this)) {
            auto fmod = FMODAudioEngine::sharedEngine();
            if (not fmod->m_metering) fmod->enableMetering();
            auto pulse = (fmod->m_pulse1 + fmod->m_pulse2 + fmod->m_pulse3) / 3;
            sprite->setOpacity(pulse * 120.f);
        }
    }
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
            //base layer (container)
            CCSprite* base_layer = CCSprite::create("GTPS_logo_001.png");
            //color
            srand(time(0));
            auto colorID = rand() % 23;
            auto color = reinterpret_cast<LevelSelectLayer*>(base_layer)
                ->colorForPage(colorID);
            //color layer
            CCSprite* color_layer = CCSprite::create("GTPS_logo_002.png");
            color_layer->setAnchorPoint(CCPointZero);
            color_layer->setBlendFunc({ GL_ONE, GL_ONE });
            color_layer->setColor(color);
            base_layer->addChild(color_layer, 0, colorID);
            //pulse layer
            CCSprite* pulse_layer = CCSprite::create("GTPS_logo_002.png");
            pulse_layer->setAnchorPoint(CCPointZero);
            pulse_layer->setBlendFunc({ GL_ONE, GL_ONE });
            pulse_layer->setOpacity(0);
            pulse_layer->schedule(schedule_selector(SpecialSprites::pulseOpacitySch));
            base_layer->addChild(pulse_layer);
            //rtn base_layer
            return base_layer;
        }
        return CCSprite::createWithSpriteFrameName(name.c_str());
    }
};