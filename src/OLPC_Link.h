/* This include can be used with both open LPC-Link and CodeRed LPC-Link 
 * The prototypes match for both libraries */

#ifndef OLPC_LINK_H
#define OLPC_LINK_H

#include <config.h>
#include <stdint.h>
#include <libusb.h>


/* Constants */
#define OLPCL_VID			0x1fC9
#define OLPCL_PID			0x0009
#define OLPCL_VIDPID			((OLPCL_VID<<16)+OLPCL_PID)
#define OLPCL_WRITE_DATA_OFFSET		0x4 /* first CR_Write packet data offset */

/* Strings */
#define OLPCL_SERIAL			"OLPCLINK_0_1\0"
#define OLPCL_DESCRIPTION		"Open LPC-Link Driver v0.1\0"

/* Sizes */
#define OLPCL_SERIAL_SIZE		0x10
#define OLPCL_DESCRIPTION_SIZE		0x40
#define OLPCL_PACKET_SIZE		0x200
#define OLPCL_PACKET_HEAD_SIZE		0x6
#define OLPCL_PACKET_DATA_SIZE 		OLPCL_PACKET_SIZE-OLPCL_PACKET_HEAD_SIZE

/* Commands */
#define OLPCL_CMD_READ			0x9
#define OLPCL_CMD_WRITE			0xA
#define OLPCL_CMD_QSTATUS		0x11

/* Error codes */
#define OLPCL_ERROR_TRANSFER_FAILED 	0x12 /* partial transfer, libusb_bulk_transfer error */
#define OLPCL_ERROR_BAD_RESPONSE	0x4 /* bad response packet - usually requested cmd != returned cmd */
#define OLPCL_ERROR_BAD_HEAD_SIZE 	0x10 /* bad head size in response packet (GetStatus) */ 

/* Globals */

extern libusb_context * p_ctx;
extern libusb_device_handle * phandle;
extern unsigned char gb_Opened;
extern unsigned const char pc_SerialNumber[];
extern unsigned const char pc_Description[];

/* Data Structures */

typedef struct {
	uint8_t cmd;		/* packet command */
	uint8_t error;		/* error (mostly unused) */
	uint16_t hsize;		/* size of the header - where from this point data starts *tricky* */
	uint16_t psize;		/* data payload size */
	union {
		uint32_t dsize;
		uint8_t data[OLPCL_PACKET_DATA_SIZE];
	} __attribute__ ((packed)); /* this is why i postponed the release - serious alignment breakage ffs! */
} olpcl_packet;		/* packet used in every libusb_bulk_transfer */

typedef struct {
	uint32_t magic1;	/* those magics stay magical for no */
	uint32_t magic2;	/* no slightest clue what they do */
	uint32_t vidpid;	
	uint32_t magic3;	
	unsigned char serial[OLPCL_SERIAL_SIZE];
	unsigned char description[OLPCL_DESCRIPTION_SIZE];
} olpcl_devinfo; 	/* infolist structure for CR_GetDeviceInfoList */

/* Stubs - they do basically nothing */
int CR_ResetDevice (void);
int CR_Purge (void);
int CR_SetTimeouts (void);
int CR_GetStatus (void);
int CR_GetBitMode (void);
int CR_SetUSBParameters (void);

/* Prototypes */
int CR_FindMatchingDevices (uint16_t idVendor, uint16_t idProduct, libusb_device ** dev);

/* the libusb_device_handle * unused - is my guess, cant be sure what is supposed to be there */
int CR_Read (libusb_device_handle * unused, unsigned char * dest,  uint32_t size, uint32_t * transferred);
int CR_Write (void * unused, void * source, uint32_t size, uint32_t * transferred);

int CR_GetDeviceInfoList (olpcl_devinfo * infolist);
int CR_CreateDeviceInfoList (uint32_t * status);

/* In the CR_OpenEx pass pointer to anything in dummy (NOT NULL!) */
int CR_OpenEx (void * unused1, void * unused2, void * dummy); 
int CR_GetQueueStatus (void * unused, uint32_t * status);

#endif
