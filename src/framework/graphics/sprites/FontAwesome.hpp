// Modified from osu.Framework/Graphics/Sprites/FontAwesome.cs

#include "IconConstructor.hpp"
#include "../../../utils.hpp"

// theres no way one will use the entirety of this
namespace FontAwesome {

    namespace Brands
    {
        #define Get(icon) IconConstructor{icon, "FontAwesome-Brands.fnt"_spr}

        /// 500px
        static IconConstructor Icon500Px = Get(0xf26e);

        /// Accessible Icon
        static IconConstructor AccessibleIcon = Get(0xf368);

        /// Accusoft
        static IconConstructor Accusoft = Get(0xf369);

        /// Acquisitions Incorporated
        static IconConstructor AcquisitionsIncorporated = Get(0xf6af);

        /// App.net
        static IconConstructor Adn = Get(0xf170);

        /// Adobe
        static IconConstructor Adobe = Get(0xf778);

        /// Adversal
        static IconConstructor Adversal = Get(0xf36a);

        /// affiliatetheme
        static IconConstructor Affiliatetheme = Get(0xf36b);

        /// Airbnb
        static IconConstructor Airbnb = Get(0xf834);

        /// Algolia
        static IconConstructor Algolia = Get(0xf36c);

        /// Alipay
        static IconConstructor Alipay = Get(0xf642);

        /// Amazon
        static IconConstructor Amazon = Get(0xf270);

        /// Amazon Pay
        static IconConstructor AmazonPay = Get(0xf42c);

        /// Amilia
        static IconConstructor Amilia = Get(0xf36d);

        /// Android
        static IconConstructor Android = Get(0xf17b);

        /// AngelList
        static IconConstructor Angellist = Get(0xf209);

        /// Angry Creative
        static IconConstructor Angrycreative = Get(0xf36e);

        /// Angular
        static IconConstructor Angular = Get(0xf420);

        /// App Store
        static IconConstructor AppStore = Get(0xf36f);

        /// iOS App Store
        static IconConstructor AppStoreIos = Get(0xf370);

        /// Apper Systems AB
        static IconConstructor Apper = Get(0xf371);

        /// Apple
        static IconConstructor Apple = Get(0xf179);

        /// Apple Pay
        static IconConstructor ApplePay = Get(0xf415);

        /// Artstation
        static IconConstructor Artstation = Get(0xf77a);

        /// Asymmetrik, Ltd.
        static IconConstructor Asymmetrik = Get(0xf372);

        /// Atlassian
        static IconConstructor Atlassian = Get(0xf77b);

        /// Audible
        static IconConstructor Audible = Get(0xf373);

        /// Autoprefixer
        static IconConstructor Autoprefixer = Get(0xf41c);

        /// avianex
        static IconConstructor Avianex = Get(0xf374);

        /// Aviato
        static IconConstructor Aviato = Get(0xf421);

        /// Amazon Web Services (AWS)
        static IconConstructor Aws = Get(0xf375);

        /// Bandcamp
        static IconConstructor Bandcamp = Get(0xf2d5);

        /// Battle.net
        static IconConstructor BattleNet = Get(0xf835);

        /// Behance
        static IconConstructor Behance = Get(0xf1b4);

        /// Behance Square
        static IconConstructor BehanceSquare = Get(0xf1b5);

        /// BIMobject
        static IconConstructor Bimobject = Get(0xf378);

        /// Bitbucket
        static IconConstructor Bitbucket = Get(0xf171);

        /// Bitcoin
        static IconConstructor Bitcoin = Get(0xf379);

        /// Bity
        static IconConstructor Bity = Get(0xf37a);

        /// Font Awesome Black Tie
        static IconConstructor BlackTie = Get(0xf27e);

        /// BlackBerry
        static IconConstructor Blackberry = Get(0xf37b);

        /// Blogger
        static IconConstructor Blogger = Get(0xf37c);

        /// Blogger B
        static IconConstructor BloggerB = Get(0xf37d);

        /// Bluetooth
        static IconConstructor Bluetooth = Get(0xf293);

        /// Bluetooth
        static IconConstructor BluetoothB = Get(0xf294);

        /// Bootstrap
        static IconConstructor Bootstrap = Get(0xf836);

        /// BTC
        static IconConstructor Btc = Get(0xf15a);

        /// Buffer
        static IconConstructor Buffer = Get(0xf837);

        /// Büromöbel-Experte GmbH &amp; Co. KG.
        static IconConstructor Buromobelexperte = Get(0xf37f);

        /// BuySellAds
        static IconConstructor Buysellads = Get(0xf20d);

        /// Canadian Maple Leaf
        static IconConstructor CanadianMapleLeaf = Get(0xf785);

        /// Amazon Pay Credit Card
        static IconConstructor CcAmazonPay = Get(0xf42d);

        /// American Express Credit Card
        static IconConstructor CcAmex = Get(0xf1f3);

        /// Apple Pay Credit Card
        static IconConstructor CcApplePay = Get(0xf416);

        /// Diner's Club Credit Card
        static IconConstructor CcDinersClub = Get(0xf24c);

        /// Discover Credit Card
        static IconConstructor CcDiscover = Get(0xf1f2);

        /// JCB Credit Card
        static IconConstructor CcJcb = Get(0xf24b);

        /// MasterCard Credit Card
        static IconConstructor CcMastercard = Get(0xf1f1);

        /// Paypal Credit Card
        static IconConstructor CcPaypal = Get(0xf1f4);

        /// Stripe Credit Card
        static IconConstructor CcStripe = Get(0xf1f5);

        /// Visa Credit Card
        static IconConstructor CcVisa = Get(0xf1f0);

        /// Centercode
        static IconConstructor Centercode = Get(0xf380);

        /// Centos
        static IconConstructor Centos = Get(0xf789);

        /// Chrome
        static IconConstructor Chrome = Get(0xf268);

        /// Chromecast
        static IconConstructor Chromecast = Get(0xf838);

        /// cloudscale.ch
        static IconConstructor Cloudscale = Get(0xf383);

        /// Cloudsmith
        static IconConstructor Cloudsmith = Get(0xf384);

        /// cloudversify
        static IconConstructor Cloudversify = Get(0xf385);

        /// Codepen
        static IconConstructor Codepen = Get(0xf1cb);

        /// Codie Pie
        static IconConstructor Codiepie = Get(0xf284);

        /// Confluence
        static IconConstructor Confluence = Get(0xf78d);

        /// Connect Develop
        static IconConstructor Connectdevelop = Get(0xf20e);

        /// Contao
        static IconConstructor Contao = Get(0xf26d);

        /// cPanel
        static IconConstructor Cpanel = Get(0xf388);

        /// Creative Commons
        static IconConstructor CreativeCommons = Get(0xf25e);

        /// Creative Commons Attribution
        static IconConstructor CreativeCommonsBy = Get(0xf4e7);

        /// Creative Commons Noncommercial
        static IconConstructor CreativeCommonsNc = Get(0xf4e8);

        /// Creative Commons Noncommercial (Euro Sign)
        static IconConstructor CreativeCommonsNcEu = Get(0xf4e9);

        /// Creative Commons Noncommercial (Yen Sign)
        static IconConstructor CreativeCommonsNcJp = Get(0xf4ea);

        /// Creative Commons No Derivative Works
        static IconConstructor CreativeCommonsNd = Get(0xf4eb);

        /// Creative Commons Public Domain
        static IconConstructor CreativeCommonsPd = Get(0xf4ec);

        /// Alternate Creative Commons Public Domain
        static IconConstructor CreativeCommonsPdAlt = Get(0xf4ed);

        /// Creative Commons Remix
        static IconConstructor CreativeCommonsRemix = Get(0xf4ee);

        /// Creative Commons Share Alike
        static IconConstructor CreativeCommonsSa = Get(0xf4ef);

        /// Creative Commons Sampling
        static IconConstructor CreativeCommonsSampling = Get(0xf4f0);

        /// Creative Commons Sampling +
        static IconConstructor CreativeCommonsSamplingPlus = Get(0xf4f1);

        /// Creative Commons Share
        static IconConstructor CreativeCommonsShare = Get(0xf4f2);

        /// Creative Commons CC0
        static IconConstructor CreativeCommonsZero = Get(0xf4f3);

        /// Critical Role
        static IconConstructor CriticalRole = Get(0xf6c9);

        /// CSS 3 Logo
        static IconConstructor Css3 = Get(0xf13c);

        /// Alternate CSS3 Logo
        static IconConstructor Css3Alt = Get(0xf38b);

        /// Cuttlefish
        static IconConstructor Cuttlefish = Get(0xf38c);

        /// Dungeons &amp; Dragons
        static IconConstructor DAndD = Get(0xf38d);

        /// D&amp;D Beyond
        static IconConstructor DAndDBeyond = Get(0xf6ca);

        /// DashCube
        static IconConstructor Dashcube = Get(0xf210);

        /// Delicious
        static IconConstructor Delicious = Get(0xf1a5);

        /// deploy.dog
        static IconConstructor Deploydog = Get(0xf38e);

        /// Deskpro
        static IconConstructor Deskpro = Get(0xf38f);

        /// DEV
        static IconConstructor Dev = Get(0xf6cc);

        /// deviantART
        static IconConstructor Deviantart = Get(0xf1bd);

        /// DHL
        static IconConstructor Dhl = Get(0xf790);

        /// Diaspora
        static IconConstructor Diaspora = Get(0xf791);

        /// Digg Logo
        static IconConstructor Digg = Get(0xf1a6);

        /// Digital Ocean
        static IconConstructor DigitalOcean = Get(0xf391);

        /// Discord
        static IconConstructor Discord = Get(0xf392);

        /// Discourse
        static IconConstructor Discourse = Get(0xf393);

        /// DocHub
        static IconConstructor Dochub = Get(0xf394);

        /// Docker
        static IconConstructor Docker = Get(0xf395);

        /// Draft2digital
        static IconConstructor Draft2digital = Get(0xf396);

        /// Dribbble
        static IconConstructor Dribbble = Get(0xf17d);

        /// Dribbble Square
        static IconConstructor DribbbleSquare = Get(0xf397);

        /// Dropbox
        static IconConstructor Dropbox = Get(0xf16b);

        /// Drupal Logo
        static IconConstructor Drupal = Get(0xf1a9);

        /// Dyalog
        static IconConstructor Dyalog = Get(0xf399);

        /// Earlybirds
        static IconConstructor Earlybirds = Get(0xf39a);

        /// eBay
        static IconConstructor Ebay = Get(0xf4f4);

        /// Edge Browser
        static IconConstructor Edge = Get(0xf282);

        /// Elementor
        static IconConstructor Elementor = Get(0xf430);

        /// Ello
        static IconConstructor Ello = Get(0xf5f1);

        /// Ember
        static IconConstructor Ember = Get(0xf423);

        /// Galactic Empire
        static IconConstructor Empire = Get(0xf1d1);

        /// Envira Gallery
        static IconConstructor Envira = Get(0xf299);

        /// Erlang
        static IconConstructor Erlang = Get(0xf39d);

        /// Ethereum
        static IconConstructor Ethereum = Get(0xf42e);

        /// Etsy
        static IconConstructor Etsy = Get(0xf2d7);

        /// Evernote
        static IconConstructor Evernote = Get(0xf839);

        /// ExpeditedSSL
        static IconConstructor Expeditedssl = Get(0xf23e);

        /// Facebook
        static IconConstructor Facebook = Get(0xf09a);

        /// Facebook F
        static IconConstructor FacebookF = Get(0xf39e);

        /// Facebook Messenger
        static IconConstructor FacebookMessenger = Get(0xf39f);

        /// Facebook Square
        static IconConstructor FacebookSquare = Get(0xf082);

        /// Fantasy Flight-games
        static IconConstructor FantasyFlightGames = Get(0xf6dc);

        /// FedEx
        static IconConstructor Fedex = Get(0xf797);

        /// Fedora
        static IconConstructor Fedora = Get(0xf798);

        /// Figma
        static IconConstructor Figma = Get(0xf799);

        /// Firefox
        static IconConstructor Firefox = Get(0xf269);

        /// First Order
        static IconConstructor FirstOrder = Get(0xf2b0);

        /// Alternate First Order
        static IconConstructor FirstOrderAlt = Get(0xf50a);

        /// firstdraft
        static IconConstructor Firstdraft = Get(0xf3a1);

        /// Flickr
        static IconConstructor Flickr = Get(0xf16e);

        /// Flipboard
        static IconConstructor Flipboard = Get(0xf44d);

        /// Fly
        static IconConstructor Fly = Get(0xf417);

        /// Font Awesome
        static IconConstructor IconFontAwesome = Get(0xf2b4);

        /// Alternate Font Awesome
        static IconConstructor FontAwesomeAlt = Get(0xf35c);

        /// Font Awesome Flag
        static IconConstructor FontAwesomeFlag = Get(0xf425);

        /// Font Awesome Full Logo
        static IconConstructor FontAwesomeLogoFull = Get(0xf4e6);

        /// Fonticons
        static IconConstructor Fonticons = Get(0xf280);

        /// Fonticons Fi
        static IconConstructor FonticonsFi = Get(0xf3a2);

        /// Fort Awesome
        static IconConstructor FortAwesome = Get(0xf286);

        /// Alternate Fort Awesome
        static IconConstructor FortAwesomeAlt = Get(0xf3a3);

        /// Forumbee
        static IconConstructor Forumbee = Get(0xf211);

        /// Foursquare
        static IconConstructor Foursquare = Get(0xf180);

        /// Free Code Camp
        static IconConstructor FreeCodeCamp = Get(0xf2c5);

        /// FreeBSD
        static IconConstructor Freebsd = Get(0xf3a4);

        /// Fulcrum
        static IconConstructor Fulcrum = Get(0xf50b);

        /// Galactic Republic
        static IconConstructor GalacticRe= Get(0xf50c);

        /// Galactic Senate
        static IconConstructor GalacticSenate = Get(0xf50d);

        /// Get Pocket
        static IconConstructor GetPocket = Get(0xf265);

        /// GG Currency
        static IconConstructor Gg = Get(0xf260);

        /// GG Currency Circle
        static IconConstructor GgCircle = Get(0xf261);

        /// Git
        static IconConstructor Git = Get(0xf1d3);

        /// Git Square
        static IconConstructor GitSquare = Get(0xf1d2);

        /// GitHub
        static IconConstructor Github = Get(0xf09b);

        /// Alternate GitHub
        static IconConstructor GithubAlt = Get(0xf113);

        /// GitHub Square
        static IconConstructor GithubSquare = Get(0xf092);

        /// GitKraken
        static IconConstructor Gitkraken = Get(0xf3a6);

        /// GitLab
        static IconConstructor Gitlab = Get(0xf296);

        /// Gitter
        static IconConstructor Gitter = Get(0xf426);

        /// Glide
        static IconConstructor Glide = Get(0xf2a5);

        /// Glide G
        static IconConstructor GlideG = Get(0xf2a6);

        /// Gofore
        static IconConstructor Gofore = Get(0xf3a7);

        /// Goodreads
        static IconConstructor Goodreads = Get(0xf3a8);

        /// Goodreads G
        static IconConstructor GoodreadsG = Get(0xf3a9);

        /// Google Logo
        static IconConstructor Google = Get(0xf1a0);

        /// Google Drive
        static IconConstructor GoogleDrive = Get(0xf3aa);

        /// Google Play
        static IconConstructor GooglePlay = Get(0xf3ab);

        /// Google Plus
        static IconConstructor GooglePlus = Get(0xf2b3);

        /// Google Plus G
        static IconConstructor GooglePlusG = Get(0xf0d5);

        /// Google Plus Square
        static IconConstructor GooglePlusSquare = Get(0xf0d4);

        /// Google Wallet
        static IconConstructor GoogleWallet = Get(0xf1ee);

        /// Gratipay (Gittip)
        static IconConstructor Gratipay = Get(0xf184);

        /// Grav
        static IconConstructor Grav = Get(0xf2d6);

        /// Gripfire, Inc.
        static IconConstructor Gripfire = Get(0xf3ac);

        /// Grunt
        static IconConstructor Grunt = Get(0xf3ad);

        /// Gulp
        static IconConstructor Gulp = Get(0xf3ae);

        /// Hacker News
        static IconConstructor HackerNews = Get(0xf1d4);

        /// Hacker News Square
        static IconConstructor HackerNewsSquare = Get(0xf3af);

        /// Hackerrank
        static IconConstructor Hackerrank = Get(0xf5f7);

        /// Hips
        static IconConstructor Hips = Get(0xf452);

        /// HireAHelper
        static IconConstructor HireAHelper = Get(0xf3b0);

        /// Hooli
        static IconConstructor Hooli = Get(0xf427);

        /// Hornbill
        static IconConstructor Hornbill = Get(0xf592);

        /// Hotjar
        static IconConstructor Hotjar = Get(0xf3b1);

        /// Houzz
        static IconConstructor Houzz = Get(0xf27c);

        /// HTML 5 Logo
        static IconConstructor Html5 = Get(0xf13b);

        /// HubSpot
        static IconConstructor Hubspot = Get(0xf3b2);

        /// IMDB
        static IconConstructor Imdb = Get(0xf2d8);

        /// Instagram
        static IconConstructor Instagram = Get(0xf16d);

        /// Intercom
        static IconConstructor Intercom = Get(0xf7af);

        /// Internet-explorer
        static IconConstructor InternetExplorer = Get(0xf26b);

        /// InVision
        static IconConstructor Invision = Get(0xf7b0);

        /// ioxhost
        static IconConstructor Ioxhost = Get(0xf208);

        /// itch.io
        static IconConstructor ItchIo = Get(0xf83a);

        /// iTunes
        static IconConstructor Itunes = Get(0xf3b4);

        /// Itunes Note
        static IconConstructor ItunesNote = Get(0xf3b5);

        /// Java
        static IconConstructor Java = Get(0xf4e4);

        /// Jedi Order
        static IconConstructor JediOrder = Get(0xf50e);

        /// Jenkis
        static IconConstructor Jenkins = Get(0xf3b6);

        /// Jira
        static IconConstructor Jira = Get(0xf7b1);

        /// Joget
        static IconConstructor Joget = Get(0xf3b7);

        /// Joomla Logo
        static IconConstructor Joomla = Get(0xf1aa);

        /// JavaScript (JS)
        static IconConstructor Js = Get(0xf3b8);

        /// JavaScript (JS) Square
        static IconConstructor JsSquare = Get(0xf3b9);

        /// jsFiddle
        static IconConstructor Jsfiddle = Get(0xf1cc);

        /// Kaggle
        static IconConstructor Kaggle = Get(0xf5fa);

        /// Keybase
        static IconConstructor Keybase = Get(0xf4f5);

        /// KeyCDN
        static IconConstructor Keycdn = Get(0xf3ba);

        /// Kickstarter
        static IconConstructor Kickstarter = Get(0xf3bb);

        /// Kickstarter K
        static IconConstructor KickstarterK = Get(0xf3bc);

        /// KORVUE
        static IconConstructor Korvue = Get(0xf42f);

        /// Laravel
        static IconConstructor Laravel = Get(0xf3bd);

        /// last.fm
        static IconConstructor Lastfm = Get(0xf202);

        /// last.fm Square
        static IconConstructor LastfmSquare = Get(0xf203);

        /// Leanpub
        static IconConstructor Leanpub = Get(0xf212);

        /// Less
        static IconConstructor Less = Get(0xf41d);

        /// Line
        static IconConstructor Line = Get(0xf3c0);

        /// LinkedIn
        static IconConstructor Linkedin = Get(0xf08c);

        /// LinkedIn In
        static IconConstructor LinkedinIn = Get(0xf0e1);

        /// Linode
        static IconConstructor Linode = Get(0xf2b8);

        /// Linux
        static IconConstructor Linux = Get(0xf17c);

        /// lyft
        static IconConstructor Lyft = Get(0xf3c3);

        /// Magento
        static IconConstructor Magento = Get(0xf3c4);

        /// Mailchimp
        static IconConstructor Mailchimp = Get(0xf59e);

        /// Mandalorian
        static IconConstructor Mandalorian = Get(0xf50f);

        /// Markdown
        static IconConstructor Markdown = Get(0xf60f);

        /// Mastodon
        static IconConstructor Mastodon = Get(0xf4f6);

        /// MaxCDN
        static IconConstructor Maxcdn = Get(0xf136);

        /// MedApps
        static IconConstructor Medapps = Get(0xf3c6);

        /// Medium
        static IconConstructor Medium = Get(0xf23a);

        /// Medium M
        static IconConstructor MediumM = Get(0xf3c7);

        /// MRT
        static IconConstructor Medrt = Get(0xf3c8);

        /// Meetup
        static IconConstructor Meetup = Get(0xf2e0);

        /// Megaport
        static IconConstructor Megaport = Get(0xf5a3);

        /// Mendeley
        static IconConstructor Mendeley = Get(0xf7b3);

        /// Microsoft
        static IconConstructor Microsoft = Get(0xf3ca);

        /// Mix
        static IconConstructor Mix = Get(0xf3cb);

        /// Mixcloud
        static IconConstructor Mixcloud = Get(0xf289);

        /// Mizuni
        static IconConstructor Mizuni = Get(0xf3cc);

        /// MODX
        static IconConstructor Modx = Get(0xf285);

        /// Monero
        static IconConstructor Monero = Get(0xf3d0);

        /// Napster
        static IconConstructor Napster = Get(0xf3d2);

        /// Neos
        static IconConstructor Neos = Get(0xf612);

        /// Nimblr
        static IconConstructor Nimblr = Get(0xf5a8);

        /// Nintendo Switch
        static IconConstructor NintendoSwitch = Get(0xf418);

        /// Node.js
        static IconConstructor Node = Get(0xf419);

        /// Node.js JS
        static IconConstructor NodeJs = Get(0xf3d3);

        /// npm
        static IconConstructor Npm = Get(0xf3d4);

        /// NS8
        static IconConstructor Ns8 = Get(0xf3d5);

        /// Nutritionix
        static IconConstructor Nutritionix = Get(0xf3d6);

        /// Odnoklassniki
        static IconConstructor Odnoklassniki = Get(0xf263);

        /// Odnoklassniki Square
        static IconConstructor OdnoklassnikiSquare = Get(0xf264);

        /// Old Republic
        static IconConstructor OldRe= Get(0xf510);

        /// OpenCart
        static IconConstructor Opencart = Get(0xf23d);

        /// OpenID
        static IconConstructor Openid = Get(0xf19b);

        /// Opera
        static IconConstructor Opera = Get(0xf26a);

        /// Optin Monster
        static IconConstructor OptinMonster = Get(0xf23c);

        /// Open Source Initiative
        static IconConstructor Osi = Get(0xf41a);

        /// page4 Corporation
        static IconConstructor Page4 = Get(0xf3d7);

        /// Pagelines
        static IconConstructor Pagelines = Get(0xf18c);

        /// Palfed
        static IconConstructor Palfed = Get(0xf3d8);

        /// Patreon
        static IconConstructor Patreon = Get(0xf3d9);

        /// Paypal
        static IconConstructor Paypal = Get(0xf1ed);

        /// Penny Arcade
        static IconConstructor PennyArcade = Get(0xf704);

        /// Periscope
        static IconConstructor Periscope = Get(0xf3da);

        /// Phabricator
        static IconConstructor Phabricator = Get(0xf3db);

        /// Phoenix Framework
        static IconConstructor PhoenixFramework = Get(0xf3dc);

        /// Phoenix Squadron
        static IconConstructor PhoenixSquadron = Get(0xf511);

        /// PHP
        static IconConstructor Php = Get(0xf457);

        /// Pied Piper Logo
        static IconConstructor PiedPiper = Get(0xf2ae);

        /// Alternate Pied Piper Logo
        static IconConstructor PiedPiperAlt = Get(0xf1a8);

        /// Pied Piper-hat
        static IconConstructor PiedPiperHat = Get(0xf4e5);

        /// Pied Piper PP Logo (Old)
        static IconConstructor PiedPiperPp = Get(0xf1a7);

        /// Pinterest
        static IconConstructor Pinterest = Get(0xf0d2);

        /// Pinterest P
        static IconConstructor PinterestP = Get(0xf231);

        /// Pinterest Square
        static IconConstructor PinterestSquare = Get(0xf0d3);

        /// PlayStation
        static IconConstructor Playstation = Get(0xf3df);

        /// Product Hunt
        static IconConstructor ProductHunt = Get(0xf288);

        /// Pushed
        static IconConstructor Pushed = Get(0xf3e1);

        /// Python
        static IconConstructor Python = Get(0xf3e2);

        /// QQ
        static IconConstructor Qq = Get(0xf1d6);

        /// QuinScape
        static IconConstructor Quinscape = Get(0xf459);

        /// Quora
        static IconConstructor Quora = Get(0xf2c4);

        /// R Project
        static IconConstructor RProject = Get(0xf4f7);

        /// Raspberry Pi
        static IconConstructor RaspberryPi = Get(0xf7bb);

        /// Ravelry
        static IconConstructor Ravelry = Get(0xf2d9);

        /// React
        static IconConstructor React = Get(0xf41b);

        /// ReactEurope
        static IconConstructor Reacteurope = Get(0xf75d);

        /// ReadMe
        static IconConstructor Readme = Get(0xf4d5);

        /// Rebel Alliance
        static IconConstructor Rebel = Get(0xf1d0);

        /// red river
        static IconConstructor RedRiver = Get(0xf3e3);

        /// reddit Logo
        static IconConstructor Reddit = Get(0xf1a1);

        /// reddit Alien
        static IconConstructor RedditAlien = Get(0xf281);

        /// reddit Square
        static IconConstructor RedditSquare = Get(0xf1a2);

        /// Redhat
        static IconConstructor Redhat = Get(0xf7bc);

        /// Renren
        static IconConstructor Renren = Get(0xf18b);

        /// replyd
        static IconConstructor Replyd = Get(0xf3e6);

        /// Researchgate
        static IconConstructor Researchgate = Get(0xf4f8);

        /// Resolving
        static IconConstructor Resolving = Get(0xf3e7);

        /// Rev.io
        static IconConstructor Rev = Get(0xf5b2);

        /// Rocket.Chat
        static IconConstructor Rocketchat = Get(0xf3e8);

        /// Rockrms
        static IconConstructor Rockrms = Get(0xf3e9);

        /// Safari
        static IconConstructor Safari = Get(0xf267);

        /// Salesforce
        static IconConstructor Salesforce = Get(0xf83b);

        /// Sass
        static IconConstructor Sass = Get(0xf41e);

        /// SCHLIX
        static IconConstructor Schlix = Get(0xf3ea);

        /// Scribd
        static IconConstructor Scribd = Get(0xf28a);

        /// Searchengin
        static IconConstructor Searchengin = Get(0xf3eb);

        /// Sellcast
        static IconConstructor Sellcast = Get(0xf2da);

        /// Sellsy
        static IconConstructor Sellsy = Get(0xf213);

        /// Servicestack
        static IconConstructor Servicestack = Get(0xf3ec);

        /// Shirts in Bulk
        static IconConstructor Shirtsinbulk = Get(0xf214);

        /// Shopware
        static IconConstructor Shopware = Get(0xf5b5);

        /// SimplyBuilt
        static IconConstructor Simplybuilt = Get(0xf215);

        /// SISTRIX
        static IconConstructor Sistrix = Get(0xf3ee);

        /// Sith
        static IconConstructor Sith = Get(0xf512);

        /// Sketch
        static IconConstructor Sketch = Get(0xf7c6);

        /// skyatlas
        static IconConstructor Skyatlas = Get(0xf216);

        /// Skype
        static IconConstructor Skype = Get(0xf17e);

        /// Slack Logo
        static IconConstructor Slack = Get(0xf198);

        /// Slack Hashtag
        static IconConstructor SlackHash = Get(0xf3ef);

        /// Slideshare
        static IconConstructor Slideshare = Get(0xf1e7);

        /// Snapchat
        static IconConstructor Snapchat = Get(0xf2ab);

        /// Snapchat Ghost
        static IconConstructor SnapchatGhost = Get(0xf2ac);

        /// Snapchat Square
        static IconConstructor SnapchatSquare = Get(0xf2ad);

        /// SoundCloud
        static IconConstructor Soundcloud = Get(0xf1be);

        /// Sourcetree
        static IconConstructor Sourcetree = Get(0xf7d3);

        /// Speakap
        static IconConstructor Speakap = Get(0xf3f3);

        /// Speaker Deck
        static IconConstructor SpeakerDeck = Get(0xf83c);

        /// Spotify
        static IconConstructor Spotify = Get(0xf1bc);

        /// Squarespace
        static IconConstructor Squarespace = Get(0xf5be);

        /// Stack Exchange
        static IconConstructor StackExchange = Get(0xf18d);

        /// Stack Overflow
        static IconConstructor StackOverflow = Get(0xf16c);

        /// StayLinked
        static IconConstructor Staylinked = Get(0xf3f5);

        /// Steam
        static IconConstructor Steam = Get(0xf1b6);

        /// Steam Square
        static IconConstructor SteamSquare = Get(0xf1b7);

        /// Steam Symbol
        static IconConstructor SteamSymbol = Get(0xf3f6);

        /// Sticker Mule
        static IconConstructor StickerMule = Get(0xf3f7);

        /// Strava
        static IconConstructor Strava = Get(0xf428);

        /// Stripe
        static IconConstructor Stripe = Get(0xf429);

        /// Stripe S
        static IconConstructor StripeS = Get(0xf42a);

        /// Studio Vinari
        static IconConstructor Studiovinari = Get(0xf3f8);

        /// StumbleUpon Logo
        static IconConstructor Stumbleupon = Get(0xf1a4);

        /// StumbleUpon Circle
        static IconConstructor StumbleuponCircle = Get(0xf1a3);

        /// Superpowers
        static IconConstructor Superpowers = Get(0xf2dd);

        /// Supple
        static IconConstructor Supple = Get(0xf3f9);

        /// Suse
        static IconConstructor Suse = Get(0xf7d6);

        /// Symfony
        static IconConstructor Symfony = Get(0xf83d);

        /// TeamSpeak
        static IconConstructor Teamspeak = Get(0xf4f9);

        /// Telegram
        static IconConstructor Telegram = Get(0xf2c6);

        /// Telegram Plane
        static IconConstructor TelegramPlane = Get(0xf3fe);

        /// Tencent Weibo
        static IconConstructor TencentWeibo = Get(0xf1d5);

        /// The Red Yeti
        static IconConstructor TheRedYeti = Get(0xf69d);

        /// Themeco
        static IconConstructor Themeco = Get(0xf5c6);

        /// ThemeIsle
        static IconConstructor Themeisle = Get(0xf2b2);

        /// Think Peaks
        static IconConstructor ThinkPeaks = Get(0xf731);

        /// Trade Federation
        static IconConstructor TradeFederation = Get(0xf513);

        /// Trello
        static IconConstructor Trello = Get(0xf181);

        /// TripAdvisor
        static IconConstructor Tripadvisor = Get(0xf262);

        /// Tumblr
        static IconConstructor Tumblr = Get(0xf173);

        /// Tumblr Square
        static IconConstructor TumblrSquare = Get(0xf174);

        /// Twitch
        static IconConstructor Twitch = Get(0xf1e8);

        /// Twitter
        static IconConstructor Twitter = Get(0xf099);

        /// Twitter Square
        static IconConstructor TwitterSquare = Get(0xf081);

        /// Typo3
        static IconConstructor Typo3 = Get(0xf42b);

        /// Uber
        static IconConstructor Uber = Get(0xf402);

        /// Ubuntu
        static IconConstructor Ubuntu = Get(0xf7df);

        /// UIkit
        static IconConstructor Uikit = Get(0xf403);

        /// Uniregistry
        static IconConstructor Uniregistry = Get(0xf404);

        /// Untappd
        static IconConstructor Untappd = Get(0xf405);

        /// UPS
        static IconConstructor Ups = Get(0xf7e0);

        /// USB
        static IconConstructor Usb = Get(0xf287);

        /// United States Postal Service
        static IconConstructor Usps = Get(0xf7e1);

        /// us-Sunnah Foundation
        static IconConstructor Ussunnah = Get(0xf407);

        /// Vaadin
        static IconConstructor Vaadin = Get(0xf408);

        /// Viacoin
        static IconConstructor Viacoin = Get(0xf237);

        /// Video
        static IconConstructor Viadeo = Get(0xf2a9);

        /// Video Square
        static IconConstructor ViadeoSquare = Get(0xf2aa);

        /// Viber
        static IconConstructor Viber = Get(0xf409);

        /// Vimeo
        static IconConstructor Vimeo = Get(0xf40a);

        /// Vimeo Square
        static IconConstructor VimeoSquare = Get(0xf194);

        /// Vimeo
        static IconConstructor VimeoV = Get(0xf27d);

        /// Vine
        static IconConstructor Vine = Get(0xf1ca);

        /// VK
        static IconConstructor Vk = Get(0xf189);

        /// VNV
        static IconConstructor Vnv = Get(0xf40b);

        /// Vue.js
        static IconConstructor Vuejs = Get(0xf41f);

        /// Waze
        static IconConstructor Waze = Get(0xf83f);

        /// Weebly
        static IconConstructor Weebly = Get(0xf5cc);

        /// Weibo
        static IconConstructor Weibo = Get(0xf18a);

        /// Weixin (WeChat)
        static IconConstructor Weixin = Get(0xf1d7);

        /// What's App
        static IconConstructor Whatsapp = Get(0xf232);

        /// What's App Square
        static IconConstructor WhatsappSquare = Get(0xf40c);

        /// WHMCS
        static IconConstructor Whmcs = Get(0xf40d);

        /// Wikipedia W
        static IconConstructor WikipediaW = Get(0xf266);

        /// Windows
        static IconConstructor Windows = Get(0xf17a);

        /// Wix
        static IconConstructor Wix = Get(0xf5cf);

        /// Wizards of the Coast
        static IconConstructor WizardsOfTheCoast = Get(0xf730);

        /// Wolf Pack Battalion
        static IconConstructor WolfPackBattalion = Get(0xf514);

        /// WordPress Logo
        static IconConstructor Wordpress = Get(0xf19a);

        /// Wordpress Simple
        static IconConstructor WordpressSimple = Get(0xf411);

        /// WPBeginner
        static IconConstructor Wpbeginner = Get(0xf297);

        /// WPExplorer
        static IconConstructor Wpexplorer = Get(0xf2de);

        /// WPForms
        static IconConstructor Wpforms = Get(0xf298);

        /// wpressr
        static IconConstructor Wpressr = Get(0xf3e4);

        /// Xbox
        static IconConstructor Xbox = Get(0xf412);

        /// Xing
        static IconConstructor Xing = Get(0xf168);

        /// Xing Square
        static IconConstructor XingSquare = Get(0xf169);

        /// Y Combinator
        static IconConstructor YCombinator = Get(0xf23b);

        /// Yahoo Logo
        static IconConstructor Yahoo = Get(0xf19e);

        /// Yammer
        static IconConstructor Yammer = Get(0xf840);

        /// Yandex
        static IconConstructor Yandex = Get(0xf413);

        /// Yandex International
        static IconConstructor YandexInternational = Get(0xf414);

        /// Yarn
        static IconConstructor Yarn = Get(0xf7e3);

        /// Yelp
        static IconConstructor Yelp = Get(0xf1e9);

        /// Yoast
        static IconConstructor Yoast = Get(0xf2b1);

        /// YouTube
        static IconConstructor Youtube = Get(0xf167);

        /// YouTube Square
        static IconConstructor YoutubeSquare = Get(0xf431);

        /// Zhihu
        static IconConstructor Zhihu = Get(0xf63f);

        #undef Get
    }

    namespace Regular
    {
        #define Get(icon) IconConstructor{icon, "FontAwesome-Regular.fnt"_spr};

        /// Address Book
        static IconConstructor AddressBook = Get(0xf2b9);

        /// Address Card
        static IconConstructor AddressCard = Get(0xf2bb);

        /// Angry Face
        static IconConstructor Angry = Get(0xf556);

        /// Alternate Arrow Circle Down
        static IconConstructor ArrowAltCircleDown = Get(0xf358);

        /// Alternate Arrow Circle Left
        static IconConstructor ArrowAltCircleLeft = Get(0xf359);

        /// Alternate Arrow Circle Right
        static IconConstructor ArrowAltCircleRight = Get(0xf35a);

        /// Alternate Arrow Circle Up
        static IconConstructor ArrowAltCircleUp = Get(0xf35b);

        /// bell
        static IconConstructor Bell = Get(0xf0f3);

        /// Bell Slash
        static IconConstructor BellSlash = Get(0xf1f6);

        /// bookmark
        static IconConstructor Bookmark = Get(0xf02e);

        /// Building
        static IconConstructor Building = Get(0xf1ad);

        /// Calendar
        static IconConstructor Calendar = Get(0xf133);

        /// Alternate Calendar
        static IconConstructor CalendarAlt = Get(0xf073);

        /// Calendar Check
        static IconConstructor CalendarCheck = Get(0xf274);

        /// Calendar Minus
        static IconConstructor CalendarMinus = Get(0xf272);

        /// Calendar Plus
        static IconConstructor CalendarPlus = Get(0xf271);

        /// Calendar Times
        static IconConstructor CalendarTimes = Get(0xf273);

        /// Caret Square Down
        static IconConstructor CaretSquareDown = Get(0xf150);

        /// Caret Square Left
        static IconConstructor CaretSquareLeft = Get(0xf191);

        /// Caret Square Right
        static IconConstructor CaretSquareRight = Get(0xf152);

        /// Caret Square Up
        static IconConstructor CaretSquareUp = Get(0xf151);

        /// Bar Chart
        static IconConstructor ChartBar = Get(0xf080);

        /// Check Circle
        static IconConstructor CheckCircle = Get(0xf058);

        /// Check Square
        static IconConstructor CheckSquare = Get(0xf14a);

        /// Circle
        static IconConstructor Circle = Get(0xf111);

        /// Clipboard
        static IconConstructor Clipboard = Get(0xf328);

        /// Clock
        static IconConstructor Clock = Get(0xf017);

        /// Clone
        static IconConstructor Clone = Get(0xf24d);

        /// Closed Captioning
        static IconConstructor ClosedCaptioning = Get(0xf20a);

        /// comment
        static IconConstructor Comment = Get(0xf075);

        /// Alternate Comment
        static IconConstructor CommentAlt = Get(0xf27a);

        /// Comment Dots
        static IconConstructor CommentDots = Get(0xf4ad);

        /// comments
        static IconConstructor Comments = Get(0xf086);

        /// Compass
        static IconConstructor Compass = Get(0xf14e);

        /// Copy
        static IconConstructor Copy = Get(0xf0c5);

        /// Copyright
        static IconConstructor Copyright = Get(0xf1f9);

        /// Credit Card
        static IconConstructor CreditCard = Get(0xf09d);

        /// Dizzy Face
        static IconConstructor Dizzy = Get(0xf567);

        /// Dot Circle
        static IconConstructor DotCircle = Get(0xf192);

        /// Edit
        static IconConstructor Edit = Get(0xf044);

        /// Envelope
        static IconConstructor Envelope = Get(0xf0e0);

        /// Envelope Open
        static IconConstructor EnvelopeOpen = Get(0xf2b6);

        /// Eye
        static IconConstructor Eye = Get(0xf06e);

        /// Eye Slash
        static IconConstructor EyeSlash = Get(0xf070);

        /// File
        static IconConstructor File = Get(0xf15b);

        /// Alternate File
        static IconConstructor FileAlt = Get(0xf15c);

        /// Archive File
        static IconConstructor FileArchive = Get(0xf1c6);

        /// Audio File
        static IconConstructor FileAudio = Get(0xf1c7);

        /// Code File
        static IconConstructor FileCode = Get(0xf1c9);

        /// Excel File
        static IconConstructor FileExcel = Get(0xf1c3);

        /// Image File
        static IconConstructor FileImage = Get(0xf1c5);

        /// PDF File
        static IconConstructor FilePdf = Get(0xf1c1);

        /// Powerpoint File
        static IconConstructor FilePowerpoint = Get(0xf1c4);

        /// Video File
        static IconConstructor FileVideo = Get(0xf1c8);

        /// Word File
        static IconConstructor FileWord = Get(0xf1c2);

        /// flag
        static IconConstructor Flag = Get(0xf024);

        /// Flushed Face
        static IconConstructor Flushed = Get(0xf579);

        /// Folder
        static IconConstructor Folder = Get(0xf07b);

        /// Folder Open
        static IconConstructor FolderOpen = Get(0xf07c);

        /// Font Awesome Full Logo
        static IconConstructor FontAwesomeLogoFull = Get(0xf4e6);

        /// Frowning Face
        static IconConstructor Frown = Get(0xf119);

        /// Frowning Face With Open Mouth
        static IconConstructor FrownOpen = Get(0xf57a);

        /// Futbol
        static IconConstructor Futbol = Get(0xf1e3);

        /// Gem
        static IconConstructor Gem = Get(0xf3a5);

        /// Grimacing Face
        static IconConstructor Grimace = Get(0xf57f);

        /// Grinning Face
        static IconConstructor Grin = Get(0xf580);

        /// Alternate Grinning Face
        static IconConstructor GrinAlt = Get(0xf581);

        /// Grinning Face With Smiling Eyes
        static IconConstructor GrinBeam = Get(0xf582);

        /// Grinning Face With Sweat
        static IconConstructor GrinBeamSweat = Get(0xf583);

        /// Smiling Face With Heart-Eyes
        static IconConstructor GrinHearts = Get(0xf584);

        /// Grinning Squinting Face
        static IconConstructor GrinSquint = Get(0xf585);

        /// Rolling on the Floor Laughing
        static IconConstructor GrinSquintTears = Get(0xf586);

        /// Star-Struck
        static IconConstructor GrinStars = Get(0xf587);

        /// Face With Tears of Joy
        static IconConstructor GrinTears = Get(0xf588);

        /// Face With Tongue
        static IconConstructor GrinTongue = Get(0xf589);

        /// Squinting Face With Tongue
        static IconConstructor GrinTongueSquint = Get(0xf58a);

        /// Winking Face With Tongue
        static IconConstructor GrinTongueWink = Get(0xf58b);

        /// Grinning Winking Face
        static IconConstructor GrinWink = Get(0xf58c);

        /// Lizard (Hand)
        static IconConstructor HandLizard = Get(0xf258);

        /// Paper (Hand)
        static IconConstructor HandPaper = Get(0xf256);

        /// Peace (Hand)
        static IconConstructor HandPeace = Get(0xf25b);

        /// Hand Pointing Down
        static IconConstructor HandPointDown = Get(0xf0a7);

        /// Hand Pointing Left
        static IconConstructor HandPointLeft = Get(0xf0a5);

        /// Hand Pointing Right
        static IconConstructor HandPointRight = Get(0xf0a4);

        /// Hand Pointing Up
        static IconConstructor HandPointUp = Get(0xf0a6);

        /// Pointer (Hand)
        static IconConstructor HandPointer = Get(0xf25a);

        /// Rock (Hand)
        static IconConstructor HandRock = Get(0xf255);

        /// Scissors (Hand)
        static IconConstructor HandScissors = Get(0xf257);

        /// Spock (Hand)
        static IconConstructor HandSpock = Get(0xf259);

        /// Handshake
        static IconConstructor Handshake = Get(0xf2b5);

        /// HDD
        static IconConstructor Hdd = Get(0xf0a0);

        /// Heart
        static IconConstructor Heart = Get(0xf004);

        /// hospital
        static IconConstructor Hospital = Get(0xf0f8);

        /// Hourglass
        static IconConstructor Hourglass = Get(0xf254);

        /// Identification Badge
        static IconConstructor IdBadge = Get(0xf2c1);

        /// Identification Card
        static IconConstructor IdCard = Get(0xf2c2);

        /// Image
        static IconConstructor Image = Get(0xf03e);

        /// Images
        static IconConstructor Images = Get(0xf302);

        /// Keyboard
        static IconConstructor Keyboard = Get(0xf11c);

        /// Kissing Face
        static IconConstructor Kiss = Get(0xf596);

        /// Kissing Face With Smiling Eyes
        static IconConstructor KissBeam = Get(0xf597);

        /// Face Blowing a Kiss
        static IconConstructor KissWinkHeart = Get(0xf598);

        /// Grinning Face With Big Eyes
        static IconConstructor Laugh = Get(0xf599);

        /// Laugh Face with Beaming Eyes
        static IconConstructor LaughBeam = Get(0xf59a);

        /// Laughing Squinting Face
        static IconConstructor LaughSquint = Get(0xf59b);

        /// Laughing Winking Face
        static IconConstructor LaughWink = Get(0xf59c);

        /// Lemon
        static IconConstructor Lemon = Get(0xf094);

        /// Life Ring
        static IconConstructor LifeRing = Get(0xf1cd);

        /// Lightbulb
        static IconConstructor Lightbulb = Get(0xf0eb);

        /// Alternate List
        static IconConstructor ListAlt = Get(0xf022);

        /// Map
        static IconConstructor Map = Get(0xf279);

        /// Neutral Face
        static IconConstructor Meh = Get(0xf11a);

        /// Face Without Mouth
        static IconConstructor MehBlank = Get(0xf5a4);

        /// Face With Rolling Eyes
        static IconConstructor MehRollingEyes = Get(0xf5a5);

        /// Minus Square
        static IconConstructor MinusSquare = Get(0xf146);

        /// Alternate Money Bill
        static IconConstructor MoneyBillAlt = Get(0xf3d1);

        /// Moon
        static IconConstructor Moon = Get(0xf186);

        /// Newspaper
        static IconConstructor Newspaper = Get(0xf1ea);

        /// Object Group
        static IconConstructor ObjectGroup = Get(0xf247);

        /// Object Ungroup
        static IconConstructor ObjectUngroup = Get(0xf248);

        /// Paper Plane
        static IconConstructor PaperPlane = Get(0xf1d8);

        /// Pause Circle
        static IconConstructor PauseCircle = Get(0xf28b);

        /// Play Circle
        static IconConstructor PlayCircle = Get(0xf144);

        /// Plus Square
        static IconConstructor PlusSquare = Get(0xf0fe);

        /// Question Circle
        static IconConstructor QuestionCircle = Get(0xf059);

        /// Registered Trademark
        static IconConstructor Registered = Get(0xf25d);

        /// Crying Face
        static IconConstructor SadCry = Get(0xf5b3);

        /// Loudly Crying Face
        static IconConstructor SadTear = Get(0xf5b4);

        /// Save
        static IconConstructor Save = Get(0xf0c7);

        /// Share Square
        static IconConstructor ShareSquare = Get(0xf14d);

        /// Smiling Face
        static IconConstructor Smile = Get(0xf118);

        /// Beaming Face With Smiling Eyes
        static IconConstructor SmileBeam = Get(0xf5b8);

        /// Winking Face
        static IconConstructor SmileWink = Get(0xf4da);

        /// Snowflake
        static IconConstructor Snowflake = Get(0xf2dc);

        /// Square
        static IconConstructor Square = Get(0xf0c8);

        /// Star
        static IconConstructor Star = Get(0xf005);

        /// star-half
        static IconConstructor StarHalf = Get(0xf089);

        /// Sticky Note
        static IconConstructor StickyNote = Get(0xf249);

        /// Stop Circle
        static IconConstructor StopCircle = Get(0xf28d);

        /// Sun
        static IconConstructor Sun = Get(0xf185);

        /// Hushed Face
        static IconConstructor Surprise = Get(0xf5c2);

        /// thumbs-down
        static IconConstructor ThumbsDown = Get(0xf165);

        /// thumbs-up
        static IconConstructor ThumbsUp = Get(0xf164);

        /// Times Circle
        static IconConstructor TimesCircle = Get(0xf057);

        /// Tired Face
        static IconConstructor Tired = Get(0xf5c8);

        /// Alternate Trash
        static IconConstructor TrashAlt = Get(0xf2ed);

        /// User
        static IconConstructor User = Get(0xf007);

        /// User Circle
        static IconConstructor UserCircle = Get(0xf2bd);

        /// Window Close
        static IconConstructor WindowClose = Get(0xf410);

        /// Window Maximize
        static IconConstructor WindowMaximize = Get(0xf2d0);

        /// Window Minimize
        static IconConstructor WindowMinimize = Get(0xf2d1);

        /// Window Restore
        static IconConstructor WindowRestore = Get(0xf2d2);

        #undef Get
    }

    namespace Solid
    {
        #define Get(icon) IconConstructor{icon, "FontAwesome-Solid.fnt"_spr};

        /// Ad
        static IconConstructor Ad = Get(0xf641);

        /// Address Book
        static IconConstructor AddressBook = Get(0xf2b9);

        /// Address Card
        static IconConstructor AddressCard = Get(0xf2bb);

        /// adjust
        static IconConstructor Adjust = Get(0xf042);

        /// Air Freshener
        static IconConstructor AirFreshener = Get(0xf5d0);

        /// align-center
        static IconConstructor AlignCenter = Get(0xf037);

        /// align-justify
        static IconConstructor AlignJustify = Get(0xf039);

        /// align-left
        static IconConstructor AlignLeft = Get(0xf036);

        /// align-right
        static IconConstructor AlignRight = Get(0xf038);

        /// Allergies
        static IconConstructor Allergies = Get(0xf461);

        /// ambulance
        static IconConstructor Ambulance = Get(0xf0f9);

        /// American Sign Language Interpreting
        static IconConstructor AmericanSignLanguageInterpreting = Get(0xf2a3);

        /// Anchor
        static IconConstructor Anchor = Get(0xf13d);

        /// Angle Double Down
        static IconConstructor AngleDoubleDown = Get(0xf103);

        /// Angle Double Left
        static IconConstructor AngleDoubleLeft = Get(0xf100);

        /// Angle Double Right
        static IconConstructor AngleDoubleRight = Get(0xf101);

        /// Angle Double Up
        static IconConstructor AngleDoubleUp = Get(0xf102);

        /// angle-down
        static IconConstructor AngleDown = Get(0xf107);

        /// angle-left
        static IconConstructor AngleLeft = Get(0xf104);

        /// angle-right
        static IconConstructor AngleRight = Get(0xf105);

        /// angle-up
        static IconConstructor AngleUp = Get(0xf106);

        /// Angry Face
        static IconConstructor Angry = Get(0xf556);

        /// Ankh
        static IconConstructor Ankh = Get(0xf644);

        /// Fruit Apple
        static IconConstructor AppleAlt = Get(0xf5d1);

        /// Archive
        static IconConstructor Archive = Get(0xf187);

        /// Archway
        static IconConstructor Archway = Get(0xf557);

        /// Alternate Arrow Circle Down
        static IconConstructor ArrowAltCircleDown = Get(0xf358);

        /// Alternate Arrow Circle Left
        static IconConstructor ArrowAltCircleLeft = Get(0xf359);

        /// Alternate Arrow Circle Right
        static IconConstructor ArrowAltCircleRight = Get(0xf35a);

        /// Alternate Arrow Circle Up
        static IconConstructor ArrowAltCircleUp = Get(0xf35b);

        /// Arrow Circle Down
        static IconConstructor ArrowCircleDown = Get(0xf0ab);

        /// Arrow Circle Left
        static IconConstructor ArrowCircleLeft = Get(0xf0a8);

        /// Arrow Circle Right
        static IconConstructor ArrowCircleRight = Get(0xf0a9);

        /// Arrow Circle Up
        static IconConstructor ArrowCircleUp = Get(0xf0aa);

        /// arrow-down
        static IconConstructor ArrowDown = Get(0xf063);

        /// arrow-left
        static IconConstructor ArrowLeft = Get(0xf060);

        /// arrow-right
        static IconConstructor ArrowRight = Get(0xf061);

        /// arrow-up
        static IconConstructor ArrowUp = Get(0xf062);

        /// Alternate Arrows
        static IconConstructor ArrowsAlt = Get(0xf0b2);

        /// Alternate Arrows Horizontal
        static IconConstructor ArrowsAltH = Get(0xf337);

        /// Alternate Arrows Vertical
        static IconConstructor ArrowsAltV = Get(0xf338);

        /// Assistive Listening Systems
        static IconConstructor AssistiveListeningSystems = Get(0xf2a2);

        /// asterisk
        static IconConstructor Asterisk = Get(0xf069);

        /// At
        static IconConstructor At = Get(0xf1fa);

        /// Atlas
        static IconConstructor Atlas = Get(0xf558);

        /// Atom
        static IconConstructor Atom = Get(0xf5d2);

        /// Audio Description
        static IconConstructor AudioDescription = Get(0xf29e);

        /// Award
        static IconConstructor Award = Get(0xf559);

        /// Baby
        static IconConstructor Baby = Get(0xf77c);

        /// Baby Carriage
        static IconConstructor BabyCarriage = Get(0xf77d);

        /// Backspace
        static IconConstructor Backspace = Get(0xf55a);

        /// backward
        static IconConstructor Backward = Get(0xf04a);

        /// Bacon
        static IconConstructor Bacon = Get(0xf7e5);

        /// Balance Scale
        static IconConstructor BalanceScale = Get(0xf24e);

        /// ban
        static IconConstructor Ban = Get(0xf05e);

        /// Band-Aid
        static IconConstructor BandAid = Get(0xf462);

        /// barcode
        static IconConstructor Barcode = Get(0xf02a);

        /// Bars
        static IconConstructor Bars = Get(0xf0c9);

        /// Baseball Ball
        static IconConstructor BaseballBall = Get(0xf433);

        /// Basketball Ball
        static IconConstructor BasketballBall = Get(0xf434);

        /// Bath
        static IconConstructor Bath = Get(0xf2cd);

        /// Battery Empty
        static IconConstructor BatteryEmpty = Get(0xf244);

        /// Battery Full
        static IconConstructor BatteryFull = Get(0xf240);

        /// Battery 1/2 Full
        static IconConstructor BatteryHalf = Get(0xf242);

        /// Battery 1/4 Full
        static IconConstructor BatteryQuarter = Get(0xf243);

        /// Battery 3/4 Full
        static IconConstructor BatteryThreeQuarters = Get(0xf241);

        /// Bed
        static IconConstructor Bed = Get(0xf236);

        /// beer
        static IconConstructor Beer = Get(0xf0fc);

        /// bell
        static IconConstructor Bell = Get(0xf0f3);

        /// Bell Slash
        static IconConstructor BellSlash = Get(0xf1f6);

        /// Bezier Curve
        static IconConstructor BezierCurve = Get(0xf55b);

        /// Bible
        static IconConstructor Bible = Get(0xf647);

        /// Bicycle
        static IconConstructor Bicycle = Get(0xf206);

        /// Binoculars
        static IconConstructor Binoculars = Get(0xf1e5);

        /// Biohazard
        static IconConstructor Biohazard = Get(0xf780);

        /// Birthday Cake
        static IconConstructor BirthdayCake = Get(0xf1fd);

        /// Blender
        static IconConstructor Blender = Get(0xf517);

        /// Blender Phone
        static IconConstructor BlenderPhone = Get(0xf6b6);

        /// Blind
        static IconConstructor Blind = Get(0xf29d);

        /// Blog
        static IconConstructor Blog = Get(0xf781);

        /// bold
        static IconConstructor Bold = Get(0xf032);

        /// Lightning Bolt
        static IconConstructor Bolt = Get(0xf0e7);

        /// Bomb
        static IconConstructor Bomb = Get(0xf1e2);

        /// Bone
        static IconConstructor Bone = Get(0xf5d7);

        /// Bong
        static IconConstructor Bong = Get(0xf55c);

        /// book
        static IconConstructor Book = Get(0xf02d);

        /// Book of the Dead
        static IconConstructor BookDead = Get(0xf6b7);

        /// Medical Book
        static IconConstructor BookMedical = Get(0xf7e6);

        /// Book Open
        static IconConstructor BookOpen = Get(0xf518);

        /// Book Reader
        static IconConstructor BookReader = Get(0xf5da);

        /// bookmark
        static IconConstructor Bookmark = Get(0xf02e);

        /// Bowling Ball
        static IconConstructor BowlingBall = Get(0xf436);

        /// Box
        static IconConstructor Box = Get(0xf466);

        /// Box Open
        static IconConstructor BoxOpen = Get(0xf49e);

        /// Boxes
        static IconConstructor Boxes = Get(0xf468);

        /// Braille
        static IconConstructor Braille = Get(0xf2a1);

        /// Brain
        static IconConstructor Brain = Get(0xf5dc);

        /// Bread Slice
        static IconConstructor BreadSlice = Get(0xf7ec);

        /// Briefcase
        static IconConstructor Briefcase = Get(0xf0b1);

        /// Medical Briefcase
        static IconConstructor BriefcaseMedical = Get(0xf469);

        /// Broadcast Tower
        static IconConstructor BroadcastTower = Get(0xf519);

        /// Broom
        static IconConstructor Broom = Get(0xf51a);

        /// Brush
        static IconConstructor Brush = Get(0xf55d);

        /// Bug
        static IconConstructor Bug = Get(0xf188);

        /// Building
        static IconConstructor Building = Get(0xf1ad);

        /// bullhorn
        static IconConstructor Bullhorn = Get(0xf0a1);

        /// Bullseye
        static IconConstructor Bullseye = Get(0xf140);

        /// Burn
        static IconConstructor Burn = Get(0xf46a);

        /// Bus
        static IconConstructor Bus = Get(0xf207);

        /// Bus Alt
        static IconConstructor BusAlt = Get(0xf55e);

        /// Business Time
        static IconConstructor BusinessTime = Get(0xf64a);

        /// Calculator
        static IconConstructor Calculator = Get(0xf1ec);

        /// Calendar
        static IconConstructor Calendar = Get(0xf133);

        /// Alternate Calendar
        static IconConstructor CalendarAlt = Get(0xf073);

        /// Calendar Check
        static IconConstructor CalendarCheck = Get(0xf274);

        /// Calendar with Day Focus
        static IconConstructor CalendarDay = Get(0xf783);

        /// Calendar Minus
        static IconConstructor CalendarMinus = Get(0xf272);

        /// Calendar Plus
        static IconConstructor CalendarPlus = Get(0xf271);

        /// Calendar Times
        static IconConstructor CalendarTimes = Get(0xf273);

        /// Calendar with Week Focus
        static IconConstructor CalendarWeek = Get(0xf784);

        /// camera
        static IconConstructor Camera = Get(0xf030);

        /// Retro Camera
        static IconConstructor CameraRetro = Get(0xf083);

        /// Campground
        static IconConstructor Campground = Get(0xf6bb);

        /// Candy Cane
        static IconConstructor CandyCane = Get(0xf786);

        /// Cannabis
        static IconConstructor Cannabis = Get(0xf55f);

        /// Capsules
        static IconConstructor Capsules = Get(0xf46b);

        /// Car
        static IconConstructor Car = Get(0xf1b9);

        /// Alternate Car
        static IconConstructor CarAlt = Get(0xf5de);

        /// Car Battery
        static IconConstructor CarBattery = Get(0xf5df);

        /// Car Crash
        static IconConstructor CarCrash = Get(0xf5e1);

        /// Car Side
        static IconConstructor CarSide = Get(0xf5e4);

        /// Caret Down
        static IconConstructor CaretDown = Get(0xf0d7);

        /// Caret Left
        static IconConstructor CaretLeft = Get(0xf0d9);

        /// Caret Right
        static IconConstructor CaretRight = Get(0xf0da);

        /// Caret Square Down
        static IconConstructor CaretSquareDown = Get(0xf150);

        /// Caret Square Left
        static IconConstructor CaretSquareLeft = Get(0xf191);

        /// Caret Square Right
        static IconConstructor CaretSquareRight = Get(0xf152);

        /// Caret Square Up
        static IconConstructor CaretSquareUp = Get(0xf151);

        /// Caret Up
        static IconConstructor CaretUp = Get(0xf0d8);

        /// Carrot
        static IconConstructor Carrot = Get(0xf787);

        /// Shopping Cart Arrow Down
        static IconConstructor CartArrowDown = Get(0xf218);

        /// Add to Shopping Cart
        static IconConstructor CartPlus = Get(0xf217);

        /// Cash Register
        static IconConstructor CashRegister = Get(0xf788);

        /// Cat
        static IconConstructor Cat = Get(0xf6be);

        /// certificate
        static IconConstructor Certificate = Get(0xf0a3);

        /// Chair
        static IconConstructor Chair = Get(0xf6c0);

        /// Chalkboard
        static IconConstructor Chalkboard = Get(0xf51b);

        /// Chalkboard Teacher
        static IconConstructor ChalkboardTeacher = Get(0xf51c);

        /// Charging Station
        static IconConstructor ChargingStation = Get(0xf5e7);

        /// Area Chart
        static IconConstructor ChartArea = Get(0xf1fe);

        /// Bar Chart
        static IconConstructor ChartBar = Get(0xf080);

        /// Line Chart
        static IconConstructor ChartLine = Get(0xf201);

        /// Pie Chart
        static IconConstructor ChartPie = Get(0xf200);

        /// Check
        static IconConstructor Check = Get(0xf00c);

        /// Check Circle
        static IconConstructor CheckCircle = Get(0xf058);

        /// Check Double
        static IconConstructor CheckDouble = Get(0xf560);

        /// Check Square
        static IconConstructor CheckSquare = Get(0xf14a);

        /// Cheese
        static IconConstructor Cheese = Get(0xf7ef);

        /// Chess
        static IconConstructor Chess = Get(0xf439);

        /// Chess Bishop
        static IconConstructor ChessBishop = Get(0xf43a);

        /// Chess Board
        static IconConstructor ChessBoard = Get(0xf43c);

        /// Chess King
        static IconConstructor ChessKing = Get(0xf43f);

        /// Chess Knight
        static IconConstructor ChessKnight = Get(0xf441);

        /// Chess Pawn
        static IconConstructor ChessPawn = Get(0xf443);

        /// Chess Queen
        static IconConstructor ChessQueen = Get(0xf445);

        /// Chess Rook
        static IconConstructor ChessRook = Get(0xf447);

        /// Chevron Circle Down
        static IconConstructor ChevronCircleDown = Get(0xf13a);

        /// Chevron Circle Left
        static IconConstructor ChevronCircleLeft = Get(0xf137);

        /// Chevron Circle Right
        static IconConstructor ChevronCircleRight = Get(0xf138);

        /// Chevron Circle Up
        static IconConstructor ChevronCircleUp = Get(0xf139);

        /// chevron-down
        static IconConstructor ChevronDown = Get(0xf078);

        /// chevron-left
        static IconConstructor ChevronLeft = Get(0xf053);

        /// chevron-right
        static IconConstructor ChevronRight = Get(0xf054);

        /// chevron-up
        static IconConstructor ChevronUp = Get(0xf077);

        /// Child
        static IconConstructor Child = Get(0xf1ae);

        /// Church
        static IconConstructor Church = Get(0xf51d);

        /// Circle
        static IconConstructor Circle = Get(0xf111);

        /// Circle Notched
        static IconConstructor CircleNotch = Get(0xf1ce);

        /// City
        static IconConstructor City = Get(0xf64f);

        /// Medical Clinic
        static IconConstructor ClinicMedical = Get(0xf7f2);

        /// Clipboard
        static IconConstructor Clipboard = Get(0xf328);

        /// Clipboard with Check
        static IconConstructor ClipboardCheck = Get(0xf46c);

        /// Clipboard List
        static IconConstructor ClipboardList = Get(0xf46d);

        /// Clock
        static IconConstructor Clock = Get(0xf017);

        /// Clone
        static IconConstructor Clone = Get(0xf24d);

        /// Closed Captioning
        static IconConstructor ClosedCaptioning = Get(0xf20a);

        /// Cloud
        static IconConstructor Cloud = Get(0xf0c2);

        /// Alternate Cloud Download
        static IconConstructor CloudDownloadAlt = Get(0xf381);

        /// Cloud with (a chance of) Meatball
        static IconConstructor CloudMeatball = Get(0xf73b);

        /// Cloud with Moon
        static IconConstructor CloudMoon = Get(0xf6c3);

        /// Cloud with Moon and Rain
        static IconConstructor CloudMoonRain = Get(0xf73c);

        /// Cloud with Rain
        static IconConstructor CloudRain = Get(0xf73d);

        /// Cloud with Heavy Showers
        static IconConstructor CloudShowersHeavy = Get(0xf740);

        /// Cloud with Sun
        static IconConstructor CloudSun = Get(0xf6c4);

        /// Cloud with Sun and Rain
        static IconConstructor CloudSunRain = Get(0xf743);

        /// Alternate Cloud Upload
        static IconConstructor CloudUploadAlt = Get(0xf382);

        /// Cocktail
        static IconConstructor Cocktail = Get(0xf561);

        /// Code
        static IconConstructor Code = Get(0xf121);

        /// Code Branch
        static IconConstructor CodeBranch = Get(0xf126);

        /// Coffee
        static IconConstructor Coffee = Get(0xf0f4);

        /// cog
        static IconConstructor Cog = Get(0xf013);

        /// cogs
        static IconConstructor Cogs = Get(0xf085);

        /// Coins
        static IconConstructor Coins = Get(0xf51e);

        /// Columns
        static IconConstructor Columns = Get(0xf0db);

        /// comment
        static IconConstructor Comment = Get(0xf075);

        /// Alternate Comment
        static IconConstructor CommentAlt = Get(0xf27a);

        /// Comment Dollar
        static IconConstructor CommentDollar = Get(0xf651);

        /// Comment Dots
        static IconConstructor CommentDots = Get(0xf4ad);

        /// Alternate Medical Chat
        static IconConstructor CommentMedical = Get(0xf7f5);

        /// Comment Slash
        static IconConstructor CommentSlash = Get(0xf4b3);

        /// comments
        static IconConstructor Comments = Get(0xf086);

        /// Comments Dollar
        static IconConstructor CommentsDollar = Get(0xf653);

        /// Compact Disc
        static IconConstructor CompactDisc = Get(0xf51f);

        /// Compass
        static IconConstructor Compass = Get(0xf14e);

        /// Compress
        static IconConstructor Compress = Get(0xf066);

        /// Alternate Compress Arrows
        static IconConstructor CompressArrowsAlt = Get(0xf78c);

        /// Concierge Bell
        static IconConstructor ConciergeBell = Get(0xf562);

        /// Cookie
        static IconConstructor Cookie = Get(0xf563);

        /// Cookie Bite
        static IconConstructor CookieBite = Get(0xf564);

        /// Copy
        static IconConstructor Copy = Get(0xf0c5);

        /// Copyright
        static IconConstructor Copyright = Get(0xf1f9);

        /// Couch
        static IconConstructor Couch = Get(0xf4b8);

        /// Credit Card
        static IconConstructor CreditCard = Get(0xf09d);

        /// crop
        static IconConstructor Crop = Get(0xf125);

        /// Alternate Crop
        static IconConstructor CropAlt = Get(0xf565);

        /// Cross
        static IconConstructor Cross = Get(0xf654);

        /// Crosshairs
        static IconConstructor Crosshairs = Get(0xf05b);

        /// Crow
        static IconConstructor Crow = Get(0xf520);

        /// Crown
        static IconConstructor Crown = Get(0xf521);

        /// Crutch
        static IconConstructor Crutch = Get(0xf7f7);

        /// Cube
        static IconConstructor Cube = Get(0xf1b2);

        /// Cubes
        static IconConstructor Cubes = Get(0xf1b3);

        /// Cut
        static IconConstructor Cut = Get(0xf0c4);

        /// Database
        static IconConstructor Database = Get(0xf1c0);

        /// Deaf
        static IconConstructor Deaf = Get(0xf2a4);

        /// Democrat
        static IconConstructor Democrat = Get(0xf747);

        /// Desktop
        static IconConstructor Desktop = Get(0xf108);

        /// Dharmachakra
        static IconConstructor Dharmachakra = Get(0xf655);

        /// Diagnoses
        static IconConstructor Diagnoses = Get(0xf470);

        /// Dice
        static IconConstructor Dice = Get(0xf522);

        /// Dice D20
        static IconConstructor DiceD20 = Get(0xf6cf);

        /// Dice D6
        static IconConstructor DiceD6 = Get(0xf6d1);

        /// Dice Five
        static IconConstructor DiceFive = Get(0xf523);

        /// Dice Four
        static IconConstructor DiceFour = Get(0xf524);

        /// Dice One
        static IconConstructor DiceOne = Get(0xf525);

        /// Dice Six
        static IconConstructor DiceSix = Get(0xf526);

        /// Dice Three
        static IconConstructor DiceThree = Get(0xf527);

        /// Dice Two
        static IconConstructor DiceTwo = Get(0xf528);

        /// Digital Tachograph
        static IconConstructor DigitalTachograph = Get(0xf566);

        /// Directions
        static IconConstructor Directions = Get(0xf5eb);

        /// Divide
        static IconConstructor Divide = Get(0xf529);

        /// Dizzy Face
        static IconConstructor Dizzy = Get(0xf567);

        /// DNA
        static IconConstructor Dna = Get(0xf471);

        /// Dog
        static IconConstructor Dog = Get(0xf6d3);

        /// Dollar Sign
        static IconConstructor DollarSign = Get(0xf155);

        /// Dolly
        static IconConstructor Dolly = Get(0xf472);

        /// Dolly Flatbed
        static IconConstructor DollyFlatbed = Get(0xf474);

        /// Donate
        static IconConstructor Donate = Get(0xf4b9);

        /// Door Closed
        static IconConstructor DoorClosed = Get(0xf52a);

        /// Door Open
        static IconConstructor DoorOpen = Get(0xf52b);

        /// Dot Circle
        static IconConstructor DotCircle = Get(0xf192);

        /// Dove
        static IconConstructor Dove = Get(0xf4ba);

        /// Download
        static IconConstructor Download = Get(0xf019);

        /// Drafting Compass
        static IconConstructor DraftingCompass = Get(0xf568);

        /// Dragon
        static IconConstructor Dragon = Get(0xf6d5);

        /// Draw Polygon
        static IconConstructor DrawPolygon = Get(0xf5ee);

        /// Drum
        static IconConstructor Drum = Get(0xf569);

        /// Drum Steelpan
        static IconConstructor DrumSteelpan = Get(0xf56a);

        /// Drumstick with Bite Taken Out
        static IconConstructor DrumstickBite = Get(0xf6d7);

        /// Dumbbell
        static IconConstructor Dumbbell = Get(0xf44b);

        /// Dumpster
        static IconConstructor Dumpster = Get(0xf793);

        /// Dumpster Fire
        static IconConstructor DumpsterFire = Get(0xf794);

        /// Dungeon
        static IconConstructor Dungeon = Get(0xf6d9);

        /// Edit
        static IconConstructor Edit = Get(0xf044);

        /// Egg
        static IconConstructor Egg = Get(0xf7fb);

        /// eject
        static IconConstructor Eject = Get(0xf052);

        /// Horizontal Ellipsis
        static IconConstructor EllipsisH = Get(0xf141);

        /// Vertical Ellipsis
        static IconConstructor EllipsisV = Get(0xf142);

        /// Envelope
        static IconConstructor Envelope = Get(0xf0e0);

        /// Envelope Open
        static IconConstructor EnvelopeOpen = Get(0xf2b6);

        /// Envelope Open-text
        static IconConstructor EnvelopeOpenText = Get(0xf658);

        /// Envelope Square
        static IconConstructor EnvelopeSquare = Get(0xf199);

        /// Equals
        static IconConstructor Equals = Get(0xf52c);

        /// eraser
        static IconConstructor Eraser = Get(0xf12d);

        /// Ethernet
        static IconConstructor Ethernet = Get(0xf796);

        /// Euro Sign
        static IconConstructor EuroSign = Get(0xf153);

        /// Alternate Exchange
        static IconConstructor ExchangeAlt = Get(0xf362);

        /// exclamation
        static IconConstructor Exclamation = Get(0xf12a);

        /// Exclamation Circle
        static IconConstructor ExclamationCircle = Get(0xf06a);

        /// Exclamation Triangle
        static IconConstructor ExclamationTriangle = Get(0xf071);

        /// Expand
        static IconConstructor Expand = Get(0xf065);

        /// Alternate Expand Arrows
        static IconConstructor ExpandArrowsAlt = Get(0xf31e);

        /// Alternate External Link
        static IconConstructor ExternalLinkAlt = Get(0xf35d);

        /// Alternate External Link Square
        static IconConstructor ExternalLinkSquareAlt = Get(0xf360);

        /// Eye
        static IconConstructor Eye = Get(0xf06e);

        /// Eye Dropper
        static IconConstructor EyeDropper = Get(0xf1fb);

        /// Eye Slash
        static IconConstructor EyeSlash = Get(0xf070);

        /// fast-backward
        static IconConstructor FastBackward = Get(0xf049);

        /// fast-forward
        static IconConstructor FastForward = Get(0xf050);

        /// Fax
        static IconConstructor Fax = Get(0xf1ac);

        /// Feather
        static IconConstructor Feather = Get(0xf52d);

        /// Alternate Feather
        static IconConstructor FeatherAlt = Get(0xf56b);

        /// Female
        static IconConstructor Female = Get(0xf182);

        /// fighter-jet
        static IconConstructor FighterJet = Get(0xf0fb);

        /// File
        static IconConstructor File = Get(0xf15b);

        /// Alternate File
        static IconConstructor FileAlt = Get(0xf15c);

        /// Archive File
        static IconConstructor FileArchive = Get(0xf1c6);

        /// Audio File
        static IconConstructor FileAudio = Get(0xf1c7);

        /// Code File
        static IconConstructor FileCode = Get(0xf1c9);

        /// File Contract
        static IconConstructor FileContract = Get(0xf56c);

        /// File CSV
        static IconConstructor FileCsv = Get(0xf6dd);

        /// File Download
        static IconConstructor FileDownload = Get(0xf56d);

        /// Excel File
        static IconConstructor FileExcel = Get(0xf1c3);

        /// File Export
        static IconConstructor FileExport = Get(0xf56e);

        /// Image File
        static IconConstructor FileImage = Get(0xf1c5);

        /// File Import
        static IconConstructor FileImport = Get(0xf56f);

        /// File Invoice
        static IconConstructor FileInvoice = Get(0xf570);

        /// File Invoice with US Dollar
        static IconConstructor FileInvoiceDollar = Get(0xf571);

        /// Medical File
        static IconConstructor FileMedical = Get(0xf477);

        /// Alternate Medical File
        static IconConstructor FileMedicalAlt = Get(0xf478);

        /// PDF File
        static IconConstructor FilePdf = Get(0xf1c1);

        /// Powerpoint File
        static IconConstructor FilePowerpoint = Get(0xf1c4);

        /// File Prescription
        static IconConstructor FilePrescription = Get(0xf572);

        /// File Signature
        static IconConstructor FileSignature = Get(0xf573);

        /// File Upload
        static IconConstructor FileUpload = Get(0xf574);

        /// Video File
        static IconConstructor FileVideo = Get(0xf1c8);

        /// Word File
        static IconConstructor FileWord = Get(0xf1c2);

        /// Fill
        static IconConstructor Fill = Get(0xf575);

        /// Fill Drip
        static IconConstructor FillDrip = Get(0xf576);

        /// Film
        static IconConstructor Film = Get(0xf008);

        /// Filter
        static IconConstructor Filter = Get(0xf0b0);

        /// Fingerprint
        static IconConstructor Fingerprint = Get(0xf577);

        /// fire
        static IconConstructor Fire = Get(0xf06d);

        /// Alternate Fire
        static IconConstructor FireAlt = Get(0xf7e4);

        /// fire-extinguisher
        static IconConstructor FireExtinguisher = Get(0xf134);

        /// First Aid
        static IconConstructor FirstAid = Get(0xf479);

        /// Fish
        static IconConstructor Fish = Get(0xf578);

        /// Raised Fist
        static IconConstructor FistRaised = Get(0xf6de);

        /// flag
        static IconConstructor Flag = Get(0xf024);

        /// flag-checkered
        static IconConstructor FlagCheckered = Get(0xf11e);

        /// United States of America Flag
        static IconConstructor FlagUsa = Get(0xf74d);

        /// Flask
        static IconConstructor Flask = Get(0xf0c3);

        /// Flushed Face
        static IconConstructor Flushed = Get(0xf579);

        /// Folder
        static IconConstructor Folder = Get(0xf07b);

        /// Folder Minus
        static IconConstructor FolderMinus = Get(0xf65d);

        /// Folder Open
        static IconConstructor FolderOpen = Get(0xf07c);

        /// Folder Plus
        static IconConstructor FolderPlus = Get(0xf65e);

        /// font
        static IconConstructor Font = Get(0xf031);

        /// Font Awesome Full Logo
        static IconConstructor FontAwesomeLogoFull = Get(0xf4e6);

        /// Football Ball
        static IconConstructor FootballBall = Get(0xf44e);

        /// forward
        static IconConstructor Forward = Get(0xf04e);

        /// Frog
        static IconConstructor Frog = Get(0xf52e);

        /// Frowning Face
        static IconConstructor Frown = Get(0xf119);

        /// Frowning Face With Open Mouth
        static IconConstructor FrownOpen = Get(0xf57a);

        /// Funnel Dollar
        static IconConstructor FunnelDollar = Get(0xf662);

        /// Futbol
        static IconConstructor Futbol = Get(0xf1e3);

        /// Gamepad
        static IconConstructor Gamepad = Get(0xf11b);

        /// Gas Pump
        static IconConstructor GasPump = Get(0xf52f);

        /// Gavel
        static IconConstructor Gavel = Get(0xf0e3);

        /// Gem
        static IconConstructor Gem = Get(0xf3a5);

        /// Genderless
        static IconConstructor Genderless = Get(0xf22d);

        /// Ghost
        static IconConstructor Ghost = Get(0xf6e2);

        /// gift
        static IconConstructor Gift = Get(0xf06b);

        /// Gifts
        static IconConstructor Gifts = Get(0xf79c);

        /// Glass Cheers
        static IconConstructor GlassCheers = Get(0xf79f);

        /// Martini Glass
        static IconConstructor GlassMartini = Get(0xf000);

        /// Alternate Glass Martini
        static IconConstructor GlassMartiniAlt = Get(0xf57b);

        /// Glass Whiskey
        static IconConstructor GlassWhiskey = Get(0xf7a0);

        /// Glasses
        static IconConstructor Glasses = Get(0xf530);

        /// Globe
        static IconConstructor Globe = Get(0xf0ac);

        /// Globe with Africa shown
        static IconConstructor GlobeAfrica = Get(0xf57c);

        /// Globe with Americas shown
        static IconConstructor GlobeAmericas = Get(0xf57d);

        /// Globe with Asia shown
        static IconConstructor GlobeAsia = Get(0xf57e);

        /// Globe with Europe shown
        static IconConstructor GlobeEurope = Get(0xf7a2);

        /// Golf Ball
        static IconConstructor GolfBall = Get(0xf450);

        /// Gopuram
        static IconConstructor Gopuram = Get(0xf664);

        /// Graduation Cap
        static IconConstructor GraduationCap = Get(0xf19d);

        /// Greater Than
        static IconConstructor GreaterThan = Get(0xf531);

        /// Greater Than Equal To
        static IconConstructor GreaterThanEqual = Get(0xf532);

        /// Grimacing Face
        static IconConstructor Grimace = Get(0xf57f);

        /// Grinning Face
        static IconConstructor Grin = Get(0xf580);

        /// Alternate Grinning Face
        static IconConstructor GrinAlt = Get(0xf581);

        /// Grinning Face With Smiling Eyes
        static IconConstructor GrinBeam = Get(0xf582);

        /// Grinning Face With Sweat
        static IconConstructor GrinBeamSweat = Get(0xf583);

        /// Smiling Face With Heart-Eyes
        static IconConstructor GrinHearts = Get(0xf584);

        /// Grinning Squinting Face
        static IconConstructor GrinSquint = Get(0xf585);

        /// Rolling on the Floor Laughing
        static IconConstructor GrinSquintTears = Get(0xf586);

        /// Star-Struck
        static IconConstructor GrinStars = Get(0xf587);

        /// Face With Tears of Joy
        static IconConstructor GrinTears = Get(0xf588);

        /// Face With Tongue
        static IconConstructor GrinTongue = Get(0xf589);

        /// Squinting Face With Tongue
        static IconConstructor GrinTongueSquint = Get(0xf58a);

        /// Winking Face With Tongue
        static IconConstructor GrinTongueWink = Get(0xf58b);

        /// Grinning Winking Face
        static IconConstructor GrinWink = Get(0xf58c);

        /// Grip Horizontal
        static IconConstructor GripHorizontal = Get(0xf58d);

        /// Grip Lines
        static IconConstructor GripLines = Get(0xf7a4);

        /// Grip Lines Vertical
        static IconConstructor GripLinesVertical = Get(0xf7a5);

        /// Grip Vertical
        static IconConstructor GripVertical = Get(0xf58e);

        /// Guitar
        static IconConstructor Guitar = Get(0xf7a6);

        /// H Square
        static IconConstructor HSquare = Get(0xf0fd);

        /// Hamburger
        static IconConstructor Hamburger = Get(0xf805);

        /// Hammer
        static IconConstructor Hammer = Get(0xf6e3);

        /// Hamsa
        static IconConstructor Hamsa = Get(0xf665);

        /// Hand Holding
        static IconConstructor HandHolding = Get(0xf4bd);

        /// Hand Holding Heart
        static IconConstructor HandHoldingHeart = Get(0xf4be);

        /// Hand Holding US Dollar
        static IconConstructor HandHoldingUsd = Get(0xf4c0);

        /// Lizard (Hand)
        static IconConstructor HandLizard = Get(0xf258);

        /// Hand with Middle Finger Raised
        static IconConstructor HandMiddleFinger = Get(0xf806);

        /// Paper (Hand)
        static IconConstructor HandPaper = Get(0xf256);

        /// Peace (Hand)
        static IconConstructor HandPeace = Get(0xf25b);

        /// Hand Pointing Down
        static IconConstructor HandPointDown = Get(0xf0a7);

        /// Hand Pointing Left
        static IconConstructor HandPointLeft = Get(0xf0a5);

        /// Hand Pointing Right
        static IconConstructor HandPointRight = Get(0xf0a4);

        /// Hand Pointing Up
        static IconConstructor HandPointUp = Get(0xf0a6);

        /// Pointer (Hand)
        static IconConstructor HandPointer = Get(0xf25a);

        /// Rock (Hand)
        static IconConstructor HandRock = Get(0xf255);

        /// Scissors (Hand)
        static IconConstructor HandScissors = Get(0xf257);

        /// Spock (Hand)
        static IconConstructor HandSpock = Get(0xf259);

        /// Hands
        static IconConstructor Hands = Get(0xf4c2);

        /// Helping Hands
        static IconConstructor HandsHelping = Get(0xf4c4);

        /// Handshake
        static IconConstructor Handshake = Get(0xf2b5);

        /// Hanukiah
        static IconConstructor Hanukiah = Get(0xf6e6);

        /// Hard Hat
        static IconConstructor HardHat = Get(0xf807);

        /// Hashtag
        static IconConstructor Hashtag = Get(0xf292);

        /// Wizard's Hat
        static IconConstructor HatWizard = Get(0xf6e8);

        /// Haykal
        static IconConstructor Haykal = Get(0xf666);

        /// HDD
        static IconConstructor Hdd = Get(0xf0a0);

        /// heading
        static IconConstructor Heading = Get(0xf1dc);

        /// headphones
        static IconConstructor Headphones = Get(0xf025);

        /// Alternate Headphones
        static IconConstructor HeadphonesAlt = Get(0xf58f);

        /// Headset
        static IconConstructor Headset = Get(0xf590);

        /// Heart
        static IconConstructor Heart = Get(0xf004);

        /// Heart Broken
        static IconConstructor HeartBroken = Get(0xf7a9);

        /// Heartbeat
        static IconConstructor Heartbeat = Get(0xf21e);

        /// Helicopter
        static IconConstructor Helicopter = Get(0xf533);

        /// Highlighter
        static IconConstructor Highlighter = Get(0xf591);

        /// Hiking
        static IconConstructor Hiking = Get(0xf6ec);

        /// Hippo
        static IconConstructor Hippo = Get(0xf6ed);

        /// History
        static IconConstructor History = Get(0xf1da);

        /// Hockey Puck
        static IconConstructor HockeyPuck = Get(0xf453);

        /// Holly Berry
        static IconConstructor HollyBerry = Get(0xf7aa);

        /// home
        static IconConstructor Home = Get(0xf015);

        /// Horse
        static IconConstructor Horse = Get(0xf6f0);

        /// Horse Head
        static IconConstructor HorseHead = Get(0xf7ab);

        /// hospital
        static IconConstructor Hospital = Get(0xf0f8);

        /// Alternate Hospital
        static IconConstructor HospitalAlt = Get(0xf47d);

        /// Hospital Symbol
        static IconConstructor HospitalSymbol = Get(0xf47e);

        /// Hot Tub
        static IconConstructor HotTub = Get(0xf593);

        /// Hot Dog
        static IconConstructor Hotdog = Get(0xf80f);

        /// Hotel
        static IconConstructor Hotel = Get(0xf594);

        /// Hourglass
        static IconConstructor Hourglass = Get(0xf254);

        /// Hourglass End
        static IconConstructor HourglassEnd = Get(0xf253);

        /// Hourglass Half
        static IconConstructor HourglassHalf = Get(0xf252);

        /// Hourglass Start
        static IconConstructor HourglassStart = Get(0xf251);

        /// Damaged House
        static IconConstructor HouseDamage = Get(0xf6f1);

        /// Hryvnia
        static IconConstructor Hryvnia = Get(0xf6f2);

        /// I Beam Cursor
        static IconConstructor ICursor = Get(0xf246);

        /// Ice Cream
        static IconConstructor IceCream = Get(0xf810);

        /// Icicles
        static IconConstructor Icicles = Get(0xf7ad);

        /// Identification Badge
        static IconConstructor IdBadge = Get(0xf2c1);

        /// Identification Card
        static IconConstructor IdCard = Get(0xf2c2);

        /// Alternate Identification Card
        static IconConstructor IdCardAlt = Get(0xf47f);

        /// Igloo
        static IconConstructor Igloo = Get(0xf7ae);

        /// Image
        static IconConstructor Image = Get(0xf03e);

        /// Images
        static IconConstructor Images = Get(0xf302);

        /// inbox
        static IconConstructor Inbox = Get(0xf01c);

        /// Indent
        static IconConstructor Indent = Get(0xf03c);

        /// Industry
        static IconConstructor Industry = Get(0xf275);

        /// Infinity
        static IconConstructor Infinity = Get(0xf534);

        /// Info
        static IconConstructor Info = Get(0xf129);

        /// Info Circle
        static IconConstructor InfoCircle = Get(0xf05a);

        /// italic
        static IconConstructor Italic = Get(0xf033);

        /// Jedi
        static IconConstructor Jedi = Get(0xf669);

        /// Joint
        static IconConstructor Joint = Get(0xf595);

        /// Journal of the Whills
        static IconConstructor JournalWhills = Get(0xf66a);

        /// Kaaba
        static IconConstructor Kaaba = Get(0xf66b);

        /// key
        static IconConstructor Key = Get(0xf084);

        /// Keyboard
        static IconConstructor Keyboard = Get(0xf11c);

        /// Khanda
        static IconConstructor Khanda = Get(0xf66d);

        /// Kissing Face
        static IconConstructor Kiss = Get(0xf596);

        /// Kissing Face With Smiling Eyes
        static IconConstructor KissBeam = Get(0xf597);

        /// Face Blowing a Kiss
        static IconConstructor KissWinkHeart = Get(0xf598);

        /// Kiwi Bird
        static IconConstructor KiwiBird = Get(0xf535);

        /// Landmark
        static IconConstructor Landmark = Get(0xf66f);

        /// Language
        static IconConstructor Language = Get(0xf1ab);

        /// Laptop
        static IconConstructor Laptop = Get(0xf109);

        /// Laptop Code
        static IconConstructor LaptopCode = Get(0xf5fc);

        /// Laptop Medical
        static IconConstructor LaptopMedical = Get(0xf812);

        /// Grinning Face With Big Eyes
        static IconConstructor Laugh = Get(0xf599);

        /// Laugh Face with Beaming Eyes
        static IconConstructor LaughBeam = Get(0xf59a);

        /// Laughing Squinting Face
        static IconConstructor LaughSquint = Get(0xf59b);

        /// Laughing Winking Face
        static IconConstructor LaughWink = Get(0xf59c);

        /// Layer Group
        static IconConstructor LayerGroup = Get(0xf5fd);

        /// leaf
        static IconConstructor Leaf = Get(0xf06c);

        /// Lemon
        static IconConstructor Lemon = Get(0xf094);

        /// Less Than
        static IconConstructor LessThan = Get(0xf536);

        /// Less Than Equal To
        static IconConstructor LessThanEqual = Get(0xf537);

        /// Alternate Level Down
        static IconConstructor LevelDownAlt = Get(0xf3be);

        /// Alternate Level Up
        static IconConstructor LevelUpAlt = Get(0xf3bf);

        /// Life Ring
        static IconConstructor LifeRing = Get(0xf1cd);

        /// Lightbulb
        static IconConstructor Lightbulb = Get(0xf0eb);

        /// Link
        static IconConstructor Link = Get(0xf0c1);

        /// Turkish Lira Sign
        static IconConstructor LiraSign = Get(0xf195);

        /// List
        static IconConstructor List = Get(0xf03a);

        /// Alternate List
        static IconConstructor ListAlt = Get(0xf022);

        /// list-ol
        static IconConstructor ListOl = Get(0xf0cb);

        /// list-ul
        static IconConstructor ListUl = Get(0xf0ca);

        /// location-arrow
        static IconConstructor LocationArrow = Get(0xf124);

        /// lock
        static IconConstructor Lock = Get(0xf023);

        /// Lock Open
        static IconConstructor LockOpen = Get(0xf3c1);

        /// Alternate Long Arrow Down
        static IconConstructor LongArrowAltDown = Get(0xf309);

        /// Alternate Long Arrow Left
        static IconConstructor LongArrowAltLeft = Get(0xf30a);

        /// Alternate Long Arrow Right
        static IconConstructor LongArrowAltRight = Get(0xf30b);

        /// Alternate Long Arrow Up
        static IconConstructor LongArrowAltUp = Get(0xf30c);

        /// Low Vision
        static IconConstructor LowVision = Get(0xf2a8);

        /// Luggage Cart
        static IconConstructor LuggageCart = Get(0xf59d);

        /// magic
        static IconConstructor Magic = Get(0xf0d0);

        /// magnet
        static IconConstructor Magnet = Get(0xf076);

        /// Mail Bulk
        static IconConstructor MailBulk = Get(0xf674);

        /// Male
        static IconConstructor Male = Get(0xf183);

        /// Map
        static IconConstructor Map = Get(0xf279);

        /// Map Marked
        static IconConstructor MapMarked = Get(0xf59f);

        /// Alternate Map Marked
        static IconConstructor MapMarkedAlt = Get(0xf5a0);

        /// map-marker
        static IconConstructor MapMarker = Get(0xf041);

        /// Alternate Map Marker
        static IconConstructor MapMarkerAlt = Get(0xf3c5);

        /// Map Pin
        static IconConstructor MapPin = Get(0xf276);

        /// Map Signs
        static IconConstructor MapSigns = Get(0xf277);

        /// Marker
        static IconConstructor Marker = Get(0xf5a1);

        /// Mars
        static IconConstructor Mars = Get(0xf222);

        /// Mars Double
        static IconConstructor MarsDouble = Get(0xf227);

        /// Mars Stroke
        static IconConstructor MarsStroke = Get(0xf229);

        /// Mars Stroke Horizontal
        static IconConstructor MarsStrokeH = Get(0xf22b);

        /// Mars Stroke Vertical
        static IconConstructor MarsStrokeV = Get(0xf22a);

        /// Mask
        static IconConstructor Mask = Get(0xf6fa);

        /// Medal
        static IconConstructor Medal = Get(0xf5a2);

        /// medkit
        static IconConstructor Medkit = Get(0xf0fa);

        /// Neutral Face
        static IconConstructor Meh = Get(0xf11a);

        /// Face Without Mouth
        static IconConstructor MehBlank = Get(0xf5a4);

        /// Face With Rolling Eyes
        static IconConstructor MehRollingEyes = Get(0xf5a5);

        /// Memory
        static IconConstructor Memory = Get(0xf538);

        /// Menorah
        static IconConstructor Menorah = Get(0xf676);

        /// Mercury
        static IconConstructor Mercury = Get(0xf223);

        /// Meteor
        static IconConstructor Meteor = Get(0xf753);

        /// Microchip
        static IconConstructor Microchip = Get(0xf2db);

        /// microphone
        static IconConstructor Microphone = Get(0xf130);

        /// Alternate Microphone
        static IconConstructor MicrophoneAlt = Get(0xf3c9);

        /// Alternate Microphone Slash
        static IconConstructor MicrophoneAltSlash = Get(0xf539);

        /// Microphone Slash
        static IconConstructor MicrophoneSlash = Get(0xf131);

        /// Microscope
        static IconConstructor Microscope = Get(0xf610);

        /// minus
        static IconConstructor Minus = Get(0xf068);

        /// Minus Circle
        static IconConstructor MinusCircle = Get(0xf056);

        /// Minus Square
        static IconConstructor MinusSquare = Get(0xf146);

        /// Mitten
        static IconConstructor Mitten = Get(0xf7b5);

        /// Mobile Phone
        static IconConstructor Mobile = Get(0xf10b);

        /// Alternate Mobile
        static IconConstructor MobileAlt = Get(0xf3cd);

        /// Money Bill
        static IconConstructor MoneyBill = Get(0xf0d6);

        /// Alternate Money Bill
        static IconConstructor MoneyBillAlt = Get(0xf3d1);

        /// Wavy Money Bill
        static IconConstructor MoneyBillWave = Get(0xf53a);

        /// Alternate Wavy Money Bill
        static IconConstructor MoneyBillWaveAlt = Get(0xf53b);

        /// Money Check
        static IconConstructor MoneyCheck = Get(0xf53c);

        /// Alternate Money Check
        static IconConstructor MoneyCheckAlt = Get(0xf53d);

        /// Monument
        static IconConstructor Monument = Get(0xf5a6);

        /// Moon
        static IconConstructor Moon = Get(0xf186);

        /// Mortar Pestle
        static IconConstructor MortarPestle = Get(0xf5a7);

        /// Mosque
        static IconConstructor Mosque = Get(0xf678);

        /// Motorcycle
        static IconConstructor Motorcycle = Get(0xf21c);

        /// Mountain
        static IconConstructor Mountain = Get(0xf6fc);

        /// Mouse Pointer
        static IconConstructor MousePointer = Get(0xf245);

        /// Mug Hot
        static IconConstructor MugHot = Get(0xf7b6);

        /// Music
        static IconConstructor Music = Get(0xf001);

        /// Wired Network
        static IconConstructor NetworkWired = Get(0xf6ff);

        /// Neuter
        static IconConstructor Neuter = Get(0xf22c);

        /// Newspaper
        static IconConstructor Newspaper = Get(0xf1ea);

        /// Not Equal
        static IconConstructor NotEqual = Get(0xf53e);

        /// Medical Notes
        static IconConstructor NotesMedical = Get(0xf481);

        /// Object Group
        static IconConstructor ObjectGroup = Get(0xf247);

        /// Object Ungroup
        static IconConstructor ObjectUngroup = Get(0xf248);

        /// Oil Can
        static IconConstructor OilCan = Get(0xf613);

        /// Om
        static IconConstructor Om = Get(0xf679);

        /// Otter
        static IconConstructor Otter = Get(0xf700);

        /// Outdent
        static IconConstructor Outdent = Get(0xf03b);

        /// Pager
        static IconConstructor Pager = Get(0xf815);

        /// Paint Brush
        static IconConstructor PaintBrush = Get(0xf1fc);

        /// Paint Roller
        static IconConstructor PaintRoller = Get(0xf5aa);

        /// Palette
        static IconConstructor Palette = Get(0xf53f);

        /// Pallet
        static IconConstructor Pallet = Get(0xf482);

        /// Paper Plane
        static IconConstructor PaperPlane = Get(0xf1d8);

        /// Paperclip
        static IconConstructor Paperclip = Get(0xf0c6);

        /// Parachute Box
        static IconConstructor ParachuteBox = Get(0xf4cd);

        /// paragraph
        static IconConstructor Paragraph = Get(0xf1dd);

        /// Parking
        static IconConstructor Parking = Get(0xf540);

        /// Passport
        static IconConstructor Passport = Get(0xf5ab);

        /// Pastafarianism
        static IconConstructor Pastafarianism = Get(0xf67b);

        /// Paste
        static IconConstructor Paste = Get(0xf0ea);

        /// pause
        static IconConstructor Pause = Get(0xf04c);

        /// Pause Circle
        static IconConstructor PauseCircle = Get(0xf28b);

        /// Paw
        static IconConstructor Paw = Get(0xf1b0);

        /// Peace
        static IconConstructor Peace = Get(0xf67c);

        /// Pen
        static IconConstructor Pen = Get(0xf304);

        /// Alternate Pen
        static IconConstructor PenAlt = Get(0xf305);

        /// Pen Fancy
        static IconConstructor PenFancy = Get(0xf5ac);

        /// Pen Nib
        static IconConstructor PenNib = Get(0xf5ad);

        /// Pen Square
        static IconConstructor PenSquare = Get(0xf14b);

        /// Alternate Pencil
        static IconConstructor PencilAlt = Get(0xf303);

        /// Pencil Ruler
        static IconConstructor PencilRuler = Get(0xf5ae);

        /// People Carry
        static IconConstructor PeopleCarry = Get(0xf4ce);

        /// Hot Pepper
        static IconConstructor PepperHot = Get(0xf816);

        /// Percent
        static IconConstructor Percent = Get(0xf295);

        /// Percentage
        static IconConstructor Percentage = Get(0xf541);

        /// Person Entering Booth
        static IconConstructor PersonBooth = Get(0xf756);

        /// Phone
        static IconConstructor Phone = Get(0xf095);

        /// Phone Slash
        static IconConstructor PhoneSlash = Get(0xf3dd);

        /// Phone Square
        static IconConstructor PhoneSquare = Get(0xf098);

        /// Phone Volume
        static IconConstructor PhoneVolume = Get(0xf2a0);

        /// Piggy Bank
        static IconConstructor PiggyBank = Get(0xf4d3);

        /// Pills
        static IconConstructor Pills = Get(0xf484);

        /// Pizza Slice
        static IconConstructor PizzaSlice = Get(0xf818);

        /// Place of Worship
        static IconConstructor PlaceOfWorship = Get(0xf67f);

        /// plane
        static IconConstructor Plane = Get(0xf072);

        /// Plane Arrival
        static IconConstructor PlaneArrival = Get(0xf5af);

        /// Plane Departure
        static IconConstructor PlaneDeparture = Get(0xf5b0);

        /// play
        static IconConstructor Play = Get(0xf04b);

        /// Play Circle
        static IconConstructor PlayCircle = Get(0xf144);

        /// Plug
        static IconConstructor Plug = Get(0xf1e6);

        /// plus
        static IconConstructor Plus = Get(0xf067);

        /// Plus Circle
        static IconConstructor PlusCircle = Get(0xf055);

        /// Plus Square
        static IconConstructor PlusSquare = Get(0xf0fe);

        /// Podcast
        static IconConstructor Podcast = Get(0xf2ce);

        /// Poll
        static IconConstructor Poll = Get(0xf681);

        /// Poll H
        static IconConstructor PollH = Get(0xf682);

        /// Poo
        static IconConstructor Poo = Get(0xf2fe);

        /// Poo Storm
        static IconConstructor PooStorm = Get(0xf75a);

        /// Poop
        static IconConstructor Poop = Get(0xf619);

        /// Portrait
        static IconConstructor Portrait = Get(0xf3e0);

        /// Pound Sign
        static IconConstructor PoundSign = Get(0xf154);

        /// Power Off
        static IconConstructor PowerOff = Get(0xf011);

        /// Pray
        static IconConstructor Pray = Get(0xf683);

        /// Praying Hands
        static IconConstructor PrayingHands = Get(0xf684);

        /// Prescription
        static IconConstructor Prescription = Get(0xf5b1);

        /// Prescription Bottle
        static IconConstructor PrescriptionBottle = Get(0xf485);

        /// Alternate Prescription Bottle
        static IconConstructor PrescriptionBottleAlt = Get(0xf486);

        /// print
        static IconConstructor Print = Get(0xf02f);

        /// Procedures
        static IconConstructor Procedures = Get(0xf487);

        /// Project Diagram
        static IconConstructor ProjectDiagram = Get(0xf542);

        /// Puzzle Piece
        static IconConstructor PuzzlePiece = Get(0xf12e);

        /// qrcode
        static IconConstructor Qrcode = Get(0xf029);

        /// Question
        static IconConstructor Question = Get(0xf128);

        /// Question Circle
        static IconConstructor QuestionCircle = Get(0xf059);

        /// Quidditch
        static IconConstructor Quidditch = Get(0xf458);

        /// quote-left
        static IconConstructor QuoteLeft = Get(0xf10d);

        /// quote-right
        static IconConstructor QuoteRight = Get(0xf10e);

        /// Quran
        static IconConstructor Quran = Get(0xf687);

        /// Radiation
        static IconConstructor Radiation = Get(0xf7b9);

        /// Alternate Radiation
        static IconConstructor RadiationAlt = Get(0xf7ba);

        /// Rainbow
        static IconConstructor Rainbow = Get(0xf75b);

        /// random
        static IconConstructor Random = Get(0xf074);

        /// Receipt
        static IconConstructor Receipt = Get(0xf543);

        /// Recycle
        static IconConstructor Recycle = Get(0xf1b8);

        /// Redo
        static IconConstructor Redo = Get(0xf01e);

        /// Alternate Redo
        static IconConstructor RedoAlt = Get(0xf2f9);

        /// Registered Trademark
        static IconConstructor Registered = Get(0xf25d);

        /// Reply
        static IconConstructor Reply = Get(0xf3e5);

        /// reply-all
        static IconConstructor ReplyAll = Get(0xf122);

        /// Republican
        static IconConstructor Republican = Get(0xf75e);

        /// Restroom
        static IconConstructor Restroom = Get(0xf7bd);

        /// Retweet
        static IconConstructor Retweet = Get(0xf079);

        /// Ribbon
        static IconConstructor Ribbon = Get(0xf4d6);

        /// Ring
        static IconConstructor Ring = Get(0xf70b);

        /// road
        static IconConstructor Road = Get(0xf018);

        /// Robot
        static IconConstructor Robot = Get(0xf544);

        /// rocket
        static IconConstructor Rocket = Get(0xf135);

        /// Route
        static IconConstructor Route = Get(0xf4d7);

        /// rss
        static IconConstructor Rss = Get(0xf09e);

        /// RSS Square
        static IconConstructor RssSquare = Get(0xf143);

        /// Ruble Sign
        static IconConstructor RubleSign = Get(0xf158);

        /// Ruler
        static IconConstructor Ruler = Get(0xf545);

        /// Ruler Combined
        static IconConstructor RulerCombined = Get(0xf546);

        /// Ruler Horizontal
        static IconConstructor RulerHorizontal = Get(0xf547);

        /// Ruler Vertical
        static IconConstructor RulerVertical = Get(0xf548);

        /// Running
        static IconConstructor Running = Get(0xf70c);

        /// Indian Rupee Sign
        static IconConstructor RupeeSign = Get(0xf156);

        /// Crying Face
        static IconConstructor SadCry = Get(0xf5b3);

        /// Loudly Crying Face
        static IconConstructor SadTear = Get(0xf5b4);

        /// Satellite
        static IconConstructor Satellite = Get(0xf7bf);

        /// Satellite Dish
        static IconConstructor SatelliteDish = Get(0xf7c0);

        /// Save
        static IconConstructor Save = Get(0xf0c7);

        /// School
        static IconConstructor School = Get(0xf549);

        /// Screwdriver
        static IconConstructor Screwdriver = Get(0xf54a);

        /// Scroll
        static IconConstructor Scroll = Get(0xf70e);

        /// Sd Card
        static IconConstructor SdCard = Get(0xf7c2);

        /// Search
        static IconConstructor Search = Get(0xf002);

        /// Search Dollar
        static IconConstructor SearchDollar = Get(0xf688);

        /// Search Location
        static IconConstructor SearchLocation = Get(0xf689);

        /// Search Minus
        static IconConstructor SearchMinus = Get(0xf010);

        /// Search Plus
        static IconConstructor SearchPlus = Get(0xf00e);

        /// Seedling
        static IconConstructor Seedling = Get(0xf4d8);

        /// Server
        static IconConstructor Server = Get(0xf233);

        /// Shapes
        static IconConstructor Shapes = Get(0xf61f);

        /// Share
        static IconConstructor Share = Get(0xf064);

        /// Alternate Share
        static IconConstructor ShareAlt = Get(0xf1e0);

        /// Alternate Share Square
        static IconConstructor ShareAltSquare = Get(0xf1e1);

        /// Share Square
        static IconConstructor ShareSquare = Get(0xf14d);

        /// Shekel Sign
        static IconConstructor ShekelSign = Get(0xf20b);

        /// Alternate Shield
        static IconConstructor ShieldAlt = Get(0xf3ed);

        /// Ship
        static IconConstructor Ship = Get(0xf21a);

        /// Shipping Fast
        static IconConstructor ShippingFast = Get(0xf48b);

        /// Shoe Prints
        static IconConstructor ShoePrints = Get(0xf54b);

        /// Shopping Bag
        static IconConstructor ShoppingBag = Get(0xf290);

        /// Shopping Basket
        static IconConstructor ShoppingBasket = Get(0xf291);

        /// shopping-cart
        static IconConstructor ShoppingCart = Get(0xf07a);

        /// Shower
        static IconConstructor Shower = Get(0xf2cc);

        /// Shuttle Van
        static IconConstructor ShuttleVan = Get(0xf5b6);

        /// Sign
        static IconConstructor Sign = Get(0xf4d9);

        /// Alternate Sign In
        static IconConstructor SignInAlt = Get(0xf2f6);

        /// Sign Language
        static IconConstructor SignLanguage = Get(0xf2a7);

        /// Alternate Sign Out
        static IconConstructor SignOutAlt = Get(0xf2f5);

        /// signal
        static IconConstructor Signal = Get(0xf012);

        /// Signature
        static IconConstructor Signature = Get(0xf5b7);

        /// SIM Card
        static IconConstructor SimCard = Get(0xf7c4);

        /// Sitemap
        static IconConstructor Sitemap = Get(0xf0e8);

        /// Skating
        static IconConstructor Skating = Get(0xf7c5);

        /// Skiing
        static IconConstructor Skiing = Get(0xf7c9);

        /// Skiing Nordic
        static IconConstructor SkiingNordic = Get(0xf7ca);

        /// Skull
        static IconConstructor Skull = Get(0xf54c);

        /// Skull &amp; Crossbones
        static IconConstructor SkullCrossbones = Get(0xf714);

        /// Slash
        static IconConstructor Slash = Get(0xf715);

        /// Sleigh
        static IconConstructor Sleigh = Get(0xf7cc);

        /// Horizontal Sliders
        static IconConstructor SlidersH = Get(0xf1de);

        /// Smiling Face
        static IconConstructor Smile = Get(0xf118);

        /// Beaming Face With Smiling Eyes
        static IconConstructor SmileBeam = Get(0xf5b8);

        /// Winking Face
        static IconConstructor SmileWink = Get(0xf4da);

        /// Smog
        static IconConstructor Smog = Get(0xf75f);

        /// Smoking
        static IconConstructor Smoking = Get(0xf48d);

        /// Smoking Ban
        static IconConstructor SmokingBan = Get(0xf54d);

        /// SMS
        static IconConstructor Sms = Get(0xf7cd);

        /// Snowboarding
        static IconConstructor Snowboarding = Get(0xf7ce);

        /// Snowflake
        static IconConstructor Snowflake = Get(0xf2dc);

        /// Snowman
        static IconConstructor Snowman = Get(0xf7d0);

        /// Snowplow
        static IconConstructor Snowplow = Get(0xf7d2);

        /// Socks
        static IconConstructor Socks = Get(0xf696);

        /// Solar Panel
        static IconConstructor SolarPanel = Get(0xf5ba);

        /// Sort
        static IconConstructor Sort = Get(0xf0dc);

        /// Sort Alpha Down
        static IconConstructor SortAlphaDown = Get(0xf15d);

        /// Sort Alpha Up
        static IconConstructor SortAlphaUp = Get(0xf15e);

        /// Sort Amount Down
        static IconConstructor SortAmountDown = Get(0xf160);

        /// Sort Amount Up
        static IconConstructor SortAmountUp = Get(0xf161);

        /// Sort Down (Descending)
        static IconConstructor SortDown = Get(0xf0dd);

        /// Sort Numeric Down
        static IconConstructor SortNumericDown = Get(0xf162);

        /// Sort Numeric Up
        static IconConstructor SortNumericUp = Get(0xf163);

        /// Sort Up (Ascending)
        static IconConstructor SortUp = Get(0xf0de);

        /// Spa
        static IconConstructor Spa = Get(0xf5bb);

        /// Space Shuttle
        static IconConstructor SpaceShuttle = Get(0xf197);

        /// Spider
        static IconConstructor Spider = Get(0xf717);

        /// Spinner
        static IconConstructor Spinner = Get(0xf110);

        /// Splotch
        static IconConstructor Splotch = Get(0xf5bc);

        /// Spray Can
        static IconConstructor SprayCan = Get(0xf5bd);

        /// Square
        static IconConstructor Square = Get(0xf0c8);

        /// Square Full
        static IconConstructor SquareFull = Get(0xf45c);

        /// Alternate Square Root
        static IconConstructor SquareRootAlt = Get(0xf698);

        /// Stamp
        static IconConstructor Stamp = Get(0xf5bf);

        /// Star
        static IconConstructor Star = Get(0xf005);

        /// Star and Crescent
        static IconConstructor StarAndCrescent = Get(0xf699);

        /// star-half
        static IconConstructor StarHalf = Get(0xf089);

        /// Alternate Star Half
        static IconConstructor StarHalfAlt = Get(0xf5c0);

        /// Star of David
        static IconConstructor StarOfDavid = Get(0xf69a);

        /// Star of Life
        static IconConstructor StarOfLife = Get(0xf621);

        /// step-backward
        static IconConstructor StepBackward = Get(0xf048);

        /// step-forward
        static IconConstructor StepForward = Get(0xf051);

        /// Stethoscope
        static IconConstructor Stethoscope = Get(0xf0f1);

        /// Sticky Note
        static IconConstructor StickyNote = Get(0xf249);

        /// stop
        static IconConstructor Stop = Get(0xf04d);

        /// Stop Circle
        static IconConstructor StopCircle = Get(0xf28d);

        /// Stopwatch
        static IconConstructor Stopwatch = Get(0xf2f2);

        /// Store
        static IconConstructor Store = Get(0xf54e);

        /// Alternate Store
        static IconConstructor StoreAlt = Get(0xf54f);

        /// Stream
        static IconConstructor Stream = Get(0xf550);

        /// Street View
        static IconConstructor StreetView = Get(0xf21d);

        /// Strikethrough
        static IconConstructor Strikethrough = Get(0xf0cc);

        /// Stroopwafel
        static IconConstructor Stroopwafel = Get(0xf551);

        /// subscript
        static IconConstructor Subscript = Get(0xf12c);

        /// Subway
        static IconConstructor Subway = Get(0xf239);

        /// Suitcase
        static IconConstructor Suitcase = Get(0xf0f2);

        /// Suitcase Rolling
        static IconConstructor SuitcaseRolling = Get(0xf5c1);

        /// Sun
        static IconConstructor Sun = Get(0xf185);

        /// superscript
        static IconConstructor Superscript = Get(0xf12b);

        /// Hushed Face
        static IconConstructor Surprise = Get(0xf5c2);

        /// Swatchbook
        static IconConstructor Swatchbook = Get(0xf5c3);

        /// Swimmer
        static IconConstructor Swimmer = Get(0xf5c4);

        /// Swimming Pool
        static IconConstructor SwimmingPool = Get(0xf5c5);

        /// Synagogue
        static IconConstructor Synagogue = Get(0xf69b);

        /// Sync
        static IconConstructor Sync = Get(0xf021);

        /// Alternate Sync
        static IconConstructor SyncAlt = Get(0xf2f1);

        /// Syringe
        static IconConstructor Syringe = Get(0xf48e);

        /// table
        static IconConstructor Table = Get(0xf0ce);

        /// Table Tennis
        static IconConstructor TableTennis = Get(0xf45d);

        /// tablet
        static IconConstructor Tablet = Get(0xf10a);

        /// Alternate Tablet
        static IconConstructor TabletAlt = Get(0xf3fa);

        /// Tablets
        static IconConstructor Tablets = Get(0xf490);

        /// Alternate Tachometer
        static IconConstructor TachometerAlt = Get(0xf3fd);

        /// tag
        static IconConstructor Tag = Get(0xf02b);

        /// tags
        static IconConstructor Tags = Get(0xf02c);

        /// Tape
        static IconConstructor Tape = Get(0xf4db);

        /// Tasks
        static IconConstructor Tasks = Get(0xf0ae);

        /// Taxi
        static IconConstructor Taxi = Get(0xf1ba);

        /// Teeth
        static IconConstructor Teeth = Get(0xf62e);

        /// Teeth Open
        static IconConstructor TeethOpen = Get(0xf62f);

        /// High Temperature
        static IconConstructor TemperatureHigh = Get(0xf769);

        /// Low Temperature
        static IconConstructor TemperatureLow = Get(0xf76b);

        /// Tenge
        static IconConstructor Tenge = Get(0xf7d7);

        /// Terminal
        static IconConstructor Terminal = Get(0xf120);

        /// text-height
        static IconConstructor TextHeight = Get(0xf034);

        /// text-width
        static IconConstructor TextWidth = Get(0xf035);

        /// th
        static IconConstructor Th = Get(0xf00a);

        /// th-large
        static IconConstructor ThLarge = Get(0xf009);

        /// th-list
        static IconConstructor ThList = Get(0xf00b);

        /// Theater Masks
        static IconConstructor TheaterMasks = Get(0xf630);

        /// Thermometer
        static IconConstructor Thermometer = Get(0xf491);

        /// Thermometer Empty
        static IconConstructor ThermometerEmpty = Get(0xf2cb);

        /// Thermometer Full
        static IconConstructor ThermometerFull = Get(0xf2c7);

        /// Thermometer 1/2 Full
        static IconConstructor ThermometerHalf = Get(0xf2c9);

        /// Thermometer 1/4 Full
        static IconConstructor ThermometerQuarter = Get(0xf2ca);

        /// Thermometer 3/4 Full
        static IconConstructor ThermometerThreeQuarters = Get(0xf2c8);

        /// thumbs-down
        static IconConstructor ThumbsDown = Get(0xf165);

        /// thumbs-up
        static IconConstructor ThumbsUp = Get(0xf164);

        /// Thumbtack
        static IconConstructor Thumbtack = Get(0xf08d);

        /// Alternate Ticket
        static IconConstructor TicketAlt = Get(0xf3ff);

        /// Times
        static IconConstructor Times = Get(0xf00d);

        /// Times Circle
        static IconConstructor TimesCircle = Get(0xf057);

        /// tint
        static IconConstructor Tint = Get(0xf043);

        /// Tint Slash
        static IconConstructor TintSlash = Get(0xf5c7);

        /// Tired Face
        static IconConstructor Tired = Get(0xf5c8);

        /// Toggle Off
        static IconConstructor ToggleOff = Get(0xf204);

        /// Toggle On
        static IconConstructor ToggleOn = Get(0xf205);

        /// Toilet
        static IconConstructor Toilet = Get(0xf7d8);

        /// Toilet Paper
        static IconConstructor ToiletPaper = Get(0xf71e);

        /// Toolbox
        static IconConstructor Toolbox = Get(0xf552);

        /// Tools
        static IconConstructor Tools = Get(0xf7d9);

        /// Tooth
        static IconConstructor Tooth = Get(0xf5c9);

        /// Torah
        static IconConstructor Torah = Get(0xf6a0);

        /// Torii Gate
        static IconConstructor ToriiGate = Get(0xf6a1);

        /// Tractor
        static IconConstructor Tractor = Get(0xf722);

        /// Trademark
        static IconConstructor Trademark = Get(0xf25c);

        /// Traffic Light
        static IconConstructor TrafficLight = Get(0xf637);

        /// Train
        static IconConstructor Train = Get(0xf238);

        /// Tram
        static IconConstructor Tram = Get(0xf7da);

        /// Transgender
        static IconConstructor Transgender = Get(0xf224);

        /// Alternate Transgender
        static IconConstructor TransgenderAlt = Get(0xf225);

        /// Trash
        static IconConstructor Trash = Get(0xf1f8);

        /// Alternate Trash
        static IconConstructor TrashAlt = Get(0xf2ed);

        /// Trash Restore
        static IconConstructor TrashRestore = Get(0xf829);

        /// Alternative Trash Restore
        static IconConstructor TrashRestoreAlt = Get(0xf82a);

        /// Tree
        static IconConstructor Tree = Get(0xf1bb);

        /// trophy
        static IconConstructor Trophy = Get(0xf091);

        /// truck
        static IconConstructor Truck = Get(0xf0d1);

        /// Truck Loading
        static IconConstructor TruckLoading = Get(0xf4de);

        /// Truck Monster
        static IconConstructor TruckMonster = Get(0xf63b);

        /// Truck Moving
        static IconConstructor TruckMoving = Get(0xf4df);

        /// Truck Side
        static IconConstructor TruckPickup = Get(0xf63c);

        /// T-Shirt
        static IconConstructor Tshirt = Get(0xf553);

        /// TTY
        static IconConstructor Tty = Get(0xf1e4);

        /// Television
        static IconConstructor Tv = Get(0xf26c);

        /// Umbrella
        static IconConstructor Umbrella = Get(0xf0e9);

        /// Umbrella Beach
        static IconConstructor UmbrellaBeach = Get(0xf5ca);

        /// Underline
        static IconConstructor Underline = Get(0xf0cd);

        /// Undo
        static IconConstructor Undo = Get(0xf0e2);

        /// Alternate Undo
        static IconConstructor UndoAlt = Get(0xf2ea);

        /// Universal Access
        static IconConstructor UniversalAccess = Get(0xf29a);

        /// University
        static IconConstructor University = Get(0xf19c);

        /// unlink
        static IconConstructor Unlink = Get(0xf127);

        /// unlock
        static IconConstructor Unlock = Get(0xf09c);

        /// Alternate Unlock
        static IconConstructor UnlockAlt = Get(0xf13e);

        /// Upload
        static IconConstructor Upload = Get(0xf093);

        /// User
        static IconConstructor User = Get(0xf007);

        /// Alternate User
        static IconConstructor UserAlt = Get(0xf406);

        /// Alternate User Slash
        static IconConstructor UserAltSlash = Get(0xf4fa);

        /// User Astronaut
        static IconConstructor UserAstronaut = Get(0xf4fb);

        /// User Check
        static IconConstructor UserCheck = Get(0xf4fc);

        /// User Circle
        static IconConstructor UserCircle = Get(0xf2bd);

        /// User Clock
        static IconConstructor UserClock = Get(0xf4fd);

        /// User Cog
        static IconConstructor UserCog = Get(0xf4fe);

        /// User Edit
        static IconConstructor UserEdit = Get(0xf4ff);

        /// User Friends
        static IconConstructor UserFriends = Get(0xf500);

        /// User Graduate
        static IconConstructor UserGraduate = Get(0xf501);

        /// User Injured
        static IconConstructor UserInjured = Get(0xf728);

        /// User Lock
        static IconConstructor UserLock = Get(0xf502);

        /// Doctor
        static IconConstructor UserMd = Get(0xf0f0);

        /// User Minus
        static IconConstructor UserMinus = Get(0xf503);

        /// User Ninja
        static IconConstructor UserNinja = Get(0xf504);

        /// Nurse
        static IconConstructor UserNurse = Get(0xf82f);

        /// User Plus
        static IconConstructor UserPlus = Get(0xf234);

        /// User Secret
        static IconConstructor UserSecret = Get(0xf21b);

        /// User Shield
        static IconConstructor UserShield = Get(0xf505);

        /// User Slash
        static IconConstructor UserSlash = Get(0xf506);

        /// User Tag
        static IconConstructor UserTag = Get(0xf507);

        /// User Tie
        static IconConstructor UserTie = Get(0xf508);

        /// Remove User
        static IconConstructor UserTimes = Get(0xf235);

        /// Users
        static IconConstructor Users = Get(0xf0c0);

        /// Users Cog
        static IconConstructor UsersCog = Get(0xf509);

        /// Utensil Spoon
        static IconConstructor UtensilSpoon = Get(0xf2e5);

        /// Utensils
        static IconConstructor Utensils = Get(0xf2e7);

        /// Vector Square
        static IconConstructor VectorSquare = Get(0xf5cb);

        /// Venus
        static IconConstructor Venus = Get(0xf221);

        /// Venus Double
        static IconConstructor VenusDouble = Get(0xf226);

        /// Venus Mars
        static IconConstructor VenusMars = Get(0xf228);

        /// Vial
        static IconConstructor Vial = Get(0xf492);

        /// Vials
        static IconConstructor Vials = Get(0xf493);

        /// Video
        static IconConstructor Video = Get(0xf03d);

        /// Video Slash
        static IconConstructor VideoSlash = Get(0xf4e2);

        /// Vihara
        static IconConstructor Vihara = Get(0xf6a7);

        /// Volleyball Ball
        static IconConstructor VolleyballBall = Get(0xf45f);

        /// Volume Down
        static IconConstructor VolumeDown = Get(0xf027);

        /// Volume Mute
        static IconConstructor VolumeMute = Get(0xf6a9);

        /// Volume Off
        static IconConstructor VolumeOff = Get(0xf026);

        /// Volume Up
        static IconConstructor VolumeUp = Get(0xf028);

        /// Vote Yea
        static IconConstructor VoteYea = Get(0xf772);

        /// Cardboard VR
        static IconConstructor VrCardboard = Get(0xf729);

        /// Walking
        static IconConstructor Walking = Get(0xf554);

        /// Wallet
        static IconConstructor Wallet = Get(0xf555);

        /// Warehouse
        static IconConstructor Warehouse = Get(0xf494);

        /// Water
        static IconConstructor Water = Get(0xf773);

        /// Square Wave
        static IconConstructor WaveSquare = Get(0xf83e);

        /// Weight
        static IconConstructor Weight = Get(0xf496);

        /// Hanging Weight
        static IconConstructor WeightHanging = Get(0xf5cd);

        /// Wheelchair
        static IconConstructor Wheelchair = Get(0xf193);

        /// WiFi
        static IconConstructor Wifi = Get(0xf1eb);

        /// Wind
        static IconConstructor Wind = Get(0xf72e);

        /// Window Close
        static IconConstructor WindowClose = Get(0xf410);

        /// Window Maximize
        static IconConstructor WindowMaximize = Get(0xf2d0);

        /// Window Minimize
        static IconConstructor WindowMinimize = Get(0xf2d1);

        /// Window Restore
        static IconConstructor WindowRestore = Get(0xf2d2);

        /// Wine Bottle
        static IconConstructor WineBottle = Get(0xf72f);

        /// Wine Glass
        static IconConstructor WineGlass = Get(0xf4e3);

        /// Alternate Wine Glas
        static IconConstructor WineGlassAlt = Get(0xf5ce);

        /// Won Sign
        static IconConstructor WonSign = Get(0xf159);

        /// Wrench
        static IconConstructor Wrench = Get(0xf0ad);

        /// X-Ray
        static IconConstructor XRay = Get(0xf497);

        /// Yen Sign
        static IconConstructor YenSign = Get(0xf157);

        /// Yin Yang
        static IconConstructor YinYang = Get(0xf6ad);

        #undef Get
    }
};