#pragma once
#include <_main.hpp>

#include <Geode/modify/GManager.hpp>
class $modify(GManager_resourcescpp, GManager) {
    $override void setup() {
        CCFileUtils::sharedFileUtils()->addPriorityPath(fs::path(
            Mod::get()->getTempDir() / "resources" / Mod::get()->getID() / ""
        ).string().c_str());
        GManager::setup();
    }
};
#include <Geode/modify/CCSpriteFrameCache.hpp>
class $modify(CCSpriteFrameCache_resourcescpp, CCSpriteFrameCache) {
    $override void addSpriteFramesWithFile(const char* pszPlist) {
        if (string::contains(pszPlist, "repl"_spr)) {
            log::debug("patching plist \"{}\"", pszPlist);
            auto plist_path = CCFileUtils::get()->fullPathForFilename(pszPlist, 0);
            if (auto plist_ifstream = std::fstream(plist_path)) {
                auto out_strstream = std::stringstream();
                for (std::string line; std::getline(plist_ifstream, line); ) {
                    line = string::replace(line, (Mod::get()->getID() + "/"), "");
                    out_strstream << line << std::endl;
                }
                log::debug("{}", out_strstream.str());
                std::ofstream(plist_path) << out_strstream.str();
            };
        }
        return CCSpriteFrameCache::addSpriteFramesWithFile(pszPlist);
    }
};