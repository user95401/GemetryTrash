#pragma once
#include <_main.hpp>

#include <Geode/modify/MenuGameLayer.hpp>
class $modify(MenuGameLayerExt, MenuGameLayer) {
    $override bool init() {
        auto init_result = MenuGameLayer::init();
        return init_result;
    }
    $override void update(float p0) {
        MenuGameLayer::update(p0);
        //pulse
        {
            auto fmod = FMODAudioEngine::sharedEngine();
            if (not fmod->m_metering) fmod->enableMetering();
            auto pulse = (fmod->m_pulse1 + fmod->m_pulse2 + fmod->m_pulse3) / 3;
            this->m_backgroundSprite->setOpacity(255.f - (pulse * 80.f));
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
                counter_label->setTag(0);
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
            counter_label->stopAllActions();
            counter_label->runAction(CCEaseExponentialIn::create(
                CCFadeOut::create(15.f)
            ));
            counter_label->setScale(0.36f);
            counter_label->runAction(CCEaseBackOut::create(
                CCScaleTo::create(0.25f, 0.3f)
            ));
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

        MenuGameLayer* game = nullptr;
        auto game_node = this->getChildByIDRecursive("main-menu-bg");
        if (auto game = typeinfo_cast<MenuGameLayer*>(game_node)) {
            game->removeFromParentAndCleanup(0);
            auto scroll = ScrollLayer::create(game->getContentSize());
            scroll->m_cutContent = false;
            scroll->m_peekLimitBottom = 25.f;
            scroll->m_contentLayer->addChild(game);
            scroll->m_contentLayer->setContentSize(game->getContentSize());
            this->addChild(scroll, -1);
        }

        //centerNode
        CCNode* centerNode = CCNode::create();
        centerNode->setID("centerNode");
        centerNode->setPosition(this->getContentSize() / 2);
        this->addChild(centerNode);

        //flyinamopgus!!!
        CCSprite* flyinAmogus_001 = CCSprite::create("flyinAmogus_001.png");
        if (flyinAmogus_001) {
            flyinAmogus_001->setID("flyinamopgus");
            flyinAmogus_001->setScale(2.f);
            flyinAmogus_001->setPosition({
                CCDirector::sharedDirector()->getScreenRight(),
                CCDirector::sharedDirector()->getScreenTop() }
            );
            flyinAmogus_001->runAction(CCRepeatForever::create(CCRotateBy::create(0.1, 1)));
            flyinAmogus_001->setColor(reinterpret_cast<LevelSelectLayer*>(this)->colorForPage(rand() % 23));
            this->addChild(flyinAmogus_001);
            CCSprite* flyinAmogus_002 = CCSprite::create("flyinAmogus_002.png");
            flyinAmogus_001->setID("flyinAmogus_002");
            flyinAmogus_002->setAnchorPoint(CCPointZero);
            flyinAmogus_001->addChild(flyinAmogus_002);
        };

        //warn lbl 
        {
            //bg
            CCScale9Sprite* warnlblbg = CCScale9Sprite::create("square02_001.png");
            warnlblbg->setID("warnlblbg");
            if (!seenWarn) { seenWarn = true; centerNode->addChild(warnlblbg); }
            warnlblbg->setPositionY(-68.f);
            warnlblbg->setOpacity(120);
            //warn lbl
            auto warnlbl = CCLabelTTF::create("Game may contain explicit lyrics and offensive jokes - don't take it seriously!\n Also its buggy shit. Have a good game, man, thanks for playin <3",
                "Comic Sans MS", 10.f);
            warnlbl->setID("warnlbl");
            if (rand() % 3 == 1) warnlbl->setString("enTa kyky, a Tbl, gaBau 6e3 6\n cnc 4To 3aJleTeJl, Hy Tbl u curMa");
            warnlbl->setScale(2.f);
            warnlbl->setAnchorPoint(CCPoint(-0.015f, -0.15f));
            //more bg setups when
            warnlblbg->setContentSize({ warnlbl->getContentSize().width * 2 + 30, warnlbl->getContentSize().height * 2 + 20 });
            warnlblbg->setScale(0.5f);
            warnlblbg->addChild(warnlbl);
            //fadeanim
            warnlblbg->runAction(CCSequence::create(CCDelayTime::create(10.0f), CCFadeTo::create(0.5f, 0), nullptr));
            warnlbl->runAction(CCSequence::create(CCDelayTime::create(10.0f), CCFadeTo::create(0.5f, 0), nullptr));
        };

        return rtn;
    }
    inline static bool seenWarn = false;
};
