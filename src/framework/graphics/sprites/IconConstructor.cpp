#include "IconConstructor.hpp"

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

struct tCCFontDefHashElementExt : public cocos2d::tCCFontDefHashElement
{
	ccBMFontDefExt  fontDef;	// font definition
} ;


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
    tCCFontDefHashElementExt *m_pFontDefDictionary;

    //! FNTConfig: Common Height Should be signed (issue #1343)
    int m_nCommonHeight;
    //! Padding
    cocos2d::ccBMFontPadding    m_tPadding;
    //! atlas name
    std::vector<std::string> m_sAtlasNames;
    //! values for kerning
    cocos2d::tCCKerningHashElement *m_pKerningDictionary;
    
    // Character Set defines the letters that actually exist in the font
    std::set<unsigned int> *m_pCharacterSet;
public:
    BMGlyphFontConfig() : m_pFontDefDictionary(NULL)
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
        this->purgeFontDefDictionary();
        this->purgeKerningDictionary();
        m_sAtlasNames.clear();
        CC_SAFE_DELETE(m_pCharacterSet);
    };
    /**
     *  @js NA
     *  @lua NA
     */
    const char * description(){
        return fmt::format(
            "<BMGlyphFontConfig [BMGlyphManager] | Glphys:{} Kernings:{} | Images = {}>",
            HASH_COUNT(m_pFontDefDictionary),
            HASH_COUNT(m_pKerningDictionary),
            m_sAtlasNames
        ).c_str();
    };

    /** allocates a BMGlyphFontConfig with a FNT file */
    static BMGlyphFontConfig * create(const char *FNTfile) {
        create_class(BMGlyphFontConfig, initWithFNTfile, FNTfile);
    };

    /** initializes a BitmapFontConfiguration with a FNT file */
    bool initWithFNTfile(const char *FNTfile){
        m_pKerningDictionary = NULL;
        m_pFontDefDictionary = NULL;
        
        m_pCharacterSet = this->parseConfigFile(FNTfile);
        
        if (! m_pCharacterSet)
        {
            return false;
        }

        return true;
    };
    
    inline std::vector<std::string> getAtlasNames(){ return m_sAtlasNames; }
    //inline void setAtlasName(const char* atlasName) { m_sAtlasName = atlasName; }
    
    std::set<unsigned int>* getCharacterSet() const {return m_pCharacterSet;};
private:
    std::set<unsigned int>* parseConfigFile(const char *controlFile){    
        std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(controlFile,false);
        CCString *contents = CCString::createWithContentsOfFile(fullpath.c_str());

        CCAssert(contents, "BMGlyphFontConfig::parseConfigFile | Open file error.");
        
        std::set<unsigned int> *validCharsString = new std::set<unsigned int>();

        if (!contents)
        {
            CCLOG("cocos2d: Error parsing FNTfile %s", controlFile);
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
                //            [self parseInfoArguments:line];
                this->parseInfoArguments(line);
            }
            // Check to see if the start of the line is something we are interested in
            else if(line.substr(0,strlen("common lineHeight")) == "common lineHeight")
            {
                this->parseCommonArguments(line);
            }
            else if(line.substr(0,strlen("page id")) == "page id")
            {
                this->parseImageFileName(line, controlFile);
            }
            else if(line.substr(0,strlen("chars c")) == "chars c")
            {
                // Ignore this line
            }
            else if(line.substr(0,strlen("char")) == "char")
            {
                // Parse the current line and create a new CharDef
                tCCFontDefHashElementExt* element = (tCCFontDefHashElementExt*)malloc( sizeof(*element) );
                this->parseCharacterDefinition(line, &element->fontDef);

                element->key = element->fontDef.charID;
                HASH_ADD_INT(m_pFontDefDictionary, key, element);
                
                validCharsString->insert(element->fontDef.charID);
            }
    //        else if(line.substr(0,strlen("kernings count")) == "kernings count")
    //        {
    //            this->parseKerningCapacity(line);
    //        }
            else if(line.substr(0,strlen("kerning first")) == "kerning first")
            {
                this->parseKerningEntry(line);
            }
        }
        
        return validCharsString;
    };
    void parseCharacterDefinition(std::string line, ccBMFontDefExt *characterDefinition){    
        //////////////////////////////////////////////////////////////////////////
        // line to parse:
        // char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=44    xadvance=14     page=0  chnl=0 
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
        sscanf(value.c_str(), "page=%f", &characterDefinition->page);
    };
    void parseInfoArguments(std::string line){
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
    void parseCommonArguments(std::string line){
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
    void parseImageFileName(std::string line, const char *fntFile){
        //////////////////////////////////////////////////////////////////////////
        // line to parse:
        // page id=0 file="bitmapFontTest.png"
        //////////////////////////////////////////////////////////////////////////

        // page ID. Sanity check
        int index = line.find('=')+1;
        int index2 = line.find(' ', index);
        std::string value = line.substr(index, index2-index);
        CCAssert(atoi(value.c_str()) == 0, "LabelBMFont file could not be found");
        // file 
        index = line.find('"')+1;
        index2 = line.find('"', index);
        value = line.substr(index, index2-index);

        m_sAtlasNames.push_back(CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(value.c_str(), fntFile));
    };
    void parseKerningEntry(std::string line){        
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
    void purgeKerningDictionary(){
        cocos2d::tCCKerningHashElement *current;
        while(m_pKerningDictionary) 
        {
            current = m_pKerningDictionary; 
            HASH_DEL(m_pKerningDictionary,current);
            free(current);
        }
    };
    void purgeFontDefDictionary(){    
        tCCFontDefHashElementExt *current, *tmp;

        HASH_ITER(hh, m_pFontDefDictionary, current, tmp) {
            HASH_DEL(m_pFontDefDictionary, current);
            free(current);
        }
    };
};

static CCDictionaryExt<std::string, BMGlyphFontConfig*> s_pConfigurations;

BMGlyphFontConfig* FNTConfigLoadFile2( const char *fntFile) {
    BMGlyphFontConfig* pRet = NULL;

    pRet = s_pConfigurations[fntFile].operator->();
    if(!pRet) {
        pRet = BMGlyphFontConfig::create(fntFile);
        if (pRet) {
            s_pConfigurations[fntFile] = pRet;
        }        
    }

    return pRet;
}

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
    static CCDictionaryExt<std::string, BMGlyphManager*> s_pGlyphs;
public:
    /**
     *  @js ctor
     */
    BMGlyphManager()
    : m_pConfiguration(nullptr)
    , m_bLineBreakWithoutSpaces(false)
    , m_tImageOffset(CCPointZero)
    {

    };
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~BMGlyphManager();
    /** Purges the cached data.
    Removes from memory the cached configurations and the atlas name dictionary.
    @since v0.99.3
    */
    static void purgeCachedData() {};

    static BMGlyphManager* getForFontName(const char *fntFile) {
        if (!s_pGlyphs.contains(fntFile)) {
            s_pGlyphs.inner();
        }
        return s_pGlyphs[fntFile];
    }
private:
    /** creates a bitmap font atlas with an initial string and the FNT file */
    static BMGlyphManager* create(const char *fntFile, CCPoint imageOffset = {0,0}) {
        create_class(BMGlyphManager, init, fntFile, imageOffset);
    };

    /** init a bitmap font atlas with an initial string and the FNT file */
    bool init(const char *fntFile, CCPoint imageOffset = CCPointZero) {
        assert( ("Invalid params for CCLabelBMFont", fntFile==NULL));
        
        if (fntFile)
        {
            BMGlyphFontConfig *newConf = FNTConfigLoadFile2(fntFile);
            if (!newConf)
            {
                CCLOG("cocos2d: WARNING. CCLabelBMFont: Impossible to create font. Please check file: '%s'", fntFile);
                release();
                return false;
            }
            
            newConf->retain();
            CC_SAFE_RELEASE(m_pConfiguration);
            m_pConfiguration = newConf;
            
            m_sFntFile = fntFile;
            
            for (std::string& t : m_pConfiguration->getAtlasNames()) {
                if (!CCTextureCache::sharedTextureCache()->addImage(t.c_str(),false)) {
                    log::error("[BMGlyphManager]: No font bitmap named {}", t);
                    return false;
                };
            }
        }

        m_tImageOffset = imageOffset;
        
        return true;

    };
public:
    /** updates the font chars based on the string to render */
    void createFontChars();
    
    CCSprite* getCharacter(int id);

    virtual void updateLabel();

    void setFntFile(const char* fntFile);
    const char* getFntFile();
	BMGlyphFontConfig* getConfiguration() const;
#if CC_LABELBMFONT_DEBUG_DRAW
    virtual void draw();
#endif // CC_LABELBMFONT_DEBUG_DRAW
private:
    char * atlasNameFromFntFile(const char *fntFile);
    int kerningAmountForFirst(unsigned short first, unsigned short second);
    float getLetterPosXLeft( CCSprite* characterSprite );
    float getLetterPosXRight( CCSprite* characterSprite );
    
protected:    
    // name of fntFile
    std::string m_sFntFile;
    
    // initial string without line breaks
    unsigned short* m_sInitialString;
    std::string m_sInitialStringUTF8;
    
    BMGlyphFontConfig *m_pConfiguration;
    
    bool m_bLineBreakWithoutSpaces;
    // offset of the texture atlas
    CCPoint    m_tImageOffset;
    
    // we can't reuse chars here, since we need to generate a new one every `getCharacter`

    // texture RGBA
    GLubyte m_cDisplayedOpacity;
    GLubyte m_cRealOpacity;
    ccColor3B m_tDisplayedColor;
    ccColor3B m_tRealColor;
    bool m_bCascadeColorEnabled;
    bool m_bCascadeOpacityEnabled;
    /** conforms to CCRGBAProtocol protocol */
    bool        m_bIsOpacityModifyRGB;

};

/** Free function that parses a FNT file a place it on the cache
*/
CC_DLL BMGlyphFontConfig * FNTConfigLoadFile( const char *file );
/** Purges the FNT config cache
*/
CC_DLL void FNTConfigRemoveCache( void );

// end of GUI group
/// @}
/// @}

// ------------------ End BMGlyphManager ------------------

std::string unicode_to_utf8(int unicode)
{
    std::string s;

    if (unicode>=0 && unicode <= 0x7f)  // 7F(16) = 127(10)
    {
        s = static_cast<char>(unicode);

        return s;
    }
    else if (unicode <= 0x7ff)  // 7FF(16) = 2047(10)
    {
        unsigned char c1 = 192, c2 = 128;

        for (int k=0; k<11; ++k)
        {
            if (k < 6)
                c2 |= (unicode % 64) & (1 << k);
            else
                c1 |= (unicode >> 6) & (1 << (k - 6));
        }

        s = c1;
        s += c2;

        return s;
    }
    else if (unicode <= 0xffff)  // FFFF(16) = 65535(10)
    {
        unsigned char c1 = 224, c2 = 128, c3 = 128;

        for (int k=0; k<16; ++k)
        {
            if (k < 6)
                c3 |= (unicode % 64) & (1 << k);
            else if
                (k < 12) c2 |= (unicode >> 6) & (1 << (k - 6));
            else
                c1 |= (unicode >> 12) & (1 << (k - 12));
        }

        s = c1;
        s += c2;
        s += c3;

        return s;
    }
    else if (unicode <= 0x1fffff)  // 1FFFFF(16) = 2097151(10)
    {
        unsigned char c1 = 240, c2 = 128, c3 = 128, c4 = 128;

        for (int k=0; k<21; ++k)
        {
            if (k < 6)
                c4 |= (unicode % 64) & (1 << k);
            else if (k < 12)
                c3 |= (unicode >> 6) & (1 << (k - 6));
            else if (k < 18)
                c2 |= (unicode >> 12) & (1 << (k - 12));
            else
                c1 |= (unicode >> 18) & (1 << (k - 18));
        }

        s = c1;
        s += c2;
        s += c3;
        s += c4;

        return s;
    }
    else if (unicode <= 0x3ffffff)  // 3FFFFFF(16) = 67108863(10)
    {
        ;  // Actually, there are no 5-bytes unicodes
    }
    else if (unicode <= 0x7fffffff)  // 7FFFFFFF(16) = 2147483647(10)
    {
        ;  // Actually, there are no 6-bytes unicodes
    }
    else
        ;  // Incorrect unicode (< 0 or > 2147483647)

    return "";
}


IconConstructor::operator CCSprite*() {
    const char* g = glyphChar.c_str();
    const char* f = fontName.c_str();
    // since CCDictionaryExt internally assigns a CCNode as the value
    BMGlyphManager* label = BMGlyphManager::getForFontName(fontName);
    CCSprite* ret = nullptr;
    if (!label) {
        geode::log::error("[IconConstructor]: Font {} does not exist (according to cocos2d)", fontName);
        delete label;
        label = nullptr;
    } else {
        // that will add the glyph sprite, so we can extract it now
        CCSprite* c = label->getCharacter(unicode);
        ret = CCSprite::createWithTexture(c->getTexture());
        //ret->setTextureRect(c->getTextureRect());
    }
    return ret;
}