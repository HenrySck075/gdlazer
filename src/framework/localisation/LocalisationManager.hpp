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
  matjson::Value langdict;
  std::string language = "en";
public:
  static LocalisationManager* instance() {
    if (_instance==nullptr) {
      _instance = new LocalisationManager();
    }
    ""_spr;    
    return _instance;
  }
  void loadLocalisationStrings(matjson::Value& strings) {
    langdict = strings;
  }
  void setLanguage(std::string lang) {language = lang;}
  std::string getKey(std::string key, std::string fallback = "") {
    auto& ret = fallback;
    auto& t = langdict[key];
    if (!t.isNull()) {
      auto& t2 = t[language];
      auto r = t2.asString();
      ret = r.unwrapOr(fallback);
      if (r.isErr()) log::warn("[LocalisationManager]: Language {} for key {} isn't specified.", language,key);
    } 
    else log::warn("[LocalisationManager]: Key {} doesn't exist.", key);
    return ret;
  }
};