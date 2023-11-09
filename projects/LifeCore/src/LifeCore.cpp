#include "LifeCore.h"

namespace LifeCore
{

	LifeTest::LifeTest()
	{
		m_test = 10;
		m_data = LifeData();
	}

	LifeTest::~LifeTest()
	{
	}

	int LifeTest::GetVal()
	{
		return m_test;
	}
}