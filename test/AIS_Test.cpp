#include "CppUTest/TestHarness.h"

#include "AIS/AIS.h"

TEST_GROUP(AIS)
{
	AIS* ais;

	void setup()
	{
		ais = new AIS("13@o`ghP000irdtOq86<1Ovf20RM");
	}

	void teardown()
	{
		delete ais;
	}
};

TEST(AIS, GetTypeBase_Station)
{
	const char* aisStr = "4025bw1v5?8>DPcOkTOnoM102D1Q";

	AIS ais(aisStr);
	
	CHECK_EQUAL(AIS::AIS_MSG_4_BASE_STATION_REPORT,ais.get_type());
}

TEST(AIS, GetTypeStatic_and_Voyage)
{
	const char* aisStr = "539S:k40000000c3G04PPh63<00000000080000o1PVG2uGD:0000000000000000000000";

	AIS ais(aisStr);
	
	CHECK_EQUAL(AIS::AIS_MSG_5_STATIC_AND_VOYAGE,ais.get_type());
}

TEST(AIS, GetTypePos_Report)
{
	CHECK_EQUAL(AIS::AIS_MSG_1_2_3_POS_REPORT_CLASS_A, ais->get_type());
}

TEST(AIS, GetMMSI)
{
	CHECK_EQUAL(219015359, ais->get_mmsi());
}

TEST(AIS, GetRepeat)
{
	CHECK_EQUAL(0, ais->get_repeat());
}

TEST(AIS, GetLAT)
{
	CHECK_EQUAL(33441816, ais->get_latitude());
}

TEST(AIS, GetLONG)
{
	CHECK_EQUAL(6542750, ais->get_longitude());
}
