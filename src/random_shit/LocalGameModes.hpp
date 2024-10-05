#pragma once
#include <_main.hpp>
//LocalGameModes

class IslandsLayer : public CCLayer {
public:
    CREATE_FUNC(IslandsLayer);
    virtual bool init() {
        return CCLayer::init() ? [this]() {customSetup(); return true; }() : false;
    }
    virtual void keyBackClicked() {
        log::debug("{}()", __FUNCTION__);
        GameManager::get()->safePopScene();
    }
    virtual void keyDown(enumKeyCodes key) {
        CCLayer::keyDown(key);
        if (key == KEY_Left or key == KEY_Right) {
            cocos::findFirstChildRecursive<BoomScrollLayer>(
                this, [this, key](BoomScrollLayer* scroll) {
                    scroll->moveToPage(scroll->m_page + (key == KEY_Left ? -1 : 1));
                    return true;
                }
            );
        }
    }
    void customSetup() {
        //jail
        this->setTouchEnabled(1);
        this->setKeyboardEnabled(1);
        this->setKeypadEnabled(1);
        this->setMouseEnabled(1);

        addChild(createLayerBG());

        auto islandPage = [](auto id = 1)
            {
                auto layer = CCLayer::create();
                auto island = CCMenuItemExt::createSpriteExtraWithFilename(
                    fmt::format("island_{}.png", id), 1.0f, [](auto) {}
                );
                island->setEnabled(0);
                layer->addChild(CCMenu::create(island, nullptr));
                return layer;
            };

        auto pages = CCArray::create();
        pages->addObject(islandPage(1));
        pages->addObject(islandPage(2));
        pages->addObject(islandPage(3));
        pages->addObject(islandPage(4));
        pages->addObject(islandPage(5));

        auto scroll = BoomScrollLayer::create(pages, 1, 0);
        scroll->setPagesIndicatorPosition({ this->getContentWidth() / 2, 18.f });
        addChild(scroll);

        auto label = CCLabelTTF::create((const char*)u8"какиета острова ебаные лол", "arial", 12.f);
        label->setPositionX(this->getContentWidth() / 2);
        label->setPositionY(this->getContentHeight() - 18.f);
        addChild(label);

    }
};

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(InfinityMode, GJBaseGameLayer) {
    inline static int current_deaths = 0;
    inline static int current_amo_for_reward = 0;
    inline static int next_amount_for_reward = 5;
    inline static int mltp_amount_for_reward = 1;
    static void open() {
        InfinityMode::current_deaths = 0;
        auto level = GameLevelManager::get()->getMainLevel(1337, false);
        cocos2d::CCDirector::get()->replaceScene(
            cocos2d::CCTransitionFade::create(0.5f, PlayLayer::scene(level, false, false))
        );
        GameManager::get()->m_playLayer->addChild(createDataNode("INFINITY_MODE_MARK"));
    }
    bool ENABLED() {
        return nullptr != this->getChildByIDRecursive("INFINITY_MODE_MARK");
    }
    $override void spawnGroup(int p0, bool p1, double p2, gd::vector<int> const& p3, int p4, int p5) {
        if (not ENABLED()) return GJBaseGameLayer::spawnGroup(p0, p1, p2, p3, p4, p5);
        //log::debug("{}->{}({}, {}, {}, {}, {})", this, __func__, p0, p1, p2, p3, p4, p5);
        if (p0 == 21) {
            if (current_amo_for_reward == next_amount_for_reward) {
                auto orbs = 8;
                auto moons = 2;
                auto diamonds = 4;

                auto stats = GameStatsManager::sharedState();
                stats->incrementStat(GJScoreKey::TotalOrbs, orbs);
                stats->incrementStat(GJScoreKey::Moons, moons);
                stats->incrementStat(GJScoreKey::Diamonds, diamonds);

                auto rwdl = CurrencyRewardLayer::create(
                    orbs, 0, moons, diamonds,
                    CurrencySpriteType::Icon, 0,
                    CurrencySpriteType::Icon, 0,
                    m_player1->convertToWorldSpace(m_player1->getPosition()),
                    (CurrencyRewardType)0,
                    1.f, 1.f
                );
                this->addChild(rwdl);

                std::vector<std::string> aaa = { 
                    "buyItem01.ogg", "buyItem03.ogg", "gold01.ogg", "gold02.ogg", "highscoreGet02.ogg" 
                };
                FMODAudioEngine::get()->playEffect(*select_randomly(aaa.begin(), aaa.end()));
                FMODAudioEngine::get()->playEffect("achievement_01.ogg");

                current_amo_for_reward = 0;
            }
            else {
                ++current_amo_for_reward;
            }
        }
        Notification* plr_ntfy = nullptr;
        auto obj = [](int key) {
            auto rtn = GameObject::createWithKey(key);
            return CCSprite::createWithSpriteFrame(rtn->displayFrame());
            };
        if (p0 == 31) plr_ntfy = Notification::create("Enable Ghost", obj(32));
        if (p0 == 32) plr_ntfy = Notification::create("Disable Ghost", obj(33));
        if (p0 == 33) plr_ntfy = Notification::create("Show PLayer", obj(1613));
        if (p0 == 34) plr_ntfy = Notification::create("Hide PLayer", obj(1612));
        if (p0 == 37) plr_ntfy = Notification::create("Gravity 1.00", obj(2066));
        if (p0 == 38) plr_ntfy = Notification::create("Gravity 0.50", obj(2066));
        if (p0 == 39) plr_ntfy = Notification::create("Gravity 1.30", obj(2066));
        if (plr_ntfy) {
            if (auto old_plr_ntfy = typeinfo_cast<Notification*>(CCScene::get()->getChildByIDRecursive("plr_ntfy"))) {
                old_plr_ntfy->hide();
            }
            plr_ntfy->setID("plr_ntfy");
            public_cast(plr_ntfy, m_icon)->runAction(CCRepeatForever::create(CCScaleTo::create(0.f, 0.7f)));
            plr_ntfy->show();
        }
        return GJBaseGameLayer::spawnGroup(p0, p1, p2, p3, p4, p5);
    }
};

#include <Geode/modify/PlayLayer.hpp>
class $modify(PlayInfinityMode, PlayLayer) {
    bool ENABLED() {
        return nullptr != this->getChildByIDRecursive("INFINITY_MODE_MARK");
    }
    $override void updateVisibility(float p0) {
        if (not ENABLED()) return PlayLayer::updateVisibility(p0);

        m_attemptLabel->setString(fmt::format(
            "{} : {}""\n"
            "\n"
            "Current Deaths: {}""\n"
            "Total Deaths:    {}""\n"
            "\n"
            "Gravity Mod: {:.2}""\n",
            InfinityMode::current_amo_for_reward, InfinityMode::next_amount_for_reward,
            InfinityMode::current_deaths,
            getMod()->getSavedValue<int>("InfinityModeTotalDeaths"),
            m_player1->m_gravityMod
        ).c_str());
        m_attemptLabel->setAlignment(kCCTextAlignmentLeft);
        m_attemptLabel->setAnchorPoint(CCPoint(0.f, 1.f));
        m_attemptLabel->setPosition(CCPoint(8.f, this->getContentHeight() - 4.f));
        m_attemptLabel->setScale(0.4f);
        if (m_attemptLabel->getParent() != this) {
            m_attemptLabel->removeFromParentAndCleanup(0);
            this->addChild(m_attemptLabel);
        }

        auto fmod = FMODAudioEngine::sharedEngine();
        if (not fmod->m_metering) fmod->enableMetering();
        auto pulse = (fmod->m_pulse1 + fmod->m_pulse2 + fmod->m_pulse3) / 3;
        auto mainlayer = getChildByIDRecursive("main-node");
        mainlayer->removeChildByTag(6824409);
        auto colp = (pulse * 60);
        auto pulsebg = CCLayerColor::create(ccc4(colp, colp, colp, 0));
        pulsebg->setBlendFunc({ GL_ONE, GL_ONE });
        mainlayer->addChild(pulsebg, 0, 6824409);

        return PlayLayer::updateVisibility(p0);
    }
    $override void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        if (not ENABLED()) return PlayLayer::destroyPlayer(p0, p1);
        PlayLayer::destroyPlayer(p0, p1);

        if (!p0->m_isDead) return;
        //log::debug("m_isDead");

        InfinityMode::current_amo_for_reward = 0;

        InfinityMode::current_deaths += 1;
        getMod()->setSavedValue(
            "InfinityModeTotalDeaths",
            1 + getMod()->getSavedValue<int>("InfinityModeTotalDeaths")
        );

        this->m_levelSettings->m_songOffset += this->m_timePlayed;
        m_timePlayed = 0.f;

    }
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerLocalGameModes, MenuLayer) {
    $override bool init() {
        auto rtn = MenuLayer::init();

        //remove unused links
        if (auto play_button = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("play-button"))) {
            CCMenuItemExt::assignCallback<CCMenuItemSpriteExtra>(play_button,
                [this](auto) {
                    auto popup = createQuickPopup(
                        "Select Game Mode",
                        "\n \n \n \n \n \n ",
                        "Close", nullptr,
                        [this](auto, bool btn2) {
                            if (btn2) this->onPlay(this);
                        }
                    );
                    auto menu = CCMenu::create();
                    popup->m_mainLayer->addChild(menu, 1);

                    auto Levels = CCMenuItemExt::createSpriteExtra(
                        ButtonSprite::create("Main Levels", "bigFont.fnt", "geode.loader/black-square.png", 0.8f),
                        [this](auto) {
                            this->onPlay(this);
                        }
                    );
                    menu->addChild(Levels);

                    auto Islands = CCMenuItemExt::createSpriteExtra(
                        ButtonSprite::create("The Islands", "bigFont.fnt", "geode.loader/black-square.png", 0.8f),
                        [this](auto) {
                            Notification::create("in dev")->show();
                            switchToScene(IslandsLayer::create());
                        }
                    );
                    menu->addChild(Islands);

                    auto Infinity = CCMenuItemExt::createSpriteExtra(
                        ButtonSprite::create("Infinity Mode", "bigFont.fnt", "geode.loader/black-square.png", 0.8f),
                        [this](auto) {
                            InfinityMode::open();
                        }
                    );
                    menu->addChild(Infinity);

                    menu->alignItemsVerticallyWithPadding(12.f);

                    handleTouchPriority(popup);
                }
            );
        };

        return rtn;
    }
    inline static bool seenWarn = false;
};
