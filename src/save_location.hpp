#pragma once
#include <_main.hpp>

#include <Geode/modify/GManager.hpp>
class $modify(GManager) {
    void setup() {
        m_fileName = (Mod::get()->getConfigDir() / m_fileName).string().data();
		log::debug("{}().m_fileName = {}", __func__, m_fileName);
        GManager::setup();
    }
};