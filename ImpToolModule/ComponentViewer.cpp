#include "ImpToolModulePCH.h"
#include "ComponentViewer.h"
#include "Rebind.h"

ImpToolModule::ComponentViewer::ComponentViewer()
    : _viewComponent(nullptr)
    , _reflectComponents{}
    , _graphicsEngine(nullptr)
    , _viewEntity(nullptr)
    , _isRebindEntity(false)
    , _toolInput(nullptr)
    , _engineModule(nullptr)
{
    using namespace ImpReflection;

    TypeManager* mgr = TypeManager::GetInstance();

    // ���÷��ǵ� ������Ʈ�� �̸����� �����´�.
    for (auto& iter : mgr->GetTypeInfomations())
    {
        if (iter.second.type->GetTypeCategory() != TypeCategory::Class ||
            iter.second.type->GetTag() != ImpReflection::Tag::Component)
        {
            continue;
        }

        _reflectComponents.push_back(iter.first);
    }

    // ���ĺ� ����
    std::sort(_reflectComponents.begin(),
        _reflectComponents.end(),
        [](const std::string& lfm, const std::string& rfm)
        {
            std::string lfmName = ImpStringHelper::EraseNamespace(lfm);
            std::string rfmName = ImpStringHelper::EraseNamespace(rfm);

            if (lfmName < rfmName)
            {
                return true;
            }
            return false;
        });
}


void ImpToolModule::ComponentViewer::Initialize(
    ImpGraphics::IImpGraphicsEngine* grahphicsEngnie,
    ImpEngineModule::EngineModule* engineMdule,
    ImpEngineModule::InputManager* toolInput)
{
    _graphicsEngine = grahphicsEngnie;
    _engineModule   = engineMdule;
    _toolInput      = toolInput;

    _engineModule->GetEventManager()->SubScribe<ImpEngineModule::Event::LoadedWorld>(
        this);
}

void ImpToolModule::ComponentViewer::Update(ImpEngineModule::Entity* ent)
{
    using namespace ImpEngineModule;
    using namespace ImpReflection;

    _isRebindEntity = false;
    _viewEntity     = ent;

    ImGui::Begin("ComponentViewer");

    if (ent != nullptr)
    {
        ImGui::Text(ent->GetName().c_str());

        auto& components = ent->GetComponents();

        ImpReflection::TypeManager* typeMgr = ImpReflection::TypeManager::GetInstance();

        ImGui::Separator();

        std::type_index eraseIndex = ImpEngineModule::EXCEPTION;

        // Component ���� ǥ��

        for (auto& iter : components)
        {
            std::type_index index = iter.first;
            Type* type            = typeMgr->GetType(index);

            std::string name     = type->GetFullName();
            Component* component = iter.second;
            _viewComponent       = component;

            ViewComponentInfomation(component, name, type);

            // ������Ʈ ���� (Ʈ�������� �����Ѵ�)
            if (name == "ImpEngineModule::Transform")
            {
                continue;
            }

            ImGui::SameLine();
            std::string deleteButton = "X##";
            deleteButton += name;
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x -
                                 ImGui::CalcTextSize("XX").x);
            if (ImGui::Button(deleteButton.c_str()))
            {
                eraseIndex = index;
            }
        }

        ImGui::Separator();
        ViewAddComponent(ent);

        // ������ ������Ʈ ����
        if (eraseIndex != ImpEngineModule::EXCEPTION)
        {
            Component* component = ent->GetComponent(eraseIndex);
            Mesh* mesh           = dynamic_cast<Mesh*>(component);

            // �޽��� �����ϴ� ��� ���ε��� �����Ѵ�.
            if (mesh != nullptr)
            {
                mesh->SetActive(false);
                Bind::RebindMeshObject(ent, _graphicsEngine);
            }

            ent->DestroyComponent(eraseIndex);
        }
    }

    ImGui::End();


    // �ٽ� �׷��Ƚ� ������ ���ε��� �ʿ��� ���
    if (_isRebindEntity)
    {
        Bind::Rebind(ent, _graphicsEngine);
    }
}

void ImpToolModule::ComponentViewer::ViewComponentInfomation(void* object,
    const std::string& name,
    ImpReflection::Type* type)
{
    using namespace ImpReflection;

    // ������� ����

    switch (type->GetTypeCategory())
    {
        case TypeCategory::Class:
        {
            ClassType* classType = reinterpret_cast<ClassType*>(type);

            std::string fureName = ImpStringHelper::EraseUnderbar(
                ImpStringHelper::EraseNamespace(name));

            if (ImGui::TreeNode(fureName.c_str()))
            {
                for (auto& member : classType->GetMembers())
                {
                    ViewComponentInfomation(
                        (char*)object + member._offset, member._name, member._type);
                }

                ImGui::TreePop();
            }

            break;
        }
        case TypeCategory::EnumClass:
        {
            auto& members = type->GetEnumMembers();

            int* currentEnum = static_cast<int*>(object); // ����
            int currentIndex = INT_MIN; // �̳ѿ� �ش��ϴ� �迭�� �ε���
            std::vector<std::string> memberNames{}; // ����� �̸�
            std::vector<int> indexMatch;

            for (auto& member : members)
            {
                std::string name = member._name + "(";
                name += std::to_string(member._value);
                name += ")";

                memberNames.push_back(name.c_str());
                indexMatch.push_back(member._value);

                if (*currentEnum == member._value)
                {
                    currentIndex = static_cast<int>(memberNames.size()) - 1;
                }
            }

            int size = static_cast<int>(members.size());

            if (currentIndex == INT_MIN) // �ʱ�ȭ�� ���� ���
            {
                currentIndex = indexMatch[0];
            }

            std::string fureName = ImpStringHelper::EraseUnderbar(name);

            if (ImGui::BeginCombo(
                    fureName.c_str(), memberNames[currentIndex].c_str()))
            {
                for (int i = 0; i < members.size(); ++i)
                {
                    // �� �׸� ���� ���� ������ ��ư�� ����
                    const bool isSelected = (currentIndex == i);
                    if (ImGui::Selectable(memberNames[i].c_str(), isSelected))
                    {
                        currentIndex = i;
                    }

                    // ���õ� �׸� üũ ��ũ ǥ��
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
                    }
                }

                ImGui::EndCombo();
            }

            *currentEnum = indexMatch[currentIndex];

            break;
        }
        case TypeCategory::Primitive:
        {
            ViewPrimitiveType(object, name, type);
            break;
        }
        case TypeCategory::Map:
        {
            ViewMapType(object, name, type);
            break;
        }
        case TypeCategory::List:
        {
            ViewListType(object, name, type);
            break;
        }
        case TypeCategory::Vector:
        {
            ViewVectorType(object, name, type);
            break;
        }
        case TypeCategory::UnorderedMap:
        {
            ViewUnorderedMapType(object, name, type);
            break;
        }
        case TypeCategory::None:
        {
            assert(!"Tpye�� ���� ������ �����ϴ�.");
        }
        break;
        default:
        {
            // dd7
        }
        break;
    }
}

void ImpToolModule::ComponentViewer::ViewPrimitiveType(void* object,
    const std::string& name,
    ImpReflection::Type* type)
{
    std::string fureName = ImpStringHelper::EraseUnderbar(name);

    if (type->GetFullName() == "int")
    {
        int* data = reinterpret_cast<int*>(object);
        ImGui::InputInt(fureName.c_str(), data);
    }
    else if (type->GetFullName() == "unsigned int")
    {
        unsigned int* data = reinterpret_cast<unsigned int*>(object);
        ImGui::InputScalar(fureName.c_str(), ImGuiDataType_U32, data);
    }
    else if (type->GetFullName() == "float")
    {
        float* data = reinterpret_cast<float*>(object);
        if (ImGui::DragFloat(fureName.c_str(), data, 0.1f) &&
            _toolInput->IsKeyState(Key::LMouse, KeyState::Away))
        {

            _isRebindEntity = true;
        }
    }
    else if (type->GetFullName() == "double")
    {
        double* data = reinterpret_cast<double*>(object);
        ImGui::InputDouble(fureName.c_str(), data);
    }
    else if (type->GetFullName() == "std::string")
    {
        std::string* data = reinterpret_cast<std::string*>(object);

        ImGui::InputText(fureName.c_str(), data);
    }
    else if (type->GetFullName() == "bool")
    {
        bool* data = reinterpret_cast<bool*>(object);
        ImGui::Checkbox(fureName.c_str(), data);
    }
    else if (type->GetFullName() == "std::wstring")
    {
        std::wstring* data = reinterpret_cast<std::wstring*>(object);

        std::string sData = std::filesystem::path(*data).filename().string();

        if (ImGui::InputText(fureName.c_str(), &sData))
        {
            // ������ �����츸 ���ش�.
            if (sData.empty())
            {
                data->clear();
            }
        }

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(
                "ResourcePath");
            if (payLoad)
            {
                const wchar_t* receiveData = static_cast<const wchar_t*>(
                    payLoad->Data);

                std::wstring path = receiveData;
                *data             = path;

                _isRebindEntity = true;
            }
        }
    }
    else if (type->GetFullName() == "quaternion")
    {
        // ���ʹϿ¿� ���� GUI â�� ��Ÿ����
        Quaternion* data = reinterpret_cast<Quaternion*>(object);

        if (ImGui::TreeNode("rotation##quaternion"))
        {
            Vector3 euler = data->ToEuler();
            euler.x       = ImpMath::RadianToDegree(euler.x);
            euler.y       = ImpMath::RadianToDegree(euler.y);
            euler.z       = ImpMath::RadianToDegree(euler.z);

            ImGui::SetNextItemWidth(50.f);
            ImGui::InputFloat("x##quaternion", &euler.x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputFloat("y##quaternion", &euler.y);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputFloat("z##quaternion", &euler.z);

            euler.x = ImpMath::DegreeToRadian(euler.x);
            euler.y = ImpMath::DegreeToRadian(euler.y);
            euler.z = ImpMath::DegreeToRadian(euler.z);

            *data = Quaternion::MakeFromYawPitchRoll(euler.y, euler.x, euler.z);

            if (ImGui::DragFloat("xAxis", &_currentAxis.x, 0.1f))
            {
                *data *= Quaternion::MakeFromAxis(Vector3::UnitX,
                    ImpMath::DegreeToRadian(_currentAxis.x - _prevAxis.x));
                _prevAxis.x = _currentAxis.x;
            }

            if (ImGui::DragFloat("yAxis", &_currentAxis.y, 0.1f))
            {
                *data *= Quaternion::MakeFromAxis(Vector3::UnitY,
                    ImpMath::DegreeToRadian(_currentAxis.y - _prevAxis.y));
                _prevAxis.y = _currentAxis.y;
            }


            if (ImGui::DragFloat("zAxis", &_currentAxis.z, 0.1f))
            {
                *data *= Quaternion::MakeFromAxis(Vector3::UnitZ,
                    ImpMath::DegreeToRadian(_currentAxis.z - _prevAxis.z));
                _prevAxis.z = _currentAxis.z;
            }

            if (_toolInput->IsKeyState(Key::LMouse, KeyState::Away) ||
                _toolInput->IsKeyState(Key::Enter, KeyState::Away))
            {
                _currentAxis = Vector3::Zero;
                _prevAxis    = Vector3::Zero;
            }

            ImGui::TreePop();
        }
    }
    else if (type->GetFullName() == "ImpColor")
    {
        ImpStructure::ImpColor* data = reinterpret_cast<ImpStructure::ImpColor*>(
            object);

        float color[4] = { data->r, data->g, data->b, data->a };

        if (ImGui::ColorEdit4("color", color))
        {
            data->r = color[0];
            data->g = color[1];
            data->b = color[2];
            data->a = color[3];

            _isRebindEntity = true;
        }
    }
    else
    {
        // imgui���� �� ã�ƺ��� long long �̶���� �����Ҳ� ����?
        // ������ ������� �����ϴ°��� �ʿ��ϸ� �����մϴ�.
        assert(!"�����ϴ� ������ �����ϴ�.");
    }
}

void ImpToolModule::ComponentViewer::ViewVectorType(void* object,
    const std::string& name,
    ImpReflection::Type* type)
{
    /// �̹� ������Ʈ������  std::wstring���� ����ϱ� ������ object�� �׻�
    /// std::vector<std::wstring> Ÿ���̴�
    using PathVector = std::vector<std::wstring>;

    PathVector* paths         = static_cast<PathVector*>(object);
    std::string addButtonName = "Add Path##" + name;

    if (ImGui::Button(addButtonName.c_str()))
    {
        paths->push_back({});
    }


    if (ImGui::TreeNode(name.c_str()))
    {
        int deleteIndex = -1;
        std::wstring receivePath{};
        int pathIndex = -1;

        for (int i = 0; i < paths->size(); ++i)
        {
            std::string
                stringPath = std::filesystem::path((*paths)[i]).filename().string();
            std::string deleteButtonName = "X##DeleteButton" + std::to_string(i);

            if (ImGui::Button(deleteButtonName.c_str()))
            {
                deleteIndex = i;
            }

            ImGui::SameLine();
            if (ImGui::InputText(stringPath.c_str(), &stringPath))
            {
                if (stringPath.empty())
                {
                    (*paths)[i].clear();
                }
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(
                    "ResourcePath");
                if (payLoad)
                {
                    pathIndex                  = i;
                    const wchar_t* receiveData = static_cast<const wchar_t*>(
                        payLoad->Data);
                    receivePath = receiveData;
                }
            }
        }

        
        if (pathIndex != -1)
        {
            (*paths)[pathIndex] = receivePath;
        }

        // ����
        if (deleteIndex != -1 && !paths->empty())
        {
            paths->erase(paths->begin() + deleteIndex);
        }

        ImGui::TreePop();
    }
}

void ImpToolModule::ComponentViewer::ViewListType(void* object,
    const std::string& name,
    ImpReflection::Type* type)
{
    std::string output = type->GetFullName();

    ImGui::Text(output.c_str());
    ImGui::SameLine();
    ImGui::Text(name.c_str());
}

void ImpToolModule::ComponentViewer::ViewMapType(void* object,
    const std::string& name,
    ImpReflection::Type* type)
{
    std::string output = type->GetFullName();

    ImGui::Text(output.c_str());
    ImGui::SameLine();
    ImGui::Text(name.c_str());
}

void ImpToolModule::ComponentViewer::ViewUnorderedMapType(void* object,
    const std::string& name,
    ImpReflection::Type* type)
{
    std::string output = type->GetFullName();

    ImGui::Text(output.c_str());
    ImGui::SameLine();
    ImGui::Text(name.c_str());
}

void ImpToolModule::ComponentViewer::ViewAddComponent(ImpEngineModule::Entity* ent)
{
    std::string addComponentName = _reflectComponents[_addComponentIndex].c_str();
    addComponentName = ImpStringHelper::EraseNamespace(addComponentName);

    if (ImGui::BeginCombo("##combo", addComponentName.c_str()))
    {
        for (int i = 0; i < _reflectComponents.size(); ++i)
        {
            // �� �׸� ���� ���� ������ ��ư�� ����
            const bool isSeleted = (_addComponentIndex == i);

            std::string componentName = _reflectComponents[i].c_str();
            componentName = ImpStringHelper::EraseNamespace(componentName);

            if (ImGui::Selectable(componentName.c_str(), isSeleted))
            {
                _addComponentIndex = i;
            }

            // ���õ� �׸� üũ ��ũ ǥ��
            if (isSeleted)
            {
                ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::Button("+"))
    {
        std::string& componentName = _reflectComponents[_addComponentIndex];

        ImpReflection::TypeManager* mgr = ImpReflection::TypeManager::GetInstance();

        ImpReflection::Type* type = mgr->GetType(componentName);

        auto index = mgr->GetTypeIndex(componentName);

        // �̹� ������ ������Ʈ�� �ߺ����� �߰����� �ʴ´�.
        if (!ent->HasComponent(index))
        {
            ent->AddComponent(type->Invoke(), index);
            ent->GetComponent(index)->Start();

            if ("ImpEngineModule::Mesh" == componentName)
            {
                Bind::RebindMeshObject(ent, _graphicsEngine);
            }
        }
    }
}

void ImpToolModule::ComponentViewer::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadedWorld& event)
{
    _viewEntity = nullptr;
}
