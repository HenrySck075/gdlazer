#ifndef __osu_ui_containers_wave__
#define __osu_ui_containers_wave__

#include <Geode/Geode.hpp>
using namespace cocos2d;

class WaveContainer : public CCLayer {
private:
  CCSprite* wave1;
  CCSprite* wave2;
  CCSprite* wave3;
  CCSprite* wave4;

  CCNode* body;

  float angle1 = 13;
  float angle2 = -7;
  float angle3 = 4;
  float angle4 = -2;

  float appearDuration = 0.8f;
  float disappearDuration = 0.5f;
public:
  bool init(ccColor3B color, CCNode* body);
  static WaveContainer* create(ccColor3B color, CCNode* body);
  void show();
};

#endif // !__osu_ui_containers_wave__
