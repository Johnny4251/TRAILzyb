#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include <kisszyb.h>

#define DEST_CALLSIGN "CQTEST-0"
#define SRC_CALLSIGN "CQTEST-1"
#define ZYB_IMAGE_PATH "cap_image/image.zyb"

#define HEIGHT 250
#define WIDTH  250

int main(int argc, char **argv) {
	
	/* Open ZYB Image */
	const char* IMAGE_PATH = ZYB_IMAGE_PATH;
	FILE *zyb_image = fopen(IMAGE_PATH, "r");

	if(zyb_image == NULL) {
		printf("Error: Could not open %s\n", IMAGE_PATH);
		return 1;
	}
	printf("Opened %s\n",IMAGE_PATH);
	
	Config config = load_config("setup.conf");

	// build image buffer
	int img_array[WIDTH][HEIGHT];
	for (int i=0; i<WIDTH; i++) {
		for (int j=0; j<HEIGHT; j++) {
			char buff[100];
			fgets(buff, sizeof(buff), zyb_image);
			int num = atoi(buff);
			img_array[i][j] = num;
		}
	}

	// send 5 empty frames - quickly
	AX25Frame emptyFrame;
	strncpy(emptyFrame.dest_callsign, config.dest_callsign, CALLSIGN_MAX_LEN);
	strncpy(emptyFrame.source_callsign, config.source_callsign, CALLSIGN_MAX_LEN);
	strcpy(emptyFrame.data, "\\#");
	for (int i=0; i < 5; i++) {
		send_to_direwolf(&emptyFrame, "127.0.0.1", 8001);
	}

	/* Send Image Data */
	AX25Frame frame;
	strncpy(frame.dest_callsign, config.dest_callsign, CALLSIGN_MAX_LEN);
	strncpy(frame.source_callsign, config.source_callsign, CALLSIGN_MAX_LEN);
	float percent = 0.00;
	int bar_progress = 0;
	printf("TX_STATUS: [                    ] 0%% COMPLETE");
	fflush(stdout);

	for (int i=0; i < HEIGHT; i++) {
		char buff[WIDTH] = {0};
		for (int j=0; j<WIDTH; j++) {
			buff[j] = (char)img_array[i][j];
		}

		strcpy(frame.data, buff);
		send_to_direwolf(&frame, "127.0.0.1", 8001);
		
		percent = (i/250.0) * 100;

		// Update loading bar every 5% completion 
		int new_bar_progress = (int)(percent / 5);
		if(new_bar_progress > bar_progress) {
			bar_progress = new_bar_progress;
			printf("\rTX_STATUS: [");
			for (int j = 0; j < bar_progress; j++)
				printf("#");
			for (int j = bar_progress; j < 20; j++)
				printf(" ");
			printf("] %.0f%% COMPLETE", percent);
			fflush(stdout);
		}

		sleep(config.wait_time_sec);
		
	}
	printf("\n");
	/* Close zyb image and exit */
	fclose(zyb_image);
	printf("Closed %s\n", IMAGE_PATH);
	
	printf("Sending end frames\n");
	for (int i=0; i < 3; i++) {
		strcpy(frame.data, "END");
		send_to_direwolf(&frame, "127.0.0.1", 8001);
		sleep(1);
	}

	printf("DONE!\n");

	return 0;
}
