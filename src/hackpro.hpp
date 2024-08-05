#pragma once
#include <_main.hpp>

#include <Geode/modify/GameManager.hpp>
class $modify(IconHackExt, GameManager) {
    $override bool isIconUnlocked(int id, IconType type) { return true; }
    $override bool isColorUnlocked(int id, UnlockType type) { return true; }
};

#include <Geode/modify/PlayLayer.hpp>
class $modify(ComplateKickHackExt, PlayLayer) {
    $override void levelComplete() {
        return;
        return PlayLayer::levelComplete();
    }
};