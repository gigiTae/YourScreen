#pragma once

namespace ImpToolModule
{
	class ResourceViewer;
	class ToolCamera;
	
	/// <summary>
	/// World의 Entity들의 정보를 보여주는 GUI
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
		// 엔티티 GUI를 그린기
		void ShowEntityGUI(ImpEngineModule::Entity* ent);

		// 자식이 없는 Entity GUI그리기
		void ShowEntityGUIWithoutChild(ImpEngineModule::Entity* ent);

		// 저장버튼
		void SaveEntityButton(ImpEngineModule::Entity* ent);
		
		// 로드 버튼
		void LoadEntityButton(ImpEngineModule::World* world);
		
		// 엔티티경로 받기
		void DropEntity(ImpEngineModule::World* world);

		// Entity 계층구조 드래그 드랍
		void DragDropEntityHeirachy(ImpEngineModule::Entity* ent);

		// Entity 검색기능
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