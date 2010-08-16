//
// C header: execute
//
// Description:
//
//
// Author: Pavel Bakhmetiev <icafe@inbox.ru>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __NEM_EXECUTE__
#define __NEM_EXECUTE__

#include "nem_common.h"

void nem_execute_init(nfcconf_context *module_context, nfcconf_block* module_block);
int nem_execute_event_handler(const nfc_device_t* nfc_device, const tag_t* tag, const nem_event_t event);

#endif /* __NEM_EXECUTE__ */

