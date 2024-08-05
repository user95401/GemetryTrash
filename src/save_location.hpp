#pragma once
#include <_main.hpp>

#include <Geode/modify/GManager.hpp>
class $modify(GManager) {
    auto replaceFileName() {
        fs::create_directories(dirs::getSaveDir() / "GemetryTrash", fs::last_err_code);
        auto path = "GemetryTrash" / fs::path(this->m_fileName).filename();
        this->m_fileName = (path).string().data();
        return path;
    };
    $override void save() {
        replaceFileName();
        GManager::save();
    }
    $override void setup() {
        replaceFileName();
        GManager::setup();
    }
};