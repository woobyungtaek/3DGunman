#pragma once

// ��Ŷ ����ü 1����Ʈ ����
#pragma pack(push,1)

enum PacketType
{
	//...
	StringPakcet,
	BoolPakcet,
	KeyPakcet,

	PacketTypeMax
};

class SHeader
{
public:
	unsigned short usSize;		// ��Ŷ ������
	unsigned short usType;		// ��Ŷ Ÿ��
};

class Packet : public SHeader
{
public:
	Packet()
	{
		usSize = sizeof(*this) - sizeof(usSize);
		usType = 0;
	}

	char buffer[64];
};

class StringPacket : public SHeader
{
public:
	StringPacket()
	{
		usSize = sizeof(*this) - sizeof(usSize);
		usType = PacketType::StringPakcet;
	}

	char buffer[64];
};

class KeyPacket : public SHeader
{
public:
	KeyPacket()
	{
		usSize = sizeof(*this) - sizeof(usSize);
		usType = PacketType::KeyPakcet;
	}

	short key;
};

class BoolPacket : public SHeader
{
public:
	BoolPacket()
	{
		usSize = sizeof(*this) - sizeof(usSize);
		usType = PacketType::BoolPakcet;
	}

	bool value;
};

#pragma pack(pop)