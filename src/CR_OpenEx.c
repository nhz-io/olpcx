#include <OLPC_Link.h>

int CR_OpenEx(void * unused1, void * unused2, void * dummy) {
	int e;
	dummy = NULL;
	gb_Opened = 0;
	if((e=libusb_init(&p_ctx)))
		return(e);

	if(!(phandle=libusb_open_device_with_vid_pid(p_ctx, OLPCL_VID, OLPCL_PID)))
		return(-1);

	if(libusb_kernel_driver_active(phandle, 0))
		if(libusb_detach_kernel_driver(phandle, 0))
			return(-1);
	
	if((e=libusb_claim_interface(phandle, 0)))
		return(e);
	
	gb_Opened = 1;
	return(0);
}			
	
