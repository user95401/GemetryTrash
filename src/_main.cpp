#include "_main.hpp"

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/CCScene.hpp>
class $modify(WindowNameExt, CCScene) {
    $override static CCScene* create() {
        auto GameWindowHandle = WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(CCEGLView::sharedOpenGLView()->getWindow()) + 0x244));
        SetWindowTextA(GameWindowHandle, Mod::get()->getName().data());
        auto rtn = CCScene::create();
        return rtn;
    }
};

class mouse_particle : public CCParticleSystemQuad {
public:
    static inline bool created = false;
    static inline Ref<CCParticleSystemQuad> shared_ref;
    static void create() {
        if (created) return;
        created = true;
        shared_ref = Ref<CCParticleSystemQuad>(GameToolbox::particleFromString(
                "200a-1a0.54a0.94a-1a90a180a0a20a1a1a0a0a0a0a0a0a1a2a0a0a0.211765a0.1a0.207843a0.1a0.207843a0.1a1a0a0a0a0a0a0a0a0a0a0a0a1a0a0a0a0a0a0a0a10a0a0a0a1a1a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0"
        , CCParticleSystemQuad::create(), 0));
        shared_ref->setID("mouse_particle"_spr);
        shared_ref->_setZOrder(INT_MAX);
        shared_ref->schedule(schedule_selector(mouse_particle::sch));
        SceneManager::get()->keepAcrossScenes(shared_ref);
    }
    void sch(float) {
        shared_ref->runAction(CCRepeatForever::create(CCMoveTo::create(0.000f, getMousePos())));
        auto hide = false;
        if (auto game = GameManager::get()->m_gameLayer) hide = game->isRunning();
        hide ? shared_ref->stopSystem() : shared_ref->resumeSystem();
    }
    #include <Geode/modify/CCScene.hpp>
    class $modify(MouseParticleExt, MenuLayer) {
        $override bool init() {
            mouse_particle::create();
            return MenuLayer::init();
        }
    };
};

#endif