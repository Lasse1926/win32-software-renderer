#include "include/main.h"
#include "include/camera.h"
#include "include/geometry_utils.h"
#include "include/scene.h"
#include "include/transform.h"
#include "include/vector_utils.h"
#define FPS 100
typedef struct AppState {
  struct Framebuffer fb;
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

  Camera_render(cam, &state->s, pixels, fb->width, fb->height);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  AppState *state = (AppState *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  switch (msg) {
  case WM_KEYDOWN: {
    Camera *cam = &state->cam;
    Scene *s = &state->s;
    switch (wParam) {
    case 'W':
      rotate_Transform_x(&s->transform, 0.1);
      break;
    case 'S':
      rotate_Transform_x(&s->transform, -0.1);
      break;
    case 'A':
      rotate_Transform_y(&s->transform, 0.1);
      break;
    case 'D':
      rotate_Transform_y(&s->transform, -0.1);
      break;
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

    state->fb.width = 500;
    state->fb.height = 500;
    state->fb.pixels = NULL;
    state->fb.memDC = NULL;
    state->fb.bitmap = NULL;
    // color format 0xAARRGGBB
    uint32_t red     = 0x00FF2020;
    uint32_t green   = 0x0020FF20;
    uint32_t blue    = 0x002020FF;
    uint32_t yellow  = 0x0020FFFF;
    uint32_t cyan    = 0x00FFFF20;
    uint32_t magenta = 0x00FF20FF;

    // Front face
    Triangle t0;
    t0.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, -5.0f)};
    t0.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, -5.0f)};
    t0.vertices[0] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, -5.0f)};
    t0.color = red;

    Triangle t1;
    t1.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, -5.0f)};
    t1.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, -5.0f)};
    t1.vertices[0] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, -5.0f)};
    t1.color = red;

    // Back face
    Triangle t2;
    t2.vertices[2] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, 5.0f)};
    t2.vertices[1] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, 5.0f)};
    t2.vertices[0] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, 5.0f)};
    t2.color = green;

    Triangle t3;
    t3.vertices[2] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, 5.0f)};
    t3.vertices[1] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, 5.0f)};
    t3.vertices[0] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, 5.0f)};
    t3.color = green;

    // Left face
    Triangle t4;
    t4.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, 5.0f)};
    t4.vertices[1] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, -5.0f)};
    t4.vertices[0] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, -5.0f)};
    t4.color = blue;

    Triangle t5;
    t5.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, 5.0f)};
    t5.vertices[1] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, -5.0f)};
    t5.vertices[0] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, 5.0f)};
    t5.color = blue;

    // Right face
    Triangle t6;
    t6.vertices[2] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, -5.0f)};
    t6.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, 5.0f)};
    t6.vertices[0] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, 5.0f)};
    t6.color = yellow;

    Triangle t7;
    t7.vertices[2] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, -5.0f)};
    t7.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, 5.0f)};
    t7.vertices[0] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, -5.0f)};
    t7.color = yellow;

    // Top face
    Triangle t8;
    t8.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, -5.0f)};
    t8.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, -5.0f)};
    t8.vertices[0] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, 5.0f)};
    t8.color = cyan;

    Triangle t9;
    t9.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, -5.0f)};
    t9.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, 5.0f, 5.0f)};
    t9.vertices[0] = (Vertex){.position = Vec3D_XYZ(-5.0f, 5.0f, 5.0f)};
    t9.color = cyan;

    // Bottom face
    Triangle t10;
    t10.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, 5.0f)};
    t10.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, 5.0f)};
    t10.vertices[0] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, -5.0f)};
    t10.color = magenta;

    Triangle t11;
    t11.vertices[2] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, 5.0f)};
    t11.vertices[1] = (Vertex){.position = Vec3D_XYZ(5.0f, -5.0f, -5.0f)};
    t11.vertices[0] = (Vertex){.position = Vec3D_XYZ(-5.0f, -5.0f, -5.0f)};
    t11.color = magenta;

    Scene s = {0};
    s.transform = Transform_ZERO();
    s.transform.position.z = -20.0f;
    s.mesh_capacity = 8;
    s.meshes = malloc(sizeof(Triangle) * s.mesh_capacity);

    Scene_addTriangle(&s, t0);
    Scene_addTriangle(&s, t1);
    Scene_addTriangle(&s, t2);
    Scene_addTriangle(&s, t3);
    Scene_addTriangle(&s, t4);
    Scene_addTriangle(&s, t5);
    Scene_addTriangle(&s, t6);
    Scene_addTriangle(&s, t7);
    Scene_addTriangle(&s, t8);
    Scene_addTriangle(&s, t9);
    Scene_addTriangle(&s, t10);
    Scene_addTriangle(&s, t11);

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

    SelectObject(state->fb.memDC, state->fb.bitmap);

    ReleaseDC(hwnd, hdc);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)state);

    return 0;
  }

  case WM_PAINT: {
    AppState *state = (AppState *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (state) {
      BitBlt(hdc, 0, 0, state->fb.width, state->fb.height, state->fb.memDC, 0,
             0, SRCCOPY);
    }

    EndPaint(hwnd, &ps);
    return 0;
  }

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

      free(state->s.meshes);
      state->s.meshes = NULL;

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

  RECT r = {0, 0, 500, 500};

  AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
  int width = r.right - r.left;
  int height = r.bottom - r.top;

  Camera c = Camera_new(width, height, 1.047198f, 1.0f, 0.1f, 1000.0f);

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
