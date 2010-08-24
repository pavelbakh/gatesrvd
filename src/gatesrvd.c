/**
 * Gatesrv Daemon
 * Generate events on tag status change
 * Copyrigt (C) 2010 Pavel Bakhmetiev <icafe@inbox.ru>
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307 USA
*/
#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif // HAVE_CONFIG_H

#include <nfc/nfc.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include <errno.h>
#include <signal.h>

/* Dynamic load */
#include <dlfcn.h>
/* Configuration parser */
#include "nfcconf/nfcconf.h"
/* Debugging functions */
#include "debug/debug.h"
/* Module common defines */
#include "modules/nem_common.h"

#include "types.h"
#include "initdata.h"
//#include "loger.h"
#include "gatectl.h"


#define DEF_POLLING 1    /* 1 second timeout */
#define DEF_EXPIRE 0    /* no expire */

#define DEF_CONFIG_FILE SYSCONFDIR"/gatesrvd.conf"

int polling_time;
int expire_time;
int daemonize;
int debug;
char *cfgfile;
int gatectl_port;

int readers = 0; 			// количество подключенных считывателей

nfcconf_context *ctx;
const nfcconf_block *root;

typedef struct slot_st slot_t;

static module_init_fct_t module_init_fct_ptr = NULL;
static module_event_handler_fct_t module_event_handler_fct_ptr = NULL;
static nfc_device_desc_t* nfc_device_desc = NULL;

static reader_t* readers_list;		// массив с подключенными считывателями
static nfc_device_t** device_list;	// массив устроуств (считывателей)
static tag_t** oldtag_list;				// массив для хранения считанных карт

static int load_module( void ) {
    nfcconf_block **module_list, *my_module;

    module_list = nfcconf_find_blocks ( ctx, root, "module", NULL );
    if ( !module_list ) {
        ERR ( "%s", "Module item not found." );
        return -1;
    }
    my_module = module_list[0];
    free ( module_list );
    if ( !my_module ) {
        ERR ( "%s", "Module item not found." );
        return -1;
    }
    DBG("Loading module: '%s'...", my_module->name->data);
    char module_path[256]={ '\0', };
    strcat(module_path, NEMDIR"/" );
    strcat(module_path, my_module->name->data);
    strcat(module_path, ".so");
    DBG("Module found at: '%s'...", module_path);

    void *module_handler;
    module_handler = dlopen(module_path,RTLD_LAZY);
    if ( module_handler == NULL ) {
        ERR("Unable to open module: %s", dlerror());
        exit(EXIT_FAILURE);
    }

    char module_fct_name[256];
    char *error;

    module_fct_name[0]='\0';
    strcat(module_fct_name,my_module->name->data);
    strcat(module_fct_name,"_init");

    module_init_fct_ptr = dlsym(module_handler,module_fct_name);

    if ((error = dlerror()) != NULL) {
        ERR ("%s", error);
        exit(EXIT_FAILURE);
    }

    module_fct_name[0]='\0';
    strcat(module_fct_name,my_module->name->data);
    strcat(module_fct_name,"_event_handler");

    module_event_handler_fct_ptr = dlsym(module_handler,module_fct_name);
    if ((error = dlerror()) != NULL) {
        ERR ( "%s", error);
        exit(EXIT_FAILURE);
    }

    (*module_init_fct_ptr)( ctx, my_module );
    return 0;
}

static int execute_event ( const nfc_device_t *nfc_device, const reader_t *reader,
												const tag_t* tag, const nem_event_t event ) {
//    return (*module_event_handler_fct_ptr)( nfc_device, tag, event );
	int nGatePin = GATE_STOP;
	switch(event)
	{
		case EVENT_TAG_INSERTED:
		{
/*			switch(reader->readertype)
			{
				case READER_ENTER:
					nGatePin = GATE_ENTER;
//								nGateNum = GATE_DOOR;
					break;
				case READER_EXIT:
					nGatePin = GATE_EXIT;
					break;
				case READER_DOOR:
					nGatePin = GATE_DOOR;
					break;
			}
*/			open_gate(GATE_DOOR);
		    INFO( "Card inserted into device #: %d, type: %d", reader->readernum, reader->readertype);//nfc_device->acName);//, nfc_device );
			break;
		}
		case EVENT_TAG_REMOVED:
		    INFO( "Card removed from device #: %d, type: %d", reader->readernum, reader->readertype);//nfc_device->acName);//, nfc_device );
			break;
	}
	return 0;
}

static int parse_config_file() {
    ctx = nfcconf_new ( cfgfile );
    if ( !ctx ) {
        ERR ( "%s", "Error creating conf context" );
        return -1;
    }
    if ( nfcconf_parse ( ctx ) <= 0 ) {
        ERR ( "Error parsing file '%s'", cfgfile );
        return -1;
    }
    /* now parse options */
    root = nfcconf_find_block ( ctx, NULL, "gatesrvd" );
    if ( !root ) {
        ERR ( "nfc-eventd block not found in config: '%s'", cfgfile );
        return -1;
    }
    debug = nfcconf_get_bool ( root, "debug", debug );
    daemonize = nfcconf_get_bool ( root, "daemon", daemonize );
    polling_time = nfcconf_get_int ( root, "polling_time", polling_time );
    expire_time = nfcconf_get_int ( root, "expire_time", expire_time );
    readers = nfcconf_get_int ( root, "readers", readers );
    gatectl_port = nfcconf_get_int ( root, "gatectlport", GATECTL_COM_PORT );

    if ( debug ) set_debug_level ( 1 );

    DBG( "%s", "Looking for readers." );
    nfcconf_block **device_list, *my_device;

    if (readers > 0)
    {
        device_list = nfcconf_find_blocks ( ctx, root, "reader", NULL );
        if ( !device_list ) {
            ERR ( "%s", "Reader item not found." );
            return -1;
        }
        int i = 0;
        my_device = device_list[i];
        while ( my_device != NULL )
        {
				char* device_driver = (char*)nfcconf_get_str( my_device, "driver", "" );
                strcpy(readers_list[i].readerdriver, device_driver);
                char* device_name = (char*)nfcconf_get_str( my_device, "name", "" );
                strcpy(readers_list[i].readername, device_name);
                readers_list[i].readernum = nfcconf_get_int ( my_device, "id", 0 );
                readers_list[i].readertype = nfcconf_get_int ( my_device, "type", 0 );
                i++;
                my_device = device_list[i];
        }

        INFO( "Found %d readers configuration block(s).", i );
        free ( device_list );
        if (i  == 0)
        {
            ERR("No readers defined in config file");
            return -1;
        }
    }
    else
    {
        ERR("No readres defined in config file");
        return -1;
    }

    return 0;
}

static int parse_args ( int argc, char *argv[] ) {
    int i;
    int res;
    polling_time = DEF_POLLING;
    expire_time = DEF_EXPIRE;
    debug   = 0;
    daemonize  = 0;
    cfgfile = DEF_CONFIG_FILE;
    /* first of all check whether debugging should be enabled */
    for ( i = 0; i < argc; i++ ) {
        if ( ! strcmp ( "debug", argv[i] ) ) set_debug_level ( 1 );
    }
    /* try to find a configuration file entry */
    for ( i = 0; i < argc; i++ ) {
        if ( strstr ( argv[i], "config_file=" ) ) {
            cfgfile = 1 + strchr ( argv[i], '=' );
            break;
        }
    }
    /* parse configuration file */
    if ( parse_config_file() < 0 ) {
        ERR ( "Error parsing configuration file %s", cfgfile );
        exit ( EXIT_FAILURE );
    }

    /* and now re-parse command line to take precedence over cfgfile */
    for ( i = 1; i < argc; i++ ) {
        if ( strcmp ( "daemon", argv[i] ) == 0 ) {
            daemonize = 1;
            continue;
        }
        if ( strcmp ( "nodaemon", argv[i] ) == 0 ) {
            daemonize = 0;
            continue;
        }
        if ( strstr ( argv[i], "polling_time=" ) ) {
            res = sscanf ( argv[i], "polling_time=%d", &polling_time );
            continue;
        }
        if ( strstr ( argv[i], "expire_time=" ) ) {
            res = sscanf ( argv[i], "expire_time=%d", &expire_time );
            continue;
        }
        if ( strstr ( argv[i], "debug" ) ) {
            continue;  /* already parsed: skip */
        }
        if ( strstr ( argv[i], "nodebug" ) ) {
            set_debug_level ( 0 );
            continue;  /* already parsed: skip */
        }
        if ( strstr ( argv[i], "config_file=" ) ) {
            continue; /* already parsed: skip */
        }
        ERR ( "unknown option %s", argv[i] );

        /* arriving here means syntax error */
        printf( "NFC Event Daemon\n" );
        printf( "Usage %s [[no]debug] [[no]daemon] [polling_time=<time>] [expire_time=<limit>] [config_file=<file>]", argv[0] );
        printf( "\nDefaults: debug=0 daemon=0 polltime=%d (ms) expiretime=0 (none) config_file=%s", DEF_POLLING, DEF_CONFIG_FILE );
        exit ( EXIT_FAILURE );
    } /* for */
    /* end of config: return */
    return 0;
}

/**
* @fn ned_get_tag(nfc_device_t* nfc_device, tag_t* tag)
* @brief try to find a valid tag
* @return pointer on a valid tag or NULL.
*/
tag_t*
ned_get_tag(nfc_device_t* nfc_device, tag_t* tag) {
  nfc_target_info_t ti;
  tag_t* rv = NULL;

  if ( tag == NULL ) {
      // We are looking for any tag.
      // Poll for a ISO14443A (MIFARE) tag
      if ( nfc_initiator_select_tag ( nfc_device, NM_ISO14443A_106, NULL, 0, &ti ) ) {
          rv = malloc(sizeof(tag_t));
          rv->ti = ti;
          rv->modulation = NM_ISO14443A_106;
      }
  } else {
      // tag is not NULL, we are looking for specific tag
      // debug_print_tag(tag);
      if ( nfc_initiator_select_tag ( nfc_device, tag->modulation, tag->ti.nai.abtUid, tag->ti.nai.szUidLen, &ti ) ) {
          rv = tag;
      }
  }

  if (rv != NULL) {
      nfc_initiator_deselect_tag ( nfc_device );
  }

  return rv;
}

int
main ( int argc, char *argv[] ) {
//    tag_t* old_tag = NULL;
    tag_t* new_tag;

    readers_list = malloc(MAX_READERS * sizeof(reader_t));
    device_list = malloc(MAX_READERS * sizeof(nfc_device_t));
    oldtag_list = malloc(MAX_READERS * sizeof(tag_t));

//    log_message(LOGFILE, "Start GATE server program");

    int expire_count = 0;

    INFO ("%s", PACKAGE_STRING);

    /* parse args and configuration file */
    parse_args ( argc, argv );

    /* put my self into background if flag is set */
    if ( daemonize ) {
        DBG ( "%s", "Going to be daemon..." );
        if ( daemon ( 0, debug ) < 0 ) {
            ERR ( "Error in daemon() call: %s", strerror ( errno ) );
            return 1;
        }
    }

    // Init gate control devices
	if(init_gatectl(gatectl_port) != 0)
	{
//	    log_message(LOGFILE, "Gate controller open port faled");
		return EXIT_FAILURE;
	}
	int nGateNum = GATE_STOP;
	open_gate(nGateNum);

    INFO ("%s", "Gate controller initialized");

//    load_module();

    /*
     * Wait endlessly for all events in the list of readers
     * We only stop in case of an error
     *
     * COMMENT:
     * There are no way in libnfc API to detect if a card is present or no
     * so the way we proceed is to look for an tag
     * Any ideas will be welcomed
     */
    nfc_device_t* nfc_device;
    nfc_device = NULL;

// Основной цикл программы

    do {
        sleep ( polling_time );
        for(int i = 0; i < readers; i++)
        {
			if(device_list[i] == NULL) // Попытка соединится с устройством
			{
		        nfc_device_desc = malloc(sizeof(nfc_device_desc_t));
		        strncpy(nfc_device_desc->acDevice, readers_list[i].readername, sizeof(readers_list[i].readername));
		        nfc_device_desc->pcDriver = readers_list[i].readerdriver;
		        nfc_device_desc->pcPort   = "";
		        nfc_device_desc->uiSpeed  = 9600;
		        nfc_device_desc->uiBusIndex  = 0;
	        	nfc_device = nfc_connect( nfc_device_desc );
		        if(nfc_device != NULL)
		        {
			        INFO( "Device name: %s, driver: %s Setting ...", nfc_device_desc->acDevice, nfc_device_desc->pcDriver);

			        nfc_initiator_init ( nfc_device );

			        // Drop the field for a while
			        nfc_configure ( nfc_device, NDO_ACTIVATE_FIELD, false );

			        nfc_configure ( nfc_device, NDO_INFINITE_SELECT, false );

			        // Configure the CRC and Parity settings
			        nfc_configure ( nfc_device, NDO_HANDLE_CRC, true );
			        nfc_configure ( nfc_device, NDO_HANDLE_PARITY, true );

			        // Enable field so more power consuming cards can power themselves up
			        nfc_configure ( nfc_device, NDO_ACTIVATE_FIELD, true );

			        device_list[i] = nfc_device;
			        INFO( "Connected to NFC device: %s", nfc_device->acName);//, nfc_device );
			        nfc_device = NULL;
			   }
		        free(nfc_device_desc);
			}
			else // Проверка состояния устройство (карта приложена или снята)
			{
	        	INFO("Reader#: %d/%d", i + 1, readers);
	            new_tag = ned_get_tag(device_list[i], oldtag_list[i]);
	            if(oldtag_list[i] != new_tag)
	            {
					if ( new_tag == NULL )
						execute_event ( device_list[i], &readers_list[i],oldtag_list[i], EVENT_TAG_REMOVED );
					else
						execute_event ( device_list[i], &readers_list[i], new_tag, EVENT_TAG_INSERTED );
					oldtag_list[i] = new_tag;
	            }
			}
        }
    } while ( 1 );

//disconnect:
    for(int i = 0; i < readers; i++)
    {
            nfc_disconnect(device_list[i]);
            DBG ( "NFC device (0x%08x) is disconnected", device_list[i] );
    }

	close_gatectl();

    free ( readers_list );
    free ( device_list );
    free(oldtag_list);

    /* If we get here means that an error or exit status occurred */
    DBG ( "%s", "Exited from main loop" );
    exit ( EXIT_FAILURE );
} /* main */

