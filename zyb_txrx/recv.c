#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <kisszyb.h>

#define HEIGHT 250
#define WIDTH 250

int main(int argc, char **argv) {

	if(argc < 2) {
		printf("USAGE: %s <image_path>\n", argv[0]);
		return -1;
	}

	char *filepath = (char*) malloc(sizeof(argv[1]));
	strcpy(filepath, argv[1]);

	FILE *file = fopen(filepath, "r");
	if(file) {
		printf("File already exists.. overwrite? ");
		char response;
	        scanf(" %c", &response);

		if (response == 'y' || response == 'Y') {
                	printf("overwriting %s\n", filepath);
		} else {
			fclose(file);
			printf("exiting...\n");
			return -1;
		}
		fclose(file);
        }
	
	file = fopen(filepath, "w");
	printf("Image will be saved as: %s\n", filepath);

	int frames_recvd = 0;
	printf("\n");
	while(1) {
		char row[WIDTH];
		recv_from_direwolf(row, WIDTH+5, "127.0.0.1", 8001);
				
		// check for end
		if(strncmp(row+2, "END", strlen("END")) == 0) {
			break;
		}

		if(strlen(row) > 2) {
		    for(int i=0; i<HEIGHT; i++) {
			    fprintf(file, "%d\n", (uint8_t)row[i+2]);
		    }
		    frames_recvd += 1;
		    
        	}
		printf("\rFrame Count: \t%d/250  ", frames_recvd);
		fflush(stdout);
	}
	printf("Frame Count: \t%d/250  ", frames_recvd);
	fflush(stdout);

	int less_recvd = 250 - frames_recvd;
	printf("Finished with: %d frames missing!\n", less_recvd);

	fclose(file);
	return 0;
}
