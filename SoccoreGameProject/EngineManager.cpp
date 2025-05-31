#include "EngineManager.h"
#include "BufferManager.h" // Vertex���g������

void EngineManager::Initialize() {
    // DeviceManager������
    m_deviceManager.Initialize();

    // SwapChainManager������
    auto* device = m_deviceManager.GetDevice();
    auto* cmdQueue = m_deviceManager.GetCommandQueue();
    m_swapChainManager.Initialize(m_hWnd, device, cmdQueue, 1280, 720);
    m_depthBufferManager.Initialize(device, 1280, 720);
    m_pipelineManager.Initialize(device, L"assets/VertexShader.cso", L"assets/PixelShader.cso");

    // ������cmdList���擾
    auto* cmdList = m_deviceManager.GetCommandList();

    // �e�N�X�`���}�l�[�W�����������e�N�X�`�����[�h
    m_textureManager.Initialize(device);
    m_texIdx = m_textureManager.LoadTexture(L"assets/penguin1.png", m_deviceManager.GetCommandList());

    // �K�v�ȃo�b�t�@�E���\�[�X�͂����ł܂Ƃ߂ČĂ�
    CreateTestQuad();
    // LoadPenguinModel();  // �ȂǍ���ǂ�ǂ�ǉ���OK
}


void EngineManager::Start() {
    // �Q�[���J�n���̏����i�K�v�Ȃ���΋��OK�j
}

void EngineManager::Update() {
    // ���t���[���X�V����
}

void EngineManager::Draw() {
    // 1. �o�b�N�o�b�t�@�擾
    UINT backBufferIndex = m_swapChainManager.GetSwapChain()->GetCurrentBackBufferIndex();
    auto* cmdList = m_deviceManager.GetCommandList();

    // 2. �o���A�ŕ`��p�ɐ؂�ւ�
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_swapChainManager.GetBackBuffer(backBufferIndex);
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    cmdList->ResourceBarrier(1, &barrier);

    // 3. RTV/DSV�n���h���擾
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_swapChainManager.GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * m_swapChainManager.GetRTVHeapSize();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_depthBufferManager.GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();

    // 4. �����_�[�^�[�Q�b�g�ݒ�
    cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

    // 5. �N���A
    const float clearColor[] = { 0.1f, 0.3f, 0.6f, 1.0f };
    cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // 6. �r���[�|�[�g�E�V�U�[��`�̐ݒ�
    float width = static_cast<float>(m_swapChainManager.GetWidth());
    float height = static_cast<float>(m_swapChainManager.GetHeight());
    D3D12_VIEWPORT viewport = { 0, 0, width, height, 0.0f, 1.0f };
    D3D12_RECT scissorRect = { 0, 0, (LONG)width, (LONG)height };
    cmdList->RSSetViewports(1, &viewport);
    cmdList->RSSetScissorRects(1, &scissorRect);

    // 7. �p�C�v���C���E���[�g�V�O�l�`���ݒ�
    cmdList->SetPipelineState(m_pipelineManager.GetPipelineState());
    cmdList->SetGraphicsRootSignature(m_pipelineManager.GetRootSignature());

    // 8. SRV�q�[�v�Z�b�g & ���[�g�p�����[�^0�Ԃ�SRV���o�C���h
    ID3D12DescriptorHeap* heaps[] = { m_textureManager.GetSRVHeap() };
    cmdList->SetDescriptorHeaps(_countof(heaps), heaps);
    cmdList->SetGraphicsRootDescriptorTable(0, m_textureManager.GetSRV(m_texIdx));

    // 9. �l�p�`�`��R�}���h
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D12_VERTEX_BUFFER_VIEW vbv = m_bufferManager.GetVertexBufferView();
    D3D12_INDEX_BUFFER_VIEW ibv = m_bufferManager.GetIndexBufferView();
    cmdList->IASetVertexBuffers(0, 1, &vbv);
    cmdList->IASetIndexBuffer(&ibv);
    cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

    // 10. �o���A��PRESENT�ɖ߂�
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    cmdList->ResourceBarrier(1, &barrier);

    // 11. �R�}���h���X�g���s
    cmdList->Close();
    ID3D12CommandList* commandLists[] = { cmdList };
    m_deviceManager.GetCommandQueue()->ExecuteCommandLists(1, commandLists);
    m_deviceManager.WaitForGpu(); // GPU�����҂�

    // 12. Present
    m_swapChainManager.GetSwapChain()->Present(1, 0);

    // 13. �R�}���h�A���P�[�^/���X�g���Z�b�g
    m_deviceManager.GetCommandAllocator()->Reset();
    cmdList->Reset(m_deviceManager.GetCommandAllocator(), nullptr);
}




void EngineManager::Shutdown() {
    // �I������
    m_deviceManager.Cleanup();
	m_swapChainManager.Cleanup();
}


//================================================================================================================================

void EngineManager::CreateTestQuad() {
    // �A�X�y�N�g��␳
    float width = static_cast<float>(m_swapChainManager.GetWidth());
    float height = static_cast<float>(m_swapChainManager.GetHeight());
    float aspect = width / height;

    // �A�X�y�N�g�␳�ς݂̒��_���W
    std::vector<Vertex> vertices = {
        { -0.5f / aspect, -0.5f, 0.0f, 0.0f, 1.0f }, // ����
        { -0.5f / aspect,  0.5f, 0.0f, 0.0f, 0.0f }, // ����
        {  0.5f / aspect, -0.5f, 0.0f, 1.0f, 1.0f }, // �E��
        {  0.5f / aspect,  0.5f, 0.0f, 1.0f, 0.0f }, // �E��
    };
    std::vector<uint16_t> indices = { 0, 1, 2, 2, 1, 3 };
    m_bufferManager.CreateVertexBuffer(m_deviceManager.GetDevice(), vertices);
    m_bufferManager.CreateIndexBuffer(m_deviceManager.GetDevice(), indices);
}

