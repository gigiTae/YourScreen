#pragma once
#include "Meta.h"

namespace ImpReflection
{
	/// <summary>
	/// Type�� ���� �������� �����ϴ� Ŭ����
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

		// Type �̸��� ��ȯ�Ѵ�.
		virtual std::string GetFullName()const { return _name; }
		
		// string �����͸� object�� �ҷ��´�
		virtual void Read(void* object, const std::string& data, size_t begin, size_t end) const abstract;

		// ������Ʈ�� �����͸� string �����ͷ� ��ȯ�Ѵ�.
		virtual void Write(const void* object,std::string& data, int indentLevel = 0)const abstract;

		// category�� enum class�� ��� member���� ��ȯ�Ѵ�.
		virtual const std::vector<EnumMember>& GetEnumMembers() const;

		// Ÿ�� ī�װ��� ��ȯ�Ѵ�.
		TypeCategory GetTypeCategory()const { return _category; }

		// Type�� size�� ��ȯ�Ѵ�.
		size_t GetSize() const { return _size; }

		// type�� �ش��ϴ� new �⺻�����ڸ� ȣ���Ѵ� (void* ���¸� ĳ�����ؼ� ����Ѵ�)
		[[nodiscard]] void* Invoke()const { return defaultConstructor(); }

		// �⺻�����ڷ� �޸� �Ҵ����Ŀ� �����͸� ��ü�� �ε��ϰ� ��ü�� �ּҸ� �����Ѵ�.
		[[nodiscard]] void* Invoke(const std::string& data, size_t begin, size_t end)const;

		// �±׸� ��ȯ�Ѵ�.
		Tag GetTag()const { return _tag; }

	protected:
		// Type�� ���� �������� �����Ѵ�. 
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
