#pragma once


namespace ImpReflection
{
	/// <summary>
	/// ���÷��� ���θ� �Ǻ��ϰ�
	/// ���÷����� ����ü�� ��쿡�� �������Ͽ� ����Ѵ�.
	/// ������ Ŭ�������� �ָ��ϴ�....
	/// </summary>
	template <typename T>
	class Meta
	{
	public:
		constexpr static bool IsReflect = false;
	};

}
