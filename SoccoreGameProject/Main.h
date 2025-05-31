#pragma once
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<dxgi1_6.h>
#include<cmath>
#include<d3d12.h>
#include<cassert>
#include<Windows.h>
#include<DirectXMath.h>
using namespace DirectX;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


void WaitDrawDone();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

//�E�B���h�E------------------------------------------------------------------------
LPCWSTR	WindowTitle = L"loveDX12";
const int ClientWidth = 1280;
const int ClientHeight = 720;
const int ClientPosX = (GetSystemMetrics(SM_CXSCREEN) - ClientWidth) / 2;//�����\��
const int ClientPosY = (GetSystemMetrics(SM_CYSCREEN) - ClientHeight) / 2;//�����\��
const float Aspect = static_cast<float>(ClientWidth) / ClientHeight;
#if 1 
DWORD WindowStyle = WS_OVERLAPPEDWINDOW;
#else 
DWORD WindowStyle = WS_POPUP;//Alt + F4�ŕ���
#endif
HWND HWnd;

//�f�o�C�X--------------------------------------------------------------------------
ComPtr<ID3D12Device>  Device;
//�R�}���h
ID3D12CommandAllocator* CommandAllocator;
ID3D12GraphicsCommandList* CommandList;
ID3D12CommandQueue* CommandQueue;
//�t�F���X
ID3D12Fence* Fence;
HANDLE FenceEvent;
UINT64 FenceValue;
//�f�o�b�O
HRESULT Hr;
//���\�[�X----------------------------------------------------------------------
//�o�b�N�o�b�t�@
IDXGISwapChain4* SwapChain;
ID3D12Resource* BackBufs[2];
UINT BackBufIdx;
ID3D12DescriptorHeap* BbvHeap;//"Bbv"��"BackBufView"�̗�
UINT BbvHeapSize;
//�f�v�X�X�e���V���o�b�t�@
ID3D12Resource* DepthStencilBuf;
ID3D12DescriptorHeap* DsvHeap;//"Dsv"��"DepthStencilBufView"�̗�
//���_�ʒu�o�b�t�@
ID3D12Resource* PositionBuf;
D3D12_VERTEX_BUFFER_VIEW PositionBufView;

//�e�N�X�`���ʒu�o�b�t�@
ID3D12Resource* TexcoordBuf;
D3D12_VERTEX_BUFFER_VIEW TexcoordBufView;

//���_�C���f�b�N�X�o�b�t�@
ID3D12Resource* IndexBuf;
D3D12_INDEX_BUFFER_VIEW	IndexBufView;
//�R���X�^���g�o�b�t�@�O
struct CONST_BUF0 {
    XMMATRIX mat;
};
//�R���X�^���g�o�b�t�@�P
struct CONST_BUF1 {
    XMFLOAT4 diffuse;
};
//���b�V��_a------------------------
CONST_BUF0* MappedConstBuf0_a;
CONST_BUF1* MappedConstBuf1_a;
ID3D12Resource* ConstBuf0_a;
ID3D12Resource* ConstBuf1_a;
ID3D12Resource* TextureBuf_a;
//ID3D12DescriptorHeap* CbvTbvHeap_a;

//���b�V��_b------------------------
CONST_BUF0* MappedConstBuf0_b;
CONST_BUF1* MappedConstBuf1_b;
ID3D12Resource* ConstBuf0_b;
ID3D12Resource* ConstBuf1_b;
ID3D12Resource* TextureBuf_b;
//ID3D12DescriptorHeap* CbvTbvHeap_b;
ID3D12DescriptorHeap* CbvTbvHeap;


//�p�C�v���C��--------------------------------------------------------------------
ID3D12RootSignature* RootSignature;
ID3D12PipelineState* PipelineState;
D3D12_VIEWPORT Viewport;
D3D12_RECT ScissorRect;


