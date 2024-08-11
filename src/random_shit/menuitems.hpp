#pragma once
#include <_main.hpp>

#include <Geode/modify/CCMenuItemSprite.hpp>
class $modify(CCMenuItemSpriteSoundExt, CCMenuItemSprite) {
    $override void selected() {
        if (SETTING(bool, "No Button Click Sound")) return CCMenuItemSprite::selected();
        FMODAudioEngine::get()->playEffect("btnClick.ogg");
        return CCMenuItemSprite::selected();
    }
};

#include <Geode/modify/CCMenuItem.hpp>
class $modify(CCMenuItemDialogExt, CCMenuItem) {
    struct Fields : DialogDelegate {
        std::function<void(DialogLayer*)> onDialogClosed = [](DialogLayer* p0) {};
        void dialogClosed(DialogLayer* p0) override {
            onDialogClosed(p0);
        }
    };
    $override void activate() {
        if (SETTING(bool, "No Button UDare Dialog")) return CCMenuItem::activate();
        srand(time(0));
        if ((rand() % 100 > 3) or findDataNode(this, "hasDialog")) {
            return CCMenuItem::activate();
        }
        if (auto spriteitem = typeinfo_cast<CCMenuItemSpriteExtra*>(this)) {
            spriteitem->addChild(cocos::createDataNode("hasDialog"));
            if (auto itemimage = typeinfo_cast<CCNode*>(spriteitem->getNormalImage())) {
                DialogLayerExt::DialogIcon = itemimage;
            };
            auto* dialog = DialogLayer::createDialogLayer(
                DialogObject::create(
                    "Button",
                    "<cr>You dare touch me!?</c>\n<d100>Ok.",
                    5, 1, false, ccc3(255, 255, 255)
                ),
                nullptr, 1 // idk, Background
            );
            dialog->m_delegate = (m_fields.self());
            m_fields->onDialogClosed = [this](DialogLayer* p0) {
                CCMenuItem::activate();
                };
            dialog->animateInRandomSide();
            dialog->addToMainScene();
        };
    }
};

#include <Geode/modify/CCMenuItem.hpp>
class $modify(CCMenuItemAnimExt, CCMenuItem) {
    $override void activate() {
        GEODE_ANDROID(return CCMenuItem::activate());

        if (SETTING(bool, "No Button Anim")) return CCMenuItem::activate();

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
