/* A parser of the NMEA0183 VDM packet containing AIS messages */
/* Usage:
	cat <nmea0183 stream> | vdm_dump -

*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../AIS.h"
#include "../../AIS.cpp"


static struct timeval global_now; /* Global time  - lazy solution*/


void printtime(struct timeval now)
{
	static struct timeval prevTime;
	static int offset = -1;

	struct timeval nextprev;
	
	if (offset == -1) {
		// First time 
		offset = now.tv_sec;
	}
	now.tv_sec -= offset;
	nextprev = now;
	if (now.tv_usec < prevTime.tv_usec) {
		now.tv_usec += 1000000;
		now.tv_sec--;
	}
	
	printf ("%03d.%06d, %03d.%06d ", 
		(int)nextprev.tv_sec, (int)nextprev.tv_usec, 
		(int)(now.tv_sec-prevTime.tv_sec), (int)(now.tv_usec - prevTime.tv_usec));

	prevTime = nextprev;
}

void printsystime(void) 
{
	gettimeofday(&global_now, NULL);
	printtime(global_now);
}

static int lineno = 0;

void printPosReport(AIS& ais)
{
  printf("%d (PosReport): ", ais.get_numeric_type());
  printf("mmsi=%d", ais.get_mmsi());
  printf("\n");
}

void printBaseStationReport(AIS& ais)
{
  printf("%d (StationReport): ", ais.get_numeric_type());
  printf("mmsi=%d", ais.get_mmsi());
  printf("\n");
}

void printStaticAndVoyage(AIS& ais)
{
  printf("%d (StaticAndVoyage)): ", ais.get_numeric_type());
  printf("mmsi=%d ", ais.get_mmsi());
  printf("name=%s ", ais.get_shipname());
  printf("\n");
}

void printCsPosReport(AIS& ais)
{
  printf("%d (iCsPosReport): ", ais.get_numeric_type());
  printf("mmsi=%d", ais.get_mmsi());
  printf("\n");
}

void printStaticData(AIS& ais)
{
  printf("%d (StaticData): ", ais.get_numeric_type());
  printf("mmsi=%d", ais.get_mmsi());
  printf("\n");
}

void printType(AIS& ais) 
{
	switch( ais.get_type() ) {
	case AIS::AIS_MSG_1_2_3_POS_REPORT_CLASS_A:
		printPosReport(ais);
		break;
	case AIS::AIS_MSG_4_BASE_STATION_REPORT:
		printBaseStationReport(ais);
		break;
	case AIS::AIS_MSG_5_STATIC_AND_VOYAGE:
		printStaticAndVoyage(ais);
		break;
	case AIS::AIS_MSG_18_CS_POS_REPORT_CLASS_B:
		printCsPosReport(ais);
		break;
	case AIS::AIS_MSG_24_STATIC_DATA_REPORT:
		printStaticData(ais);
		break;
	default:
		printf("Unsupported AIS. Type=%d\n", ais.get_numeric_type());
		break;
	}
}

void handle_ais(const char* aisString)
{
	AIS ais(aisString);

	printType(ais);
	


}

void read_stdin(void)
{
	size_t size;
	char* line=NULL;
	FILE* fd = fopen("/dev/stdin", "r");
	int total = 0;
	int next = 0;
	char buf[1000];
	buf[0] = '\0';

	while(getline(&line, &size,fd) >= 0) {

		if ( 0 == memcmp("VDM", &line[3], 3)) {
 			// Yes a VDM
			char* aisstart = line;
			char* aisend;
			char* totalStr;
			char* fragStr;
			char* fragEnd;
			totalStr = strchr(aisstart, ',');
			if (!totalStr) {
				continue;
			}
			totalStr++;
			fragStr = strchr(totalStr, ',');
			if (!fragStr) {
				continue;
			}
			*fragStr='\0';
			fragStr++;
			fragEnd = strchr(fragStr, ',');
			if (!fragEnd) {
				continue;
			}
			*fragEnd = '\0';
			fragEnd++;
			int i =0;
			aisstart = fragEnd;
			while (i < 2 && aisstart) {
				aisstart = strchr(aisstart, ',');
				if (aisstart) {
					aisstart++;
					i++;
				}
			}
			if (! aisstart) {
				continue;
			}
			aisend = strchr(aisstart, ',');
			if (!aisend) {
				continue;
			}
			*aisend = '\0';

			int totalFrags = atoi(totalStr);
			int thisFrag = atoi(fragStr);
			printf("%d of %d: %s\n", thisFrag, totalFrags, aisstart);
			
			if (totalFrags == 1 && thisFrag == 1) {	
				handle_ais(aisstart);
				continue;
			}
			if (totalFrags > 1 && thisFrag == 1) {
				// First in multifrag
				next = 2;
				total = totalFrags;
				strcpy(buf, aisstart);
				continue;
			}
			if (totalFrags == total && thisFrag == next) {
				// next expected in multifrag
				next++;
				strcat(buf, aisstart);
				if (totalFrags == thisFrag) {
					// packet complete
					handle_ais(buf);
					total = 0;
					next = 0;
				}
			} else {
				// Something wrong - restart
				total = 0;
				next = 0;
				printf("**** PACKET SKIPPED\n");
			}
		}		
	}
	free(line);
	fclose(fd);
}


int main(int argc, char* argv[])
{
   if (argc == 2 && *(argv[1]) == '-') {
	printf("Expecting stdin\n");
	read_stdin();
    } else {
      printf("Not supported yet\n");

    }
    return 0;
}
