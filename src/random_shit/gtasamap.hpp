#include <Geode/Geode.hpp>

using namespace geode::prelude;

class FakeMapLayer : public CCLayer {
public:
	void onBtn(CCObject* ae) {
		auto btn = dynamic_cast<CCMenuItem*>(ae);
		if (not btn) return;
		if (btn->getID() == "backBtn") {
			keyBackClicked();
		}
	}
	auto btnSel() {
		return menu_selector(FakeMapLayer::onBtn);
	}
	virtual void keyBackClicked() {
		CCDirector::get()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
	}
	bool init() {
		auto rtn = this->CCLayer::init();

		this->setTouchEnabled(1);
		this->setMouseEnabled(1);
		this->setKeypadEnabled(1);
		this->setKeyboardEnabled(1);

		//menu
		auto menu = CCMenu::create();
		if (menu) {
			menu->setID("menu");
			menu->setLayout(AnchorLayout::create());
			addChild(menu);
		};

		//backbtn
		CCSprite* GJ_arrow_01_001 = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		CCMenuItemSpriteExtra* backBtn = {
			CCMenuItemSpriteExtra::create(
				GJ_arrow_01_001 ? GJ_arrow_01_001 : CCSprite::create(),
				this, btnSel()
			)
		};
		if (backBtn) {
			//layout
			AnchorLayoutOptions* layoutOptions = {
				AnchorLayoutOptions::create()
				->setAnchor(Anchor::TopLeft)
				->setOffset({ 28.f, -28.f })
			};
			backBtn->setLayoutOptions(layoutOptions);
			//id
			backBtn->setID("backBtn");
			//add to menu
			menu->addChild(backBtn);
			menu->updateLayout();
		}

		//aa
		auto map = CCSprite::create("gtasamap.png");
		if (map) {
			map->setScale(5.5f);
			map->setAnchorPoint(CCPointZero);
			//scroll
			auto scroll = ScrollLayer::create({ 380.f, 270.f });
			if (scroll) {
				scroll->m_disableHorizontal = 0;
				scroll->m_disableVertical = 0;
				scroll->m_contentLayer->setContentSize(map->getScaledContentSize());
				scroll->m_contentLayer->addChild(map);
				map->setContentSize(map->getScaledContentSize());
				//layout
				AnchorLayoutOptions* layoutOptions = {
					AnchorLayoutOptions::create()
					->setAnchor(Anchor::Center)
					->setOffset(scroll->getContentSize() / -2)
				};
				scroll->setLayoutOptions(layoutOptions);
				//add
				menu->addChild(scroll);
				menu->updateLayout();
			};
		}

		return rtn;
	}
	static auto create() {
		auto rtn = new FakeMapLayer;
		rtn->init();
		return rtn;
	}
	static auto scene() {
		auto layer = create();
		auto scene = CCScene::create();
		scene->addChild(layer);
		return scene;
	}
};

#include <Geode/modify/CreatorLayer.hpp>
class $modify(gtasamapCreatorLayer, CreatorLayer) {
	$override void onAdventureMap(CCObject*) {
		CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, FakeMapLayer::scene()));
	}
	$override bool init() {
		if (!CreatorLayer::init()) return false;

		if (auto mapButton = dynamic_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("map-button"))) {
			auto newBtnSprite = CCSprite::createWithSpriteFrameName("GJ_mapBtn_001.png");
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
