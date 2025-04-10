#include "BMGlyphManager.hpp"

static CCDictionaryExt<std::string, BMGlyphFontConfig*> s_dConfigurations;

BMGlyphFontConfig* FNTConfigLoadFile2( std::string fntFile) {
  BMGlyphFontConfig* pRet = NULL;
  //DebugBreak();
  if(!s_dConfigurations.contains(fntFile)) {
    pRet = BMGlyphFontConfig::create(fntFile);
    if (pRet) {
      s_dConfigurations[fntFile] = pRet;
    }    
  } else {
    pRet = s_dConfigurations[fntFile];
  }

  return pRet;
}

void BMGlyphFontConfig::parseCharacterDefinition(std::string line, ccBMFontDefExt *characterDefinition) {
  //////////////////////////////////////////////////////////////////////////
  // line to parse:
  // char id=32   x=0   y=0   width=0   height=0   xoffset=0   yoffset=44  xadvance=14   page=0  chnl=0 
  //////////////////////////////////////////////////////////////////////////

  // Character ID
  int index = line.find("id=");
  int index2 = line.find(' ', index);
  std::string value = line.substr(index, index2-index);
  sscanf(value.c_str(), "id=%u", &characterDefinition->charID);

  // Character x
  index = line.find("x=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "x=%f", &characterDefinition->rect.origin.x);
  // Character y
  index = line.find("y=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "y=%f", &characterDefinition->rect.origin.y);
  // Character width
  index = line.find("width=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "width=%f", &characterDefinition->rect.size.width);
  // Character height
  index = line.find("height=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "height=%f", &characterDefinition->rect.size.height);
  // Character xoffset
  index = line.find("xoffset=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "xoffset=%hd", &characterDefinition->xOffset);
  // Character yoffset
  index = line.find("yoffset=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "yoffset=%hd", &characterDefinition->yOffset);
  // Character xadvance
  index = line.find("xadvance=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "xadvance=%hd", &characterDefinition->xAdvance);
  // Character page
  index = line.find("page=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "page=%u", &characterDefinition->page);
};

std::set<unsigned int>* BMGlyphFontConfig::parseConfigFile(std::string controlFile) {  
  std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(controlFile.c_str(),false);
  CCString *contents = CCString::createWithContentsOfFile(fullpath.c_str());

  if (!contents) {
    log::error("[BMGlyphFontConfig::parseConfigFile]: Open file error.");
    return {};
  }
  
  std::set<unsigned int> *validCharsString = new std::set<unsigned int>();

  if (!contents)
  {
    //CCLOG("cocos2d: Error parsing FNTfile %s", controlFile);
    return nullptr;
  }

  // parse spacing / padding
  std::string line;
  std::string strLeft = contents->getCString();
  while (strLeft.length() > 0)
  {
    int pos = strLeft.find('\n');

    if (pos != (int)std::string::npos)
    {
      // the data is more than a line.get one line
      line = strLeft.substr(0, pos);
      strLeft = strLeft.substr(pos + 1);
    }
    else
    {
      // get the left data
      line = strLeft;
      strLeft.erase();
    }

    if(line.substr(0,strlen("info face")) == "info face") 
    {
      // XXX: info parsing is incomplete
      // Not needed for the Hiero editors, but needed for the AngelCode editor
      //      [self parseInfoArguments:line];
      parseInfoArguments(line);
    }
    // Check to see if the start of the line is something we are interested in
    else if(line.substr(0,strlen("common lineHeight")) == "common lineHeight")
    {
      parseCommonArguments(line);
    }
    else if(line.substr(0,strlen("page id")) == "page id")
    {
      parseImageFileName(line, controlFile);
    }
    else if(line.substr(0,strlen("chars c")) == "chars c")
    {
      // Ignore this line
    }
    else if(line.substr(0,strlen("char")) == "char")
    {
      // Parse the current line and create a new CharDef
      ccBMFontDefExt fontDef;
      parseCharacterDefinition(line, &fontDef);

      m_pFontDefDictionary[fontDef.charID] = fontDef;
      
      validCharsString->insert(fontDef.charID);
    }
//    else if(line.substr(0,strlen("kernings count")) == "kernings count")
//    {
//      parseKerningCapacity(line);
//    }
    else if(line.substr(0,strlen("kerning first")) == "kerning first")
    {
      parseKerningEntry(line);
    }
  }
  
  return validCharsString;
};

void BMGlyphFontConfig::parseInfoArguments(std::string line) {
  //////////////////////////////////////////////////////////////////////////
  // possible lines to parse:
  // info face="Script" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=1,4,3,2 spacing=0,0 outline=0
  // info face="Cracked" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
  //////////////////////////////////////////////////////////////////////////

  // padding
  int index = line.find("padding=");
  int index2 = line.find(' ', index);
  std::string value = line.substr(index, index2-index);
  sscanf(value.c_str(), "padding=%d,%d,%d,%d", &m_tPadding.top, &m_tPadding.right, &m_tPadding.bottom, &m_tPadding.left);
  CCLOG("cocos2d: padding: %d,%d,%d,%d", m_tPadding.left, m_tPadding.top, m_tPadding.right, m_tPadding.bottom);
};

void BMGlyphFontConfig::parseCommonArguments(std::string line) {
  //////////////////////////////////////////////////////////////////////////
  // line to parse:
  // common lineHeight=104 base=26 scaleW=1024 scaleH=512 pages=1 packed=0
  //////////////////////////////////////////////////////////////////////////

  // Height
  int index = line.find("lineHeight=");
  int index2 = line.find(' ', index);
  std::string value = line.substr(index, index2-index);
  sscanf(value.c_str(), "lineHeight=%d", &m_nCommonHeight);
  // scaleW. sanity check
  index = line.find("scaleW=") + strlen("scaleW=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  CCAssert(atoi(value.c_str()) <= CCConfiguration::sharedConfiguration()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");
  // scaleH. sanity check
  index = line.find("scaleH=") + strlen("scaleH=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  CCAssert(atoi(value.c_str()) <= CCConfiguration::sharedConfiguration()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");

  // packed (ignore) What does this mean ??
};

void BMGlyphFontConfig::parseImageFileName(std::string line, std::string fntFile) {
  //////////////////////////////////////////////////////////////////////////
  // line to parse:
  // page id=0 file="bitmapFontTest.png"
  //////////////////////////////////////////////////////////////////////////

  int pageId = 0;
  // page ID
  int index = line.find('=')+1;
  int index2 = line.find(' ', index);
  std::string value = line.substr(index, index2-index);
  pageId = std::stoi(value);
  // file 
  index = line.find('"')+1;
  index2 = line.find('"', index);
  value = line.substr(index, index2-index);
  m_sAtlasNames[pageId] = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(value.c_str(), fntFile.c_str());
};

void BMGlyphFontConfig::parseKerningEntry(std::string line) {    
  //////////////////////////////////////////////////////////////////////////
  // line to parse:
  // kerning first=121  second=44  amount=-7
  //////////////////////////////////////////////////////////////////////////

  // first
  int first;
  int index = line.find("first=");
  int index2 = line.find(' ', index);
  std::string value = line.substr(index, index2-index);
  sscanf(value.c_str(), "first=%d", &first);

  // second
  int second;
  index = line.find("second=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "second=%d", &second);

  // amount
  int amount;
  index = line.find("amount=");
  index2 = line.find(' ', index);
  value = line.substr(index, index2-index);
  sscanf(value.c_str(), "amount=%d", &amount);

  cocos2d::tCCKerningHashElement *element = (cocos2d::tCCKerningHashElement *)calloc( sizeof( *element ), 1 );
  element->amount = amount;
  element->key = (first<<16) | (second&0xffff);
  HASH_ADD_INT(m_pKerningDictionary,key, element);
};

void BMGlyphFontConfig::purgeKerningDictionary() {
  cocos2d::tCCKerningHashElement *current;
  while(m_pKerningDictionary) 
  {
    current = m_pKerningDictionary; 
    HASH_DEL(m_pKerningDictionary,current);
    free(current);
  }
};

void BMGlyphFontConfig::purgeFontDefDictionary() {  
  m_pFontDefDictionary.clear();
};

GDF_NS_START

static CCDictionaryExt<std::string, BMGlyphManager*> s_dGlyphs;

BMGlyphManager* BMGlyphManager::getForFontName(std::string fntFile) {
  if (!s_dGlyphs.contains(fntFile)) {
    s_dGlyphs.inner()->setObject(BMGlyphManager::create(fntFile), fntFile);
  }
  return s_dGlyphs[fntFile];
}

bool BMGlyphManager::init(std::string fntFile, CCPoint imageOffset) {
  log::debug("[BMGlyphManager]: {}", fntFile);
  //assert( ("Invalid params for CCLabelBMFont", fntFile==nullptr));
  
  // idk what else
  if (!fntFile.empty())
  {
    BMGlyphFontConfig *newConf = FNTConfigLoadFile2(fntFile);
    if (!newConf)
    {
      log::debug("[BMGlyphManager]: Cannot create font '{}'", fntFile);
      release();
      return false;
    }
    
    m_pConfiguration = newConf;
    
    m_sFntFile = fntFile;
    
    for (auto const& [pageId, t] : m_pConfiguration->getAtlasNames()) {
      auto goog = CCTextureCache::sharedTextureCache()->addImage(t.c_str(),false);
      if (goog == nullptr) {
        log::error("[BMGlyphManager]: No font bitmap named {}", t);
        return false;
      };
      m_dTextures.inner()->setObject(goog, pageId);
    }
  }

  m_tImageOffset = imageOffset;
  
  return true;

};

CCResizableSprite* BMGlyphManager::getCharacter(int id) {
  CCResizableSprite* spr = nullptr;
  ccBMFontDefExt charDef;
  CCTexture2D* texture;
  //if (IsDebuggerPresent()) DebugBreak();

  if (m_pConfiguration->m_pFontDefDictionary.contains(id)) {
    charDef = m_pConfiguration->m_pFontDefDictionary[id];
    texture = m_dTextures[charDef.page];
    spr = CCResizableSprite::createWithTexture(texture);
    spr->setTextureRect(CC_RECT_PIXELS_TO_POINTS(charDef.rect));
    auto quality = CCDirector::get()->getLoadedTextureQuality();
    spr->setWrappedSpriteScale(
      quality == kTextureQualityHigh
      ? 1 : quality == kTextureQualityMedium 
      ? 0.5 : 0.25
    );
  } else {
  }
  return spr;
}
GDF_NS_END