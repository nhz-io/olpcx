#include <config.h>
#include <OLPC_Link.h>

/* Globals */
libusb_context * p_ctx;
libusb_device_handle * phandle;
unsigned char gb_Opened;
unsigned const char pc_SerialNumber[OLPCL_SERIAL_SIZE] = OLPCL_SERIAL;
unsigned const char pc_Description[OLPCL_DESCRIPTION_SIZE] = OLPCL_DESCRIPTION;

/* Every function below is just a stub - originally unimplemented */
int CR_ResetDevice(void) { return(0); }
int CR_Purge(void) { return(0); }
int CR_SetTimeouts(void) { return(0); }
int CR_GetStatus(void) { return(0); }
int CR_GetBitMode(void) { return(0); }
int CR_SetUSBParameters(void) { return(0x11); }
int CR_SetBitMode(void) { return(0); }
int CR_SetLatencyTimer(void) { return(0x11); }

