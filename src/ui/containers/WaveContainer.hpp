#ifndef __osu_ui_containers_wave__
#define __osu_ui_containers_wave__

#include <Geode/Geode.hpp>
#include "../color/OverlayColorProvider.hpp"
using namespace geode::prelude;

using ColorScheme = osu::Game::Overlays::OverlayColorScheme;

namespace osu {
  namespace Game {
    namespace Graphics {
      namespace Containers {
        class WaveContainer : public geode::Popup<ColorScheme, CCNode*> {
        private:
          bool hiding = false;

          Overlays::OverlayColorProvider* provider;

          CCPoint touchLoc = ccp(0,0);
          CCRect touchBoundary;

          CCDrawNode* wave1;
          CCDrawNode* wave2;
          CCDrawNode* wave3;
          CCDrawNode* wave4;

          CCNode* body;

          float angle1 = 13;
          float angle2 = -7;
          float angle3 = 4;
          float angle4 = -2;

          float pos1;
          float pos2;
          float pos3;
          float pos4;

          float appearDuration = 0.8f;
          float disappearDuration = 0.5f;

          CCDrawNode* createWave(float w, CCSize size, float angle, ccColor4B col);
          void onClose(cocos2d::CCObject*) override;
          void keyBackClicked() override;

          void ccTouchEnded(CCTouch* t, CCEvent* what) override;
          bool ccTouchBegan(CCTouch* t, CCEvent* what) override;
        public:
          bool setup(ColorScheme color, CCNode* body) override;
          // @note RobTop addition
          bool customSetup(CCNode* body);
          static WaveContainer* create(ColorScheme color, CCNode* body);
          void show() override;
          void hide();
        };
      }
    }
  }
}

#endif // !__osu_ui_containers_wave__
