#include <OLPC_Link.h>

int CR_Close(void) {
	int e;
	gb_Opened = 0;
	if((e=libusb_release_interface(phandle, 0)))
		return(e);

	libusb_close(phandle);
	libusb_exit(p_ctx);
	return(0);
}	
