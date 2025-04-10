#include "Language.hpp"
#include <map>

std::map<Language, std::string> enumName = {
  {Language::en, "en"},
  // ar
  {Language::be, "be"},
  {Language::bg, "bg"},
  {Language::ca, "ca"},
  {Language::cs, "cs"},
  {Language::da, "da"},
  {Language::de, "de"},
  {Language::el, "el"},
  {Language::es, "es"},
  // fa_ir
  {Language::fi, "fi"},
  // fil
  {Language::fr, "fr"},
  // he
  {Language::hr_hr, "hr_hr"},
  {Language::hu, "hu"},
  {Language::id, "id"},
  {Language::it, "it"},
  {Language::ja, "ja"},
  {Language::ko, "ko"},
  {Language::lt, "lt"},
  {Language::lv_lv, "lv_lv"},
  {Language::ms_my, "ms_my"},
  {Language::nl, "nl"},
  {Language::no, "no"},
  {Language::pl, "pl"},
  {Language::pt, "pt"},
  {Language::pt_br, "pt_br"},
  {Language::ro, "ro"},
  {Language::ru, "ru"},
  // si_lk
  {Language::sk, "sk"},
  {Language::sl, "sl"},
  {Language::sr, "sr"},
  {Language::sv, "sv"},
  // tg_tj
  {Language::th, "th"},
  //tl
  {Language::tr, "tr"},
  {Language::uk, "uk"},
  {Language::vi, "vi"},
  {Language::zh, "zh"},
  // zh_hk
  {Language::zh_hant, "zh_hant"},
};

std::map<Language, std::string> displayName = {
  {Language::en, "English"},
  {Language::be, "Беларуская мова"},
  {Language::bg, "Български"},
  {Language::ca, "Català"},
  {Language::cs, "Česky"},
  {Language::da, "Dansk"},
  {Language::de, "Deutsch"},
  {Language::el, "Ελληνικά"},
  {Language::es, "español"},
  // {Language::en, "فارسی"},
  {Language::fi, "Suomi"},
  // {Language::en, "Filipino"},
  {Language::fr, "français"},
  // {Language::en, "עברית"},
  {Language::hr_hr, "Hrvatski"},
  {Language::hu, "Magyar"},
  {Language::id, "Bahasa Indonesia"},
  {Language::it, "Italiano"},
  {Language::ja, "日本語"},
  {Language::ko, "한국어"},
  {Language::lt, "Lietuvių"},
  {Language::lv_lv, "Latviešu"},
  {Language::ms_my, "Melayu"},
  {Language::nl, "Nederlands"},
  {Language::no, "Norsk"},
  {Language::pl, "polski"},
  {Language::pt, "Português"},
  {Language::pt_br, "Português (Brasil)"},
  {Language::ro, "Română"},
  {Language::ru, "Русский"},
  // {Language::en, "සිංහල"},
  {Language::sk, "Slovenčina"},
  {Language::sl, "Slovenščina"},
  {Language::sr, "Српски"},
  {Language::sv, "Svenska"},
  // {Language::en, "Тоҷикӣ"},
  {Language::th, "ไทย"},
  //"Tagalog"
  {Language::tr, "Türkçe"},
  {Language::uk, "Українська мова"},
  {Language::vi, "Tiếng Việt"},
  {Language::zh, "简体中文"},
  // {Language::en, "繁體中文（香港）"},
  {Language::zh_hant, "繁體中文（台灣）"}
};

std::string getLanguageDisplayName(Language lang) {
  return displayName[lang];
}
std::string getLanguageEnumName(Language lang) {
  return enumName[lang];
}