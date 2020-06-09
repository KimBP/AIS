/**********************************************************************
  AISTest.cpp.h - Copyright (c) 2016-2020 Kim Bøndergaarg <kim@fam-boendergaard.dk>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  ``Software''), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **********************************************************************/


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
