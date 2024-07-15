#ifndef __osu_ui_containers_wave__
#define __osu_ui_containers_wave__

#include <Geode/Geode.hpp>
using namespace cocos2d;

class WaveContainer : public geode::Popup<ccColor3B, CCNode*> {
private:
  CCDrawNode* wave1;
  CCDrawNode* wave2;
  CCDrawNode* wave3;
  CCDrawNode* wave4;

  CCNode* body;

  float angle1 = 13;
  float angle2 = -7;
  float angle3 = 4;
  float angle4 = -2;

  float appearDuration = 0.8f;
  float disappearDuration = 0.5f;

  CCDrawNode* createWave(CCSize size, float angle, ccColor3B col);
public:
  bool setup(ccColor3B color, CCNode* body) override;
  // @note RobTop addition
  bool customSetup(ccColor3B color, CCNode* body);
  static WaveContainer* create(ccColor3B color, CCNode* body);
  void show() override;
};

#endif // !__osu_ui_containers_wave__
