#include "Event.hpp"
#include "../../utils.hpp"

GDI_NS_START
void SenderStack::push(frameworks::EventTarget* sender) {
  m_stack.push_back(sender);
}
void SenderStack::pop() {m_stack.pop_back();}

SenderStack::operator frameworks::EventTarget*() {return m_stack.back();}
frameworks::EventTarget* SenderStack::operator->() {return m_stack.back();}
GDI_NS_END

GDF_NS_START
void Event::stopPropagation() {
  m_propagateStopped = true;
};
void Event::stopImmediatePropagation() {
  stopPropagation();
  m_immediatePropagateStopped = true;
};
GDF_NS_END