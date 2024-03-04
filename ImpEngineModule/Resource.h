#pragma once

namespace ImpEngineModule
{
	
	/// <summary>
	/// ���ҽ����� ������ �����ϴ� Ŭ����
	/// </summary>
	class Resource
	{
	public:
		Resource(ResourceCategory category, const std::wstring& path);
		virtual ~Resource(){}

		/// ���ҽ��� �ε��Ѵ�.
		virtual void Load() abstract;

		/// ���ҽ��� ��� ��ȯ
		const std::wstring& GetPath() const {  return _path; }
		
		/// ���ҽ� ��� �缳��
		void SetPath(std::wstring path) { _path = path; }
	private:
		std::wstring _path; // ��� 
		ResourceCategory _category; // ���ҽ� ����
	};



}