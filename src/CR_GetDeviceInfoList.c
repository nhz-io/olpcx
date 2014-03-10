#include <OLPC_Link.h>
#include <string.h>

int CR_GetDeviceInfoList(olpcl_devinfo * infolist) {
	if(gb_Opened) {
		infolist->magic1 = 1;
		infolist->magic2 = 3;
		return(0);
	}
	infolist->magic1 = 0;
	infolist->magic2 = 4;
	infolist->vidpid = OLPCL_VIDPID;
	infolist->magic3 = 0;
	
	memcpy(infolist->serial, pc_SerialNumber, OLPCL_SERIAL_SIZE);
	memcpy(infolist->description, pc_Description, OLPCL_DESCRIPTION_SIZE);
	return(0);
} 
