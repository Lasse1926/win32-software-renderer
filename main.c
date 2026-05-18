#include "include/main.h"
#include "include/camera.h"
#include "include/geometry_utils.h"
#include "include/scene.h"
#include "include/transform.h"
#include "include/vector_utils.h"
#include <stdio.h>
#define FPS 100

void Render(HWND hwnd)
{
    struct Framebuffer* fb =
        (struct Framebuffer*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (!fb || !fb->pixels)
        return;

    uint32_t* pixels = (uint32_t*)fb->pixels;

    Camera c = {
      .transform = Transform_ZERO(),
    };

    Triangle t;

    t.vertices[2] = (Vertex){.position = Vec3D_XYZ(100.0f, 100.0f,0.0f)}; 
    t.vertices[1] = (Vertex){.position = Vec3D_XYZ(200.0f, 100.0f,0.0f)}; 
    t.vertices[0] = (Vertex){.position = Vec3D_XYZ(100.0f, 200.0f,0.0f)}; 

    Scene s;

    s.mesh_capacity = 8;
    s.mesh_length = 0;
    s.meshes = malloc(sizeof(Triangle) * s.mesh_capacity);

    Scene_addTriangle(&s,t);

    Camera_render(&c,&s,pixels,fb->width,fb->height);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            struct Framebuffer* fb =
                (struct Framebuffer*)malloc(sizeof(struct Framebuffer));

            if (!fb)
                return -1;

            fb->width = 500;
            fb->height = 500;
            fb->pixels = NULL;
            fb->memDC = NULL;
            fb->bitmap = NULL;

            HDC hdc = GetDC(hwnd);

            fb->memDC = CreateCompatibleDC(hdc);

            BITMAPINFO bmi;
            ZeroMemory(&bmi, sizeof(bmi));

            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = fb->width;
            bmi.bmiHeader.biHeight = -fb->height; // top-down
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            fb->bitmap = CreateDIBSection(
                hdc,
                &bmi,
                DIB_RGB_COLORS,
                &fb->pixels,
                NULL,
                0
            );

            SelectObject(fb->memDC, fb->bitmap);

            ReleaseDC(hwnd, hdc);

            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)fb);

            return 0;
        }

        case WM_PAINT:
        {
            struct Framebuffer* fb =
                (struct Framebuffer*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (fb)
            {
                BitBlt(
                    hdc,
                    0, 0,
                    fb->width,
                    fb->height,
                    fb->memDC,
                    0, 0,
                    SRCCOPY
                );
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
        {
            struct Framebuffer* fb =
                (struct Framebuffer*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if (fb)
            {
                if (fb->bitmap)
                    DeleteObject(fb->bitmap);

                if (fb->memDC)
                    DeleteDC(fb->memDC);

                free(fb);
            }

            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    WNDCLASSA wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "WindowTest_MainWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    RECT r = { 0, 0, 500, 500 };

    AdjustWindowRectEx(
        &r,
        WS_OVERLAPPEDWINDOW,
        FALSE,
        WS_EX_OVERLAPPEDWINDOW
    );
    int width  = r.right - r.left;
    int height = r.bottom - r.top;
    HWND hwnd = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW,
        "WindowTest_MainWindow",
        "Test Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nShowCmd);

    MSG msg;
    LARGE_INTEGER freq;
    LARGE_INTEGER lastTime;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&lastTime);
    while (1)
    {
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return 0;

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        // --- dt start ---
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        double dt = (double)(currentTime.QuadPart - lastTime.QuadPart) / freq.QuadPart;
        lastTime = currentTime;
        // --- dt end ---

        Render(hwnd);
        InvalidateRect(hwnd, NULL, FALSE);

        if (FPS > 0){
          double target_dt = 1.0 / FPS;

          if (dt < target_dt)
          {
              Sleep((DWORD)((target_dt - dt) * 1000.0));
          }
        }
    }

    return (int)msg.wParam;
}
