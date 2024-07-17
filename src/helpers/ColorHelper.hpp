#pragma once

#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class CCColorExtension {
  public:
  static ccColor3B color3FromHex(int h) {
    return ccc3(
        ((h >> 16) & 0xFF) / 255.0, // Extract the RR byte
        ((h >> 8) & 0xFF) / 255.0, // Extract the GG byte
        ((h) & 0xFF) / 255.0  // Extract the BB byte
    );
  }
  static ccColor4B color4FromHex(int h) {
    
    return ccc4( 
        ((h >> 24) & 0xFF) / 255.0, // Extract the RR byte
        ((h >> 16) & 0xFF) / 255.0, // Extract the GG byte
        ((h >> 8) & 0xFF) / 255.0,  // Extract the BB byte
        ((h) & 0xFF) / 255.0        // Extract the AA byte
    );
  }
};
