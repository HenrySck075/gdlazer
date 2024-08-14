#include "MouseEvent.hpp"

ListenerResult MouseFilter::handle(MiniFunction<MouseFilter::Callback> fn, MouseEventOld* event) {
    if (!m_target->isRunning()) return ListenerResult::Propagate;
    auto mouseloc = event->getLocation();
    bool clicking = mouseloc.equals(ccp(-2, -2));
    bool clickstart = mouseloc.equals(ccp(-4, -4));
    CCBool* awaitclick = static_cast<CCBool*>(m_target->getUserObject("clicking"_spr));
    if (!(clicking || clickstart)) {fn(MouseType::Move, mouseloc);}

    #define ret(type) fn(type, mouseloc) ? (m_keepPropagating ? ListenerResult::Propagate : ListenerResult::Stop) : ListenerResult::Propagate
    //  check if the mouse is clicking  && the node is awaiting mouseup event
    if (            clicking            && awaitclick) {
        ListenerResult c = ret(MouseType::MouseUp);
        if (m_entered) {
            c = ret(MouseType::Click);
        }
        awaitclick->setValue(false);
        return c;
    }
    if (clickstart && m_entered) {
        awaitclick->setValue(true);
        return ret(MouseType::MouseDown);
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
