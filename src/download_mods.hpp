#pragma once
#include <_main.hpp>
#include <Geode/utils/web.hpp>

inline std::string mods_list_data;
inline std::map<int, std::string> mods_list;
inline auto mods_list_version = std::string();
inline auto mods_list_ver_file = dirs::getModsDir() / ".list_version";

inline void downloadModsFromList(int id = 0) {
    log::debug("{}(id {})", __func__, id);
    auto url = mods_list[id];
    auto filename = fs::path(url).filename();
    auto req = web::WebRequest();
    auto listener = new EventListener<web::WebTask>;
    listener->bind(
        [id, filename](web::WebTask::Event* e) {
            auto gonext = [id]() {
                auto nextid = id + 1;
                if (mods_list.contains(nextid)) {
                    downloadModsFromList(nextid);
                }
                else {
                    std::fstream(mods_list_ver_file) << mods_list_version;
                    //game::restart();
                }
                };
            if (web::WebResponse* res = e->getValue()) {
                std::string data = res->string().unwrapOr("no res");
                if (res->code() < 399) {
                    res->into(dirs::getModsDir() / filename);
                    log::info("mod {}({}) installed!", filename, id);
                    return gonext();
                }
                else {
                    log::error("failed to install {}({}): {}", filename, id, res->string().error_or(data));
                    return gonext();
                }
            }
        }
    );
    listener->setFilter(req.send("GET", url));
}

inline void getListAndStartDownloadingMods() {
    log::debug("{}()", __func__);
    auto req = web::WebRequest();
    auto listener = new EventListener<web::WebTask>;
    listener->bind(
        [](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                std::string data = res->string().unwrapOr("no res");
                if (res->code() < 399) {
                    mods_list_data = data;

                    auto list_data_parts = string::explode("---", mods_list_data);
                    
                    auto list = list_data_parts[1];
                    auto temp_id = 0;
                    for (std::string lnk : string::explode("\n", list)) {
                        log::debug("mods_list[{}] = {}", temp_id, lnk);
                        mods_list[temp_id] = lnk;
                        temp_id++;
                    }

                    mods_list_version = string::replace(list_data_parts[0], "version:", "");

                    if (fs::exists(mods_list_ver_file)) {
                        auto current_ver = fs::read(mods_list_ver_file);
                        if (current_ver == mods_list_version) return log::debug("last list was installed");
                    }

                    downloadModsFromList();
                }
            }
        }
    );
    listener->setFilter(req.send(
        "GET", (raw_content_repo_lnk + "/data/" + "/mods.list")
    ));
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
    $override bool init() {
        if (!MenuLayer::init()) return false;
        getListAndStartDownloadingMods();
        return true;
    };
};