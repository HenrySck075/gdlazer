#include "MouseEvent.hpp"


ListenerResult MouseFilter::handle(MiniFunction<MouseFilter::Callback> fn, MouseEvent* event) {
	auto mouseloc = event->getLocation();
	fn(MouseType::Move, mouseloc);
	if (mouseloc.equals(ccp(-1, -1)) && m_entered) {
		fn(MouseType::Click, mouseloc);
		return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
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

