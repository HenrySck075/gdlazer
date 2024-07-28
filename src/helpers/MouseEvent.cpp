#include "MouseEvent.hpp"


ListenerResult MouseFilter::handle(MiniFunction<MouseFilter::Callback> fn, MouseEvent* event) {
	auto mouseloc = event->getLocation();
	bool clicking = mouseloc.equals(ccp(-2, -2));
	if (!clicking) fn(MouseType::Move, mouseloc);
	if (clicking && m_entered) {
		fn(MouseType::Click, mouseloc);
		return ListenerResult::Propagate;
	}
	if (boundingBoxFromContentSize(m_target).containsPoint(mouseloc)) {
		if (!m_entered) {
			fn(MouseType::Enter, mouseloc);
			m_entered = true;
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	} else {
		if (m_entered) {
			fn(MouseType::Exit, mouseloc);
			m_entered = false;
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	}
	return ListenerResult::Propagate;
};

