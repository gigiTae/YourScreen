#pragma once

namespace ImpToolModule
{
	class ResourceViewer;
	class ToolCamera;
	
	/// <summary>
	/// World�� Entity���� ������ �����ִ� GUI
	/// 
	/// </summary>
	class EntityViewer : ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
	{
	public:
		EntityViewer();
		~EntityViewer();

	public:
		void Initialize(ImpEngineModule::EngineModule* engineModule, ResourceViewer* resViewer,
			ImpGraphics::IImpGraphicsEngine* grahphicsEngnie, ToolCamera* toolCamera);

		void Update();

		ImpEngineModule::Entity* GetSelectEntity() const { return _selectEntity; }
		void SetSelectEntity(size_t id);

	private:
		// ��ƼƼ GUI�� �׸���
		void ShowEntityGUI(ImpEngineModule::Entity* ent);

		// �ڽ��� ���� Entity GUI�׸���
		void ShowEntityGUIWithoutChild(ImpEngineModule::Entity* ent);

		// �����ư
		void SaveEntityButton(ImpEngineModule::Entity* ent);
		
		// �ε� ��ư
		void LoadEntityButton(ImpEngineModule::World* world);
		
		// ��ƼƼ��� �ޱ�
		void DropEntity(ImpEngineModule::World* world);

		// Entity �������� �巡�� ���
		void DragDropEntityHeirachy(ImpEngineModule::Entity* ent);

		// Entity �˻����
		void SearchEntityGUI();

	private:
		void Receive(ImpEngineModule::World* world, const ImpEngineModule::Event::LoadedWorld& event)override;

	private:
		ImpEngineModule::World* _world = nullptr;
		ImpEngineModule::Entity* _selectEntity = nullptr;
		ImpEngineModule::WorldManager* _worldManager = nullptr;
		ImpToolModule::ResourceViewer* _resourceView = nullptr;
		ImpToolModule::ToolCamera* _toolCamera = nullptr;

		ImpGraphics::IImpGraphicsEngine* _graphicsEngine = nullptr;

		bool _isShowPopup = false;
		std::string _searchName;
	};


}