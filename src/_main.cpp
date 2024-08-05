#include <_main.hpp>

#ifdef GEODE_IS_WINDOWS
#include <Geode/modify/CCScene.hpp>
class $modify(WindowNameExt, CCScene) {
    $override static CCScene* create() {
        auto GameWindowHandle = WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(CCEGLView::sharedOpenGLView()->getWindow()) + 0x244));
        SetWindowTextA(GameWindowHandle, Mod::get()->getName().data());
        return CCScene::create();
    }
};
#endif

#include <Geode/modify/GameManager.hpp>
class $modify(IconHackExt, GameManager) {
    $override bool isIconUnlocked(int id, IconType type) { return true; }
    $override bool isColorUnlocked(int id, UnlockType type) { return true; }
};