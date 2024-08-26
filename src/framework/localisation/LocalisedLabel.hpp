#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include <Geode/loader/Event.hpp>
#include "LocalisationManager.hpp"
using namespace cocos2d;

class LocalisedLabel : public CCLabelTTF {
private:
    std::string key;
    std::string fallback;
    EventListener<EventFilter<LanguageUpdate>> langUpdateListener;
public:
    template<typename... Args>
    void updateString(Args... format) {
        setString(
            fmt::format(
                LocalisationManager::instance()->getKey(key, fallback).c_str(),
                format
            )
        );
    }
    static LocalisedLabel* create(std::string key, std::string fallback, std::string fileName) {
        LocalisedLabel* ret = new LocalisedLabel(); 
        if (ret && ret->initWithFile(fileName.c_str())) { 
            ret->key = key;
            ret->fallback = fallback;
            ret->langUpdateListener.bind([ret](LanguageUpdate* no){
                ret->updateString();
                return ListenerResult::Propagate;
            });
            ret->autorelease(); 
        } else { 
            do { if (ret) { (ret)->release(); (ret) = 0; } } while (0); 
        }; 
        return ret;
    }
};