#include <OLPC_Link.h>
#include <errno.h>

int CR_FindMatchingDevices(uint16_t idVendor, uint16_t idProduct, libusb_device ** dev) {
	int e = 0;
	ssize_t count;
	struct libusb_device_descriptor desc;
	libusb_device ** list;
	*dev = NULL;

	if(!idVendor || !idProduct || !dev)	
		return(EINVAL);	/* Bad args */
	
	if((e=libusb_init(&p_ctx))) 
		return(e);	/* Init failed */
	
	if(!(count=libusb_get_device_list(p_ctx, &list))) {
		libusb_exit(p_ctx);
		return(LIBUSB_ERROR_NO_DEVICE);	/* Zero devices in list */
	}
	if(count<0) {
		libusb_exit(p_ctx);
		return(count);	/* Failed to get device list */
	}
	while(--count >= 0) {
		if(list[count] == NULL) {
			libusb_free_device_list(list, 1);
			libusb_exit(p_ctx);
			return(LIBUSB_ERROR_NO_DEVICE); /* Something seriously wrong */
		}
		if((e=libusb_get_device_descriptor(list[count], &desc))) {
			libusb_free_device_list(list, 1);
			libusb_exit(p_ctx);
			return(e); /* Could not get device descriptor */
		}
		if(desc.idVendor == idVendor && desc.idProduct == idProduct) {
			*dev = libusb_ref_device(list[count]);
			libusb_free_device_list(list, 1);
			return(0);
		}
	}

	libusb_free_device_list(list, 1);
	libusb_exit(p_ctx);
	return(0);
}
