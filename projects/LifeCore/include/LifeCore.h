#pragma once

namespace LifeCore
{

	struct LifeData
	{
		int test;
	};

	class LifeTest
	{
	public: 
		LifeTest();
		~LifeTest();

		int GetVal();

	private:
		int m_test;
		LifeData m_data;
	};
}