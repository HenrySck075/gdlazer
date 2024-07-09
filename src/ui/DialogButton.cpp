#include "DialogButton.hpp"

void DialogButton::onMouseAccessState(MouseMoveType type, CCPoint location) {
	if (type == MouseMoveType::Enter) {
		this->onMouseEnter();
	}
	else {
		this->onMouseExit();
	}
};
