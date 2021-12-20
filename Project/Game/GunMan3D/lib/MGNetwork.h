#pragma once

#include <WS2tcpip.h>
#include <Windows.h>
#include <MSWSock.h>
#include <unordered_map>
#include <list>
#include <thread>
#include <queue>
#include <string>
#include "safePool.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

// ���� ����
struct SOverlapped;
class Server;
class Client;
class SHeader;

struct SSocket
{
	SSocket()
	{
		socket = INVALID_SOCKET;
		usPort = 0;
	}

	SOCKET socket;
	std::string strIP;
	unsigned short usPort;
};

class ProcessPacket
{
public:
	virtual void Receive(SSocket*, char* buffer) {}
	virtual void AddClient(SOCKET) {}
	virtual void RemoveClient(SOCKET) {}
};

class MGNetwork
{
public:
	MGNetwork();
	~MGNetwork();

public:
	void OpenServer(unsigned short port, unsigned int headCount);					// port ��ȣ�� �ִ� �ο� ���� �����Ͽ� ������ ���� �Լ�
	void ConnectServer(std::string name, unsigned short port, std::string ip);		// port ��ȣ�� ip�� �̿��Ͽ� ������ �����ϴ� �Լ�

protected:
	WSADATA m_wsaData;				// ����
	HANDLE m_hIOCP;					// IOCP �ڵ�

	SOCKET m_ListenSocket;			// ���� ����
	unsigned int m_HeadCount;		// ������ ������ �ο� ���� ��

	LPFN_CONNECTEX				m_lpfnConnectEx;		// ConnectEx Ȯ�� �Լ� ������
	LPFN_DISCONNECTEX			m_lpfnDisconnectEx;		// DisconnectEx Ȯ�� �Լ� ������
	LPFN_ACCEPTEX				m_lpfnAcceptEx;			// AcceptEx Ȯ�� �Լ� ������
	LPFN_GETACCEPTEXSOCKADDRS	m_lpfnGetAcceptAddr;	// GetAcceptExSockAddrs Ȯ�� �Լ� ������

	std::thread*				m_workThreads;
	CRITICAL_SECTION			m_workCS;
	std::queue<std::string>		m_MSGQueue;
	char						m_MSGBuffer[512];
	std::thread*				m_MSGThread;
	bool						m_bMSGclose;

public:
	std::unordered_map<std::string, SOCKET>		m_ServerList;		// ����� ���� ���
	std::list<SOCKET>							m_ClientList;		// ����� Ŭ���̾�Ʈ ���
	
#pragma region Network Ŭ���� ���� �غ��
protected:
	bool BindListenSocket(unsigned short port);		// ���� ���� ���ε��ϴ� �Լ�

	bool GetlpfnConnectEx();						// Ȯ�� �Լ� ConnectEx �����͸� �������� �Լ�
	bool GetlpfnAcceptEx();							// Ȯ�� �Լ� AcceptEx �����͸� �������� �Լ�
	bool GetlpfnDisconnectEx();						// Ȯ�� �Լ� DisconnectEx �����͸� �������� �Լ�
	bool GetlpfnGetAcceptExAddr();					// Ȯ�� �Լ� AcceptExAddr �����͸� �������� �Լ�
#pragma endregion

#pragma region ���� ��Ʈ��ũ ���ۺ�
public:
	virtual bool SendPakcet(SHeader*, SOCKET sock);

public:
	ProcessPacket*				m_pPacketSystem;

protected:
	void CreateWorkThread();						// IOCP ť���� �޼����� �����ͼ� ó���� �����带 �����ϴ� �Լ�
	void WorkThread();								// IOCP ť���� �޼����� �����ͼ� ó���ϴ� ������ �Լ�
	void MSGThread();

	void DoAccept();
	void DoDisconnect(SOverlapped*);

	bool DoReceive(SSocket*, SOverlapped*);
	bool DoSend(SSocket*, SOverlapped*);

	bool AddClientList(SOCKET);
	void RemoveClientList(SOCKET);

	virtual void ProcessConnect(SOverlapped*);
	virtual void ProcessAccept(SOverlapped*);
	virtual void ProcessDisconnect(SOverlapped*);

	virtual void ProcessRecv(DWORD dwNumberOfBytesTransferred, SOverlapped* psOverlapped, SSocket* psSocket);
	virtual void ProcessSend(DWORD dwNumberOfBytesTransferred, SOverlapped* psOverlapped, SSocket* psSocket);
#pragma endregion

};

