#include "OsuLogo.hpp"
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"
#include "../../../frameworks/utils/Interpolation.hpp"
#include <cmath>
#include <numeric>

float lin2dB(float linear)
{
  return -(clamp(log10(linear) * 20.0f, -80.f, 1.0f));
}
GDL_NS_START
using namespace frameworks;
bool OsuLogo::init() {
  ClickableContainer::init("osu-logo-select.wav"_spr);
  /*
  m_listener = template addEventListener<EventFilter<BeatEvent>>([this](float elapsed) {
    const char* sfxName;
    if (beatCount == 3) {
      sfxName = "osu-logo-heartbeat.wav"_spr;
    }
    else {
      sfxName = "osu-logo-downbeat.wav"_spr;
    }
    FMODAudioEngine::sharedEngine()->playEffect(sfxName);
    beatCount++;
    if (beatCount == 4) { beatCount = 0; };
    return ListenerResult::Propagate;
  });
  */
  logoSprite = CCSprite::createWithSpriteFrameName("logo.png"_spr);
  setAnchorPoint({0.5, 0.5});

  if (logoSprite != nullptr) {
    logoSprite->setID("m");
    ClickableContainer::setContentSize(logoSprite->getContentSize());
    logoSprite->setPosition(logoSprite->getContentSize() / 2);
    logoSprite->setScale(0.8);
    addChild(logoSprite);
  }
  audio = AudioManager::get();

  // run update
  scheduleUpdate();

  // create the visualizer later
  // so its scheduleUpdate call runs after this
  auto visualizer = LogoVisualization::create();
  addChild(visualizer);
  return true;
}
void OsuLogo::update(float delta) {
  CCNode::update(delta);
  
  float radius = std::fminf(getContentWidth(), getContentHeight()) / 2.f;
  float oldRadius = getBorderRadius();
  if (radius != oldRadius) setBorderRadius(radius);

  float dominantVol = 0;
  float window = 2048;
  audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &dominantVol,nullptr,0);
  audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_WINDOWSIZE, &window,nullptr,0);
  // this will go wrong
  logoSprite->setScale(Interpolation::damp(logoSprite->getScaleX(),1.f-std::max(0.f,dominantVol/window-0.4f)*0.04f,0.9,audio->getElapsed()));
}
/*
void OsuLogo::onBeat(float delta) {
  auto spr = static_cast<CCSprite*>(getChildByID("m"));
  spr->setScale(1);
  spr->runAction(CCScaleTo::create(0.5, 0.8));
}
*/

/// If 0.0024f feels to small then multiply it
static const float bars_per_visualizer = 200;
static const float decay_per_millisecond = 0.0024f;
static const float time_between_updates = 50.f;
/// @brief amount of time passed
static float t = 0;
static int offset = 0;
static const int step = 5;
float bar_length = 40;
bool LogoVisualization::init() {
  if (!Container::init()) return false;
  //bar_length = processUnit(600, Unit::UIKit, true);
  drawNode = CCDrawNode::create();
  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* e){
    drawNode->setContentSize(CCNode::getContentSize());
    drawNode->setPosition(CCNode::getContentSize()/2);
    return true;
  });
  drawNode->setAnchorPoint({0.5,0.5});
  setContentSize({100,100},Unit::Percent);
  addChild(drawNode);
  return true;
}
/*
void drawSegment2(CCDrawNode* drawNode, const CCPoint& startPoint, const CCPoint& endPoint, float radius, const ccColor4F& color) // taken from the depths of google
{
    CCPoint direction = ccpNormalize(ccpSub(endPoint, startPoint));
    CCPoint perpendicular = ccp(-direction.y, direction.x);

    CCPoint vertices[4];
    vertices[0] = ccpAdd(startPoint, ccpMult(perpendicular, radius));
    vertices[1] = ccpSub(startPoint, ccpMult(perpendicular, radius));
    vertices[2] = ccpSub(endPoint, ccpMult(perpendicular, radius));
    vertices[3] = ccpAdd(endPoint, ccpMult(perpendicular, radius));

    drawNode->drawPolygon(vertices, 4, color, 0, {0,0,0,0});
}
*/
void LogoVisualization::update(float delta) {
  CCNode::update(delta);
  t+=delta;
  auto logo = static_cast<OsuLogo*>(m_pParent);
  drawNode->setScale(logo->logoSprite->getScale());
  if (t<time_between_updates/1000) return;
  float dominantVol = 0;
  int window = 2048;
  //FMOD_DSP_PARAMETER_FFT* spectrum;
  logo->audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &dominantVol,nullptr,0);
  // idk
  // https://qa.fmod.com/t/how-to-get-spectrum-data-including-amplitudes-with-fmod-dsp-parameter-fft/12183/2
  //logo->audio->getDSP()->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)spectrum,0,nullptr,0);
  logo->audio->getDSP()->getParameterInt(FMOD_DSP_FFT_WINDOWSIZE, &window,nullptr,0);
  // idk if its right (i doubt it is)
  //float dominantVol = (spectrum->spectrum[0][(int)dominant]+spectrum->spectrum[1][(int)dominant])/2;
  float dominantRatio = dominantVol/window;
  drawNode->clear();

  int s = bars_per_visualizer/(int)(step);
  for (int i = 0; i < bars_per_visualizer; i++) {
    float rot = kmDegreesToRadians((i / bars_per_visualizer) * 360);
    auto center = CCNode::getContentSize()/2;
    CCPoint start{center.width,CCNode::getContentSize().height-5};
    if ((i-offset) % s == 0) {
      bars[i] = bar_length*dominantRatio;
    } else {
      if (bars[i] != 0) {
        bars[i] -= (decay_per_millisecond/(time_between_updates/1000))*50;
        if (bars[i] < 0) bars[i] = 0;
      }
    }
    drawNode->drawSegment(
      ccpRotateByAngle(start,center,rot),
      ccpRotateByAngle(start+CCPoint{0,bars[i]},center,rot),
      8,{1.f,1.f,1.f,1.f}
    );
  }
  t = 0;
  offset++;
  if (offset%s == 0) offset=0;
}
GDL_NS_END
