#include "CCNodeHover.hpp"

bool CCNodeHover::init() {
    m_listener = this->template addEventListener<MouseFilter>([this](MouseType type, CCPoint location) {
        if (!this->m_hoverEnabled) return;
        if (type == MouseType::Enter) {
            m_entered = true;
            this->onMouseEnter();
            return;
        }
        if (type == MouseType::Exit) {
            m_entered = false;
            this->onMouseExit();
        }
    }, true);

    return true;
}
