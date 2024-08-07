#pragma once
#include <_main.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(PlayLayerExt, PlayLayer) {
	$override bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        auto init_result = PlayLayer::init(level, useReplay, dontCreateObjects);

        return init_result;
    }
};