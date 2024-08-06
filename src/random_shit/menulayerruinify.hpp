#pragma once
#include <_main.hpp>

//dont play other songs
#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerExt, MenuLayer) {
    $override bool init() {
        return MenuLayer::init();
    }
};
