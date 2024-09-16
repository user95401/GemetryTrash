#pragma once
#include <_main.hpp>

#include <Geode/modify/MenuGameLayer.hpp>
class $modify(MenuGameLayerExt, MenuGameLayer) {
    $override bool init() {
        auto init_result = MenuGameLayer::init();

        auto pulsebg = CCSprite::create("game_bg_13_001.png");
        pulsebg->setID("pulsebg");
        pulsebg->setScale(1.250f);
        pulsebg->setAnchorPoint({ 0.0f, 1.f });
        pulsebg->setBlendFunc({ GL_ONE, GL_ONE });
        pulsebg->setColor({0,0,0});
        m_groundLayer->addChild(pulsebg, 10);

        return init_result;
    }
    $override void update(float p0) {
        MenuGameLayer::update(p0);
        //pulse
        if (SETTING(bool, "No Menu Game BG Pulse")) void();
        else {
            auto fmod = FMODAudioEngine::sharedEngine();
            if (not fmod->m_metering) fmod->enableMetering();
            auto pulse = (fmod->m_pulse1 + fmod->m_pulse2 + fmod->m_pulse3) / 3;
            this->m_backgroundSprite->setOpacity(255.f - (pulse * 80.f));
            if (auto pulsebg = typeinfo_cast<CCSprite*>(m_groundLayer->getChildByID("pulsebg"))) {
                pulsebg->setColor(darken3B(this->m_backgroundSprite->getColor(), 255 - pulse * 60.f));
            }
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
        if (SETTING(bool, "No New Explode in Menu Game")) void();
        GEODE_WINDOWS(else this->m_playerObject->playDeathEffect());
        MenuGameLayer::destroyPlayer();
        if (SETTING(bool, "No Players Destroyed Counter")) void();
        else {
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

        if (auto title = this->getChildByIDRecursive("main-title")) {

            auto pos = title->getPosition();
            auto parent = title->getParent();

            title->removeFromParentAndCleanup(0);

            auto menu = CCMenu::createWithItem(CCMenuItemExt::createSpriteExtra(
                title, [this](auto) {
                    MDPopup* mdpop = nullptr;
                    mdpop = MDPopup::create(
                        "Gemetry Trash", //reinterpret_cast<LoadingLayer*>(CCNode::create())->getLoadingString(),
                        "The project by a mod maker. The server is anarchic - easy mod, easy rate.""\n"
                        "\n"
                        "Probably the first GDPS, that is almost entirely a geode mod, which includes both textures and modifications. Modifications are also completely random - maybe something cool, or maybe some shit.""\n"
                        "\n"
                        "The things:"   "\n"
                        "- easy rate"   "\n"
                        "- easy mod"    "\n"
                        "- random explode sound, menu music"    "\n"
                        "- some changes in textures"    "\n"
                        "- kill counter in the main menu"   "\n"
                        "- the main levels have been changed"   "\n"
                        "- automatic updates"   "\n"
                        "- many more little things" "\n"
                        "\n"
                        "https://discord.gg/UyQytJsrGZ" "\n"
                        "https://gamejolt.com/games/GEOMETRY_TRASH_GDPS/776479/"    "\n"
                        ,
                        "Settings", "Credits",
                        [this, mdpop](bool btn2) {
                            if (not btn2) openSettingsPopup(getMod());
                            else openSupportPopup(getMod());
                        }
                    );
                    mdpop->show();
                }
            ));

            auto btn = getChild<CCMenuItemSpriteExtra>(menu, 0); 
            btn->setPosition(CCPointZero);
            btn->m_colorEnabled = 1;
            btn->m_animationEnabled = 0;

            menu->setPosition(pos);
            parent->addChild(menu);

            auto verLabel = CCLabelBMFont::create(
                fmt::format(
                    "Platform: {}" "\n"
                    "Version: {}" "\n"
                    "Geode: {}", 
                    GEODE_PLATFORM_NAME,
                    Mod::get()->getVersion().toString(),
                    Mod::get()->getMetadata().getGeodeVersion()
                ).c_str(), 
                fmt::format("gjFont{:02d}.fnt", rand() % 60).c_str(),
                266, kCCTextAlignmentLeft
            );
            verLabel->limitLabelWidth(76.f, 0.525f, 0.1f);
            verLabel->setPosition(CCPoint(-218.f, -36.f));
            verLabel->setAnchorPoint(CCPoint(0.f, 1.f));
            verLabel->setColor(ccColor3B(77, 77, 77));
            verLabel->setBlendFunc({ GL_ONE, GL_ONE });
            menu->addChild(verLabel);

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

        return rtn;
    }
    inline static bool seenWarn = false;
};
