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
	BOOL			m_readed;						//card readed
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

#define KEY_OLD_A _T("")
#define KEY_DATA_A _T("sp0208")
#define KEY_CASH_A _T("cash08")
#define KEY_CASH_B _T("160208")

#define READER_ENTER						0		// вход
#define READER_EXIT							1		// выход
#define READER_PUTCARD					2		// картоприемник
#define READER_SERVICE						3		// сервис
#define READER_NOTYPE						4		//

#define DATASECTORS							15	// Секторов с данными

// сектор 0
#define CARDDATA_BLOCK					1		// Блок основных данных карты
// сектор 1
#define LASTNAME_BLOCK					4		// Фамилия держателя карты
#define FIRSTNAME_BLOCK					5		// Имя держателя карты
#define MIDDLENAME_BLOCK				6		// Отчество держателя карты
// сектор 2
#define RASPISANIE_BLOCK					8		// Начальный блок области расписания
// сектор 15
#define CASHV_BLOCK							62		// Блок кошелька

#define MAX_LESSONS							72		// Максимальное число занятий

#define PASS_ENTER								0		// Карта приложена к считывателю входа
#define PASS_EXIT									1		// Карта приложена к считывателю выхода
#define PASS_PUTCARD							2		// Карта приложена к считывателю картоприемника

#define PASS_ALLOW_SUCCESS			0		// Успешный проход
#define PASS_ALLOWERROR				1		// Выход с ошибкой расписания
#define PASS_DENIED_BROKENLIST	40	// Проход запрещен - карта в списке сломанных
#define PASS_DENIED_STOPLIST			41	// Проход запрещен - карта в стоп-листе
#define PASS_DENIED_BADREADER	42	// Проход запрещен - неверный считываетель
#define PASS_DENIED_CARDTYPE		43	// Проход запрещен - неверный тип карты
#define PASS_DENIED_RASPISANIE		44	// Проход запрещен - проход не по расписанию
#define PASS_DENIED_MEDCHECK		45	// Проход запрещен - нет меддопуска
#define PASS_DENIED_DELAY				46	// Проход запрещен - повторный проход

#define PASS_BY_CARDTYPE				0		// Проход по типу карты
#define PASS_BY_DATE							1		// Проход по дате занятий
#define PASS_BY_TIME							2		// Проход по дате и времени занятий

// Тип устройства
#define DEV_TRIPOD								1		// Турникет
#define DEV_LOCK									2		// Замок
#define DEV_CARDHOLDER					3		// Картоприемник

// Длительность управляющих сигналов
#define CARDHOLDER_TIMER_LONG	3		// для картоприемника
#define TRIPOD_TIMER_LONG				2		// для турникета
#define LOCK_TIMER_LONG					5		// для замка

// Тип карты
#define CARD_GUEST 							0		// гостевая
#define CARD_ABONEMENT 					1		// абонемент
#define CARD_WORKER 						2		// сотрудник
#define CARD_WORKEREX 					3		// спортсмен
#define CARD_SECURITY 						4		// СБ
#define CARD_ARENDA 						5		// арендатор
#define CARD_TICKET 							6		// билет
#define CARD_ALLROAD 						7		// ...

#endif
