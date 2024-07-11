#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BELL   "\007"
#define DELETE "\033[6A\033[5K"

#define ARG_SIZE 8

bool is_digit(char c) {
	return c >= '0' || c <= '9';
}

void init_ascii(char* ascii[11][5][1]) {
	char *semi[][1] = 
					  { {"  "},
						{"++"},
						{"  "},
						{"++"},
						{"  "} };


	char *zero[][1] = 
					  { {" 0000 "},
						{"00  00"},
						{"00  00"},
						{"00  00"},
						{" 0000 "} };

	char *one[][1]  = 
					  { {"1111  "},
						{"  11  "},
						{"  11  "},
						{"  11  "},
						{"111111"} };

	char *two[][1]  = 
					  { {" 2222 "},
						{"22  22"},
						{"   22 "},
						{"  22  "},
						{"222222"} };

	char *three[][1]  = 
					  { {" 3333 "},
						{"33  33"},
						{"   333"},
						{"33  33"},
						{" 3333 "} };

	char *four[][1]  = 
					  { {"44  44"},
						{"44  44"},
						{"444444"},
						{"    44"},
						{"    44"} };

	char *five[][1]  = 
					  { {"555555"},
						{"55    "},
						{"55555 "},
						{"    55"},
						{"55555 "} };

	char *six[][1]  = 
					  { {" 6666 "},
						{"66    "},
						{"66666 "},
						{"66  66"},
						{" 6666 "} };

	char *seven[][1]  = 
					  { {"777777"},
						{"   77 "},
						{"  77  "},
						{" 77   "},
						{"77    "} };

	char *eight[][1]  = 
					  { {" 8888 "},
						{"88  88"},
						{" 8888 "},
						{"88  88"},
						{" 8888 "} };

	char *nine[][1]  = 
					  { {" 9999 "},
						{"99  99"},
						{" 99999"},
						{"    99"},
						{" 9999 "} };

	memcpy(ascii[0],  (char **) semi,  40);
	memcpy(ascii[1],  (char **) zero,  40);
	memcpy(ascii[2],  (char **) one,   40);
	memcpy(ascii[3],  (char **) two,   40);
	memcpy(ascii[4],  (char **) three, 40);
	memcpy(ascii[5],  (char **) four,  40);
	memcpy(ascii[6],  (char **) five,  40);
	memcpy(ascii[7],  (char **) six,   40);
	memcpy(ascii[8],  (char **) seven, 40);
	memcpy(ascii[9],  (char **) eight, 40);
	memcpy(ascii[10], (char **) nine,  40);	
}

bool arg_check(int argc, const char *arg) {
	if (argc != 2) return false;
	if (strlen(arg) != ARG_SIZE) return false;

	for (size_t i = 0; i < ARG_SIZE; i++) {
		char c = arg[i];
		if (!is_digit(c))
			return false;
		if (i == 2 || i == 5)
			if (c != ':')
				return false;
	}

	if (arg[0] >= '2' && arg[1] >= '4')
		return false;
	if (arg[3] >= '6')
		return false;
	if (arg[6] >= '6')
		return false;

	return true; 
}

int convert(char* time) {
	int h = ((time[0] - '0') * 36000000) + ((time[1] - '0') * 3600000); 
	int m = ((time[3] - '0') * 600000)   + ((time[4] - '0') * 60000);
	int s = ((time[6] - '0') * 10000)    + ((time[7] - '0') * 1000);

	return h + m + s;
}

void print_ascii_t(char* ascii[11][5][1], const char* cur_t, int seconds) {
	char output[245] = "";
	for (size_t j = 0; j < 5; j++) {
		for (size_t i = 0; i < 8; i++) {
			char c = cur_t[i];
			char temp[8];
			if (c == ':') {
				if (seconds % 2) {
					strcat(output, "   ");
				} else {
					sprintf(temp, "%3s", ascii[0][0][j]);
					strcat(output, temp);
				}
			} else if (c >= '0' && c <= '9') {
				sprintf(temp, "%7s", ascii[c - '0' + 1][0][j]);
				strcat(output, temp);
			}
		}
		strcat(output, "\n");
	}

	printf(DELETE);
	printf("\n%s", output);
	fflush(stdout);

}

int main(int argc, char** argv) {

	if (!arg_check(argc, argv[1])) {
		printf("The argument must in the format HH:MM:SS\n"); 
		return EXIT_FAILURE;
	}

	int msec_left = 0, prev = -1, trigger = convert(argv[1]);
	
	char* ascii[11][5][1];
	init_ascii(ascii);
	
	printf("\n\n\n\n\n");

	clock_t start = clock();

	do {

		clock_t difference = clock() - start;
		msec_left = trigger - (difference * 1000 / CLOCKS_PER_SEC);

		if (msec_left / 1000 != prev) {

			int seconds = (msec_left / 1000) % 60;
			int minutes = (msec_left / (1000 * 60)) % 60;
			int hours   = (msec_left / (1000 * 60 * 60)) % 24;

			char cur_t[12];
			snprintf(cur_t, sizeof(cur_t), "%02d:%02d:%02d", hours, minutes, seconds);

			print_ascii_t(ascii, cur_t, seconds);

			prev = msec_left / 1000;

		}
	} while (msec_left > 0);

	printf(BELL);

	return EXIT_SUCCESS;
}
