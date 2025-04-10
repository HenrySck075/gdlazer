#pragma once

#include <string>

/// tbf this sucks
/// @note comments are from the original code
enum class Language {
  en,
  // TODO: Requires Arabic glyphs to be added to resources (and possibly also RTL support).
  // [Description(@"اَلْعَرَبِيَّةُ")]
  // ar,
  be,
  bg,
  ca,
  cs,
  da,
  de,
  el,
  es,
  // TODO: Requires Arabic glyphs to be added to resources (and possibly also RTL support).
  // fa_ir,
  fi,
  // TODO: Doesn't work as appropriate satellite assemblies aren't copied from resources (see: https://github.com/ppy/osu/discussions/18851#discussioncomment-3042170)
  // fil,
  fr,
  // TODO: Requires Hebrew glyphs to be added to resources (and possibly also RTL support).
  // he,
  hr_hr,
  hu,
  id,
  it,
  ja,
  ko,
  lt,
  lv_lv,
  ms_my,
  nl,
  no,
  pl,
  pt,
  pt_br,
  ro,
  ru,
  // TODO: Requires Sinhala glyphs to be added to resources.
  // Additionally, no translations available yet.
  // si_lk,
  sk,
  sl,
  sr,
  sv,
  // Tajik has no associated localisations yet, and is not supported on Windows versions <10.
  // TODO: update language mapping in osu-resources to redirect tg-TJ to tg-Cyrl-TJ (which is supported on earlier Windows versions)
  // tg_tj,
  th,
  // Tagalog has no associated localisations yet, and is not supported on Xamarin platforms or Windows versions <10.
  // Can be revisited if localisations ever arrive.
  //tl,
  tr,
  uk,
  vi,
  zh,
  // Traditional Chinese (Hong Kong) is listed in web sources but has no associated localisations
  // and was wrongly falling back to Simplified Chinese.
  // Can be revisited if localisations ever arrive.
  // zh_hk,
  zh_hant,
};

std::string getLanguageDisplayName(Language lang);
std::string getLanguageEnumName(Language lang);