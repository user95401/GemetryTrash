#pragma once
#include <_main.hpp>

#include <Geode/modify/CCMenuItemSprite.hpp>
class $modify(CCMenuItemSpriteSoundExt, CCMenuItemSprite) {
    $override void selected() {
        FMODAudioEngine::get()->playEffect("btnClick.ogg");
        return CCMenuItemSprite::selected();
    }
};

#include <Geode/modify/CCMenuItem.hpp>
class $modify(CCMenuItemAnimExt, CCMenuItem) {
    $override void activate() {
        GEODE_ANDROID(return CCMenuItem::activate());

        CCMenu* menu = typeinfo_cast<CCMenu*>(this->getParent());
        if (not menu) return CCMenuItem::activate();

        menu->removeChildByTag(568249);
        auto plr_container = CCNode::create();
        menu->addChild(plr_container, -1, 568249);

        plr_container->setPosition(this->getPosition());

        auto plrdmmya = PlayerObject::create(
            0, 0, new PlayLayer(), new PlayLayer(), 0
        );
        plr_container->addChild(plrdmmya, 11, 568249);
        plrdmmya->removeAllChildrenWithCleanup(0);

        plrdmmya->playDeathEffect();

        CCMenuItem::activate();
    }
};
