
#ifndef _AIS_H_
#define _AIS_H_

#include <Arduino.h>
class AIS
{

public:
	AIS(const char *AISbitstream, unsigned int fillBits = 0);
	static const int msg_max = 60;
	bool getdata(unsigned int begin, unsigned int cnt, uint8_t *data, bool isigned = false);

	unsigned long get_mmsi();
	unsigned int get_SOG();
	unsigned int get_COG();
	unsigned int get_HDG();
	long get_latitude();
	long get_longitude();
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
	unsigned int get_u16(unsigned start, unsigned len);
	unsigned long get_u32(unsigned start, unsigned len);
	long get_i32(unsigned start, unsigned len);
	int8_t get_i8(unsigned start, unsigned len);
	uint8_t get_u8(unsigned start, unsigned len);

private:
	uint8_t msg[msg_max];
	unsigned int msgLen;
	uint8_t msgType;
};


#endif // _AIS_H_
