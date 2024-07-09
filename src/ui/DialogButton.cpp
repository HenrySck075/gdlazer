void DialogButton::setMouseListener()
{
    // Create a mouse listener
    mouseListener->onMouseMove = CC_CALLBACK_1(DialogButton::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}
