#include "IntroTriangles.hpp"
#include "math.h"
#include "iostream"
   // this line is requested by catto
#define delayRepeat(duration, ...) CCSequence::create(__VA_ARGS__, CCDelayTime::create(duration), nullptr)

bool osuIntroTriangles::init() {
	// Intro text delays (in ms): 200 (wel), 400 (come), 700 ( to), 900 ( osu!), 1600 (triangle glitches)
	// Icons set showcase delays: 1450 (spaced out), 1650 (close together, larger), 1850 (larger)
	// osu! logo: ji

    auto label = CCLabelBMFont::create("","torus-light.fnt"_spr);
    //label->setFntFile("LazerFont.fnt"_spr);
    label->setID("welcomeText");
    label->setExtraKerning(5);

    auto d = CCDirector::sharedDirector()->getWinSize();
    label->setAnchorPoint(CCPoint{0.5,0.5});
    label->setPosition(d / 2);


#undef def_triangle_draw_node
#define delayCallFunc(delay, func) CCDelayTime::create(delay), CCCallFunc::create(this, callfunc_selector(func))
    auto seq = CCSequence::create(
        // welcome to osu! (Stage 1)
        delayCallFunc(this->text_1, osuIntroTriangles::text_1_func),
        delayCallFunc(this->text_2 - this->text_1, osuIntroTriangles::text_2_func),
        delayCallFunc(this->text_3 - this->text_2, osuIntroTriangles::text_3_func),
        delayCallFunc(this->text_4 - this->text_3, osuIntroTriangles::text_4_func),

        // text glitch triangles
        CCDelayTime::create(this->text_glitch - this->text_4),
        CCRepeat::create(
            delayRepeat(this->time_between_triangles, CCCallFunc::create(this, callfunc_selector(osuIntroTriangles::renderTriangles))),
            17
        ),

        // iconset showcase (Stage 2)
        CCCallFunc::create(this, callfunc_selector(osuIntroTriangles::rulesets_1_func)),
        delayCallFunc(this->rulesets_2 - this->rulesets_1, osuIntroTriangles::rulesets_2_func),
        delayCallFunc(this->rulesets_3 - this->rulesets_2, osuIntroTriangles::rulesets_3_func),

        // osu! logo (Stage 3)
        delayCallFunc(this->logo_1 - this->rulesets_3, osuIntroTriangles::logo_1_func),
        nullptr
    );
   
    auto trianglesNode = CCLayer::create();
    trianglesNode->setID("textGlitchNode");
    trianglesNode->setContentSize(CCSize{ 400,100 });
    trianglesNode->setPosition(d / 2);
    trianglesNode->setAnchorPoint(CCPoint{ 0.5,0.5 });
    trianglesNode->ignoreAnchorPointForPosition(false);
    this->addChild(label);
    this->addChild(trianglesNode);

    
    FMODAudioEngine::sharedEngine()->playMusic("triangles.mp3"_spr,false,0.f,0);
    this->runAction(seq);

    return true;
}

osuIntroTriangles* osuIntroTriangles::create() {
    auto ret = new osuIntroTriangles();
    if (ret && ret->init()) {
        ret->autorelease();
    }
    else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void osuIntroTriangles::text_1_func() {
    static_cast<CCLabelBMFont*>(this->getChildByID("welcomeText"))->setString("wel");
}
void osuIntroTriangles::text_2_func() {
    static_cast<CCLabelBMFont*>(this->getChildByID("welcomeText"))->setString("welcome");
}
void osuIntroTriangles::text_3_func() {
    static_cast<CCLabelBMFont*>(this->getChildByID("welcomeText"))->setString("welcome to");
}
void osuIntroTriangles::text_4_func() {
    static_cast<CCLabelBMFont*>(this->getChildByID("welcomeText"))->setString("welcome to osu!");
    auto a = osuCallFuncP::create(0,13,5,this, callfuncp_selector(osuIntroTriangles::text_4_set_spacing));
    a->setTag(7);
    this->runAction(a);
}
void osuIntroTriangles::text_4_set_spacing(float spacing) {
    auto welcomeTextNode = static_cast<CCLabelBMFont*>(this->getChildByID("welcomeText"));
    CCObject* obj;
    int c = 0;
    CCARRAY_FOREACH(welcomeTextNode->getChildren(), obj) {
        auto o = static_cast<CCNode*>(obj);
        o->setPositionX(o->getPositionX() + (spacing * c));
        c++;
    }
    welcomeTextNode->setContentSize(welcomeTextNode->getContentSize() + CCSize{ (spacing * c) , 0.f });
}

void osuIntroTriangles::renderTriangles() {
    auto node = static_cast<CCLayer*>(this->getChildByID("textGlitchNode"));

    int triangleCount = (int)(randomFloat()*3)+1; // m
    auto nodeSize = node->getContentSize();
    for (int d = 0; d < triangleCount; d++) {
        float dist = (randomFloat() * 20 + 5);
        CCPoint location = CCPoint{ randomFloat() * nodeSize.width + dist, randomFloat() * nodeSize.height + dist};
        
        auto spr = CCSprite::createWithSpriteFrameName(randomBool() ? "tri_outline.png"_spr : "tri_fill.png"_spr);
        spr->setScale(dist * 6.24 / 156);
        spr->setPosition(location);
        node->addChild(spr);

        spr->runAction(CCFadeOut::create(0.12));

        /*
        node->drawDot(location, 5, ccColor4F{1,1,1,1.f});
        CCPoint lt[2] = { location, top };
        CCPoint ll[2] = { location, left };
        CCPoint lr[2] = { location, right };
        node->drawLines(lt, 2, 3.f, ccColor4F{ 1,0,0,1 });
        node->drawLines(ll, 2, 3.f, ccColor4F{ 0,1,0.5,1 });
        node->drawLines(lr, 2, 3.f, ccColor4F{ 0,0,1,1 });
        */
    }


}


void osuIntroTriangles::rulesets_1_func() {
    // cancel every ongoing actions related to stage 1
    this->stopActionByTag(7);

    auto tgn = this->getChildByID("textGlitchNode");
    auto m = tgn->getChildren();
    auto l = tgn->getChildrenCount();
    for (int i = 0; i < l; i++) {
        static_cast<CCNode*>(m->objectAtIndex(i))->stopAllActions();
    }
    this->removeAllChildren();
    //static_cast<CCNode*>(this->getChildByID("welcomeText"))->setVisible(false);

    auto n = CCLayer::create();
    n->setID("iconsets");
    n->setLayout(RowLayout::create()->setGap(50));
    n->setScale(0.7);
    n->setAnchorPoint(CCPoint{ 0.5,0.5 });
    n->setPosition(CCDirector::get()->getWinSize() / 2);
    n->ignoreAnchorPointForPosition(false);

    #define addIcon(name) n->addChild(CCSprite::createWithSpriteFrameName(name))

    addIcon("cube.png"_spr);
    addIcon("ship.png"_spr);
    addIcon("ball.png"_spr);
    addIcon("ufo.png"_spr);
    addIcon("wave.png"_spr);
    addIcon("robot.png"_spr);
    addIcon("spider.png"_spr);
    addIcon("swing.png"_spr);
    addIcon("jetpack.png"_spr);
    n->updateLayout();
    auto a = osuCallFuncP::create(50, 30, this->rulesets_2 - this->rulesets_1 + 0.3, this, callfuncp_selector(osuIntroTriangles::rulesets_1_change_gap));
    a->setTag(9);
    this->runAction(a);

    this->addChild(n);
}
void osuIntroTriangles::rulesets_1_change_gap(float gap) {
    auto n = this->getChildByID("iconsets");
    n->setLayout(RowLayout::create()->setGap(gap));
    n->updateLayout();
}
void osuIntroTriangles::rulesets_2_func() {
    auto n = this->getChildByID("iconsets");
    this->stopActionByTag(9);
    n->setLayout(RowLayout::create()->setGap(15));
    n->setScale(1.1);
    n->updateLayout();
}
void osuIntroTriangles::rulesets_3_func() {
    auto n = this->getChildByID("iconsets");
    n->setLayout(RowLayout::create()->setGap(5));
    n->setScale(1.6);
    n->updateLayout();
}

void osuIntroTriangles::logo_1_func() {
    this->getChildByID("iconsets")->removeFromParent();
}