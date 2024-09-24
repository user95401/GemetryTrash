#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

class TheTowerSecondFloor : public CCLayer {
public:
	CREATE_FUNC(TheTowerSecondFloor);
	bool init() {
		this->CCLayer::init();

		auto msg = MDTextArea::create(
			"# really cuming son"
			"\n""but later xd. not now. not when we hasnt players enouth",
			this->getContentSize() - (CCSize(1, 0.5) * 280)
		);
		addChildAtPosition(msg, Anchor::Center);

		auto bg = CCScale9Sprite::createWithSpriteFrameName("towerBG_01_001.png", {15, 6, 6, 15 });
		bg->setContentSize(msg->getContentSize() + (CCSize(1,1) * 88));
		bg->setZOrder(-1);
		addChildAtPosition(bg, Anchor::Center);

		return 1;
	}
};

#include <Geode/modify/CCMenuItem.hpp>
class $modify(CCMenuItemTheTowerSecondFloor, CCMenuItem) {
	$override void activate() {

		auto listener = typeinfo_cast<CCNode*>(this->m_pListener);
		if (getID() == "second-floor-button" and listener) {
			auto mainnode = listener->getChildByIDRecursive("main-node");
			if (mainnode) mainnode->runAction(CCEaseSineInOut::create(CCMoveTo::create(
				1.f, { mainnode->getPositionX(), listener->getContentHeight() / -2 }
			)));
			auto floor2 = TheTowerSecondFloor::create();
			floor2->setPositionY(listener->getContentHeight());
			mainnode->addChild(floor2);
			return;
		}

		return CCMenuItem::activate();
	}
};
