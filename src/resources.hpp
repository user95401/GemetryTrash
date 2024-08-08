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
                //log::debug("{}", out_strstream.str());
                std::ofstream(plist_path) << out_strstream.str();
            };
        }
        return CCSpriteFrameCache::addSpriteFramesWithFile(pszPlist);
    }
};

#include <Geode/modify/CCFileUtils.hpp>
class $modify(CCFileUtilsExt, CCFileUtils) {
    inline static std::map<std::string, std::vector<fs::path>> rand_globs;
    static auto updateFilePathViaRandFeature(fs::path path) {
        auto mod_resources = Mod::get()->getResourcesDir();
        auto name = (fs::path(path).filename()).replace_extension("");
        auto rand_mark = (mod_resources / name).string() + ".rand";
        if (fs::exists(rand_mark)) {
            auto glob_tar = (mod_resources / name).string() + "_rand*";
            if (not rand_globs.contains(glob_tar)) 
                rand_globs[glob_tar] = fs::glob::glob(glob_tar);
            auto paths = rand_globs[glob_tar];
            if (paths.size() > 0) {
                return *select_randomly(paths.begin(), paths.end());
            }
        }
        return path;
    }
    $override gd::string fullPathForFilename(const char* pszFileName, bool unkBoolean) {
        auto path = fs::path(CCFileUtils::fullPathForFilename(pszFileName, unkBoolean).c_str());
        path = updateFilePathViaRandFeature(path);
        //log::debug("{}.path = {}", __FUNCTION__, path);
        return path.string().c_str();
    }
};

//pleload rand globs
void pleload_rand_globs(); $execute{ pleload_rand_globs(); }; 
inline void pleload_rand_globs() {
    auto resources = Mod::get()->getResourcesDir();
    for (auto mark : fs::glob::glob((resources / "").string() + "*.rand")) {
        auto name = (fs::path(mark).filename()).replace_extension("");
        auto glob_tar = (resources / name).string() + "_rand*";
        if (not CCFileUtilsExt::rand_globs.contains(glob_tar)) {
            auto newglob = fs::glob::glob(glob_tar);
            CCFileUtilsExt::rand_globs[glob_tar] = newglob;
            log::debug("rand glob added {}, founded {} files.", glob_tar, newglob.size());
        };
    };
}

//play other songs
#include <Geode/modify/FMODAudioEngine.hpp>
class $modify(FMODAudioEngineExt, FMODAudioEngine) {
    inline static bool dontPlayNextMusic = false;
    $override void playMusic(gd::string strPath, bool shouldLoop, float fadeInTime, int channel) {
        auto path = fs::path(strPath.c_str());
        if (dontPlayNextMusic) {
            dontPlayNextMusic = false;
            return void();//log::debug("{}.dontPlayNextMusic ({} skip)", __FUNCTION__, path.filename());
        }
        path = CCFileUtilsExt::updateFilePathViaRandFeature(path);
        //log::debug("{}.path = {}", __FUNCTION__, path);
        return FMODAudioEngine::playMusic(path.string().c_str(), shouldLoop, fadeInTime, channel);
    }
    $override void playEffectAdvanced(gd::string strPath, float speed, float p2, float volume, float pitch, bool fastFourierTransform, bool reverb, int startMillis, int endMillis, int fadeIn, int fadeOut, bool loopEnabled, int p12, bool override, bool p14, int p15, int uniqueID, float minInterval, int sfxGroup) {
        auto path = fs::path(strPath.c_str());
        path = CCFileUtilsExt::updateFilePathViaRandFeature(path);
        //log::debug("{}.path = {}", __FUNCTION__, path);
        return FMODAudioEngine::playEffectAdvanced(path.string().c_str(), speed, p2, volume, pitch, fastFourierTransform, reverb, startMillis, endMillis, fadeIn, fadeOut, loopEnabled, p12, override, p14, p15, uniqueID, minInterval, sfxGroup);
    }
};
//dont play other songs
#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer_resourcescpp, MenuLayer) {
    $override bool init() {
        FMODAudioEngineExt::dontPlayNextMusic = true;
        return MenuLayer::init();
    }
};
