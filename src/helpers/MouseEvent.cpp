#include "MouseEvent.hpp"


ListenerResult MouseFilter::handle(MiniFunction<MouseFilter::Callback> fn, MouseEvent* event) {
	auto mouseloc = event->getLocation();
	if (boundingBoxFromContentSize(m_target).containsPoint(mouseloc)) {
		if (!m_entered) {
			fn(MouseType::Enter, mouseloc);
			m_entered = true;
			log::debug("[MouseFilter]: Mouse entered.");
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	} else {
		if (m_entered) {
			fn(MouseType::Exit, mouseloc);
			m_entered = false;
			log::debug("[MouseFilter]: Mouse exited.");
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	}
	return ListenerResult::Propagate;
};

