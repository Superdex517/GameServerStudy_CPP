﻿#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"

int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	//사이즈 입력 조심!!
	char sendData[] = "Hello World";

	//bufferWrite, reader는 복잡하게 사용했던 패킷을 보기 편하게 만들어준것
	while (true)
	{
		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

		BufferWriter bw(sendBuffer->Buffer(), 4096);

		PacketHeader* header = bw.Reserve<PacketHeader>();
		bw << (uint64)1001 << (uint32)100 << (uint16)10;
		bw.Write(sendData, sizeof(sendData));


		header->size = bw.WriteSize();
		header->id = 1; // 1 : Hello Msg

		sendBuffer->Close(bw.WriteSize());

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}