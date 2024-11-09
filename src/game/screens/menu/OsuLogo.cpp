#include "OsuLogo.hpp"
#include "../../../framework/graphics/sprites/CCResizableSprite.hpp"
#include "../../../framework/utils/Interpolation.hpp"
#include <cmath>
#include <numeric>

float lin2dB(float linear)
{
  return -(clamp(log10(linear) * 20.0f, -80.f, 1.0f));
}
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
  float dominantVol = 0;
  float window = 2048;
  audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &dominantVol,nullptr,0);
  audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_WINDOWSIZE, &window,nullptr,0);
  // this will go wrong
  logoSprite->setScale(Interpolation::Damp(logoSprite->getScaleX(),1.f-std::max(0.f,dominantVol/window-0.4f)*0.04f,0.9,audio->getElapsed()));
}
/*
void OsuLogo::onBeat(float delta) {
  auto spr = static_cast<CCSprite*>(getChildByID("m"));
  spr->setScale(1);
  spr->runAction(CCScaleTo::create(0.5, 0.8));
}
*/

/// If 0.0024f feels to small then multiply it
const float LogoVisualization::bars_per_visualizer = 200;
static const float decay_per_millisecond = 0.0024f;
static const float time_between_updates = 50.f;
/// @brief amount of time passed
static float t = 0;
static int offset = 0;
static const int step = 5;
static float bar_length = 600;
bool LogoVisualization::init() {
  if (!Container::init()) return false;
  bar_length = processUnit(600, Unit::UIKit, false);
  setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
  scheduleUpdate();
  return true;
}
void LogoVisualization::update(float delta) {
  CCNode::update(delta);
  t+=delta;
  if (t<time_between_updates) return;
  float dominantVol = 0;
  float* spectrum;
  float window = 2048;
  auto logo = static_cast<OsuLogo*>(m_pParent);
  logo->audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &dominantVol,nullptr,0);
  logo->audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_SPECTRUMDATA, spectrum,nullptr,0);
  logo->audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_WINDOWSIZE, &window,nullptr,0);
  float dominantRatio = dominantVol/window;
  std::vector<int> v(spectrum, spectrum + sizeof spectrum / sizeof spectrum[0]);
  log::debug("{}",v);
  drawNode->setScale(logo->logoSprite->getScale());

  drawNode->clear();

  std::vector<CCPoint> lines;
  lines.reserve(bars_per_visualizer*2);
  for (int i = 0; i < bars_per_visualizer; i++) {
    float rot = degreeToRadius(i / bars_per_visualizer * 360);
    auto center = CCNode::getContentSize()/2;
    CCPoint start{center.width,CCNode::getContentSize().height};
    if (i % (int)step == 0) {
      bars[i] = bar_length*dominantRatio;
    } else {
      bars[i] -= decay_per_millisecond*t;
    }
    lines.push_back(ccpRotateByAngle(start,center,rot));
    lines.push_back(ccpRotateByAngle(start+CCPoint{0,bars[i]},center,rot));
  }
  drawNode->drawLines(lines.data(),bars_per_visualizer,20,{255,255,255,255*0.2});
  t = 0;
}