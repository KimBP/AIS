
#ifndef _AIS_H_
#define _AIS_H_

/* Information about individual AIS message types currently supported, see:
 *
 * http://catb.org/gpsd/AIVDM.html#_types_1_2_and_3_position_report_class_a
 * http://catb.org/gpsd/AIVDM.html#_type_18_standard_class_b_cs_position_report'
 */

#include <Arduino.h>
class AIS
{

public:
	AIS(const char *AISbitstream, unsigned int fillBits = 0);
	static const int msg_max = 60;
	bool getdata(unsigned int begin, unsigned int cnt, uint8_t *data, bool isigned = false);

	uint32_t get_mmsi();
	uint16_t get_SOG();
	uint16_t get_COG();
	uint16_t get_HDG();
	int32_t get_latitude();
	int32_t get_longitude();
	int8_t get_rot();  // Rate of Turn
	uint8_t get_repeat();
	uint8_t get_navStatus();
	uint8_t get_posAccuracy();
	uint8_t get_type();
	uint8_t get_timeStamp();
	uint8_t get_manIndicator();
	uint8_t get_raim();
	const char* get_shipname();
	const char* get_destination();
	const char* get_callsign();

private:
	void decode(unsigned int fillBits);
	int getbit(unsigned int idx);
	uint16_t get_u16(unsigned start, unsigned len);
	uint32_t get_u32(unsigned start, unsigned len);
	int32_t get_i32(unsigned start, unsigned len);
	int8_t get_i8(unsigned start, unsigned len);
	uint8_t get_u8(unsigned start, unsigned len);
	void get_string(char* str, unsigned start, unsigned bitlen, unsigned cnt);

private:
	static const uint8_t shipname_strlen = 20; // a bits
	static const uint8_t destination_strlen = 20; // a bits
	static const uint8_t callsign_strlen = 7; // a 6 bits
	uint8_t msg[msg_max];
	unsigned int msgLen;
	uint8_t msgType;
	//TODO: Allocates strings based on msgType
	char shipname[shipname_strlen + 1];
	char destination[destination_strlen+1];
	char callsign[callsign_strlen+1];
};


#endif // _AIS_H_
