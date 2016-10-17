
#ifndef _AIS_H_
#define _AIS_H_

#include <Arduino.h>
class AIS
{

public:
	AIS(const char *AISbitstream);
	static const int msg_max = 100;
	bool getdata(unsigned int begin, unsigned int cnt, uint8_t *data);
	template <typename T>
		static T fixSign(unsigned int bits, T data)
		{
			if (bits >= 8*sizeof(data))
				return data;

			T one = 1; // Ensure shift of 1 results in right type
			T sign = one << (bits-1);
			if (data > sign) {
				for (unsigned int i= bits; i < 8*sizeof(data); i++) {
					data |= (one << i);
				}
			}
			return data;
		}

private:
	void decode();
	int getbit(unsigned int idx);

private:
	uint8_t msg[msg_max];
	unsigned int msgLen;
};


#endif // _AIS_H_
