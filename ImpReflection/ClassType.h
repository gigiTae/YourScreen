#pragma once
#include "Type.h"
#include "Meta.h"

namespace ImpReflection
{
	/// <summary>
	/// Class, Struct ������ �����Ѵ�
	/// Ŭ������ �������, ������� Ÿ��, ��������� �����µ��
	/// 
	/// ������ ��Ӱ����ؼ� ������ ���������� ���Ѵ� - �ð��� ���´ٸ� ����
	/// </summary>
	class ClassType :
		public Type
	{
	public:
		struct Member
		{
			std::string _name; // ������� �̸�
			size_t _offset; // ������� ������
			Type* _type; // ������� Type����
		};
	public:
		ClassType(void(*Initailize)(ClassType*))
			:Type("", 0, TypeCategory::Class), _members{}
		{
			/// Macro���� �������� �ʱ�ȭ ������ Initialize�Լ��� ���޹޾Ƽ� �����Ѵ�.
			Initailize(this);
		}
		
		void Write(const void* object, std::string& data, int indentLevel /* = 0 */)const override
		{
			data += GetFullName();
			data += "{\n";
			for (const auto& member : _members)
			{
				data += std::string((2 * (static_cast<size_t>(indentLevel) + 1)), ' ');
				data += member._name;
				data += " = ";
				member._type->Write(reinterpret_cast<const char*>(object) + member._offset, data, indentLevel + 1);
				data += '\n';
			}
			
			data += std::string(4 * static_cast<size_t>(indentLevel), ' ') + "}";
		}

		void Read(void* object, const std::string& data, size_t begin, size_t end)const override
		{
			size_t open = ImpStringHelper::FindOpeningBrace(data, begin);
			size_t close = ImpStringHelper::FindClosingBrace(data, begin);

#ifdef _DEBUG
			std::string objectData = data.substr(open, close - open + 1);
#endif

			assert(open != std::string::npos && "data�� ���� ������ Ȯ���� �ּ���");

			for (const Member& member : _members)
			{
				size_t memberOpen = ImpStringHelper::FindOpeningBrace(data, open + 1);
				size_t memberClose = ImpStringHelper::FindClosingBrace(data, open + 1);
#ifdef _DEBUG
				std::string memberData = data.substr(memberOpen, memberClose - memberOpen + 1);
#endif
				member._type->Read(reinterpret_cast<char*>(object) + member._offset,
					data, memberOpen, memberClose);

				open = memberClose + 1;
			}

		}

		/// class, struct�� ���÷��ǵ� ��������� �������� �����´�.
		const std::vector<Member>& GetMembers()const { return _members; }

	private:
		std::vector<Member> _members;

		template <typename T> friend class ImpReflection::Meta;
	};


}