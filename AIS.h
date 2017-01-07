
#ifndef _AIS_H_
#define _AIS_H_

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

private:
	void decode(unsigned int fillBits);
	int getbit(unsigned int idx);
	uint16_t get_u16(unsigned start, unsigned len);
	uint32_t get_u32(unsigned start, unsigned len);
	int32_t get_i32(unsigned start, unsigned len);
	int8_t get_i8(unsigned start, unsigned len);
	uint8_t get_u8(unsigned start, unsigned len);

private:
	uint8_t msg[msg_max];
	unsigned int msgLen;
	uint8_t msgType;
};


#endif // _AIS_H_
