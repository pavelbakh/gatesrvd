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
	BYTE			m_cardtype;			//card type
	BYTE			m_status;			//card status
	char			m_cardserial[9];	//card serial
	char			m_firstname[17];	//first name
	char			m_lastname[17];		//last name
	char			m_middlename[17];	//middle name
	BYTE			m_lessonsnum;		//lessons number
	BYTE			m_medday;			//check med day
	BYTE			m_medmonth;			//check med month
	BYTE			m_medyear;			//check med year
	DWORD			m_cash;				//cash on card
	BOOL			m_readed;			//card readed
	char			m_raspisanie[576];	//lessons stirng MAX_LESSONS * 8
} card_t;

typedef struct {
	int nReadernum;
	int nReaderType;
} reader_t;

#define KEY_OLD_A _T("")
#define KEY_DATA_A _T("sp0208")
#define KEY_CASH_A _T("cash08")
#define KEY_CASH_B _T("160208")

#define READER_ENTER	0	// enter
#define READER_EXIT		1	// exit
#define READER_PUTCARD	2	// cardholder
#define READER_SERVICE	3	// service
#define READER_NOTYPE	4	//

#define DATASECTORS			15		// ����� ��������

// ������ 0
#define CARDDATA_BLOCK		1		// ���� �������� ������ �����
#define ABONEMENT_BLOCK		2		// ���� � �������� �����������
// ������ 1
#define LASTNAME_BLOCK		4		// ������� ��������� �����
#define FIRSTNAME_BLOCK		5		// ��� ��������� �����
#define MIDDLENAME_BLOCK	6		// �������� ��������� �����
// ������ 2
#define RASPISANIE_BLOCK	8		// ��������� ���� ������� ����������
// ������ 15
#define CASHV_BLOCK			62		// ���������� ��������

#define MAX_LESSONS			72		// ������������ ����� ������� ��� ������� ���� �����

#define PASS_ENTER			0		// ����� ��������� � ����������� �����
#define PASS_EXIT			1		// ����� ��������� � ����������� ������
#define PASS_PUTCARD		2		// ����� ��������� � ����������� ��������������

#define PASS_ALLOW_SUCCESS		0	// �������� ������
#define PASS_ALLOWERROR			1	// ������ �������� � ������� ������� ������
#define PASS_DENIED_BROKENLIST	40	// ������ �������� - ����� � ������ ��������� ����
#define PASS_DENIED_STOPLIST	41	// ������ �������� - ����� � ����-�����
#define PASS_DENIED_BADREADER	42	// ������ �������� - ����� ��������� �� � ���� ����������� (��� ��������������)
#define PASS_DENIED_CARDTYPE	43	// ������ �������� ��� ������� ���� ����
#define PASS_DENIED_RASPISANIE	44	// ������ �������� - ������� ������ �� �� ����������
#define PASS_DENIED_MEDCHECK	45	// ������ �������� - ������� ��� ����������
#define PASS_DENIED_DELAY		46	// ������ �������� - ������� ���������� ������� ������ ����������� �������

#define PASS_BY_CARDTYPE		0	// ������ �� ���� �����
#define PASS_BY_DATE			1	// ������ �� ����� ����������
#define PASS_BY_TIME			2	// ������ �� ����� � ������� ����������

// ��� ����������
#define DEV_TRIPOD				1	// ��������
#define DEV_LOCK				2	// ������������
#define DEV_CARDHOLDER			3	// �������������
// ������������ ����������� ��������
#define CARDHOLDER_TIMER_LONG	3	// ����� �������� ��������� �������������� (� �����)
#define TRIPOD_TIMER_LONG		2	// ����� �������� ����� ��������� (� �����)
#define LOCK_TIMER_LONG			5	// ����� �������� ����������������� ����� (� �����)

// ��� �����
#define CARD_GUEST 				0	// ��������
#define CARD_ABONEMENT 			1	// ���������
#define CARD_WORKER 			2	// ���������
#define CARD_WORKEREX 			3	// ��������� (� ������������ ������� �������)
#define CARD_SECURITY 			4	// ��������� ������
#define CARD_ARENDA 			5	// ���������
#define CARD_TICKET 			6	// �����
#define CARD_ALLROAD 			7	// ...

#endif
