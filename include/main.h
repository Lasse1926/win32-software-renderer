#pragma once

#include <stdint.h>
#include <windows.h>
#include <winuser.h>

struct Framebuffer {
    int width;
    int height;
    void* pixels;
    HDC memDC;
    HBITMAP bitmap;
};
