#pragma once
#include <_main.hpp>

#include <Geode/modify/CCScene.hpp>
class $modify(EditorCaveSounds, CCScene) {
    void random(float) {
        if (auto editor = GameManager::get()->m_levelEditorLayer) {
            if (editor->isRunning()) {
                srand(time(0));
                if ((rand() % 100 < 38)) {
                    auto sound_name = fmt::format("cave{}.ogg", rand() % 14);
                    FMODAudioEngine::get()->playEffect(sound_name);
                }
            }
        }
    };
    $override static CCScene* create(void) {
        auto __this = CCScene::create();
        if (SETTING(bool, "Editor Cave Sounds")) {
            __this->schedule(schedule_selector(EditorCaveSounds::random), 12.f);
        };
        return __this;
    }
};