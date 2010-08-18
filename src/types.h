#ifndef __TYPES_H__
#define __TYPES_H__

#include <nfc/nfc.h>

typedef enum {
  EVENT_TAG_INSERTED,
  EVENT_TAG_REMOVED,
  EVENT_EXPIRE_TIME
} nem_event_t;

typedef struct {
  nfc_modulation_t modulation;
  nfc_target_info_t ti;
} tag_t;

typedef struct  {
	BYTE			m_cardtype;					//card type
	BYTE			m_status;						//card status
	char			m_cardserial[9];			//card serial
	char			m_firstname[17];			//first name
	char			m_lastname[17];			//last name
	char			m_middlename[17];		//middle name
	BYTE			m_lessonsnum;				//lessons number
	BYTE			m_medday;					//check med day
	BYTE			m_medmonth;				//check med month
	BYTE			m_medyear;					//check med year
	DWORD		m_cash;							//cash on card
	char			m_raspisanie[576];		//lessons stirng MAX_LESSONS * 8
} card_t;

typedef struct {
	int 			nReadernum;						// Номер считывателя
	int 			nReaderType;					// Тип считываетля
	BOOL		bReadFIO;							// Считывать ФИО
	BOOL		bRaspisanie;						// Считывать расписание
	int			nFreeTime;						// Дополнительное время
	int			nZone;								// Номер зоны
	BOOL		bMedCheck;						// Проверять меддопуск
	int			nDelay;								// Задержка повторного прохода
	BOOL		m_readed;							// Карта считана
	card_t		card;									// Данные считанной карты
} reader_t;


#endif
