#include "include/main.h"
#include "include/camera.h"
#include "include/geometry_utils.h"
#include "include/scene.h"
#include "include/transform.h"
#include "include/vector_utils.h"
#include "model_loader.h"
#include <stdio.h>
static const int FPS = 0;
#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240
typedef struct AppState {
  struct Framebuffer fb;
  float *depth_buffer;
  Camera cam;
  Scene s;
} AppState;

void Render(HWND hwnd) {
  AppState *state = (AppState *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  if (!state || !state->fb.pixels)
    return;

  Camera *cam = &state->cam;
  struct Framebuffer *fb = &state->fb;
  uint32_t *pixels = (uint32_t *)fb->pixels;
  float *db = state->depth_buffer;

  Camera_render(cam, &state->s, pixels, fb->width, fb->height, db);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  AppState *state = (AppState *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  switch (msg) {
  case WM_KEYDOWN: {
    Camera *cam = &state->cam;
    Scene *s = &state->s;
    switch (wParam) {
    case 'W': {
      Vec3D axis = get_Transform_basis_x(&cam->transform);
      rotate_Transform_around_point_axis(
          &cam->transform, Vec3D_XYZ(0.0f, 0.0f, -20.0f), 0.3f, axis);
      break;
    }
    case 'S': {
      Vec3D axis = get_Transform_basis_x(&cam->transform);
      rotate_Transform_around_point_axis(
          &cam->transform, Vec3D_XYZ(0.0f, 0.0f, -20.0f), -0.3f, axis);
      break;
    }
    case 'A': {
      rotate_Transform_around_point_y(&cam->transform,
                                      Vec3D_XYZ(0.0f, 0.0f, -20.0f), 0.3f);
      break;
    }
    case 'D': {
      rotate_Transform_around_point_y(&cam->transform,
                                      Vec3D_XYZ(0.0f, 0.0f, -20.0f), -0.3f);
      break;
    }
    }
    return 0;
  }

  case WM_KEYUP: {
    // stop movement, etc.
    return 0;
  }
  case WM_CREATE: {
    AppState *state = (AppState *)malloc(sizeof(AppState));
    if (!state)
      return -1;

    CREATESTRUCT *cs = (CREATESTRUCT *)lParam;
    Camera *cam_param = (Camera *)cs->lpCreateParams;
    state->cam = *cam_param;

    state->fb.width = RENDER_WIDTH;
    state->fb.height = RENDER_HEIGHT;
    state->fb.pixels = NULL;
    state->fb.memDC = NULL;
    state->fb.bitmap = NULL;

    Scene s = {0};
    Model m = {0};
    Model m2 = {0};

    m.transform = Transform_ZERO();
    m.transform.scale = Vec3D_XYZ(10.0f,10.0f,10.0f);
    m.transform.position.z = -40.0f;
    rotate_Transform_y(&m.transform, 1.570796);
    m.mesh_capacity = 8;
    m.meshes = malloc(sizeof(Triangle) * m.mesh_capacity);

    m2.transform = Transform_ZERO();
    m2.transform.scale = Vec3D_XYZ(5.0f,5.0f,5.0f);
    m2.transform.position.z = -20.0f;
    m2.mesh_capacity = 8;
    m2.meshes = malloc(sizeof(Triangle) * m2.mesh_capacity);

    load_Model_from_Obj(&m,"Primitives_raster/primitive_cube.obj");
    load_Model_from_Obj(&m2,"Primitives_raster/primitive_cube.obj");

    Scene_addModel(&s, m);
    Scene_addModel(&s, m2);

    state->s = s;

    HDC hdc = GetDC(hwnd);

    state->fb.memDC = CreateCompatibleDC(hdc);

    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = state->fb.width;
    bmi.bmiHeader.biHeight = -state->fb.height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    state->fb.bitmap =
        CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &state->fb.pixels, NULL, 0);

    state->depth_buffer =
        malloc(state->fb.width * state->fb.height * sizeof(float));

    SelectObject(state->fb.memDC, state->fb.bitmap);

    ReleaseDC(hwnd, hdc);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)state);

    return 0;
  }

  case WM_PAINT: {
    AppState *state = (AppState *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);

    int winW = rc.right - rc.left;
    int winH = rc.bottom - rc.top;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, winW, winH);
    HGDIOBJ oldBmp = SelectObject(memDC, memBitmap);

    // fill background in memory buffer
    HBRUSH bg = (HBRUSH)GetStockObject(GRAY_BRUSH);
    FillRect(memDC, &rc, bg);

    if (state) {
      float scaleX = (float)winW / state->fb.width;
      float scaleY = (float)winH / state->fb.height;
      float scale = (scaleX < scaleY) ? scaleX : scaleY;

      int dstW = (int)(state->fb.width * scale);
      int dstH = (int)(state->fb.height * scale);

      int offsetX = (winW - dstW) / 2;
      int offsetY = (winH - dstH) / 2;

      SetStretchBltMode(memDC, COLORONCOLOR);

      StretchBlt(memDC, offsetX, offsetY, dstW, dstH, state->fb.memDC, 0, 0,
                 state->fb.width, state->fb.height, SRCCOPY);
    }

    BitBlt(hdc, 0, 0, winW, winH, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBmp);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    EndPaint(hwnd, &ps);
    return 0;
  }
  case WM_SIZING: {
    RECT *rect = (RECT *)lParam;

    int width = rect->right - rect->left;
    int height = rect->bottom - rect->top;

    const float targetAspect = 320.0f / 240.0f;

    switch (wParam) {
    case WMSZ_RIGHT:
    case WMSZ_LEFT: {
      height = (int)(width / targetAspect);
      rect->bottom = rect->top + height;
    } break;

    case WMSZ_TOP:
    case WMSZ_BOTTOM: {
      width = (int)(height * targetAspect);
      rect->right = rect->left + width;
    } break;

    case WMSZ_TOPLEFT:
    case WMSZ_TOPRIGHT:
    case WMSZ_BOTTOMLEFT:
    case WMSZ_BOTTOMRIGHT: {
      float currentAspect = (float)width / (float)height;

      if (currentAspect > targetAspect) {
        width = (int)(height * targetAspect);
      } else {
        height = (int)(width / targetAspect);
      }

      rect->right = rect->left + width;
      rect->bottom = rect->top + height;
    } break;
    }

    return TRUE;
  }
  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY: {
    AppState *state = (AppState *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (state) {
      if (state->fb.bitmap) {
        DeleteObject(state->fb.bitmap);
        state->fb.bitmap = NULL;
      }

      if (state->fb.memDC) {
        DeleteDC(state->fb.memDC);
        state->fb.memDC = NULL;
      }

      for (int i = 0; i < state->s.model_length; i++) {
        free(state->s.models[i].meshes);
        state->s.models[i].meshes = NULL;
        state->s.models[i].mesh_capacity = 0;
        state->s.models[i].mesh_length = 0;
      }

      free(state->s.models);
      state->s.models = NULL;

      state->s.model_length = 0;
      state->s.model_capacity = 0;

      free(state);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
    }

    PostQuitMessage(0);
    return 0;
  }
  }

  return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {

  WNDCLASSA wc;
  ZeroMemory(&wc, sizeof(wc));

  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = "WindowTest_MainWindow";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  RegisterClassA(&wc);

  RECT r = {0, 0, RENDER_WIDTH, RENDER_HEIGHT};

  AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
  int width = r.right - r.left;
  int height = r.bottom - r.top;

  Camera c = Camera_new(RENDER_WIDTH, RENDER_HEIGHT, 1.047198f,
                        (float)RENDER_WIDTH / RENDER_HEIGHT, 0.1f, 1000.0f);

  HWND hwnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, "WindowTest_MainWindow",
                              "Test Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                              CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL,
                              NULL, hInstance, &c);

  ShowWindow(hwnd, nShowCmd);

  MSG msg;
  LARGE_INTEGER freq;
  LARGE_INTEGER lastTime;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&lastTime);
  while (1) {
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT)
        return 0;

      TranslateMessage(&msg);
      DispatchMessageA(&msg);
    }

    // --- dt start ---
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double dt =
        (double)(currentTime.QuadPart - lastTime.QuadPart) / freq.QuadPart;
    // printf("%f\n", dt);
    lastTime = currentTime;
    // --- dt end ---

    Render(hwnd);
    InvalidateRect(hwnd, NULL, FALSE);

    if (FPS > 0) {
      double target_dt = 1.0 / FPS;

      if (dt < target_dt) {
        Sleep((DWORD)((target_dt - dt) * 1000.0));
      }
    }
  }

  return (int)msg.wParam;
}
