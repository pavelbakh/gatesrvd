//
// C header: dbus
//
// Description:
//
//
// Author: Pavel Bakhmetiev <icafe@inbox.ru>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __NEM_DBUS__
#define __NEM_DBUS__

#include "nem_common.h"

#define NFC_DBUS_SERVICE      "org.freedevice.NFC"
#define NFC_DBUS_PATH         "/org/freedevice/NFC"
#define NFC_DBUS_INTERFACE    "org.freedevice.NFC"

void nem_dbus_init(nfcconf_context *module_context, nfcconf_block* module_block);
int nem_dbus_event_handler(const nfc_device_t* nfc_device, const tag_t* tag, const nem_command_t cmd);

#endif /* __NEM_DBUS__ */
