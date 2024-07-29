#include "MouseEvent.hpp"


ListenerResult MouseFilter::handle(MiniFunction<MouseFilter::Callback> fn, MouseEvent* event) {
	auto mouseloc = event->getLocation();
	bool clicking = mouseloc.equals(ccp(-2, -2));
	if (!clicking) fn(MouseType::Move, mouseloc);
	#define ret(type) m_keepPropangating && !fn(type, mouseloc) ? ListenerResult::Propagate : ListenerResult::Stop
	if (clicking && m_entered) {
		return ret(MouseType::Click);
	}
	if (boundingBoxFromContentSize(m_target).containsPoint(mouseloc)) {
		if (!m_entered) {
			m_entered = true;
			return ret(MouseType::Enter);
		}
	} else {
		if (m_entered) {
			m_entered = false;
			return ret(MouseType::Exit);
		}
	}
	#undef ret
	// all cases are handled so there's no way this will reach here
	return ListenerResult::Propagate;
};

