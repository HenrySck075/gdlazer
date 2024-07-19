#include "CCNodeHover.hpp"

bool CCNodeHover::init() {
    m_listener = this->template addEventListener<MouseFilter>([this](MouseType type, CCPoint location) {
        if (type == MouseType::Enter) {
            this->onMouseEnter();
            return;
        }
        if (type == MouseType::Exit) {
            this->onMouseExit();
        }
    }, false);

    return true;
}
