#pragma once

#include <functional>
#include <vector>
#include <cstdint>

using FrameCallback = std::function<void(uint64_t timeStamp)>;

/// Scheduler phases for frame processing
enum class SchedulerPhase {
  idle,
  beginFrame,
  transientCallbacks,
  midFrameMicrotasks,
  persistentCallbacks,
  postFrameCallbacks,
};

/// Binds the scheduler to the framework.
/// Manages frame callbacks and the frame processing pipeline.
class SchedulerBinding {
protected:
  std::vector<FrameCallback> m_persistentCallbacks;
  std::vector<FrameCallback> m_postFrameCallbacks;
  SchedulerPhase m_schedulerPhase = SchedulerPhase::idle;
  uint64_t m_currentFrameTimeStamp = 0;

  virtual ~SchedulerBinding() = default;

public:
  /// Add a callback to be invoked every frame.
  void addPersistentFrameCallback(FrameCallback callback) {
    m_persistentCallbacks.push_back(callback);
  }

  /// Add a callback to be invoked after the current frame.
  void addPostFrameCallback(FrameCallback callback) {
    m_postFrameCallbacks.push_back(callback);
  }

  /// Called when a frame is beginning.
  /// Base implementation does nothing; subclasses override as needed.
  virtual void handleBeginFrame(uint64_t timeStamp) {
    m_currentFrameTimeStamp = timeStamp;
    m_schedulerPhase = SchedulerPhase::transientCallbacks;
  }

  /// Called to produce a new frame.
  /// Invokes persistent callbacks, then post-frame callbacks.
  virtual void handleDrawFrame() {
    if (m_schedulerPhase != SchedulerPhase::midFrameMicrotasks) {
      return;
    }

    try {
      // PERSISTENT FRAME CALLBACKS
      m_schedulerPhase = SchedulerPhase::persistentCallbacks;
      for (auto& callback : m_persistentCallbacks) {
        callback(m_currentFrameTimeStamp);
      }

      // POST-FRAME CALLBACKS
      m_schedulerPhase = SchedulerPhase::postFrameCallbacks;
      auto localPostFrameCallbacks = m_postFrameCallbacks;
      m_postFrameCallbacks.clear();
      for (auto& callback : localPostFrameCallbacks) {
        callback(m_currentFrameTimeStamp);
      }
    } catch (...) {
      // Silently ignore exceptions during frame processing
    }

    m_schedulerPhase = SchedulerPhase::idle;
  }

  SchedulerPhase getSchedulerPhase() const { return m_schedulerPhase; }
  void setSchedulerPhase(SchedulerPhase phase) { m_schedulerPhase = phase; }
};
