#include "pch.h"
#include "SerialPort.h"

HANDLE  m_hComm;/** ���ھ�� */
bool s_bExit = false;/** �߳��˳���־���� */
volatile HANDLE m_hListenThread;/** �߳̾�� */
CRITICAL_SECTION m_csCommunicationSync;//!< �����������/** ͬ������,�ٽ������� */
const UINT SLEEP_TIME_INTERVAL = 5;

bool InitPort(UINT portNo /*= 1*/, UINT baud /*= CBR_9600*/, char parity /*= 'N'*/,
	UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/)
{

	/** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */
	if (!openPort(portNo))
	{
		return false;
	}

	/** �����ٽ�� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** �Ƿ��д����� */
	BOOL bIsSuccess = TRUE;

	/** �ڴ˿���������������Ļ�������С,���������,��ϵͳ������Ĭ��ֵ.
	*  �Լ����û�������Сʱ,Ҫע�������Դ�һЩ,���⻺�������
	*/
	/*if (bIsSuccess )
	{
	bIsSuccess = SetupComm(m_hComm,10,10);
	}*/

	/** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */
	COMMTIMEOUTS  CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	if (bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
	}

	DCB  dcb;
	if (bIsSuccess)
	{
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);// ��ANSI�ַ���ת��ΪUNICODE�ַ���
		wchar_t* pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB((LPCWSTR)pwText, &dcb);/** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */
		dcb.fRtsControl = RTS_CONTROL_ENABLE;/** ����RTS flow���� */
		delete[] pwText;/** �ͷ��ڴ�ռ� */
	}

	if (bIsSuccess)
	{
		bIsSuccess = SetCommState(m_hComm, &dcb);/** ʹ��DCB�������ô���״̬ */
	}

	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);/**  ��մ��ڻ����� */
	LeaveCriticalSection(&m_csCommunicationSync);/** �뿪�ٽ�� */

	return bIsSuccess == TRUE;
}

bool openPort(UINT portNo)
{
	EnterCriticalSection(&m_csCommunicationSync);//�����ٽ���

	char szPort[50];
	sprintf_s(szPort, "COM%d", portNo);//�Ѵ��ں�ת��Ϊ�豸��

	/** ��ָ���Ĵ��� */
	m_hComm = CreateFileA(szPort,	/** �豸��,COM1,COM2�� */
		GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */
		0,                            /** ����ģʽ,0��ʾ������ */
		NULL,						/** ��ȫ������,һ��ʹ��NULL */
		OPEN_EXISTING,				/** �ò�����ʾ�豸�������,���򴴽�ʧ�� */
		0,
		0);

	if (m_hComm == INVALID_HANDLE_VALUE)/** �����ʧ�ܣ��ͷ���Դ������ */
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	LeaveCriticalSection(&m_csCommunicationSync);/** �˳��ٽ��� */

	return true;
}

void ClosePort()
{
	/** ����д��ڱ��򿪣��ر��� */
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
}

//bool OpenListenThread()
//{
//	if (m_hListenThread != INVALID_HANDLE_VALUE)/** ����߳��Ƿ��Ѿ������� */
//	{
//		/** �߳��Ѿ����� */
//		return false;
//	}
//
//	s_bExit = false;
//	UINT threadId;/** �߳�ID */
//	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, OpenListenThread, 0, &threadId);/** �����������ݼ����߳� */
//	//m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);/** �����������ݼ����߳� */
//	if (!m_hListenThread)
//	{
//		return false;
//	}
//	
//	if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))/** �����̵߳����ȼ�,������ͨ�߳� */
//	{
//		return false;
//	}
//
//	return true;
//}

UINT GetBytesInCOM()
{
	DWORD dwError = 0;	/** ������ */
	COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */
	}

	return BytesInQue;
}

//UINT WINAPI ListenThread(void* pParam)
//{
//	/** �õ������ָ�� */
//	//CChessDlg *pSerialPort = reinterpret_cast<CChessDlg*>(pParam);
//
//	// �߳�ѭ��,��ѯ��ʽ��ȡ��������
//	//while (!pSerialPort->s_bExit)
//	while (!s_bExit)
//	{
//		//UINT BytesInQue = pSerialPort->GetBytesInCOM();
//		UINT BytesInQue = GetBytesInCOM();
//		/** ����������뻺������������,����Ϣһ���ٲ�ѯ */
//		if (BytesInQue == 0)
//		{
//			Sleep(SLEEP_TIME_INTERVAL);
//			continue;
//		}
//
//		/** ��ȡ���뻺�����е����ݲ������ʾ */
//		char cRecved = 0x00;
//		do
//		{
//			cRecved = 0x00;
//			//if (pSerialPort->ReadChar(cRecved) == true)
//			if (ReadChar(cRecved) == true)
//			{
//				//printf("%c", cRecved);
//				//continue;
//				receiveMessage[messageIndex++] = cRecved;
//				if (messageIndex >= 4)
//				{
//					//std::cout << receiveMessage << std::endl;
//					printf("%s\n", receiveMessage);
//					messageIndex = 0;
//					sss.MoveChess(receiveMessage[0] - 48, receiveMessage[1] - 48, receiveMessage[2] - 48, receiveMessage[3] - 48);
//				}
//			}
//		} while (--BytesInQue);
//	}
//
//	return 0;
//}

bool ReadChar(char& cRecved)
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	EnterCriticalSection(&m_csCommunicationSync);/** �ٽ������� */
	bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);/** �ӻ�������ȡһ���ֽڵ����� */
	if ((!bResult))
	{
		/** ��ȡ������,���Ը��ݸô�����������ԭ�� */
		DWORD dwError = GetLastError();

		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	LeaveCriticalSection(&m_csCommunicationSync);/** �뿪�ٽ��� */

	return (BytesRead == 1);

}

bool WriteData(unsigned char* pData, unsigned int length)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	EnterCriticalSection(&m_csCommunicationSync);/** �ٽ������� */

	/** �򻺳���д��ָ���������� */
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** ��մ��ڻ����� */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	LeaveCriticalSection(&m_csCommunicationSync);/** �뿪�ٽ��� */

	return true;
}
