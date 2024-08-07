#pragma once
#include <_main.hpp>

#include <Geode/modify/MenuGameLayer.hpp>
class $modify(MenuGameLayerExt, MenuGameLayer) {
    $override void update(float p0) {
        MenuGameLayer::update(p0);
        //pulse
        {
            auto fmod = FMODAudioEngine::sharedEngine();
            if (not fmod->m_metering) fmod->enableMetering();
            auto pulse = (fmod->m_pulse1 + fmod->m_pulse2 + fmod->m_pulse3) / 3;
            this->m_backgroundSprite->setOpacity(255.f - (pulse * 60.f));
        }
    }
    $override void tryJump(float p0) {
        MenuGameLayer::tryJump(p0);
        cocos::findFirstChildRecursive<CCSprite>(this->m_playerObject,
            [this](CCSprite* node) {
                node->setOpacity(255);
                return false;
            }
        );
    }
    $override void destroyPlayer() {
        this->m_playerObject->playDeathEffect();
        MenuGameLayer::destroyPlayer();
        //counter
        {
            auto counter_layer = typeinfo_cast<CCLayer*>(this->getChildByIDRecursive(
                "counter_layer"
            ));
            auto counter_label = typeinfo_cast<CCLabelBMFont*>(this->getChildByIDRecursive(
                "counter_label"
            ));
            if (not counter_layer) {
                counter_layer = CCLayer::create();
                counter_layer->setAnchorPoint(CCPointZero);
                counter_layer->setID("counter_layer");
                this->addChild(counter_layer);
                counter_label = CCLabelBMFont::create("", "gjFont51.fnt");
                counter_label->setID("counter_label");
                counter_label->setAlignment(kCCTextAlignmentRight);
                counter_label->setAnchorPoint({ 1.f,0.f });
                counter_label->setScale(0.3f);
                counter_layer->addChildAtPosition(
                    counter_label, Anchor::BottomRight, { -10.f, 100.f }
                );
            }
            counter_label->setTag(1 + counter_label->getTag());
            int total = GameStatsManager::sharedState()->getStat("9");
            int now = counter_label->getTag();
            counter_label->setString(fmt::format(
                "Players Destroyed:\nTotally {}\nNow {}",
                total, now
            ).data());
        }
    }
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerExt, MenuLayer) {
    $override bool init() {
        srand(time(0)); //bool(rand() % 2)

        //rand bg
        auto background = rand() % 60;
        auto ground = rand() % 23;
        if (background <= 7) ground = background;
        else ground = 7 + (rand() % (23 - 7));
        GameManager::get()->loadBackground(background);
        GameManager::get()->loadGround(ground);

        auto rtn = MenuLayer::init();

        //remove unused links
        if(auto node = this->getChildByIDRecursive("facebook-button"))
            node->setVisible(0);
        if (auto node = this->getChildByIDRecursive("twitter-button"))
            node->setVisible(0);
        if (auto node = this->getChildByIDRecursive("youtube-button"))
            node->setVisible(0);
        if (auto node = this->getChildByIDRecursive("twitch-button"))
            node->setVisible(0);
        if (auto node = this->getChildByIDRecursive("more-games-button"))
            node->setVisible(0);

        return rtn;
    }
};
