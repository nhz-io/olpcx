#include <OLPC_Link.h>

int CR_CreateDeviceInfoList(uint32_t * status) {
	libusb_device * dev = NULL;
	*status = 0;
	CR_FindMatchingDevices(0x1fc9, 0x0009, &dev);
	if(dev != NULL)
		*status = 1;
	return(0);
}
		
