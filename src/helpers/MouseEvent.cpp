#include "MouseEvent.hpp"



ListenerResult MouseMoveFilter::handle(MiniFunction<MouseMoveFilter::Callback> fn, MouseMoveEvent* event) {
	if (m_target->boundingBox().containsPoint(m_target->convertToNodeSpace(event->getLocation()))) {
		fn(event->getLocation());
		return m_keepPropangating?ListenerResult::Propagate:ListenerResult::Stop;
	}
	return ListenerResult::Propagate;
};
ListenerResult MouseEnterExitFilter::handle(MiniFunction<MouseEnterExitFilter::Callback> fn, MouseMoveEvent* event) {
	auto mouseloc = m_target->convertToNodeSpace(event->getLocation());
	log::debug("[MouseEnterExitFilter]: {}", mouseloc);
	if (m_target->boundingBox().containsPoint(mouseloc)) {
		if (!m_entered) {
			fn(MouseMoveType::Enter, mouseloc);
			m_entered = true;
			log::debug("[MouseEnterExitFilter]: Mouse entered.");
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	} else {
		if (m_entered) {
			fn(MouseMoveType::Exit, mouseloc);
			m_entered = false;
			log::debug("[MouseEnterExitFilter]: Mouse exited.");
			return m_keepPropangating ? ListenerResult::Propagate : ListenerResult::Stop;
		}
	}
	return ListenerResult::Propagate;
};

