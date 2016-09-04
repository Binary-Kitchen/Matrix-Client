#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "matrix.h"
#include "tools.h"

static bool sil, dev;

static enum matrix_cmd m_mode = MATRIX_MODE_MONOCHROME;

static int sockfd = 0;
static struct sockaddr_in servaddr;
static socklen_t len;

static void matrix_send(picture_t * pic);

int matrix_init(bool silence, bool develop, const char *server, int port)
{
	struct hostent *hp;
	
	sil = silence;
	dev = develop;
	
	if (dev == true)
		return 0;
	
	bzero(&servaddr, sizeof(servaddr));
	
	printf("Connecting to %s:%d\n", server, port);
	
	hp = gethostbyname(server);
	if (!hp) {
		fprintf(stderr, "Could not obtain address of %s\n", server);
		return -1;
	}
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	
	return matrix_cmd(MATRIX_MODE_MONOCHROME);
}

void matrix_close(void)
{
	if (sockfd)
		close(sockfd);
}

void matrix_update(picture_t * pic)
{
	if (sil == false)
		picture_print(pic, m_mode);
	if (dev == false)
		matrix_send(pic);
}

int matrix_cmd(enum matrix_cmd cmd)
{
	uint32_t matrix_retval;
	ssize_t n;

	if (cmd == MATRIX_MODE_GRAYSCALE || cmd == MATRIX_MODE_MONOCHROME)
		m_mode = cmd;

	if (dev == true)
		return 0;

	sendto(sockfd, &cmd, sizeof(uint32_t), 0,
	       (struct sockaddr *)&servaddr, sizeof(servaddr));

	// Give 10ms
	usleep(10000);

	n = recvfrom(sockfd, (void *)&matrix_retval, sizeof(matrix_retval),
		     MSG_DONTWAIT, (struct sockaddr *)&servaddr, &len);

	if (n != 4 || matrix_retval != cmd)
		return -1;

	return 0;
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
	int x, y;
	unsigned char brightness;

	// Independent from mode, this is the maximum size
	matrix_frame_t buffer;
	// Zero buffer
	bzero(&buffer, sizeof(buffer));

	// Convert pic to matrix picture
	if (m_mode == MATRIX_MODE_MONOCHROME) {
		for (x = 0; x < NUM_COLS; x++) {
			for (y = 0; y < NUM_ROWS; y++) {
				brightness = picture_getPixel(pic, x, y);
				if (brightness)
					matrix_setPixelMonochrome((matrix_picture_t *)&buffer,
											  x, y);
			}
		}

	} else if (m_mode == MATRIX_MODE_GRAYSCALE) {
		for (x = 0; x < NUM_COLS; x++) {
			for (y = 0; y < NUM_ROWS; y++) {
				brightness = picture_getPixel(pic, x, y);
				if (brightness) {
					brightness = brightness / PIX_FACTOR;
					if (brightness == 8)
						brightness--;
					matrix_setPixelGreyscale(&buffer, x, y, brightness);
				}
			}
		}
	}
	sendto(sockfd, buffer, sizeof(matrix_picture_t), 0,
		   (struct sockaddr *)&servaddr, sizeof(servaddr));
}
