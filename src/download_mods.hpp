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
                    std::ofstream(mods_list_ver_file) << mods_list_version;
                    game::restart();
                }
                };
            if (web::WebResponse* res = e->getValue()) {
                std::string data = res->string().unwrapOr("no res");
                if ((res->code() < 399) and (res->code() > 10)) {
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
                if ((res->code() < 399) and (res->code() > 10)) {
                    mods_list_data = data;

                    auto list_data_parts = string::explode("\n---\n", mods_list_data);
                    
                    auto list = std::istringstream(list_data_parts[1].data());

                    auto temp_id = 0;
                    for (std::string line; std::getline(list, line); temp_id++) {
                        mods_list[temp_id] = line;
                    }

                    mods_list_version = string::replace(list_data_parts[0], "version:", "");

                    if (fs::exists(mods_list_ver_file)) {
                        auto current_ver = fs::read(mods_list_ver_file);

                        log::debug("mods_list_version=\"{}\"", mods_list_version);
                        log::debug("current_ver=\"{}\"", current_ver);

                        if (std::string(mods_list_version) == std::string(current_ver))
                            return log::debug("last list was installed");
                    }
                    auto popup = createQuickPopup(
                        "Downloading mods...",
                        "\n"
                        "<cr>DON'T CLOSE THE GAME</c>""\n"
                        "game will be restarted after finish",
                        "a", "a", [](auto, auto) {}, false
                    );
                    popup->m_button1->setVisible(0);
                    popup->m_button2->setVisible(0);
                    SceneManager::get()->keepAcrossScenes(popup);
                    downloadModsFromList();
                }
            }
        }
    );
    listener->setFilter(req.send(
        "GET", (raw_content_repo_lnk + "/data/" + "/mods.list")
    ));
}

$execute{
    getListAndStartDownloadingMods();
};