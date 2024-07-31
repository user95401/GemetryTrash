#include <_main.hpp>
#include <Geode/utils/web.hpp>

std::map<int, std::string> mods;

void downloadMod(auto ew) {

}

void getListAndStartDownloadingMods() {
    auto req = web::WebRequest();
    auto listener = new EventListener<web::WebTask>;
    listener->bind(
        [](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                std::string data = res->string().unwrapOr("no res");
                if (res->code() < 399) {
                    onDownloadingEnd();
                }
            }
        }
    );
    listener->setFilter(req.send("GET", url));
}

$execute{
    getListAndStartDownloadingMods();
}