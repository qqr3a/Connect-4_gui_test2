#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <iostream>
#include <conio.h> 
#include "include\\game.h"
#include "include\\auto.h"



static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int buttonwidth;
int buttonhight;
int gapspacebutton;
int buttonposx;
int buttonposy;
        


void DrawCell(int x, int y, int player) {
    const int gridOffsetX = 488;
    const int gridOffsetY = 100;
    const int cellSize = 135;

    ImVec2 top_left = ImVec2(y * cellSize + gridOffsetX, (6 - 1 - x) * cellSize + gridOffsetY);
    ImVec2 bottom_right = ImVec2((y + 1) * cellSize + gridOffsetX, (6 - x) * cellSize + gridOffsetY);
    

    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (player == 0) {
        const ImU32 colour_int = ImColor(ImVec4(0.5, 0.5, 0.5, 1.0));
        draw->AddRect(top_left, bottom_right, colour_int);
    } else if (player == 1) {
        const ImU32 colour_int = ImColor(ImVec4(0.6, 0.3, 0.3, 1.0));
        draw->AddRectFilled(top_left, bottom_right, colour_int);
    } else if (player == 2) {
        const ImU32 colour_int = ImColor(ImVec4(0.3, 0.3, 0.6, 1.0));
        draw->AddRectFilled(top_left, bottom_right, colour_int);
    }
}





bool done;

const int numrows=6;
const int numcols=7;
int grid[numrows][numcols] = {{0}};

int player, usercol;

int numempty;
bool start=true;
bool winner=false;



int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Connect4", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);


    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0,0,0,0);
    style.WindowRounding = 0;

    ImVec4 clear_color = ImVec4(0.12f, 0.12f, 0.12f,0.9f);

    // Main loop
    
    
    int column,row;
    while (!done){   
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;


        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        

        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);

        if (start==true){
            if (ImGui::Begin("Connect4",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar)){
                ImGui::SetCursorPos(ImVec2(710,510));

                if (ImGui::Button("Start",ImVec2(500,100))){
                    start = false;
                    done = false;
                    for (int column=0;column<7;column++){
                        for (int row=0;row<6;row++){

                            grid[row][column] = 0;
                        }
                    }
                    numempty=42;
                    player = 1;
                    winner = false;
                }
            }ImGui::End();

        } else if (winner==false && numempty>0){
            if (ImGui::Begin("Connect4",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar)){
                usercol= -1;
                
                for (int column=0;column<7;column++){
                    for (int row=0;row<6;row++){
                        DrawCell(row,column,grid[row][column]);
                    }
                }

            buttonwidth=135;
            buttonhight=50;
            gapspacebutton=buttonwidth;
            buttonposx=488;
            buttonposy=1080-100-buttonhight;

            ImGui::SetWindowFontScale(1.5);
            
            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("1",ImVec2(buttonwidth,buttonhight))){
                usercol = 1;
            }

            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("2",ImVec2(buttonwidth,buttonhight))){
                usercol = 2;
            }

            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("3",ImVec2(buttonwidth,buttonhight))){
                usercol = 3;
            }

            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("4",ImVec2(buttonwidth,buttonhight))){
                usercol = 4;
            }

            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("5",ImVec2(buttonwidth,buttonhight))){
                usercol = 5;
            }

            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("6",ImVec2(buttonwidth,buttonhight))){
                usercol = 6;
            }

            ImGui::SetCursorPos(ImVec2(buttonposx,buttonposy));
            buttonposx+=gapspacebutton;
            if (ImGui::Button("7",ImVec2(buttonwidth,buttonhight))){
                usercol = 7;
            }
        
        

                
        if (player==2){
            usercol=autoplayer(grid, numrows, numcols, 3)+1;
            row=addtocolumn(grid,usercol-1,numrows,player);
            
        } else if (usercol!=-1 && player==1 && validcolumn(numrows,numcols,grid,usercol)){
            row=addtocolumn(grid,usercol-1,numrows,player);

                
        }


        if (usercol>=0){
            winner=fourinaline(grid, row, usercol-1, player,numrows,numcols);

            numempty-=1;
            if (player==1) {player=2;} else {player=1;}

            printGrid(grid,numrows,numcols);
        }
                

        
        
        }ImGui::End();
        }  else {
            if (ImGui::Begin("Gave over",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar)){
                for (int column=0;column<7;column++){
                    for (int row=0;row<6;row++){
                        DrawCell(row,column,grid[row][column]);
                    }
                }
                ImGui::SetWindowFontScale(2.1);
                ImGui::SetCursorPos(ImVec2(735,925));

                if (ImGui::Button("Play again",ImVec2(450,70))){
                    start=true;
                }
                
            }ImGui::End();

        }

        


        // Rendering
        ImGui::EndFrame();
        
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}



bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); 
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) 
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
