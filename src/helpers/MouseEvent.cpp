#include "MouseEvent.hpp"

ListenerResult MouseMoveFilter::handle(MiniFunction<MouseMoveFilter::Callback> fn, MouseMoveEvent* event) {
	if (m_target->boundingBox().containsPoint(event->getLocation())) {
		fn(event->getLocation());
		return m_keepPropangating?ListenerResult::Propagate:ListenerResult::Stop;
	}
	return ListenerResult::Propagate;
};
ListenerResult MouseEnterExitFilter::handle(MiniFunction<MouseEnterExitFilter::Callback> fn, MouseMoveEvent* event) {
	if (m_target->boundingBox().containsPoint(event->getLocation())) {
		if (!m_entered) {
			fn(MouseMoveType::Enter, event->getLocation());
			m_entered = true;
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	} else {
		if (m_entered) {
			fn(MouseMoveType::Exit, event->getLocation());
			m_entered = false;
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	}
	return ListenerResult::Propagate;
};


class $modify(CCEGLView) {
	void onGLFWMouseMoveCallback(GLFWwindow * window, double x, double y) {
		CCEGLView::onGLFWMouseMoveCallBack(window, x, y);
		MouseMoveEvent(CCPoint{ x,y }).post();
	};
};