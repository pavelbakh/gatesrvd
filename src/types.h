#ifndef __TYPES_H__
#define __TYPES_H__

#include <nfc/nfc.h>

typedef enum {
	EVENT_TAG_INSERTED,
	EVENT_TAG_REMOVED
} event_t;

typedef enum {
  CMD_PASS_SUCCESS,
  CMD_PASS_DENIED,
  CMD_PASS_FAIL,
  CMD_INIT_GATE,
  CMD_EXIT_GATE
} nem_command_t;

typedef struct {
  nfc_modulation_t modulation;
  nfc_target_info_t ti;
} tag_t;

typedef struct  {
	int				cardtype;						//card type
	int				status;							//card status
	char			cardserial[9];					//card serial
	char			firstname[17];				//first name
	char			lastname[17];				//last name
	char			middlename[17];			//middle name
	int				lessonsnum;					//lessons number
	int				medday;							//check med day
	int				medmonth;					//check med month
	int				medyear;						//check med year
	long			cash;								//cash on card
	char			raspisanie[576];			//lessons stirng MAX_LESSONS * 8
} card_t;

typedef struct {
	int 			readernum;						// Номер считывателя
	int 			readertype;						// Тип считываетля
	int			readfio;								// Считывать ФИО
	int			raspisanie;							// Считывать расписание
	int			freetime;							// Дополнительное время
	int			zone;									// Номер зоны
	int			medcCheck;						// Проверять меддопуск
	int			delay;									// Задержка повторного прохода
	int			readed;								// Карта считана
	char		readername[256];			// Имя считывателя
	char		readerdriver[256];			// Наименование драйвера для считывателя
	card_t		card;									// Данные считанной карты
} reader_t;


#endif
