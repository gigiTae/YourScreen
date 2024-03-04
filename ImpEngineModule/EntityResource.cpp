#include "EngineModulePCH.h"
#include "EntityResource.h"
#include "Entity.h"


ImpEngineModule::EntityResource::EntityResource(const std::wstring& path, size_t poolSize /*= 1*/)
	:Resource(ResourceCategory::Entity, path), _poolSize(poolSize),
	_prototype{}, _entityPool{}
{

}

ImpEngineModule::EntityResource::~EntityResource()
{
}

void ImpEngineModule::EntityResource::Load()
{
	// Entity Prefab을 가지고 있는다. 
	const std::wstring& path = GetPath();

	_prototype = std::make_unique<Entity>();

	// Entity의 이름은 파일명
	std::filesystem::path filePath(path);

	std::string name = filePath.filename().string();
	ImpStringHelper::RemoveSubstring(name, ".ent");

	_prototype->SetName(name);

	LoadEntity();

	FillEntityPool();
}

std::shared_ptr<ImpEngineModule::Entity> ImpEngineModule::EntityResource::GetEntity()
{
	// Pool에서 사용가능한 오브젝트를 가져온다.
	for (auto& ent : _entityPool)
	{
		if (ent.use_count() == 1)
		{
			// 한번 사용하고 왔으므로 프토토타입과 같은상태로 만들어준다.
			if (ent->GetState() == Entity::EntityState::Destroyed)
			{
				// 복사생성자 호출
				*ent = *_prototype;
			}
			return ent;
		}
	}

	// 사용가능한 오브젝트가 없는경우 새로 생성한다.
	std::shared_ptr<ImpEngineModule::Entity> ent(Clone());
	_entityPool.push_back(ent);

#ifdef _DEBUG
	// 오브젝트풀의 사이즈가 늘어났으므로 로그를 남긴다.
 	std::wstring log = L"Entity Pool Resize : ";
	log += std::filesystem::path(_prototype->GetName()).c_str();
	log += L"(";
	log += std::to_wstring(_entityPool.size());
	log += L")\n";
	OutputDebugString(log.c_str());	
#endif 

	return ent;
}

const std::unique_ptr<ImpEngineModule::Entity>& ImpEngineModule::EntityResource::GetPrototype() const
{
    return _prototype;
}

ImpEngineModule::Entity* ImpEngineModule::EntityResource::Clone()const
{
	Entity* ent = new Entity(*_prototype);

	return ent;
}

void ImpEngineModule::EntityResource::LoadEntity()
{
	std::ifstream readData(GetPath());

	ImpReflection::TypeManager* typeMgr = ImpReflection::TypeManager::GetInstance();

	if (readData.is_open())
	{
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		size_t nowRead = 0, open = 0, close = 0;

		nowRead = fileContent.find('\n') + 1;

		std::string typeName = ImpStringHelper::GetTypeName(fileContent, nowRead);

		if (typeName.empty())
		{
			// 깡통 Entity의 경우
			assert(!L"깡통 Entity입니다");
			return;
		}
		// 리플렉션을 이용해서 Entity 정보를 불러온다.
		do
		{
			TypeIndex index = typeMgr->GetTypeIndex(typeName);

			assert(index != ImpEngineModule::EXCEPTION);

			ImpReflection::Type* type = typeMgr->GetType(index);

			open = ImpStringHelper::FindOpeningBrace(fileContent, nowRead);
			close = ImpStringHelper::FindClosingBrace(fileContent, nowRead);

			std::string sub = fileContent.substr(open, close - open - 1);

			void* component = type->Invoke(fileContent, open, close);

			_prototype->AddComponent(component, index);

			// Component }\nComponentName 이므로 +2
			nowRead = close + 2;
			typeName = ImpStringHelper::GetTypeName(fileContent, nowRead);

		} while (!typeName.empty());
	}
	else
	{
		std::wstring path = GetPath();

		MessageBox(0, L"LoadEntityResource 실패", 0, 0);
	}

}

void ImpEngineModule::EntityResource::FillEntityPool()
{
	// 문제점 : 동일한 포인터를 가진 요소들로 복사된다.
	//_entityPool.assign(_poolSize, std::make_shared<Entity>(_entity));

	for (size_t i = 0; i < _poolSize; ++i)
	{
		_entityPool.push_back(std::make_shared<Entity>(*_prototype.get()));
	}
}

void ImpEngineModule::EntityResource::Clone(Entity* ent) const
{
	// 이동생성자 
	(*ent) = (*_prototype);
}


