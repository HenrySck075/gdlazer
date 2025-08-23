#pragma once

struct RectangleF {
    float x;
    float y;
    float width;
    float height;

    static RectangleF Empty;
};

RectangleF RectangleF::Empty {0,0,0,0};