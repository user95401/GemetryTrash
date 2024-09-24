#pragma once
#include <_main.hpp>

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayerExt, LoadingLayer) {
    $override bool init(bool p0) {
        srand(time(0)); //bool(rand() % 2)

        auto rtn = LoadingLayer::init(p0);

        auto bg = typeinfo_cast<CCSprite*>(this->getChildByID("bg-texture"));
        if (bg) {
            auto background = rand() % 59;
            auto new_bg = CCSprite::create(fmt::format(
                "game_bg_{:02d}_001.png", background
            ).c_str());
            if (new_bg) bg->setDisplayFrame(new_bg->displayFrame());
        }

        GameManager::get()->fadeInMusic("loading_theme.mp3");

        return rtn;
    }
};