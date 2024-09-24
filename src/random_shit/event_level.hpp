#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

#include <Geode/modify/CreatorLayer.hpp>
class $modify(eventlevelCreatorLayer, CreatorLayer) {
    $override void onEventLevel(CCObject*) {
		DailyLevelPage::create(GJTimedLevelType::Event)->show();
	}
	$override bool init() {
		if (!CreatorLayer::init()) return false;

		if (auto mapButton = dynamic_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("event-button"))) {
			auto newBtnSprite = CCSprite::createWithSpriteFrameName("GJ_eventBtn_001.png");
			auto oldImage = mapButton->getNormalImage();
			if (newBtnSprite and oldImage) {
				newBtnSprite->setScale(oldImage->getScale());
				newBtnSprite->setAnchorPoint(oldImage->getAnchorPoint());
				auto oldSize = CCSize(mapButton->getContentSize());
				mapButton->setNormalImage(newBtnSprite);
				mapButton->setContentSize(oldSize);
			}
		}

		return true;
	}
};
