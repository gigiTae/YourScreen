#pragma once
#include "Meta.h"

namespace ImpReflection
{
	/// <summary>
	/// Type에 대한 정보들을 저장하는 클래스
	/// </summary>
	class Type
	{
	public:
		struct EnumMember
		{
			std::string _name;
			int _value;
		};

	public:
		Type(const std::string& name, size_t size, TypeCategory category, Tag tag = Tag::Default);

		virtual ~Type(){}

		// Type 이름을 반환한다.
		virtual std::string GetFullName()const { return _name; }
		
		// string 데이터를 object에 불러온다
		virtual void Read(void* object, const std::string& data, size_t begin, size_t end) const abstract;

		// 오브젝트의 데이터를 string 데이터로 변환한다.
		virtual void Write(const void* object,std::string& data, int indentLevel = 0)const abstract;

		// category가 enum class인 경우 member들을 반환한다.
		virtual const std::vector<EnumMember>& GetEnumMembers() const;

		// 타입 카테고리를 반환한다.
		TypeCategory GetTypeCategory()const { return _category; }

		// Type의 size를 반환한다.
		size_t GetSize() const { return _size; }

		// type에 해당하는 new 기본생성자를 호출한다 (void* 형태를 캐스팅해서 사용한다)
		[[nodiscard]] void* Invoke()const { return defaultConstructor(); }

		// 기본생성자로 메모리 할당이후에 데이터를 객체에 로드하고 객체의 주소를 전달한다.
		[[nodiscard]] void* Invoke(const std::string& data, size_t begin, size_t end)const;

		// 태그를 반환한다.
		Tag GetTag()const { return _tag; }

	protected:
		// Type에 대한 정보들을 설정한다. 
		void Set(TypeCategory category, const std::string& name, size_t size,
			std::function<void*()> func, Tag tag = Tag::Component);

	private:
		TypeCategory _category = TypeCategory::None;
		std::string _name;
		size_t _size;
		std::function<void*()> defaultConstructor;
		Tag _tag = Tag::Default;
	};
}
