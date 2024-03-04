#include "ImpReflectionPCH.h"
#include "Type.h"


ImpReflection::Type::Type(const std::string& name, size_t size, TypeCategory category, Tag tag /*= Tag::Compoonent*/)
	:_name(name), _size(size), _category(category), _tag(tag)
{

}

const std::vector<ImpReflection::Type::EnumMember>& ImpReflection::Type::GetEnumMembers() const
{
	// 재정의 하지 않은 경우에는 빈컨테이너를 반환한다 
	static std::vector<ImpReflection::Type::EnumMember> vec;

	return vec;
}

void* ImpReflection::Type::Invoke(const std::string& data, size_t begin, size_t end) const
{
	void* object = Invoke();
	Read(object, data, begin, end);

	return object;
}

void ImpReflection::Type::Set(TypeCategory category, const std::string& name,
	size_t size, std::function<void*()> func, Tag tag /*= Tad::Component*/)
{
	_category = category;
	_name = name;
	_size = size;
	defaultConstructor = func;
	_tag = tag;
}
