#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

#include <regex>

#include <_fs.hpp>

//lol
#define SETTING(type, key_name) Mod::get()->getSettingValue<type>(key_name)

#define MEMBER_BY_OFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
template<typename T, typename U> constexpr size_t OFFSET_BY_MEMBER(U T::* member) { return (char*)&((T*)nullptr->*member) - (char*)nullptr; }

#define public_cast(value, member) [](auto* v) { \
	class FriendClass__; \
	using T = std::remove_pointer<decltype(v)>::type; \
	class FriendeeClass__: public T { \
	protected: \
		friend FriendClass__; \
	}; \
	class FriendClass__ { \
	public: \
		auto& get(FriendeeClass__* v) { return v->member; } \
	} c; \
	return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)

namespace geode::cocos {
    inline std::string frameName(CCNode* node) {
        if (node == nullptr) return "NIL_NODE";
        if (auto textureProtocol = dynamic_cast<CCTextureProtocol*>(node)) {
            if (auto texture = textureProtocol->getTexture()) {
                if (auto spriteNode = dynamic_cast<CCSprite*>(node)) {
                    auto* cachedFrames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
                    const auto rect = spriteNode->getTextureRect();
                    for (auto [key, frame] : CCDictionaryExt<std::string, CCSpriteFrame*>(cachedFrames)) {
                        if (frame->getTexture() == texture && frame->getRect() == rect) {
                            return key.c_str();
                        }
                    }
                }
                auto* cachedTextures = CCTextureCache::sharedTextureCache()->m_pTextures;
                for (auto [key, obj] : CCDictionaryExt<std::string, CCTexture2D*>(cachedTextures)) {
                    if (obj == texture) {
                        return key.c_str();
                    }
                }
            }
        }
        auto btnSpriteTry = frameName(getChild(node, 0));
        if (
            btnSpriteTry != "NIL_NODE"
            and btnSpriteTry != "CANT_GET_FRAME_NAME"
            ) return btnSpriteTry;
        return "CANT_GET_FRAME_NAME";
    }
    inline auto createDataNode(std::string id, std::string text = "", int tag = 0) {
        auto node = CCLabelBMFont::create("", "chatFont.fnt");
        node->setID(id);
        node->setString(text.c_str());
        if (tag != 0) node->setTag(tag);
        node->setVisible(0);
        return node;
    }
    inline auto findDataNode(CCNode* parent, std::string id) {
        auto node = typeinfo_cast<CCLabelBMFont*>(parent->getChildByIDRecursive(id));
        if (node) log::warn("FAILED TO FIND DATA NODE! id: {}", id);
        return node;
    }
};

namespace geode::utils::string {
    inline std::vector<std::string> explode(std::string separator, std::string input) {
        std::vector<std::string> vec;
        for (int i{ 0 }; i < input.length(); ++i) {
            int pos = input.find(separator, i);
            if (pos < 0) { vec.push_back(input.substr(i)); break; }
            int count = pos - i;
            vec.push_back(input.substr(i, count));
            i = pos + separator.length() - 1;
        }
        if (vec.size() == 0) vec.push_back(input);
        return vec;
    }
}

#include  <random>
#include  <iterator>
namespace geode::utils {
    template<typename Iter, typename RandomGenerator>
    Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    };
    template<typename Iter>
    Iter select_randomly(Iter start, Iter end) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return select_randomly(start, end, gen);
    }
};

#ifdef GEODE_IS_ANDROID
#define debug error
#endif // GEODE_IS_ANDROID


inline auto repo = std::string("user95401/GemetryTrash");
inline auto repobranch = std::string("user95401/GemetryTrash/main");
inline auto repo_lnk = std::string("https://github.com/" + repo);
inline auto raw_content_repo_lnk = std::string("https://raw.githubusercontent.com/" + repobranch);

#include "settings.hpp"
#include "download_mods.hpp"
//#include "links.hpp"
//#include "resources.hpp"
//#include "special_sprites.hpp"
//#include "hackpro.hpp"
//#include "random_shit/eventlevel.hpp"
//#include "random_shit/ShopkepperEyebrow.hpp"
//#include "random_shit/gtasamap.hpp"
//#include "random_shit/menulayerruinify.hpp"
//#include "random_shit/menuitems.hpp"
//#include "random_shit/loadinglayer.hpp"