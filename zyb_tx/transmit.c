#include <string.h>
#include <kisszyb.h>

int main(int argc, char **argv) {

	char buff[256] = "\\#";
	AX25Frame myFrame;

	strncpy(myFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
	strncpy(myFrame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);

	strcpy(myFrame.data, buff);
	send_to_direwolf(&myFrame, "127.0.0.1", 8001);

	return 0;
}
