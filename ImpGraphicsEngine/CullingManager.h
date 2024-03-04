#pragma once

namespace ImpGraphics
{
struct Sphere;
struct Frustum;

class IObject;

class CullingManager
{
public:
    CullingManager();
    ~CullingManager();

    void AddObject(size_t objectNum, IObject* object); // Addobject ��
    bool IsInsideFrustum(size_t objectNum, IObject* object);
    bool IsInsideFrustumRight(size_t objectNum, IObject* object);
    void DeleteObject(size_t objectNum); // DeleteObject ��

    void AddLight(size_t lightNum, float radius);
    bool IsInsideFrustum(size_t lightNum);
    bool IsInsideFrustumRight(size_t lightNum);
    void UpdateLight(size_t lightNum, DirectX::XMFLOAT3 center);
    void DeleteLight(size_t lightNum);

    // Tool ��
    void SetViewFrustum(DirectX::XMMATRIX* proj);
    // ����
    void UpdateCameraPosition(DirectX::XMFLOAT3 position,
                              DirectX::XMFLOAT4 rotationQut); // update ī�޶��

    void SetViewFrustumRight(DirectX::XMMATRIX* proj);
    void UpdateCameraPositionRight(DirectX::XMFLOAT3 position,
                              DirectX::XMFLOAT4 rotationQut); // update ī�޶��

private:
    bool IsIntersectSphereAndFrustum(const Sphere& sphere, const Frustum& frustum) const;

private:
    Frustum* _viewFrustum;
    Frustum* _viewFrustumRight;

    std::unordered_map<size_t, Sphere*> _defaultSpheres; // �ϴ� Ʈ������ ��Ʈ������ �������� ���� ���Ǿ�
    std::unordered_map<size_t, Sphere*> _lightSpheres; // �ϴ� Point�� Spot�� �߰��ϸ� ����
}; 

}