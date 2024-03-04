#include "ImpToolModulePCH.h"
#include "EntityViewer.h"
#include "ResourceViewer.h"
#include "Rebind.h"
#include "ToolCamera.h"

ImpToolModule::EntityViewer::EntityViewer()
{
}

ImpToolModule::EntityViewer::~EntityViewer()
{
}

void ImpToolModule::EntityViewer::Initialize(
    ImpEngineModule::EngineModule* engineModule,
    ResourceViewer* resViewer,
    ImpGraphics::IImpGraphicsEngine* grahphicsEngnie,
    ToolCamera* toolCamera)
{
    _world          = engineModule->GetWorld();
    _worldManager   = engineModule->GetWorldManager();
    _resourceView   = resViewer;
    _graphicsEngine = grahphicsEngnie;
    _toolCamera     = toolCamera;

    engineModule->GetEventManager()->SubScribe<ImpEngineModule::Event::LoadedWorld>(
        this);
}

void ImpToolModule::EntityViewer::Update()
{
    using namespace ImpEngineModule;
    if (_world == nullptr) return;

    /// EntityViewr�� ��������
    if (ImGui::Begin("EntityViewer"))
    {
        if (ImGui::Button("Create Entity"))
        {
            Entity* ent = _world->CreateEntity();
            // Transform�� �⺻������ �����Ѵ�.
            ent->AddComponent<Transform>();
        }

        LoadEntityButton(_world);

        SearchEntityGUI();

        ImGui::Separator();

        if (_searchName.empty())
        {
            // �˻����� �ʾ����� �Ϲ������� ǥ���ϴ� GUI
            for (const std::shared_ptr<Entity>& ent : _world->GetEntities())
            {
                // Root Entity�� ǥ���Ѵ�.
                if (!ent->GetComponent<Transform>()->IsRoot())
                {
                    continue;
                }

                ShowEntityGUI(ent.get());
            }
        }
        else // �˻��� ������ �����ִ� GUI
        {
            for (const std::shared_ptr<Entity>& ent : _world->GetEntities())
            {
                size_t index = ent->GetName().find(_searchName);

                if (index != std::string::npos)
                {
                    ShowEntityGUIWithoutChild(ent.get());
                }
            }
        }
        ImGui::End();
    }

    // ���õ� Entity�� ���������̸� �������� �ʴ´�.
    if (_selectEntity && _selectEntity->GetState() != Entity::EntityState::Active)
    {
        _selectEntity = nullptr;
    }

    // Save �Ϸ� �˾�â
    if (_isShowPopup)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);

        ImGui::OpenPopup("Save an Entity");

        if (ImGui::BeginPopupModal("Save an Entity", &_isShowPopup))
        {
            std::filesystem::path path = ImpEngineModule::PathManager::GetEntityDataPath();

            ImGui::Text("< Directory Path >");
            ImGui::Text(path.string().c_str());

            ImGui::EndPopup();
        }
    }

    ImGui::Separator();
}


void ImpToolModule::EntityViewer::SetSelectEntity(size_t id)
{
    _selectEntity = _world->GetEntity(id);
}

void ImpToolModule::EntityViewer::ShowEntityGUI(ImpEngineModule::Entity* ent)
{
    using namespace ImpEngineModule;

    if (ent->GetState() != ImpEngineModule::Entity::EntityState::Active)
    {
        return;
    }

    std::string entID      = std::to_string(ent->GetID());
    std::string entityName = ent->GetName();
    std::string imguiID    = "##" + entID;

    bool isSelected = (_selectEntity == ent);

    if (isSelected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.5f, 1.f));
    }

    float fisrtCursorPosX = 0.f;

    // ��ư ������ġ ����
    if (!ent->GetComponent<Transform>()->IsLeaf() ||
        !ent->GetComponent<Transform>()->IsRoot())
    {
        fisrtCursorPosX = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX(ImGui::GetTreeNodeToLabelSpacing() + fisrtCursorPosX);
    }

    if (ImGui::Button(entID.c_str()))
    {
        if (_selectEntity == ent)
        {
            // ī�޶� ��
            Vector3 pos = ent->GetComponent<Transform>()->GetLocalPosition();
            _toolCamera->LookAt(pos, 10.f);
        }

        _selectEntity = ent;
    }

    DragDropEntityHeirachy(ent);

    if (isSelected)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();

    // Entity �̸��Է¹�ư
    ImGui::SetNextItemWidth(100.f);
    if (ImGui::InputText(imguiID.c_str(), &entityName))
    {
        ImpEngineModule::NameCheck::CheckNameDuplication(_world, ent, entityName);
        ent->SetName(entityName);
    }

    ImGui::SameLine();

    SaveEntityButton(ent);

    ImGui::SameLine();

    // ��ƼƼ �ı� ��ư
    std::string destroyButton = "Destroy##" + std::to_string(ent->GetID());

    if (ImGui::Button(destroyButton.c_str()))
    {
        _world->DestroyEntity(ent);
    }

    // �ڽ� ��ƼƼ ��۹�ư
    if (!ent->GetComponent<Transform>()->IsLeaf())
    {
        ImGui::SameLine();

        ImGui::SetCursorPosX(fisrtCursorPosX);

        imguiID += "node";

        if (ImGui::TreeNode(imguiID.c_str()))
        {
            for (Transform* child : ent->GetComponent<Transform>()->GetChildren())
            {
                ShowEntityGUI(child->GetEntity());
            }

            ImGui::TreePop();
        }
    }

    ImGui::Separator();
}

void ImpToolModule::EntityViewer::ShowEntityGUIWithoutChild(ImpEngineModule::Entity* ent)
{
    using namespace ImpEngineModule;

    if (ent->GetState() != ImpEngineModule::Entity::EntityState::Active)
    {
        return;
    }

    std::string entID      = std::to_string(ent->GetID());
    std::string entityName = ent->GetName();
    std::string imguiID    = "##" + entID;

    bool isSelected = (_selectEntity == ent);

    if (isSelected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.5f, 1.f));
    }

    if (ImGui::Button(entID.c_str()))
    {
        if (_selectEntity == ent)
        {
            // ī�޶� ��
            Vector3 pos = ent->GetComponent<Transform>()->GetLocalPosition();
            _toolCamera->LookAt(pos, 10.f);
        }
        _selectEntity = ent;
    }

    DragDropEntityHeirachy(ent);

    if (isSelected)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();

    // Entity �̸��Է¹�ư
    ImGui::SetNextItemWidth(100.f);
    if (ImGui::InputText(imguiID.c_str(), &entityName))
    {
        ImpEngineModule::NameCheck::CheckNameDuplication(_world, ent, entityName);
        ent->SetName(entityName);
    }

    ImGui::SameLine();

    SaveEntityButton(ent);

    ImGui::SameLine();

    // ��ƼƼ �ı� ��ư
    std::string destroyButton = "Destroy##" + std::to_string(ent->GetID());

    if (ImGui::Button(destroyButton.c_str()))
    {
        _world->DestroyEntity(ent);
    }
    ImGui::Separator();
}

void ImpToolModule::EntityViewer::SaveEntityButton(ImpEngineModule::Entity* ent)
{
    std::string saveButton = "Save##" + std::to_string(ent->GetID());

    if (ImGui::Button(saveButton.c_str()))
    {
        std::filesystem::path path = _resourceView->GetCurrentResourcePath();

        path += "/";
        Save::SaveEntity(path, ent);
        _isShowPopup = true;
    }
}

void ImpToolModule::EntityViewer::LoadEntityButton(ImpEngineModule::World* world)
{
    ImGui::SameLine();

    if (ImGui::Button("Load Entity"))
    {
    }

    DropEntity(world);
}


void ImpToolModule::EntityViewer::DropEntity(ImpEngineModule::World* world)
{
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(
            "ResourcePath");

        if (payLoad)
        {
            const wchar_t* receiveData = static_cast<const wchar_t*>(payLoad->Data);

            // Ȯ���ڸ� Ȯ���Ѵ�
            std::filesystem::path path(receiveData);
            if (path.extension() != L".ent")
            {
                return;
            }

            // TODO :: ���ҽ������� �������������� ó���� �ʿ��ϴ�.
            ImpEngineModule::Entity* ent = world->CreateEntity();
            ImpEngineModule::EntityResource entRes{ receiveData };

            entRes.Load();
            entRes.Clone(ent);

            // �̸��ߺ� Ȯ��
            std::string entName = ent->GetName();
            ImpEngineModule::NameCheck::CheckNameDuplication(world, ent, entName);
            ent->SetName(entName);

            // world, component ����
            for (auto& component : ent->GetComponents())
            {
                component.second->Set(_world, ent);
            }

            ent->Start();

            Bind::Rebind(ent, _graphicsEngine);
        }
    }
}

void ImpToolModule::EntityViewer::DragDropEntityHeirachy(ImpEngineModule::Entity* ent)
{
    using namespace ImpEngineModule;

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::Text(ent->GetName().c_str());

        ImGui::SetDragDropPayload("EntityID", ent, sizeof(Entity));
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("EntityID");

        if (payLoad)
        {
            Entity* child = static_cast<Entity*>(payLoad->Data);

            Transform* parentT = ent->GetComponent<Transform>();
            Transform* childT  = child->GetComponent<Transform>();

            // child�� �ڽ��� parent �ΰ�쿡�� �ƹ��� ó���� ���� �ʴ´�.
            if (childT->IsDescendant(parentT))
            {
                return;
            }

            // �ٽ��ѹ� �÷������� ���θ� �и��Ѵ�.
            if (childT->GetParent() == parentT)
            {
                parentT->RemoveChild(childT);
            }
            else
            {
                parentT->AddChild(childT);
            }
        }
    }
}

void ImpToolModule::EntityViewer::SearchEntityGUI()
{
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150.f);
    ImGui::InputText("Search", &_searchName);
}

void ImpToolModule::EntityViewer::Receive(
    ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadedWorld& event)
{
    _selectEntity = nullptr;
}
