#include <Geode/Geode.hpp>
#include <random>

/// The "nothing useful feature": Replace the mod dev name (chance to hit the actual name is low so good luck).

static const char* c_smug[] = {
  /// Love Live! School idol project 
  // Muse
  "Honoka Kosaka",
  "Kotori Minami",
  "Umi Sonoda",
  "Rin Hoshizora",
  "Hanayo Koizumi",
  "Maki Nishikino",
  "Nico Yazawa",
  "Eli Ayase",
  "Nozomi Tojo",
  // A-RISE
  "Tsubasa Kira",
  "Anju Yuki",
  "Erena Todo",
  // Aqours
  "Chika Takami",
  "You Watanabe",
  "Riko Sakurauchi",
  "Ruby Kurosawa",
  "Hanamaru Kunikida",
  "Yoshiko Tsushima",
  "Mari Ohara",
  "Dia Kurosawa",
  "Kanan Matsuura",
  // Saint Snow
  "Sarah Kazuno",
  "Leah Kazuno",
  // Nijigasaki High School Idol Club
  "Yu Takasaki",
  "Ayumu Uehara",
  "Setsuna Yuki",
  "Ai Miyashita",
  "Kasumi Nakasu",
  "Shizuku Osaka",
  "Rina Tennoji",
  "Emma Verde",
  "Karin Asaka",
  "Kanata Konoe",
  "Shioriko Mifune",
  "Mia Taylor",
  "Lanzhu Zhong",
  "Kaoruko Mifune",
  //"Hana Hazuki",
  // Sunny Passion
  "Yuna Hijirisawa",
  "Mao Hiiragi",
  // Liella!
  "Kanon Shibuya",
  "Keke Tang",
  "Chisato Arashi",
  "Sumire Heanna",
  "Ren Hazuki",
  "Kinako Sakurakoji",
  "Mei Yoneme",
  "Shiki Wakana",
  "Natsumi Onitsuka",
  "Wien Margarete",
  "Tomari Onitsuka",
  // Hasunosora High School Idol Club
  "Kaho Hinoshita",
  "Kozue Otomune",
  "Sayaka Murano",
  "Tsuzuri Yugiri",
  "Rurino Osawa",
  "Megumi Fujishima",
  "Ceras Yanagida Lilienfeld",
  "Sachi Ogami",
  "Ginko Momose",
  "Kosuzu Kachimachi",
  "Hime Anyoji",
  // the name was too long  for my malfunctioning keyboard
  "Rurika Tsubaki",
  "Anzu Takizawa",
  "Yuzuha Sumeragi",
  "Misuzu Wakatsuki",
  "Yukino Hojo",
  "Toa Kurusu",
  "Hikaru Amakusa",
  "Rena Suzuka",
  "Maya Mikasa",
  "Sayaka Harukaze",
  "Izumi Katsuragi",
  // Izikurai-Bu!
  "Polka Takahashi",
  "Mai Azabu",
  "Akira Goto",
  "Hanabi Komagata",
  "Miracle Kanazawa",
  "Noriko Chofu",
  "Yukuri Harumiya",
  "Aurora Konohana",
  "Midori Yamada",
  "Shion Sasaki",

  /// BanG Dream!
  "Toyama Kasumi",
  "Hanazono Tae",
  "Ushigome Rimi",
  "Yamabuki Saaya",
  "Ichigaya Arisa",
  "Mitake Ran",
  "Aoba Moca",
  "Uehara Himari",
  "Udagawa Tomoe",
  "Hazawa Tsugumi",
  "Maruyama Aya",
  "Hikawa Hina",
  "Shirasagi Chisato",
  "Yamato Maya",
  "Wakamiya Eve",
  "Minato Yukina",
  "Hikawa Sayo",
  "Imai Lisa",
  "Udagawa Ako",
  "Shirokane Rinko",
  "Tsurumaki Kokoro",
  "Seta Kaoru",
  "Kitazawa Hagumi",
  "Matsubara Kanon",
  "Okusawa Misaki",
  "Kurata Mashiro",
  "Kirigaya Touko",
  "Hiromachi Nanami",
  "Futaba Tsukushi",
  "Yashio Rui",
  "Wakana Rei",
  "Satou Masuki",
  "Satou Masuki",
  "Nyubara Reona",
  "Tamade Chiyu",
  "Takamatsu Tomori",
  "Chihaya Anon",
  "Kaname Raana",
  "Nagasaki Soyo",
  "Shiina Taki",
  "Misumi Uika",
  "Wakaba Mutsumi",
  "Yahata Umiri",
  "Yuutenji Nyamu",
  "Togawa Sakiko",
  "Nakamachi Arale",
  "Miyanaga Nonoka",
  "Minetsuki Ritsu",
  "Sengoku Yuno",
  "Fuji Miyako",
  "Ushigome Yuri",
  "Uzawa Rii",
  "Wanibe Nanana",
  "Nijikki Hinako",
  "Umino Natsuki",
  "Taiko Satomi",
  "Kawabata Mayu",
  "Mori Fumika",
  "Sumita Mana",
  "Serizawa Mio",
  "Yamase Serina",
  "Koganei Shino",

  /// The iDOLM@STER
  "Aranzena Bassilbera",
  "Anastasia",
  "Arisa Mochida",
  "Chika Yokoyama",
  "Chinatsu Aikawa",
  "Clarice",
  "Erika Akanishi",
  "Fuka Asano",
  "Hasumi Nagatomi",
  "Hijiri Mochizuki",
  "Haruka Amami",
  "Mano Sakuragi",
  "Mai Fukuyama",
  "Makoto Kikuchi",
  "Kana Imai",
  "Chihaya Kisaragi",
  "Yayoi Takatsuki",
  "Miki Hoshii",
  "Yukiho Hagiwara",
  "Takane Shijou",
  "Nana Abe",
  "Azusa Miura",
  "Hibiki Ganaha",
  "Uzuki Shimamura",
  "Hiori Kazano",
  "Yukimi Sajo",
  "Mirai Kasuga",
  "Shizuka Mogami",
  "Tsubasa Ibuki",
  "Yuka Nakano",
  "Yukari Mizumoto",
  "Aiko Takamori",
  "Minami Nitta",
  "Kanako Mimura",
  "Riamu Yumemi",
  "Meguru Hachimiya",
  "Rin Shibuya",
  "Airi Totoki",
  "Mika Jougasaki",
  "Noriko Shiina",
  "Saori Okuyama",
  "Misato Manaka",
  "Miyuki Yanase",
  "Tsubaki Egami",
  "Nagisa Aino",
  "Yumi Aiba",
  "Miria Akagi",
  "Rika Jougasaki",
  "Nanami Asari",
  "Izumi Ohishi",
  "Toko Hattori",
  "Yukino Aihara",
  "Yusuke Aoi",
  "Kyosuke Aoi",
  "Suzaku Akai",

  /// D4DJ
  "Aimoto Rinku",
  "Akashi Maho",
  "Ohnaruto Muni",
  "Togetsu Rei",
  "Yamate Kyoko",
  "Inuyose Shinobu",
  "Sasago Jennifer Yuka",
  "Shimizu Esora",
  "Izumo Saki",
  "Niijima Ibuki",
  "Hanamaki Towa",
  "Fukushima Noa",
  "Seto Rika",
  "Mizushima Marika",
  "Hidaka Saori",
  "Matsuyama Dalia",
  "Aoyagi Tsubaki",
  "Tsukimiyama Nagisa",
  "Yano Hiiro",
  "Miyake Aoi",
  "Sakurada Miyu",
  "Kasuga Haruna",
  "Shiratori Kurumi",
  "Takeshita Miiko",
  "Amano Airi",
  "Kase Mana",
  "Himegami Shano",
  "Takao Toka",
  "Kaibara Michiru",
  "Ichihoshi Lumina",
  "Shinomiya Kokoa",
  "Tendo Hayate",
  "Neo",
  "Sophia",
  "Elsie",
  "Weronika",
  "Azai Eimi",
  "Bessyo Bell",
  "Shimazu Shika",
  "Date-chan",

  /// IDOLY-PRIDE
  "Ai Komiyama",
  "Aoi Igawa",
  "Chisa Shiraishi",
  "Franziska Yamada",
  "Haruko Saeki",
  "Kana Kojima",
  "Kokoro Akazaki",
  "Kotono Nagase",
  "Mana Nagase",
  "Mei Hayasaka",
  "Mihoko Takeda",
  "Nagisa Ibuki",
  "Rei Ichinose",
  "Rio Kanzaki",
  "Rui Tendo",
  "Saki Shiraishi",
  "Sakura Kawasaki",
  "Shizuku Hyodo",
  "Sumire Okuyama",
  "Suzu Narumiya",
  "Yu Suzumura",

  /// Lapis Re:LiGHTS
  "Tiara",
  "Rosetta",
  "Lavie",
  "Ashley",
  "Lynette",
  "Emilia",
  "Alpha",
  "Salsa",
  "Garnet",
  "Nadeshiko",
  "Tsubaki",
  "Kaede",
  "Ratura",
  "Champe",
  "Maryberry",
  "Angelica",
  "Lucifer",
  "Yue",
  "Millefeuille",
  "Fiona",
  "Eliza",
  "Chloe",
  "Angers",
  "Camilla",
  "Yuzuriha",

  /// Bocchi the Rock!
  "Hitori Gotoh",
  "Nijika Ijichi",
  "Ryo Yamada",
  "Ikuyo Kita",
  "Kikuri Hiroi",
  "Eliza Shimizu",
  "Shima Iwashita",
  "Yoyoko Ohtsuki",
  "Akubi Hasegawa",
  "Fuko Honjo",
  "Yuyu Uchida",
  "Seika Ijichi",
  "Alto Otogi", // PA-san
  "Nene Ohyama",
  "Elena Hinata",

  /// K-ON!!
  "Yui Hirasawa",
  "Mio Akiyama",
  "Ritsu Tainaka",
  "Tsumugi Kotobuki",
  "Azusa Nakano",

  /// Selection Project
  "Akari Amasawa",
  "Ao Yodogawa",
  "Hiromi Hamaguri",
  "Mako Toma",
  "Nagisa Imau",
  "Nodoka Yagi",
  "Rena Hananoi",
  "Seira Kurusu",
  "Shiori Yamaga",
  "Suzune Miyama",
  "Uta Koizumi",

  /// Wake Up, Girls!
  "Airi Hayashida",
  "Kaya Kikuma",
  "Mai Kondo",
  //"Makina X",
  "Mayu Shimada",
  "Megumi Yoshikawa",
  "Minami Katayama",
  "Miyu Okamoto",
  "Moka Suzuki",
  "Nanami Hisami",
  "Nanoka Aizawa",
  "Rena Suzuki",
  "Rika Takashina",
  "Shiho Iwasaki",
  "Tina Kobayakawa",
  "Yoshino Nanase",

  "Ayumi Hayashi",
  "Chiaki Sakurai",
  "Himari Mikoshiba",
  "Hina Ohishi",
  "Hitomi Ninomiya",
  "Itsuka Atsugi",
  "Mio Shirakawa",
  "Otome Morishima",
  "Sakura Sugihara",
  "Shiori Yuzusawa",
  "Suzuka Yagami",
  "Yukari Hirosawa",

  /// Girls Band Cry
  "Nina Iseri",
  "Tomo Ebizuka",
  "Momoka Kawaragi",
  "Subaru Awa",
  
  /// Jellyfish Can't Swim in the Night
  "Kano Yamanouchi",
  "Kiui Watase",
  "Mahiru Kozuki",
  "Mei Takanashi",
  "Mero Seto",
  "Shizue Baba",

  /// Ongaku Shoujo
  "Hanako Yamadagi",
  "Sasame Mitsukuri",
  "Miku Nishio",
  "Hiyo Yukino",
  "Shupe Gushiken",
  "Roro Morooka",
  "Kotoko Kintoki",
  "Uori Mukae",
  "Kiri Mukae",
  "Haru Chitose",
  "Eri Kumagai",
  "Sarasa Ryuoh",

  /// Hibike! Euphonium

  /// First years(?)
  // Flute and Piccolo
  "Yoriko Souga",
  "Kotoko Himegami",
  "Tsune Watanabe",
  "Sari Takahashi",
  "Kyouko Mihara",
  "Shirabe Inoue",
  "Meiko Oda",
  "Nozomi Kasaki",
  "Tsubomi Nakano",
  // Oboe and Bassoon (Double Reeds)
  "Mizore Yoroizuka",
  "Raina Kitamura",
  "Mikino Oka", 
  // Clarinet - E♭ Clarinet, B♭ Clarinet, and Bass Clarinet
  "Maina Kase",
  "Hirone Torizuka",
  "Rie Shima",
  "Hiyoko Ueda",
  "Sakiko Suzuka",
  "Shouko Hagiwara",
  "Youko Matsuzaki",
  "Sugami Tanaka",
  "Yumina Ooguchi",
  "Chieri Takahisa",
  "Hisae Takano",
  "Hitoshi Usui",
  "Junko Koshikawa",
  // Saxophone - Soprano, Alto, Tenor, Baritone
  "Kiriko Miya",
  "Raimu Okamoto",
  "Hiroe Hashi",
  "Shinobu Morita",
  "Chikai Maki",
  "Sumiko Hirao",
  "Aoi Saitou",
  "Chikao Takigawa",
  "Haruka Ogasawara",
  // Horn in F
  "Juri Sawada",
  "Hiro Kahashi",
  "Michiyo Morimoto",
  "Miru Kishibe",
  "Lala Hitomi",
  // Trumpet
  "Kaori Nakaseko",
  "Reina Kousaka",
  "Yuuko Yoshikawa",
  "Sana Kasano",
  "Junichi Takino",
  "Tomoe Kabe",
  "Akiko Yoshizawa",
  // Trombone
  "Hideri Noguchi",
  "Maki Akamatsu",
  "Mei Taura",
  "Shuuichi Tsukamoto",
  "Keina Iwata",
  "Sayaka Fukui",
  // Bass - Euphonium, Tuba, Contrabass
  "Asuka Tanaka",
  "Kumiko Oumae",
  "Natsuki Nakagawa",
  "Takuya Gotou",
  "Riko Nagase",
  "Hazuki Katou",
  "Sapphire Kawashima",
  // Percussion
  "Narai Tanabe",
  "Saki Kayama",
  "Miyoko Oono",
  "Junna Inoue",
  "Masako Sakai",
  "Tsubame Kamaya",

  /// Second years
  // Flute
  "Kana Etou",
  "Narumi Hiraishi",
  "Tsumiki Yamane",
  // Double Reeds
  "Ririka Kenzaki",
  "Eru Kabutodani",
  "Suruga Koteyama",
  // Clarinet
  "Maya Hashida",
  "Miki Katou",
  "Seiko Ashida",
  "Shiori Hiranuma",
  "Tairu Kitayama",
  "Taku Imura",
  "Ayako Sakasaki",
  // Saxophone
  "Haruna Hosono",
  "Kiri Matsumoto",
  "Nakaba Hattori",
  "Nanami Ootaki",
  "Seiya Suzuki",
  "Babe Uchida",
  "Machiko Nara",
  "Kyouko Ayukawa",
  "Tadashi Endou",
  // Horn in F
  "Aine Kohara",
  "Hibiki Tsuchiya",
  "Sanae Yashiki",
  "Sumiko Fukamachi",
  // Trumpet
  "Sayaka Takino",
  "Suguru Takami",
  "Tamari Asakura",
  "Yume Kohinata",
  // Trombone
  "Maiya Kikkawa",
  "Michiru Hakase",
  "Tomomi Hotei",
  // Bass
  "Kanade Hisaishi",
  "Mirei Suzuki",
  "Satsuki Suzuki",
  "Motomu Tsukinaga",
  // Percussion
  "Aota Maeda",
  "Michiko Machida",
  "Motoko Higashiura",
  "Rairi Hayashi",
  "Sousuke Maeda",
  "Une Kitada",

  /// Third years
  // Flute
  "Takumi Yoshida",
  "Hinata Mizui",
  // Double Reeds
  "Miku Kachisu",
  "Manako Hirao",
  // Clarinet
  "Sari Yoshii",
  "Eiko Matsufuji",
  "Yoshihito Kaida",
  "Ichigo Nakata",
  // Saxophone
  "Shion Kajiwara",
  "Sumika Kawaguchi",
  "Hitomi Koumoto",
  "Mashiro Mashima",
  // Horn in F
  "Yuki Takekawa",
  "Yoshino Miki",
  // Trumpet
  "Shuugo Matsutake",
  "Ruka Sakamoto",
  "Yukimi Takahashi",
  // Trombone
  "Kate Yamashita",
  "You Yoshizawa",
  "Yutaka Asano",
  // Bass
  "Mayu Kuroe",
  "Kaho Hariya",
  "Suzume Kamaya",
  "Yayoi Kamiishi",
  // Percussion
  "Kitsune Sutebe",
  "Asumi Sunaida",

  /// Rikka
  "Kanon Saijou",
  "Mion Saijou",
  "Momoka Koyama",
  "Mirai Sezaki",
  "Amika Nase",
  "Azusa Sasaki",
  "Shiho Togawa",
  "Taichi Matoba",
  /// END: Hibike! Euphonium
  

  /// Re:Stage!
  // KiRaRe
  "Mana Shikimiya",
  "Sayu Tsukisaka",
  "Mizuha Ichikishima",
  "Kae Hiiragi",
  "Kasumi Honjou",
  "Mii Hasegawa",
  // Ortensia
  "Yukari Itsumura",
  "Haruka Itsumura",
  // Stellamaris
  "Aone Shikimiya",
  "Ruka Ichijou",
  "Sango Misaki",
  // TROIS ANGES
  "Amaha Shiratori",
  "Kanade Hokaze",
  "Nagisa Himura",
  // Tetrarkhia
  "Mikuru Bandou",
  "Haku Nishidate",
  "Akari Haeno",
  "Kuroha Shirokita",
  // AsterReve
  "Yukari Itsumura",
  "Sono Mizushino",
  // TriumTone
  "Hina Asahi",
  "Miu Sakura",
  "Rinka Nakano",
  // Archouchou
  "Neon Sorano",
  "Chie Furumi",
  "Shiho Futaba",

  

  /// congrats
  "Henry Spheria", 
  "SoggyGunner"
};

#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
  if (!geode::Mod::get()->getSettingValue<bool>("dont")) return;
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1,sizeof(c_smug)/sizeof(c_smug[0])); 
  int i = dist(rng)-1;
  /*
  auto met = geode::Mod::get()->getMetadata();
  met.setDevelopers({c_smug[i]});
  geode::Mod::get()->setMetadata(met);
  */
  const auto thisModId = Mod::get()->getID();
  new EventListener<EventFilter<ModPopupUIEvent>>([thisModId](ModPopupUIEvent* event) {
    if (event->getModID() == thisModId) {
      // Find the Geode nodes you want to access first
      auto label = static_cast<CCLabelBMFont*>(
        event->getPopup()->querySelector("mod-developer-label")
      );

      // Only then do stuff with them
      if (label) {
        auto btn = CCMenuItemExt::createSprite(label, label, [](CCMenuItemSprite*){

        });
        //menu->addChild(Tooltip::create("hello"));
      }
    }
    return ListenerResult::Propagate;
  });
}
