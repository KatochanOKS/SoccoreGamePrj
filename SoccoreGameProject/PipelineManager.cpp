#include "PipelineManager.h"
#include <d3dcompiler.h>
#include <vector>
#include <fstream>
#include <cassert>
#include "d3dx12.h"

// �V�F�[�_�t�@�C���̃o�C�i���ǂݍ���
static std::vector<char> LoadShaderFile(LPCWSTR filename) {
    std::ifstream ifs(filename, std::ios::binary);
    return std::vector<char>((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

bool PipelineManager::Initialize(ID3D12Device* device, LPCWSTR vsPath, LPCWSTR psPath) {
    // ���[�g�p�����[�^�FSRV����
    CD3DX12_ROOT_PARAMETER rootParam[1] = {};
    CD3DX12_DESCRIPTOR_RANGE descRange;
    descRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0
    rootParam[0].InitAsDescriptorTable(1, &descRange, D3D12_SHADER_VISIBILITY_PIXEL);

    // �T���v���[�i�e�N�X�`���T���v�����O�p�j
    CD3DX12_STATIC_SAMPLER_DESC staticSampler(
        0, // register(s0)
        D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP
    );

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
        1, rootParam,
        1, &staticSampler,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    Microsoft::WRL::ComPtr<ID3DBlob> signature, error;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    if (FAILED(hr)) {
        if (error) OutputDebugStringA((char*)error->GetBufferPointer());
        return false;
    }
    hr = device->CreateRootSignature(
        0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)
    );
    if (FAILED(hr)) return false;

    // �V�F�[�_�ǂݍ���
    auto vsCode = LoadShaderFile(vsPath);
    auto psCode = LoadShaderFile(psPath);

    // ���_���C�A�E�g�iPOSITION, TEXCOORD������OK�j
    D3D12_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    auto& rtDesc = blendDesc.RenderTarget[0];
    rtDesc.BlendEnable = TRUE;
    rtDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    rtDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    rtDesc.BlendOp = D3D12_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    rtDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = { vsCode.data(), vsCode.size() };
    psoDesc.PS = { psCode.data(), psCode.size() };
    psoDesc.InputLayout = { layout, _countof(layout) };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;

    psoDesc.BlendState = blendDesc;
    hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
    return SUCCEEDED(hr);
}

void PipelineManager::Cleanup() {
    m_rootSignature.Reset();
    m_pipelineState.Reset();
}
