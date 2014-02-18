#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "matrix.h"
#include "tools.h"

static int sil = 0;
static int dev = 0;

static uint32_t m_mode = MATRIX_MODE_MONOCHROME;

static int sockfd = 0;
static struct sockaddr_in servaddr;
static socklen_t len;

static void matrix_send(picture_t * pic);

int matrix_init(int silence, int develop, const char *server, int port)
{
	sil = silence;
	dev = develop;
	int retval = 0;

	if (dev == 0) {
		struct hostent *hp;
		bzero(&servaddr, sizeof(servaddr));

		printf("Connecting to %s:%d\n", server, port);

		hp = gethostbyname(server);
		if (!hp) {
			fprintf(stderr, "Could not obtain address of %s\n",
				server);
			return -1;
		}

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0],
		       hp->h_length);
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);

		retval = matrix_cmd(MATRIX_MODE_MONOCHROME);
	}

	return retval;
}

void matrix_close(void)
{
	if (dev == 0) {
		close(sockfd);
	}
}

void matrix_update(picture_t * pic)
{
	if (sil == 0) {
		picture_print(pic, m_mode);
	}
	if (dev == 0) {
		matrix_send(pic);
	}
}

int matrix_cmd(uint32_t cmd)
{
	int retval = 0;
	if (cmd == MATRIX_MODE_GRAYSCALE || cmd == MATRIX_MODE_MONOCHROME) {
		m_mode = cmd;
	}

	if (dev == 0) {
		uint32_t matrix_retval;
		ssize_t n;

		sendto(sockfd, &cmd, sizeof(uint32_t), 0,
		       (struct sockaddr *)&servaddr, sizeof(servaddr));

		// Give 10ms
		usleep(10000);
		n = recvfrom(sockfd, (void *)&matrix_retval,
			     sizeof(matrix_retval), MSG_DONTWAIT,
			     (struct sockaddr *)&servaddr, &len);
		if (n != 4) {
			retval = 1;
		} else if (matrix_retval != cmd) {
			retval = 1;
		}
	}

	return retval;
}

#define MATRIX_NUM_PANELS_ROWS 5
#define MATRIX_NUM_PANELS_COLS 5

#define MATRIX_BYTES_PER_PANEL 10
#define MATRIX_BITS_PER_PANEL (MATRIX_BYTES_PER_PANEL*8)
#define MATRIX_NUM_PANELS (MATRIX_NUM_PANELS_ROWS*MATRIX_NUM_PANELS_COLS)

#define MATRIX_NUM_SHIFTERS ((MATRIX_NUM_PANELS+7)/8)

#define MATRIX_NUM_ROWS (MATRIX_NUM_PANELS_ROWS*9)
#define MATRIX_NUM_COLS (MATRIX_NUM_PANELS_COLS*8)

#define MATRIX_PANEL_NO(x,y) ( (x/8) + MATRIX_NUM_PANELS_ROWS*(y/9) )

#define MATRIX_PICTURE_SIZE (MATRIX_NUM_SHIFTERS * MATRIX_BITS_PER_PANEL)

#define MATRIX_BYTES_ROWS ((MATRIX_NUM_ROWS+7)/8)
#define MATRIX_BYTES_COLS ((MATRIX_NUM_COLS+7)/8)

#define MATRIX_NUM_PICTURES_PER_FRAME 3

#define MATRIX_COMMAND_LEN 4

#define MATRIX_CMD_IGNORE 0

typedef unsigned char matrix_picture_t[MATRIX_PICTURE_SIZE];
typedef matrix_picture_t matrix_frame_t[MATRIX_NUM_PICTURES_PER_FRAME];

static void matrix_setPixelGreyscale(matrix_frame_t * pic, unsigned int x,
				     unsigned int y, int brightness)
{
	unsigned int x_o = x % 8;
	unsigned int y_o = y % 9;

	unsigned int panel_no = MATRIX_PANEL_NO(x, y);

	unsigned int array_offset = (((x_o * 10) + y_o) * 4) + 4;
	unsigned int bit_offset = panel_no;

	array_offset += 3 - (panel_no / 8);
	bit_offset %= 8;

	int bit;
	for (bit = 0; bit < 3; bit++) {
		if (brightness & (1 << bit)) {
			*(((*pic)[bit]) + array_offset) |= (1 << bit_offset);
		}
	}
}

static void matrix_setPixelMonochrome(matrix_picture_t * pic, unsigned int x,
				      unsigned int y)
{
	unsigned int x_o = x % 8;
	unsigned int y_o = y % 9;

	unsigned int panel_no = MATRIX_PANEL_NO(x, y);

	unsigned int array_offset = (((x_o * 10) + y_o) * 4) + 4;
	unsigned int bit_offset = panel_no;

	array_offset += 3 - (panel_no / 8);
	bit_offset %= 8;

	*((*pic) + array_offset) |= (1 << bit_offset);
}

static void matrix_send(picture_t * pic)
{
	// Independent from mode, this is the maximum size
	matrix_frame_t buffer;
	// Zero buffer
	bzero(&buffer, sizeof(buffer));

	// Convert pic to matrix picture
	int x, y;

	if (m_mode == MATRIX_MODE_MONOCHROME) {
		for (x = 0; x < NUM_COLS; x++) {
			for (y = 0; y < NUM_ROWS; y++) {
				unsigned char bright =
				    picture_getPixel(pic, x, y);
				if (bright) {
					matrix_setPixelMonochrome((matrix_picture_t *) & buffer, x, y);
				}
			}
		}
		sendto(sockfd, buffer, sizeof(matrix_picture_t), 0,
		       (struct sockaddr *)&servaddr, sizeof(servaddr));
	} else if (m_mode == MATRIX_MODE_GRAYSCALE) {
		for (x = 0; x < NUM_COLS; x++) {
			for (y = 0; y < NUM_ROWS; y++) {
				unsigned char bright =
				    picture_getPixel(pic, x, y);
				if (bright) {
					bright = bright / PIX_FACTOR;
					if (bright == 8)
						bright--;
					matrix_setPixelGreyscale(&buffer, x, y,
								 bright);
				}
			}
		}
		sendto(sockfd, buffer, sizeof(matrix_frame_t), 0,
		       (struct sockaddr *)&servaddr, sizeof(servaddr));
	}
}
