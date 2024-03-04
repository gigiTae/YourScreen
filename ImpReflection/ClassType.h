#pragma once
#include "Type.h"
#include "Meta.h"

namespace ImpReflection
{
	/// <summary>
	/// Class, Struct 정보를 저장한다
	/// 클래스의 멤버변수, 멤버변수 타입, 멤버변수의 오프셋등등
	/// 
	/// 하지만 상속관련해서 정보를 저장하지는 못한다 - 시간이 남는다면 도전
	/// </summary>
	class ClassType :
		public Type
	{
	public:
		struct Member
		{
			std::string _name; // 멤버변수 이름
			size_t _offset; // 멤버변수 오프셋
			Type* _type; // 멤버변수 Type정보
		};
	public:
		ClassType(void(*Initailize)(ClassType*))
			:Type("", 0, TypeCategory::Class), _members{}
		{
			/// Macro에서 정적변수 초기화 시점에 Initialize함수를 전달받아서 실행한다.
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

			assert(open != std::string::npos && "data에 대한 정보를 확인해 주세요");

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

		/// class, struct의 리플렉션된 멤버변수의 정보를을 가져온다.
		const std::vector<Member>& GetMembers()const { return _members; }

	private:
		std::vector<Member> _members;

		template <typename T> friend class ImpReflection::Meta;
	};


}