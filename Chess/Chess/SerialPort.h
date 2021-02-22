#ifndef serialport_h_
#define serialport_h_

extern HANDLE  m_hComm;/** ���ھ�� */
extern bool s_bExit;/** �߳��˳���־���� */
extern volatile HANDLE m_hListenThread;/** �߳̾�� */
extern CRITICAL_SECTION m_csCommunicationSync;//!< �����������/** ͬ������,�ٽ������� */
extern const UINT SLEEP_TIME_INTERVAL;

bool openPort(UINT  portNo);
void ClosePort();
/*�򿪴���*/
//static UINT WINAPI ListenThread(void* pParam);/*���ڼ����߳�*/
bool InitPort(UINT  portNo = 1, UINT  baud = CBR_115200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
//bool OpenListenThread();
bool WriteData(unsigned char* pData, unsigned int length);
UINT GetBytesInCOM();
bool ReadChar(char &cRecved);

#endif

