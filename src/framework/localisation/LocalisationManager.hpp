#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
using namespace geode::prelude;

class LanguageUpdate : public Event {
  LanguageUpdate();
};

class LocalisationManager {
private:
  static LocalisationManager* _instance;
  matjson::Object langdict;
  std::string language = "en";
public:
  static LocalisationManager* instance() {
    if (_instance==nullptr) {
      _instance = new LocalisationManager();
    }
    ""_spr;    
    return _instance;
  }
  void loadLocalisationStrings(matjson::Object& strings) {
    langdict = strings;
  }
  void setLanguage(std::string lang) {language = lang;}
  std::string getKey(std::string key, std::string fallback = "") {
    auto ret = fallback;
    auto t = langdict[key];
    if (!t.is_null()) {
      auto t2 = t.as_object()[language];
      if (!t2.is_null()) ret = t2.as_string();
      else log::warn("[LocalisationManager]: Language {} for key {} isn't specified.", language,key);
    } else log::warn("[LocalisationManager]: Key {} doesn't exist.", key);
    return ret;
  }
};