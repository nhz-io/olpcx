#include <OLPC_Link.h>

int CR_GetQueueStatus (void * unused, uint * status) {	
	int e;
	int t;
	olpcl_packet packet;
	*status = 0;
	packet.cmd = OLPCL_CMD_QSTATUS;
	packet.error = 0;
	packet.hsize = 4;
	packet.psize = 0;
	
	do {
		e = libusb_bulk_transfer(phandle, (2 | LIBUSB_ENDPOINT_OUT), 
			(unsigned char *)&packet, OLPCL_PACKET_SIZE, &t, 0);
		if(e || (t != OLPCL_PACKET_SIZE)) {
			e = OLPCL_ERROR_TRANSFER_FAILED;
			break;
		}

		e = libusb_bulk_transfer(phandle, (2 | LIBUSB_ENDPOINT_IN),
			(unsigned char *)&packet, OLPCL_PACKET_SIZE, &t, 0);

		if(e || (t != OLPCL_PACKET_SIZE)) {
			e = OLPCL_ERROR_TRANSFER_FAILED;
			break;
		}

		if(packet.cmd != OLPCL_CMD_QSTATUS) {
			e = OLPCL_ERROR_BAD_RESPONSE;
			break;
		}
		if(packet.hsize != 4) {
			e = OLPCL_ERROR_BAD_HEAD_SIZE;
			break;
		}
		if(packet.error) {
			e = packet.error;
			break;
		}
		*status = ((uint32_t *)packet.data)[0];
	} while (0);
	return (e);
}
