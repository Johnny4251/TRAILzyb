#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <kisszyb.h>

#define FRAME_WAIT_TIME_SEC 10

#define DEST_CALLSIGN "CQTEST-0"
#define SRC_CALLSIGN "CQTEST-1"
#define ZYB_IMAGE_PATH "cap_image/image.zyb"

int main(int argc, char **argv) {
	
	/* Open ZYB Image */
	const char* IMAGE_PATH = ZYB_IMAGE_PATH;
	FILE *zyb_image = fopen(IMAGE_PATH, "r");

	if(zyb_image == NULL) {
		printf("Error: Could not open %s\n", IMAGE_PATH);
		return 1;
	}
	printf("Opened %s\n",IMAGE_PATH);

	// send 5 empty frames - quickly
	AX25Frame emptyFrame;
	strncpy(emptyFrame.dest_callsign, DEST_CALLSIGN, CALLSIGN_MAX_LEN);
	strncpy(emptyFrame.source_callsign, SRC_CALLSIGN, CALLSIGN_MAX_LEN);
	strcpy(emptyFrame.data, "\\#");
	for (int i=0; i < 5; i++) {
		send_to_direwolf(&emptyFrame, "127.0.0.1", 8001);
	}

	/* Send Image Data */
	char buff[256];
	AX25Frame frame;
	strncpy(frame.dest_callsign, DEST_CALLSIGN, CALLSIGN_MAX_LEN);
	strncpy(frame.source_callsign, SRC_CALLSIGN, CALLSIGN_MAX_LEN);
	while(fgets(buff, 256, zyb_image) != NULL) {
		strcpy(frame.data, buff);
		send_to_direwolf(&frame, "127.0.0.1", 8001);	
		printf("%s", buff);
		sleep(FRAME_WAIT_TIME_SEC);

	}
	printf("\n");
	

	/* Close zyb image and exit */
	fclose(zyb_image);
	printf("Closed %s\n", IMAGE_PATH);
	return 0;
}
