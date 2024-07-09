#include "MouseEvent.hpp"

ListenerResult MouseMoveFilter::handle(MiniFunction<MouseMoveFilter::Callback> fn, MouseMoveEvent* event) {
	if (m_target->boundingBox().containsPoint(event->getLocation())) {
		fn(event->getLocation());
		return m_keepPropangating?ListenerResult::Propagate:ListenerResult::Stop;
	}
	return ListenerResult::Propagate;
};
ListenerResult MouseEnterExitFilter::handle(MiniFunction<MouseEnterExitFilter::Callback> fn, MouseMoveEvent* event) {
	auto mouseloc = event->getLocation();
	if (m_target->boundingBox().containsPoint(mouseloc)) {
		if (!m_entered) {
			fn(MouseMoveType::Enter, mouseloc);
			m_entered = true;
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	} else {
		if (m_entered) {
			fn(MouseMoveType::Exit, mouseloc);
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