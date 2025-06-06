#pragma once
#include <Windows.h>
#include "DeviceManager.h"
#include "SwapChainManager.h" // 追加
#include "DepthBufferManager.h" // 追加
#include "PipelineManager.h" // 追加
#include "BufferManager.h"
#include"TextureManager.h"
class EngineManager {
public:
    void SetHWND(HWND hwnd) { m_hWnd = hwnd; }
    HWND GetHWND() const { return m_hWnd; }

    void Initialize();
    void Start();
    void Update();
    void Draw();
    void Shutdown();

    DeviceManager* GetDeviceManager() { return &m_deviceManager; }
    SwapChainManager* GetSwapChainManager() { return &m_swapChainManager; } // 追加
    DepthBufferManager* GetDepthBufferManager() { return &m_depthBufferManager; } // 追加
    BufferManager* GetBufferManager() { return &m_bufferManager; }
	PipelineManager* GetPipelineManager() { return &m_pipelineManager; } // 追加
	TextureManager* GetTextureManager() { return &m_textureManager; } // 追加

    int m_texIdx = -1;
    //============================================================================================================

    void CreateTestQuad(); // 追加
private:
    HWND m_hWnd = nullptr;
    DeviceManager m_deviceManager;
    SwapChainManager m_swapChainManager; // 追加
    DepthBufferManager m_depthBufferManager; // 追加
    PipelineManager m_pipelineManager; // 追加
    BufferManager m_bufferManager; // 追加
	TextureManager m_textureManager; // 追加
    // 他のManagerも同様に追加
};
