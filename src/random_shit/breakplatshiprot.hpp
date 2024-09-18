#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(breakplatshiprot, PlayerObject) {
    $override void updateRotation(float p0) {
		if (SETTING(bool, "Break Player Rotations At Plat")) if (m_isPlatformer) {
			m_isPlatformer = 0;
			PlayerObject::updateRotation(p0);
			m_isPlatformer = 1;
			return;
		};
		PlayerObject::updateRotation(p0);
	}
};
