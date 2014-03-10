#include <OLPC_Link.h>
#include <string.h>

int CR_Read(libusb_device_handle * unused, unsigned char * dest, uint32_t size, uint32_t * transferred) {
	int e, t;
	int dest_offset = 0;
	olpcl_packet packet;
	packet.cmd = OLPCL_CMD_READ;
	while(dest_offset < size) {
		packet.hsize = 4;
		packet.error = 0;
		packet.psize = 0;
		packet.dsize = (size > OLPCL_PACKET_DATA_SIZE) 
			? OLPCL_PACKET_DATA_SIZE
			: size;
		
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
		
		if((packet.cmd != OLPCL_CMD_READ) || (packet.hsize != 0)) {
			e = OLPCL_ERROR_BAD_RESPONSE;
			break;
		}
		
		memcpy((dest+dest_offset), packet.data, packet.psize);
		dest_offset += packet.psize;
	}
	*transferred = dest_offset;
	return(e);
}
