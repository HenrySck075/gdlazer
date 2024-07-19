#include <Geode/loader/Event.hpp>

class BeatEvent : public geode::Event {
public:
  //Time since last beat
  float m_elapsed;
  BeatEvent(float elapsed) {
    m_elapsed = elapsed;
  };
};


