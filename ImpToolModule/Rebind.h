#pragma once

namespace ImpGraphics
{
class IImpGraphicsEngine;
}

// <summary>
// ���ӿ����� �����Ϳ� ���������� ����ų� ������ �����͸�
// �׷��Ƚ������� ���ε��ϱ����ؼ� �ʿ��� �Լ� ����
//
// ToolModule���� ������ �ֱ⿡�� �ָ������� ����δ� �̰�
// �Ѱ������� ���̺귯���� �Ѱ� �� ����� ����Ƿ� ToolModule ���� �������ִ´�.
// </summary>
namespace ImpToolModule::Bind
{
// ��ƼƼ�� �׷��Ƚ� ������ ���ε��Ѵ�.
void Rebind(ImpEngineModule::Entity* ent,
            ImpGraphics::IImpGraphicsEngine* graphicsEngine);

// �޽�������Ʈ�� �����Ϳ��� ���ؼ� ���ε��Ѵ�.
void RebindMeshObject(ImpEngineModule::Entity* ent,
                      ImpGraphics::IImpGraphicsEngine* graphicsEngine);

// ���̷�Ż �޽� ������Ʈ�� �����Ϳ��� ���ؼ� ���ε��Ѵ�.
void RebindSkelatalMeshObject(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine);


// ����Ʈ�� ���ؼ� �ٽ� ���ε��Ѵ�.
void RebindLightObject(ImpEngineModule::Entity* ent,
                       ImpGraphics::IImpGraphicsEngine* graphicsEngine);

// UI ���ε�
void RebindUIObject(ImpEngineModule::Entity* ent,
                    ImpGraphics::IImpGraphicsEngine* graphicsEngine);


} // namespace ImpToolModule::Bind