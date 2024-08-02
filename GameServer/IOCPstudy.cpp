#include "pch.h"
//#include <iostream>
//#include <mutex>
//#include <atomic>
//#include <future>
//#include "ThreadManager.h"
//
//#include "RefCounting.h"
//#include "Memory.h"
//#include "Allocator.h"
//
//#include "Memory.h"
//
//void HandleError(const char* cause)
//{
//	int32 errCode = ::WSAGetLastError();
//	cout << cause << " ErrorCode : " << errCode << endl;
//}
//
//const int32 BUFSIZE = 1000;
//
//struct Session
//{
//	SOCKET socket = INVALID_SOCKET;
//	char recvBuffer[BUFSIZE] = {};
//	int32 recvBytes = 0;
//};
//
//enum IO_TYPE
//{
//	READ,
//	WRITE,
//	ACCEPT,
//	CONNECT,
//};
//
//struct OverlappedEx
//{
//	WSAOVERLAPPED overlapped = {};
//	int32 type = 0;
//};
//
//void WorkerThreadMain(HANDLE iocpHandle)
//{
//	while (true)
//	{
//		DWORD bytesTransferred = 0;
//		Session* session = nullptr;
//		OverlappedEx* overlappedEx = nullptr;
//
//		//쓰레드 대기
//		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred,
//			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);
//
//		if (ret == FALSE || bytesTransferred == 0)
//		{
//			//문제 발생 연결 끊기
//			continue;
//		}
//
//		ASSERT_CRASH(overlappedEx->type == IO_TYPE::READ);
//
//		cout << "Recv Data IOCP = " << bytesTransferred << endl;
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//	}
//}
//
//int Go()
//{
//	WSAData wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//		return 0;
//
//	SOCKADDR_IN serverAddr;
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	serverAddr.sin_port = ::htons(7777);
//
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//		return 0;
//
//	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//		return 0;
//
//	cout << "Accept" << endl;
//
//	vector<Session*> sessionManager;
//
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	for (int32 i = 0; i < 5; i++)
//	{
//		GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
//	}
//
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//		if (clientSocket == INVALID_SOCKET)
//			return 0;
//
//		Session* session = Xnew<Session>();
//		session->socket = clientSocket;
//		sessionManager.push_back(session);
//
//		cout << "Client Connected !" << endl;
//
//		//소켓을 cp에 등록
//		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, (ULONG_PTR)session, 0);
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFSIZE;
//
//		OverlappedEx* overlappedEx = new OverlappedEx();
//		overlappedEx->type = IO_TYPE::READ;
//
//		// add_ref
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//
//		// 유저가 게임 접속 종료할 경우
//		// 메모리 해제됨
//		// 근데 session 주소는 이미 넘겨줌, 유효하지 않은 주소
//		// ref count를 해줌으로 해결 가능
//		//Session* s = sessionManager.back();
//		//sessionManager.pop_back();
//		//Xdelete(s);
//	}
//
//	GThreadManager->Join();
//
//	// 윈속 종료
//	::WSACleanup();
//}