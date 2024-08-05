#pragma once
#include <_main.hpp>

using namespace geode::prelude;

inline auto server = fs::path();

inline void loadServerInf() {
	if (not server.empty()) return;
	auto req = web::WebRequest();
	auto listener = new EventListener<web::WebTask>;
	listener->bind(
		[](web::WebTask::Event* e) {
			if (web::WebResponse* res = e->getValue()) {
				std::string data = res->string().unwrapOr("no res");
				if (res->code() < 399) {
					server = data;
					log::debug("server = {}", server);
				}
				else {
				}
			}
		}
	);
	listener->setFilter(req.send(
		"GET", (raw_content_repo_lnk / "data" / "server.inf").string()
	));
}

#include <Geode/modify/CCHttpClient.hpp>
class $modify(CCHttpClient) {
	void send(CCHttpRequest * req) {
		std::string url = req->getUrl();
		if (not server.empty()) url = std::regex_replace(
			url, 
			std::regex("www.boomlings.com\\/database"), 
			server.string()
		);
		req->setUrl(url.c_str());
		CCHttpClient::send(req);
	}
};

web::WebTask web_send(web::WebRequest* __this, std::string_view method, std::string_view url) {
	if (not server.empty()) url = std::regex_replace(
		url.data(),
		std::regex("www.boomlings.com\\/database"),
		server.string()
	);
	return __this->send(method, std::string_view(url));
};

$execute{
	loadServerInf();
	Mod::get()->hook(
		reinterpret_cast<void*>(
			geode::addresser::getNonVirtual(&web::WebRequest::send)
		),
		&web_send,
		"web::WebRequest::send",
		tulip::hook::TulipConvention::Thiscall
	);
}