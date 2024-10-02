#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(PlayLayerExt, PlayLayer) {
};

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(GJBaseGameLayerExt, GJBaseGameLayer) {
	$override void gameEventTriggered(GJGameEvent p0, int p1, int p2) {
		//log::debug("{}({}, {}, {})", __FUNCTION__, (int)p0, p1, p2);
		GJBaseGameLayer::gameEventTriggered(p0, p1, p2);

		auto eventID = static_cast<int>(p0);

		PlayerObject* player = nullptr;
		if (p2 == 0) return;//a
		if (p2 == 1) player = m_player1;//a
		if (p2 == 2) player = m_player2;//a

		if (p0 == GJGameEvent::NormalJump) //jump
			if (SETTING(bool, "jelly cube")) player->animatePlatformerJump(0.9f); 

	}
};

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObjectExt, PlayerObject) {
	$override void switchedToMode(GameObjectType p0) {
		//log::debug("{}->{}({})", this, __FUNCTION__, (int)p0);
		if (p0 == GameObjectType::ShipPortal or p0 == GameObjectType::UfoPortal) {
			this->m_iconSprite->stopActionByTag(13);
			this->m_iconSprite->stopActionByTag(14);
			this->m_iconGlow->stopActionByTag(13);
			this->m_iconGlow->stopActionByTag(14);
		}
		return PlayerObject::switchedToMode(p0);
	}
	$override void bumpPlayer(float p0, int p1, bool p2, GameObject * p3) {
		if (SETTING(bool, "jelly cube")) this->animatePlatformerJump(p0);
		PlayerObject::bumpPlayer(p0, p1, p2, p3);
	}
    $override void updateRotation(float p0) {
		if ((m_isRobot or m_isSpider) and not m_isOnGround) {
			auto plat = m_isPlatformer;

			m_isPlatformer = 0;

			if (SETTING(bool, "Funny Robot Rotation")) m_isShip = 1;
			else m_isBird = 1;

			PlayerObject::updateRotation(p0);

			m_isBird = 0;
			m_isShip = 0;
			m_isPlatformer = plat;

			return;
		};
		if (SETTING(bool, "Break Player Rotations At Plat")) if (m_isPlatformer) {

			m_isPlatformer = 0;
			PlayerObject::updateRotation(p0);
			m_isPlatformer = 1;

			return;
		};
		PlayerObject::updateRotation(p0);
	}
};
