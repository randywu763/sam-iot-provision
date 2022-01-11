#include "common/include/nm_common.h"
#include "socket/include/socket.h"


static uint32	gu32PingCount		= 0;
static uint32	gu32CurrentIndex	= 0;
static uint32	gu32NumTests		= 0;

char	gacPingList[][64] = {
	"www.google.com",
	"www.twitter.com",
	"192.168.0.1"
};
void m2m_ping(char *pcPingDest, uint32 u32PingCount);

void pingCb(uint32 u32IPAddr, uint32 u32RTT, uint8 u8ERR)
{
	M2M_INFO("(%u)Ping status %u\n", gu32PingCount, u8ERR);
	gu32PingCount --;
	if(gu32PingCount > 0)
	{
		m2m_ping_req(u32IPAddr, 0, pingCb);
	}
	else
	{
		gu32CurrentIndex ++;
		if(gu32CurrentIndex < gu32NumTests)
		{
			m2m_ping(gacPingList[gu32CurrentIndex], 10);
		}
		else
		{
			M2M_INFO("Test Finished\n");
			gu32CurrentIndex = 0;
		}
	}
}

void pingTestResolveCb(uint8* pu8DomainName, uint32 u32HostIP)
{
	if(u32HostIP != 0)
	{
		m2m_ping_req(u32HostIP, 0, pingCb);
	}
}

void m2m_ping(char *pcPingDest, uint32 u32PingCount)
{
	if((pcPingDest != NULL) && (gu32PingCount == 0))
	{
		socketInit();
		registerSocketCallback(NULL, pingTestResolveCb);
		M2M_INFO("Pinging %s\n",pcPingDest);
		gu32PingCount = u32PingCount;
		if((*pcPingDest >= '1') && (*pcPingDest <= '9'))
		{
			uint32	u32IP = nmi_inet_addr(pcPingDest);
			m2m_ping_req(u32IP, 0, pingCb);
		}
		else
		{
			gethostbyname(pcPingDest);
		}
	}
}


void m2m_ping_test(void)
{
	gu32NumTests		= sizeof(gacPingList) / sizeof(gacPingList[0]);
	gu32CurrentIndex	= 0;
	m2m_ping(gacPingList[gu32CurrentIndex], 10);
}