#include <OLPC_Link.h>
#include <string.h>

int CR_Write (void * unused, void * source, uint32_t size, uint32_t * transferred) {
	int e, t;
	int source_offset = 0;
	olpcl_packet packet;
	packet.cmd = OLPCL_CMD_WRITE;
	packet.error = 0;
	packet.hsize = 4;
	
	/* First packet payload size determination
	   When First packet of WRITE command is sent, the first 4 bytes
	   of the packet data carry the total size of data to be written
	   so our actual payload is 4 bytes shorter than usual
	   every following packets of WRITE (assuming we have a data bigger
	   than OLPCL_PACKET_DATA_SIZE) don't have those 4 bytes so, the payload
	   is full size for them */
	 
	packet.psize = (size > (OLPCL_PACKET_DATA_SIZE - OLPCL_WRITE_DATA_OFFSET)) 
			? (uint16_t)(OLPCL_PACKET_DATA_SIZE - OLPCL_WRITE_DATA_OFFSET)
			: (uint16_t)size ; /* payload size */ 
	packet.dsize = size; 

	/* Copy data from source to packet payload */
	memcpy((packet.data+OLPCL_WRITE_DATA_OFFSET), (source+source_offset), packet.psize);

	e = libusb_bulk_transfer(phandle, (2 | LIBUSB_ENDPOINT_OUT), 
			(unsigned char *)&packet, OLPCL_PACKET_SIZE, &t, 0);

	if(e || (t != OLPCL_PACKET_SIZE)) {
		*transferred = source_offset;	
		return (OLPCL_ERROR_TRANSFER_FAILED);
	}

	source_offset += packet.psize;
	packet.hsize = 0; /* followup packets should have this member set to 0 */
	

	while(source_offset < size) {
		packet.psize = ((size - source_offset) > OLPCL_PACKET_DATA_SIZE)
				? (uint16_t)(OLPCL_PACKET_DATA_SIZE)
				: (uint16_t)(size - source_offset);

		memcpy(packet.data, (source+source_offset), packet.psize);
		e = libusb_bulk_transfer(phandle, (2 | LIBUSB_ENDPOINT_OUT), 
				(unsigned char *)&packet, OLPCL_PACKET_SIZE, &t, 0);

		if(e || (t != OLPCL_PACKET_SIZE)) {
			*transferred = source_offset;	
			return (OLPCL_ERROR_TRANSFER_FAILED);
		}
		source_offset += packet.psize;
	}

	*transferred = source_offset;

	e = libusb_bulk_transfer(phandle, (2 | LIBUSB_ENDPOINT_IN),
			(unsigned char *)&packet, OLPCL_PACKET_SIZE, &t, 0);
	
	do {
		if(e || (t != OLPCL_PACKET_SIZE)) {
			e = OLPCL_ERROR_TRANSFER_FAILED;
			break;
		}

		if(packet.cmd != OLPCL_CMD_WRITE) 
			e = OLPCL_ERROR_BAD_RESPONSE;
		
	} while (0);

	return(e);
}
