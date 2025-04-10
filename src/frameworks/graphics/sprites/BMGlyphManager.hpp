
/**
 * BMGlyphManager
 */

#include <Geode/cocos/sprite_nodes/CCSpriteBatchNode.h>
#include <Geode/cocos/support/data_support/uthash.h>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#include "../../../utils.hpp"
#include "CCResizableSprite.hpp"

/**
 * @addtogroup GUI
 * @{
 * @addtogroup label
 * @{
 */

/**
@struct ccBMFontDef
BMFont definition
*/
struct ccBMFontDefExt : public cocos2d::ccBMFontDef {
  //! The page number this character lives
  unsigned int page;
};


/** @brief BMGlyphFontConfig is a modification of BMGlyphFontConfig
@since v0.8
@js NA
@lua NA
*/
class BMGlyphFontConfig : public cocos2d::CCObject
{
  // XXX: Creating a public interface so that the bitmapFontArray[] is accessible
public://@public
  // BMFont definitions
  std::map<int, ccBMFontDefExt> m_pFontDefDictionary;

  //! FNTConfig: Common Height Should be signed (issue #1343)
  int m_nCommonHeight;
  //! Padding
  cocos2d::ccBMFontPadding  m_tPadding;
  //! atlas name
  std::map<int, std::string> m_sAtlasNames;
  //! values for kerning
  cocos2d::tCCKerningHashElement *m_pKerningDictionary;
  
  // Character Set defines the letters that actually exist in the font
  std::set<unsigned int> *m_pCharacterSet;
public:
  BMGlyphFontConfig() : m_pFontDefDictionary()
  , m_nCommonHeight(0)
  , m_pKerningDictionary(NULL)
  , m_pCharacterSet(NULL)
  {

  };
  /**
   *  @js NA
   *  @lua NA
   */
  virtual ~BMGlyphFontConfig() {
    geode::log::debug( "[IconConstructor]: deallocing BMGlyphFontConfig" );
    purgeFontDefDictionary();
    purgeKerningDictionary();
    m_sAtlasNames.clear();
    CC_SAFE_DELETE(m_pCharacterSet);
  };
  /**
   *  @js NA
   *  @lua NA
   */
  std::string  description(){
    return fmt::format(
      "<BMGlyphFontConfig [BMGlyphManager] | Glphys:{} Kernings:{} | Images = {}>",
      m_pFontDefDictionary.size(),
      HASH_COUNT(m_pKerningDictionary),
      m_sAtlasNames
    ).c_str();
  };

  /** allocates a BMGlyphFontConfig with a FNT file */
  static BMGlyphFontConfig * create(std::string FNTfile) {
    $create_class(BMGlyphFontConfig, initWithFNTfile, FNTfile);
  };

  /** initializes a BitmapFontConfiguration with a FNT file */
  bool initWithFNTfile(std::string FNTfile){
    m_pKerningDictionary = NULL;
    m_pFontDefDictionary = {};
    
    m_pCharacterSet = parseConfigFile(FNTfile);
    
    if (! m_pCharacterSet)
    {
      return false;
    }

    return true;
  };
  
  inline std::map<int, std::string> getAtlasNames(){ return m_sAtlasNames; }
  //inline void setAtlasName(const char* atlasName) { m_sAtlasName = atlasName; }
  
  std::set<unsigned int>* getCharacterSet() const {return m_pCharacterSet;};
private:
  std::set<unsigned int>* parseConfigFile(std::string controlFile);
  void parseCharacterDefinition(std::string line, ccBMFontDefExt *characterDefinition);
  void parseInfoArguments(std::string line);
  void parseCommonArguments(std::string line);
  void parseImageFileName(std::string line, std::string fntFile);
  void parseKerningEntry(std::string line);
  void purgeKerningDictionary();
  void purgeFontDefDictionary();
};


GDF_NS_START
/** @brief BMGlyphManager is a modification of BMGlyphManager that supports multiple pages
 * and allows you to extract a character texture from it.
 * it does not allows rendering tho

Supported editors:
http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
http://www.angelcode.com/products/bmfont/ (Free, Windows only)

@since v0.8
*/
class BMGlyphManager : public cocos2d::CCObject
{
public:
  /**
   *  @js ctor
   */
  BMGlyphManager()
  : m_pConfiguration(nullptr)
  , m_tImageOffset(CCPointZero)
  {

  };
  /**
   *  @js NA
   *  @lua NA
   */
  virtual ~BMGlyphManager() {
    CC_SAFE_RELEASE(m_pConfiguration);
  };
  /** Purges the cached data.
  Removes from memory the cached configurations and the atlas name dictionary.
  @since v0.99.3
  */
  static void purgeCachedData() {};

  static BMGlyphManager* getForFontName(std::string fntFile);
private:
  /** creates a bitmap font atlas with an initial string and the FNT file */
  static BMGlyphManager* create(std::string fntFile, CCPoint imageOffset = {0,0}) {
    $create_class(BMGlyphManager, init, fntFile, imageOffset);
  };

  /** init a bitmap font atlas with an initial string and the FNT file */
  bool init(std::string fntFile, CCPoint imageOffset = CCPointZero);
public:
  CCResizableSprite* getCharacter(int id);

	BMGlyphFontConfig* getConfiguration() const {return m_pConfiguration;};
  
  /// this will saves some iterations :3
  CCDictionaryExt<int, CCTexture2D*> m_dTextures;
protected:  
  // name of fntFile
  std::string m_sFntFile;
  
  BMGlyphFontConfig *m_pConfiguration;
  
  // offset of the texture atlas
  CCPoint  m_tImageOffset;
  
  // we can't reuse chars here, since we need to generate a new one every `getCharacter`
};
GDF_NS_END