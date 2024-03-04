
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
	/// Runtime�� ���÷����� Type�� ���� �������� �����ϴ� Ŭ����
	/// ���������� Ŭ�����̴�.
	/// </summary>
	class TypeManager
	{
		struct TypeInfomation
		{
			std::type_index index;
			ImpReflection::Type* type;
			//UserCategory userCategory; ���߿� ������Ʈ ���� ���÷����� ����ٸ� ���
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
		/// Type�̸����� TypeIndex�� ã�� �Լ�
		/// ã������ ��� Exception type_index�� ��ȯ�Ѵ�.
		std::type_index GetTypeIndex(const std::string& name)const;

		/// Type�̸����� Type�� ã�� �Լ�
		ImpReflection::Type* GetType(const std::string & name)const;

		/// TypeIndex�� Type�� ã�� �Լ�
		ImpReflection::Type* GetType(std::type_index index)const;

		/// TypeIndex�� Type�̸� ã�� �Լ�
		const std::string& GetName(std::type_index index)const;

		/// Ÿ�Կ� ���� �������� ���� �����̳ʸ� ��ȯ�Ѵ�.
		const std::unordered_map<std::string, TypeInfomation>& GetTypeInfomations()const { return _infomations; }

	private:
		/// ���÷��� �����͸� �߰��Ѵ�. 
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