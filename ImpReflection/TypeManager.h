
#pragma once
namespace ImpReflection
{
	class Type;
	template<typename T> class Meta;
	template <typename T> class VectorType;
	template<typename T> class ListType;
	template<typename T1, typename T2> class MapType;
	template<typename T1, typename T2> class UnorderedMapType;

	/// <summary>
	/// Runtime에 리플렉션한 Type에 대한 정보들을 저장하는 클래스
	/// 전역적인인 클래스이다.
	/// </summary>
	class TypeManager
	{
		struct TypeInfomation
		{
			std::type_index index;
			ImpReflection::Type* type;
			//UserCategory userCategory; 나중에 컴포넌트 말고 리플렉션이 생기다면 사용
		};

#pragma region Singleton

	public:
		static TypeManager* GetInstance() { static TypeManager mgr; return &mgr; }
		
		template <typename T>
		static std::type_index GetTypeIndex();
	private:
		TypeManager(){}
		~TypeManager(){}

#pragma endregion

	public:
		/// Type이름으로 TypeIndex를 찾는 함수
		/// 찾지못한 경우 Exception type_index를 반환한다.
		std::type_index GetTypeIndex(const std::string& name)const;

		/// Type이름으로 Type을 찾는 함수
		ImpReflection::Type* GetType(const std::string & name)const;

		/// TypeIndex로 Type을 찾는 함수
		ImpReflection::Type* GetType(std::type_index index)const;

		/// TypeIndex로 Type이름 찾는 함수
		const std::string& GetName(std::type_index index)const;

		/// 타입에 관한 정보들을 담은 컨테이너를 반환한다.
		const std::unordered_map<std::string, TypeInfomation>& GetTypeInfomations()const { return _infomations; }

	private:
		/// 리플렉션 데이터를 추가한다. 
		void AddTypeInfomation(const std::string& name, const TypeInfomation& info);

	private:
		std::unordered_map<std::string, TypeInfomation> _infomations;

		template <typename T> friend class ImpReflection::Meta;
		template <typename T> friend class ImpReflection::VectorType;
		template<typename T> friend class ImpReflection::ListType;
		template<typename T1, typename T2> friend class ImpReflection::MapType;
		template<typename T1, typename T2> friend class ImpReflection::UnorderedMapType;
	};

	template <typename T>
	std::type_index ImpReflection::TypeManager::GetTypeIndex()
	{
		return std::type_index(typeid(T));
	}

}