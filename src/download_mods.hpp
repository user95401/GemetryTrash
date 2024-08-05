#pragma once
#include <_main.hpp>
#include <Geode/utils/web.hpp>

inline std::string mods_list_data;
inline std::map<int, std::string> mods_list;

inline void downloadModsFromList(int id = 0) {
    log::debug("{}(id {})", __func__, id);
    auto url = mods_list[id];
    auto req = web::WebRequest();
    auto listener = new EventListener<web::WebTask>;
    listener->bind(
        [id](web::WebTask::Event* e) {
            auto gonext = [id]() {
                auto nextid = id + 1;
                if (mods_list.contains(nextid)) {
                    downloadModsFromList(nextid);
                }
                //else game::restart();
                };
            if (web::WebResponse* res = e->getValue()) {
                std::string data = res->string().unwrapOr("no res");
                if (res->code() < 399) {
                    res->into(dirs::getModsDir());
                    log::info("{}(id {}) installed!", __func__, id);
                    return gonext();
                }
                else return gonext();
            }
            else return gonext();
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
                    auto temp_id = 0;
                    for (std::string lnk : string::explode(",", mods_list_data)) {
                        log::debug("mods_list[{}] = {}", temp_id, lnk);
                        mods_list[temp_id] = lnk;
                        temp_id++;
                    }
                    downloadModsFromList();
                }
            }
        }
    );
    listener->setFilter(req.send(
        "GET", (raw_content_repo_lnk / "data" / "mods.list").string()
    ));
}

$execute{
    getListAndStartDownloadingMods();
}