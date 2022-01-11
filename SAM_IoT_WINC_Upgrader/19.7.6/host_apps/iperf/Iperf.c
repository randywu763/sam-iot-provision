/**
 *
 * \file
 *
 * \brief Wifi NMI Iperf demo.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
//------------------------------------------------------------------------------
#include "asf.h"
#include "Iperf.h"
#if defined(__SAMD21J18A__)
#include "bsp/include/nm_bsp_samd21_app.h"
#elif defined(__SAM4SD32C__)
#include "bsp/include/nm_bsp_sam4s_app.h"
#endif
//------------------------------------------------------------------------------
#define IPERF_MAX_UDP_STATE		4

typedef struct
{
	uint32 u32H;
	uint32 u32L;
} tstrIperfUint64;

typedef struct
{
	bool			bIsActive;
	bool			bIsPaused;
	SOCKET			sockParent;
	tstrIperfUint64	strTxBytes;
	tstrIperfUint64	strRxBytes;
	uint32			u32IPAddress;
	uint32			u32BytesLastPeriod;
	uint32			u32TxSQN;
	uint32  		u32RxPkts;
	sint32  		s32PktCount;
	uint32			u32PktByteCount;
	uint32  		u32PktLength;
	uint32			u32TimeStart;
	uint32  		u32LastTransmission;
	uint32			u32LastStatsTime;
	uint16  		u16MsPeriod;
	uint16			u16Port;
	uint8			u8PktsOutstanding;
	uint8			u8State;
} tstrIperfSocketInfo;

typedef struct
{
	bool			bIsActive;
	SOCKET			sockParent;
	tstrIperfUint64	strRxBytes;
	uint32			u32NextRxSeqNum;
	uint32			u32RxMissingPkts;
	uint32			u32TimeStart;
	uint32  		u32RxPkts;
	uint32			u32LastStatsTime;
	uint32			u32BytesLastPeriod;
	uint16			u16RxMissingPktsLastPeriod;
	uint16  		u16RxPktsLastPeriod;
	uint16			u16Port;
} tstrIperfUdpState;

typedef struct
{
	UDP_datagram	strUdpDatagram;
	client_hdr		strUdpClientHdr;
	uint8			gau8MsgBuffer[IPERF_BUFFER_SIZE];
} tstrIperfUdpMsg;

typedef struct 
{
	UDP_datagram	strUdpDatagram;
	server_hdr		strUdpServerHdr;
} tstrIperfUdpServerMsg;

extern uint32 gu32IPAddress;

static tstrIperfSocketInfo	gastrIperfSocketInfo[MAX_SOCKET];
static tstrIperfUdpState	gastrIperfUdpState[IPERF_MAX_UDP_STATE];

static tstrIperfUdpMsg gstrTxMsgBuffer;
static tstrIperfUdpMsg gstrRxMsgBuffer;
static tstrIperfUdpServerMsg gstrTxSrvMsg;

static uint8 gu8NumSockets;

static uint32_t	u32msTicks;
static uint32_t u32LastStatsTime;

//------------------------------------------------------------------------------
static char *inet_ntoa(uint32 in)
{
    static char b[18];
    uint8 *ptr;

    ptr = (uint8 *) &in;
    sprintf(b, "%d.%d.%d.%d", ptr[0], ptr[1], ptr[2], ptr[3]);

    return b;
}

//------------------------------------------------------------------------------
static void Iperf1msTimerCB(void)
{
	u32msTicks++;
}

//------------------------------------------------------------------------------
static void IperfIncrementTimerUse(void)
{
	if(gu8NumSockets == 0)
	{
		u32msTicks = 0;
		u32LastStatsTime = 0;
		nm_bsp_start_1ms_timer(Iperf1msTimerCB);
	}

	gu8NumSockets++;
}

//------------------------------------------------------------------------------
static void IperfDecrementTimerUse(void)
{
	if(gu8NumSockets == 0)
	{
		M2M_ERR("Attempt to decrement timer use when count is zero\r\n");
	}
	else
	{
		gu8NumSockets--;

		if(gu8NumSockets == 0)
			nm_bsp_stop_1ms_timer();
	}
}

//------------------------------------------------------------------------------
static uint32 IperfCalculateBandwidthKbps(tstrIperfUint64 *pstrUint64, uint32 u32Time)
{
	uint32 u32Tmp;
	
	if(pstrUint64->u32H > 0)
	{
		uint32 n = 31;
		uint32 mask = 0x80000000;
		while ((pstrUint64->u32H & mask) == 0)
		{
			n -= 1;
			mask >>= 1;
		}
		u32Tmp = (pstrUint64->u32H << (32 - n)) | (pstrUint64->u32L >> n);
		u32Time >>= n;
		u32Tmp /= (u32Time / 8);
		return u32Tmp;
	}
	else if((pstrUint64->u32H == 0) && (pstrUint64->u32L >= 536870912))
	{
		return pstrUint64->u32L / (u32Time/8);
	}
	else
	{
		return (pstrUint64->u32L*8) / u32Time;
	}
}

//------------------------------------------------------------------------------
static void IperfAddUint64(tstrIperfUint64 *pstrUint64, uint32 u32Inc)
{
	pstrUint64->u32L += u32Inc;
	if(pstrUint64->u32L < u32Inc)
		pstrUint64->u32H++;
}

//------------------------------------------------------------------------------
static sint8 IperfUDP_SendTestPacket(SOCKET sock, struct sockaddr_in* paddr)
{
	sint8 ret;
	uint32		 u32secTicks;
	uint32		 u32usTicks;

	if((gastrIperfSocketInfo[sock].bIsActive == false) || (gastrIperfSocketInfo[sock].u8State != TEST_STATE_UDP_TX))
		return M2M_ERR_FAIL;

	if(gastrIperfSocketInfo[sock].u32TxSQN == 1)
	{
		printf("[%3d] local %s port %d connected with", sock, inet_ntoa(gu32IPAddress), 0);
		printf(" %s port %d\r\n", inet_ntoa(gastrIperfSocketInfo[sock].u32IPAddress), gastrIperfSocketInfo[sock].u16Port);
	}

	u32secTicks = u32msTicks / 1000;
	u32usTicks  = (u32msTicks % 1000)*1000;

	// For UDP Client: store datagram ID into the packet header
	gstrTxMsgBuffer.strUdpDatagram.id = _htonl(gastrIperfSocketInfo[sock].u32TxSQN);
	gstrTxMsgBuffer.strUdpDatagram.tv_sec  = _htonl(u32secTicks);
	gstrTxMsgBuffer.strUdpDatagram.tv_usec = _htonl(u32usTicks);
	gstrTxMsgBuffer.strUdpClientHdr.flags  = 0;

	if(paddr != NULL)
	{
		ret = sendto(sock, &gstrTxMsgBuffer, gastrIperfSocketInfo[sock].u32PktLength, 0, (struct sockaddr*)paddr, sizeof(struct sockaddr_in));
	}
	else
	{
		ret = send(sock, &gstrTxMsgBuffer, gastrIperfSocketInfo[sock].u32PktLength, 0);
	}

	if(ret < 0)
	{
		if(ret != SOCK_ERR_BUFFER_FULL)
			M2M_ERR("Socket send error : %d", ret);
		return ret;
	}

	if(gastrIperfSocketInfo[sock].s32PktCount > 0)
		gastrIperfSocketInfo[sock].s32PktCount--;

	// Update the TX SQN
	gastrIperfSocketInfo[sock].u32TxSQN++;
	IperfAddUint64(&gastrIperfSocketInfo[sock].strTxBytes, gastrIperfSocketInfo[sock].u32PktLength);
	gastrIperfSocketInfo[sock].u32BytesLastPeriod += gastrIperfSocketInfo[sock].u32PktLength;

	return M2M_SUCCESS;
}

//------------------------------------------------------------------------------
static sint8 IperfTCP_SendTestPacket(SOCKET sock)
{
	tstrIperfSocketInfo *pstrSock;
	sint8 ret;

	pstrSock = &gastrIperfSocketInfo[sock];

	ret = send(sock, gstrTxMsgBuffer.gau8MsgBuffer, pstrSock->u32PktLength, 0);

	if(ret < 0)
	{
		if(ret != SOCK_ERR_BUFFER_FULL)
			M2M_ERR("ERROR Sock %d %d\r\n",sock,ret);
	}
	else
	{
		if(pstrSock->u32TxSQN == 0)
		{
			pstrSock->u32TimeStart = u32msTicks;
			pstrSock->u32LastStatsTime	= pstrSock->u32TimeStart;
			pstrSock->u8PktsOutstanding = 1;
		}

		pstrSock->u32TxSQN++;
		IperfAddUint64(&pstrSock->strTxBytes, pstrSock->u32PktLength);
		pstrSock->u32BytesLastPeriod += pstrSock->u32PktLength;
	}

	return ret;
}

//------------------------------------------------------------------------------
static SOCKET IperfUDP_ClientStart(tstrIperfInit* pstrIperfInit, bool bIsPaused)
{
	struct sockaddr_in	addr;
	SOCKET sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0)
		return M2M_ERR_FAIL;

	m2m_memset((uint8*)&gastrIperfSocketInfo[sock], 0, sizeof(tstrIperfSocketInfo));

	IperfIncrementTimerUse();

	gastrIperfSocketInfo[sock].bIsActive			= true;
	gastrIperfSocketInfo[sock].bIsPaused			= bIsPaused;
	gastrIperfSocketInfo[sock].u8State				= TEST_STATE_UDP_TX;
	gastrIperfSocketInfo[sock].s32PktCount			= pstrIperfInit->packets_to_send;
	gastrIperfSocketInfo[sock].u32PktLength			= pstrIperfInit->packet_len;
	gastrIperfSocketInfo[sock].u8PktsOutstanding	= 5;

	if(pstrIperfInit->data_rate > 0)
	{
		gastrIperfSocketInfo[sock].u16MsPeriod = (pstrIperfInit->packet_len*8*1000)/pstrIperfInit->data_rate;
	}

	addr.sin_family			= AF_INET;
	addr.sin_port			= _htons(pstrIperfInit->port);
	addr.sin_addr.s_addr	= _htonl((uint32_t) (pstrIperfInit->ip[0]<<24 | pstrIperfInit->ip[1]<<16 | pstrIperfInit->ip[2]<<8 | pstrIperfInit->ip[3]));

	gastrIperfSocketInfo[sock].u32IPAddress			= addr.sin_addr.s_addr;
	gastrIperfSocketInfo[sock].u16Port				= pstrIperfInit->port;

	if(bIsPaused == false)
	{
		gastrIperfSocketInfo[sock].u32TimeStart			= u32msTicks;
		gastrIperfSocketInfo[sock].u32LastTransmission	= gastrIperfSocketInfo[sock].u32TimeStart;
		gastrIperfSocketInfo[sock].u32LastStatsTime		= gastrIperfSocketInfo[sock].u32TimeStart;

		return IperfUDP_SendTestPacket(sock, &addr);
	}

	return M2M_SUCCESS;
}

//------------------------------------------------------------------------------
static sint8 IperfTCP_ClientStart(tstrIperfInit* pstrIperfInit, bool bIsPaused)
{
	struct sockaddr_in addr;
	SOCKET sock;

	sock = socket(AF_INET, SOCK_STREAM, (pstrIperfInit->tls) ? SOCKET_CONFIG_SSL_ON : SOCKET_CONFIG_SSL_OFF);

	if(sock < 0)
		return M2M_ERR_FAIL;

	addr.sin_family			= AF_INET;
	addr.sin_port			= _htons(pstrIperfInit->port);
	addr.sin_addr.s_addr	= _htonl((uint32_t) (pstrIperfInit->ip[0]<<24 | pstrIperfInit->ip[1]<<16 | pstrIperfInit->ip[2]<<8 | pstrIperfInit->ip[3]));

	// Bind and Connect to Server.
	if(bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	m2m_memset((uint8*)&gastrIperfSocketInfo[sock], 0, sizeof(tstrIperfSocketInfo));

	gastrIperfSocketInfo[sock].bIsActive	= true;
	gastrIperfSocketInfo[sock].bIsPaused	= bIsPaused;
	gastrIperfSocketInfo[sock].u8State		= TEST_STATE_TCP_TX;
	gastrIperfSocketInfo[sock].s32PktCount	= pstrIperfInit->packets_to_send;
	gastrIperfSocketInfo[sock].u32PktLength	= pstrIperfInit->packet_len;
	gastrIperfSocketInfo[sock].u32IPAddress = addr.sin_addr.s_addr;
	gastrIperfSocketInfo[sock].u16Port      = pstrIperfInit->port;

	IperfIncrementTimerUse();

	return M2M_SUCCESS;
}
//------------------------------------------------------------------------------
static sint8 IperfTLS_ClientStart(tstrIperfInit* pstrIperfInit, bool bIsPaused)
{
	struct sockaddr_in addr;
	SOCKET sock;

	sock = socket(AF_INET, SOCK_STREAM, (pstrIperfInit->tls) ? SOCKET_CONFIG_SSL_ON : SOCKET_CONFIG_SSL_OFF);

	if(sock < 0)
	    return M2M_ERR_FAIL;

	addr.sin_family			= AF_INET;
	addr.sin_port			= _htons(pstrIperfInit->port);
	addr.sin_addr.s_addr	= _htonl((uint32_t) (pstrIperfInit->ip[0]<<24 | pstrIperfInit->ip[1]<<16 | pstrIperfInit->ip[2]<<8 | pstrIperfInit->ip[3]));

	// Bind and Connect to Server.
	if(bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	m2m_memset((uint8*)&gastrIperfSocketInfo[sock], 0, sizeof(tstrIperfSocketInfo));

	gastrIperfSocketInfo[sock].bIsActive	= false;
	gastrIperfSocketInfo[sock].bIsPaused	= bIsPaused;
	gastrIperfSocketInfo[sock].u8State		= TEST_STATE_TCP_RX;
	gastrIperfSocketInfo[sock].u32IPAddress = addr.sin_addr.s_addr;
	gastrIperfSocketInfo[sock].u16Port      = pstrIperfInit->port;

	return M2M_SUCCESS;
}
//------------------------------------------------------------------------------
static sint8 IperfUDP_WriteFIN(SOCKET sock)
{
	uint32		 u32secTicks;
	uint32		 u32usTicks;

	u32secTicks = u32msTicks / 1000;
	u32usTicks  = (u32msTicks % 1000)*1000;


	// Store Sequence Number into the datagram
	gstrTxMsgBuffer.strUdpDatagram.id		= _htonl(-gastrIperfSocketInfo[sock].u32TxSQN);
	gstrTxMsgBuffer.strUdpDatagram.tv_sec	= _htonl(u32secTicks);
	gstrTxMsgBuffer.strUdpDatagram.tv_usec	= _htonl(u32usTicks);
	gstrTxMsgBuffer.strUdpClientHdr.flags	= 0;

	return send(sock, &gstrTxMsgBuffer, gastrIperfSocketInfo[sock].u32PktLength, 0);
}

//------------------------------------------------------------------------------
static void IperfPrintUdpServerStats(SOCKET sock, uint8 u8UdpStateIdx)
{
	uint32 u32TimeDelta;
	uint32 u32AvrData;
	uint32 u32KBytes;

	if((gastrIperfUdpState[u8UdpStateIdx].bIsActive == true) && (gastrIperfUdpState[u8UdpStateIdx].sockParent == sock))
	{
		u32TimeDelta = u32msTicks - gastrIperfUdpState[u8UdpStateIdx].u32TimeStart;
		u32AvrData   = IperfCalculateBandwidthKbps(&gastrIperfUdpState[u8UdpStateIdx].strRxBytes, u32TimeDelta);

		u32KBytes = (gastrIperfUdpState[u8UdpStateIdx].strRxBytes.u32H << 22) | (gastrIperfUdpState[u8UdpStateIdx].strRxBytes.u32L >> 10);

		printf("[%d.%d] Server Report:\r\n", sock, u8UdpStateIdx);
		printf("[%d.%d]    0-%5ld sec %5ld KBytes %5ld Kbits/sec %5ld/%5ld\n", sock, u8UdpStateIdx, (u32TimeDelta/1000), u32KBytes, u32AvrData, gastrIperfUdpState[u8UdpStateIdx].u32RxMissingPkts, gastrIperfUdpState[u8UdpStateIdx].u32RxPkts);
	}
}

//------------------------------------------------------------------------------
static void IperfSocketEventHandler(SOCKET sock, uint8 u8Msg, void *pvMsg)
{
	sint8 ret;
	tstrIperfSocketInfo *pstrSock;
	uint32 u32TimeDelta;

	if(sock >= MAX_SOCKET)
		return;

	pstrSock = &gastrIperfSocketInfo[sock];

	if(pstrSock->u8State == TEST_STATE_UDP_TX)
	{
		if(u8Msg == SOCKET_MSG_RECVFROM)
		{
			tstrSocketRecvMsg		*pstrRx = (tstrSocketRecvMsg*)pvMsg;
			tstrIperfUdpServerMsg	*pstrServerHdr = (tstrIperfUdpServerMsg*)pstrRx->pu8Buffer;

			// After the UDP Client completes the data transfer, the server sends a Report to the client.
			// Wait to close the socket until receive it.

			if(pstrRx->u16RemainingSize == 0)
			{
				IperfPrintStats(sock);

				if(pstrServerHdr->strUdpServerHdr.flags == _htonl(0x80000000))
				{
					uint32 u32AvrData;
					uint32 u32KBytes;
					tstrIperfUint64 strRxBytes;

					u32TimeDelta = (_ntohl(pstrServerHdr->strUdpServerHdr.stop_sec)*1000) + (_ntohl(pstrServerHdr->strUdpServerHdr.stop_usec)/1000);
					strRxBytes.u32H = _ntohl(pstrServerHdr->strUdpServerHdr.total_len1);
					strRxBytes.u32L = _ntohl(pstrServerHdr->strUdpServerHdr.total_len2);
					u32AvrData   = IperfCalculateBandwidthKbps(&strRxBytes, u32TimeDelta);

					u32KBytes = (strRxBytes.u32H << 22) | (strRxBytes.u32L >> 10);

					printf("[%d.%d] Server Report:\r\n", sock, gastrIperfSocketInfo[sock].sockParent);
					printf("[%d.%d]    0-%4ld sec %5ld KBytes %5ld Kbits/sec %5ld/%5ld\n", sock, gastrIperfSocketInfo[sock].sockParent, (u32TimeDelta/1000), u32KBytes, u32AvrData, _ntohl(pstrServerHdr->strUdpServerHdr.error_cnt), _ntohl(pstrServerHdr->strUdpServerHdr.datagrams));
					printf("[%d.%d] Sent %ld datagrams\r\n", sock, gastrIperfSocketInfo[sock].sockParent, _ntohl(pstrServerHdr->strUdpServerHdr.datagrams));
				}

				IperfSocketClose(sock);
			}
		}
	}
	else if(pstrSock->u8State == TEST_STATE_UDP_RX)
	{
		switch(u8Msg)
		{
			case SOCKET_MSG_BIND:
			{
				tstrSocketBindMsg	*pstrBind = (tstrSocketBindMsg*)pvMsg;
				if(pstrBind != NULL)
				{
					if(pstrBind->status == 0)
					{
						printf("Ready to receive\r\n");
						recvfrom(sock, &gstrRxMsgBuffer, IPERF_RX_BUFFER_SIZE,0);
					}
					else
					{
						M2M_ERR("[sock %d] Bind error.\r\n", sock);
						IperfSocketClose(sock);
					}
				}
			}
			break;

     		case SOCKET_MSG_RECVFROM:
			{
				uint32 id = 0;
				tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;
				tstrIperfUdpState	*pstrUdpState = NULL;
				tstrIperfUdpState	*pstrFreeUdpState = NULL;
				uint8 u8UdpStateIdx;
				for(u8UdpStateIdx=0; u8UdpStateIdx<IPERF_MAX_UDP_STATE; u8UdpStateIdx++)
				{
					if(gastrIperfUdpState[u8UdpStateIdx].bIsActive == true)
					{
						if((gastrIperfUdpState[u8UdpStateIdx].sockParent == sock) && (gastrIperfUdpState[u8UdpStateIdx].u16Port == pstrRx->strRemoteAddr.sin_port))
						{
							pstrUdpState = &gastrIperfUdpState[u8UdpStateIdx];
							break;
						}
					}
				}

				if(pstrRx->pu8Buffer != NULL)
				{
					UDP_datagram *pstrUDPhdr = (UDP_datagram*)pstrRx->pu8Buffer;

					id = _ntohl(pstrUDPhdr->id);

					if((id & NBIT31) == NBIT31)
					{
						if(pstrUdpState == NULL)
							break;

						gstrTxSrvMsg.strUdpDatagram.id = pstrUDPhdr->id;

						u32TimeDelta = u32msTicks - pstrUdpState->u32TimeStart;

						gstrTxSrvMsg.strUdpServerHdr.flags = _htonl(0x80000000);
						gstrTxSrvMsg.strUdpServerHdr.datagrams = _htonl(-id);
						gstrTxSrvMsg.strUdpServerHdr.error_cnt = _htonl(pstrUdpState->u32RxMissingPkts);
						gstrTxSrvMsg.strUdpServerHdr.jitter1 = 0;
						gstrTxSrvMsg.strUdpServerHdr.jitter2 = 0;
						gstrTxSrvMsg.strUdpServerHdr.outorder_cnt = 0;
						gstrTxSrvMsg.strUdpServerHdr.stop_sec = _htonl(u32TimeDelta/1000);
						gstrTxSrvMsg.strUdpServerHdr.stop_usec = _htonl(u32TimeDelta%1000)*1000;
						gstrTxSrvMsg.strUdpServerHdr.total_len1 = _htonl(pstrUdpState->strRxBytes.u32H);
						gstrTxSrvMsg.strUdpServerHdr.total_len2 = _htonl(pstrUdpState->strRxBytes.u32L);

						ret = sendto(sock, &gstrTxSrvMsg, sizeof(tstrIperfUdpServerMsg), 0, (struct sockaddr*)&pstrRx->strRemoteAddr, sizeof(struct sockaddr_in));
						if(ret != M2M_SUCCESS)
						{
							if(ret != SOCK_ERR_BUFFER_FULL)
								printf("sendto failed error = %d\r\n", ret);
						}

						IperfPrintUdpServerStats(sock, u8UdpStateIdx);
						m2m_memset((uint8*)pstrUdpState, 0, sizeof(tstrIperfUdpState));
						IperfDecrementTimerUse();
					}
					else
					{
						if(pstrUdpState == NULL)
						{
							for(u8UdpStateIdx=0; u8UdpStateIdx<IPERF_MAX_UDP_STATE; u8UdpStateIdx++)
							{
								if(gastrIperfUdpState[u8UdpStateIdx].bIsActive == false)
								{
									pstrFreeUdpState = &gastrIperfUdpState[u8UdpStateIdx];
									break;
								}
							}

							if(pstrFreeUdpState != NULL)
							{
								printf("[%d.%d] local %s port %d connected with", sock, u8UdpStateIdx, inet_ntoa(pstrSock->u32IPAddress), pstrSock->u16Port);
								printf(" %s port %d\r\r\n", inet_ntoa(pstrRx->strRemoteAddr.sin_addr.s_addr), _htons(pstrRx->strRemoteAddr.sin_port));

								m2m_memset((uint8*)pstrFreeUdpState, 0, sizeof(tstrIperfUdpState));

								pstrFreeUdpState->bIsActive = true;
								pstrFreeUdpState->sockParent = sock;
								pstrFreeUdpState->u16Port = pstrRx->strRemoteAddr.sin_port;

								IperfIncrementTimerUse();

								pstrUdpState = pstrFreeUdpState;
								pstrUdpState->u32NextRxSeqNum = 0;
								pstrUdpState->u32TimeStart = u32msTicks;
								pstrFreeUdpState->u32LastStatsTime = pstrUdpState->u32TimeStart;
							}
						}

						if(pstrUdpState == NULL)
							break;

						// Ignore duplicate
						if (pstrUdpState->u32NextRxSeqNum <= id)
						{
							if (pstrUdpState->u32NextRxSeqNum != id)
							{
								pstrUdpState->u32RxMissingPkts += id - pstrUdpState->u32NextRxSeqNum;
								pstrUdpState->u16RxMissingPktsLastPeriod += id - pstrUdpState->u32NextRxSeqNum;
							}

							pstrUdpState->u32NextRxSeqNum = id + 1;

							pstrSock->u32RxPkts++;
							pstrUdpState->u32RxPkts++;
							pstrUdpState->u16RxPktsLastPeriod++;

							if(pstrRx->s16BufferSize > 0)
							{
								IperfAddUint64(&pstrUdpState->strRxBytes, pstrRx->s16BufferSize + pstrRx->u16RemainingSize);
								pstrUdpState->u32BytesLastPeriod += (pstrRx->s16BufferSize + pstrRx->u16RemainingSize);
							}
						}
					}
					recvfrom(sock, &gstrRxMsgBuffer, IPERF_RX_BUFFER_SIZE, 0);
				}
				else if(pstrRx->s16BufferSize <= 0)
				{
					printf("UDP server error %d\r\n", pstrRx->s16BufferSize);
					IperfSocketClose(sock);
				}
			}
			break;
		}
	}
	else if(pstrSock->u8State == TEST_STATE_TCP_RX)
	{
		switch(u8Msg)
		{
			case SOCKET_MSG_CONNECT:
			{
				// TLS RX
				//  Socket is opened as client.
				printf("[%3d] local %s port %d connected with", sock, inet_ntoa(gu32IPAddress), 0);
				printf(" %s port %d\r\n", inet_ntoa(pstrSock->u32IPAddress), pstrSock->u16Port);
				ret = recv(sock, gstrRxMsgBuffer.gau8MsgBuffer, IPERF_RX_BUFFER_SIZE, 0);
			}
			break;
			case SOCKET_MSG_ACCEPT:
			{
				tstrSocketAcceptMsg *msg = (tstrSocketAcceptMsg*)pvMsg;

				if(accept(sock, NULL, 0) < 0)
                {
                    close(sock);
                    break;
                }

				IperfIncrementTimerUse();

				printf("[%d.%d] local %s port %d connected with", sock, msg->sock, inet_ntoa(pstrSock->u32IPAddress), pstrSock->u16Port);

				pstrSock = &gastrIperfSocketInfo[msg->sock];

				m2m_memset((uint8*)pstrSock, 0, sizeof(tstrIperfSocketInfo));

				pstrSock->bIsActive			= true;
				pstrSock->u8State			= TEST_STATE_TCP_RX;
				pstrSock->sockParent		= sock;
				pstrSock->u32TimeStart		= u32msTicks;
				pstrSock->u32LastStatsTime	= pstrSock->u32TimeStart;
				pstrSock->u32IPAddress		= msg->strAddr.sin_addr.s_addr;
				pstrSock->u16Port			= _ntohs(msg->strAddr.sin_port);

				ret = recv(msg->sock, gstrRxMsgBuffer.gau8MsgBuffer, IPERF_RX_BUFFER_SIZE, 0);

				printf(" %s port %d\r\n", inet_ntoa(pstrSock->u32IPAddress), pstrSock->u16Port);
			}
			break;

			case SOCKET_MSG_RECV:
			{
				tstrSocketRecvMsg *msg = (tstrSocketRecvMsg*)pvMsg;

				if(msg->s16BufferSize <= 0)
				{
					IperfPrintStats(sock);
					IperfSocketClose(sock);
				}
				else
				{
					// TLS RX -- Wait for the first RX frame.
					// This flag is always on for TCP SVR.
					if (gastrIperfSocketInfo[sock].bIsActive == false)
					{
						gastrIperfSocketInfo[sock].bIsActive = true;
						IperfIncrementTimerUse();
					}

					if(pstrSock->sockParent >= 0)
					{
						IperfAddUint64(&gastrIperfSocketInfo[pstrSock->sockParent].strRxBytes, msg->s16BufferSize);
					}

					IperfAddUint64(&pstrSock->strRxBytes, msg->s16BufferSize);
					pstrSock->u32BytesLastPeriod += msg->s16BufferSize;

					pstrSock->u32PktByteCount += msg->s16BufferSize;
					if(pstrSock->u32PktByteCount >= pstrSock->u32PktLength)
					{
						pstrSock->u32RxPkts++;
						pstrSock->u32PktByteCount -= pstrSock->u32PktLength;
					}

					recv(sock, gstrRxMsgBuffer.gau8MsgBuffer, IPERF_RX_BUFFER_SIZE, 0);
				}
			}
			break;
		}
	}
	else if(pstrSock->u8State == TEST_STATE_TCP_TX)
	{
		switch(u8Msg)
		{
			case SOCKET_MSG_CONNECT:
			{
				printf("[%3d] local %s port %d connected with", sock, inet_ntoa(gu32IPAddress), 0);
				printf(" %s port %d\r\n", inet_ntoa(pstrSock->u32IPAddress), pstrSock->u16Port);

				if(pstrSock->bIsPaused == false)
					IperfTCP_SendTestPacket(sock);
			}
			break;

			case SOCKET_MSG_SEND:
			{
				sint16 s16SentBytes = *((sint16*)pvMsg);

				if(pstrSock->bIsActive == true)
				{
					if(s16SentBytes < 0)
					{
						printf("\nError %d: PIperf finished on: SOCKET   %02u    Next TX SQN = %04lu\r\n", s16SentBytes, sock, pstrSock->u32TxSQN);
						IperfPrintStats(sock);
						IperfSocketClose(sock);
					}
					else
					{
						pstrSock->u32PktByteCount += s16SentBytes;
						while(pstrSock->u32PktByteCount >= pstrSock->u32PktLength)
						{
							pstrSock->u32PktByteCount -= pstrSock->u32PktLength;
							pstrSock->u8PktsOutstanding--;

							if(pstrSock->s32PktCount > 0)
								pstrSock->s32PktCount--;
						}

						if(pstrSock->s32PktCount == 0)
						{
							IperfPrintStats(sock);
							IperfSocketClose(sock);
						}
						else
						{
    						while((pstrSock->u8PktsOutstanding < 3) && ((pstrSock->s32PktCount == -1) || (pstrSock->u8PktsOutstanding < pstrSock->s32PktCount)))
    						{
        						if(IperfTCP_SendTestPacket(sock) == 0)
        						    pstrSock->u8PktsOutstanding++;
    						}
						}
					}
				}
			}
		}
	}

	m2m_wifi_yield();
}

//------------------------------------------------------------------------------
static sint8 IperfUDP_ServerStart(tstrIperfInit* pstrIperfInit)
{
	struct sockaddr_in addr_in;
	SOCKET sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0)
		return M2M_ERR_FAIL;

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = _htons(pstrIperfInit->port);
	addr_in.sin_addr.s_addr = 0;

	if(bind(sock, (struct sockaddr*)&addr_in, sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	m2m_memset((uint8*)&gastrIperfSocketInfo[sock], 0, sizeof(tstrIperfSocketInfo));

	gastrIperfSocketInfo[sock].bIsActive = true;
	gastrIperfSocketInfo[sock].u8State = TEST_STATE_UDP_RX;
	gastrIperfSocketInfo[sock].u32IPAddress = gu32IPAddress;
	gastrIperfSocketInfo[sock].u16Port = pstrIperfInit->port;

	return M2M_SUCCESS;
}

//------------------------------------------------------------------------------
static sint8 IperfTCP_ServerStart(tstrIperfInit* pstrIperfInit)
{
	struct sockaddr_in addr_in;
	SOCKET sock;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0)
		return M2M_ERR_FAIL;

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = _htons(pstrIperfInit->port);
	addr_in.sin_addr.s_addr = 0;

	if(bind(sock, (struct sockaddr*)&addr_in, sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	if(listen(sock, TCP_SOCK_MAX) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	if(accept(sock, NULL, 0) < 0)
    {
        close(sock);
		return M2M_ERR_FAIL;
    }

	m2m_memset((uint8*)&gastrIperfSocketInfo[sock], 0, sizeof(tstrIperfSocketInfo));

	gastrIperfSocketInfo[sock].bIsActive = true;
	gastrIperfSocketInfo[sock].u8State = TEST_STATE_TCP_RX;
	gastrIperfSocketInfo[sock].sockParent = -1;
	gastrIperfSocketInfo[sock].u32IPAddress = gu32IPAddress;
	gastrIperfSocketInfo[sock].u16Port = pstrIperfInit->port;

	return M2M_SUCCESS;
}

//------------------------------------------------------------------------------
static sint8 IperfSocketStop(SOCKET sock)
{
	if(gastrIperfSocketInfo[sock].u8State == TEST_STATE_UDP_TX)
	{
		gastrIperfSocketInfo[sock].s32PktCount = 0;
		//Just send one more packet - the one ending the stream at the server
		gastrIperfSocketInfo[sock].u8PktsOutstanding = 1;
	}
	else
	{
		IperfPrintStats(sock);
		return IperfSocketClose(sock);
	}

	return M2M_SUCCESS;
}

//------------------------------------------------------------------------------
NMI_API void IperfInit(void)
{
	uint16 u16Len;

	gu8NumSockets = 0;

	m2m_memset((uint8*)&gastrIperfSocketInfo, 0, sizeof(gastrIperfSocketInfo));
	m2m_memset((uint8*)&gastrIperfUdpState, 0, sizeof(gastrIperfUdpState));

	u16Len = IPERF_BUFFER_SIZE;

    while(u16Len-- > 0)
        gstrTxMsgBuffer.gau8MsgBuffer[u16Len] = (u16Len % 10) + '0';
}

//------------------------------------------------------------------------------
NMI_API void IperfPrintStats(SOCKET sock)
{
	uint8 u8UdpStateIdx;
	uint32 u32TimeDelta;
	uint32 u32AvrData;
	uint32 u32KBytes;

	if(gastrIperfSocketInfo[sock].bIsActive == false)
		return;

	switch(gastrIperfSocketInfo[sock].u8State)
	{
		case TEST_STATE_UDP_RX:
			for(u8UdpStateIdx=0; u8UdpStateIdx<IPERF_MAX_UDP_STATE; u8UdpStateIdx++)
				IperfPrintUdpServerStats(sock, u8UdpStateIdx);
			break;

		case TEST_STATE_TCP_RX:
			u32TimeDelta = u32msTicks - gastrIperfSocketInfo[sock].u32TimeStart;
			u32AvrData   = IperfCalculateBandwidthKbps(&gastrIperfSocketInfo[sock].strRxBytes, u32TimeDelta);

			u32KBytes = (gastrIperfSocketInfo[sock].strRxBytes.u32H << 22) | (gastrIperfSocketInfo[sock].strRxBytes.u32L >> 10);

			printf("[%d.%d] Server Report:\r\n", gastrIperfSocketInfo[sock].sockParent, sock);
			printf("[%d.%d]    0-%4ld sec %5ld KBytes %5ld Kbits/sec\r\n", gastrIperfSocketInfo[sock].sockParent, sock, (u32TimeDelta/1000), u32KBytes, u32AvrData);
			break;

		case TEST_STATE_UDP_TX:
			u32TimeDelta = u32msTicks - gastrIperfSocketInfo[sock].u32TimeStart;
			u32AvrData   = IperfCalculateBandwidthKbps(&gastrIperfSocketInfo[sock].strTxBytes, u32TimeDelta);

			u32KBytes = (gastrIperfSocketInfo[sock].strTxBytes.u32H << 22) | (gastrIperfSocketInfo[sock].strTxBytes.u32L >> 10);

			printf("[%3d] Client Report:\r\n", sock);
			printf("[%3d]    0-%4ld sec %5ld KBytes %5ld Kbits/sec\r\n", sock, (u32TimeDelta/1000), u32KBytes, u32AvrData);
			break;

		case TEST_STATE_TCP_TX:
			u32TimeDelta = u32msTicks - gastrIperfSocketInfo[sock].u32TimeStart;
			u32AvrData   = IperfCalculateBandwidthKbps(&gastrIperfSocketInfo[sock].strTxBytes, u32TimeDelta);

			u32KBytes = (gastrIperfSocketInfo[sock].strTxBytes.u32H << 22) | (gastrIperfSocketInfo[sock].strTxBytes.u32L >> 10);

			printf("[%3d] Client Report:\r\n", sock);
			printf("[%3d]    0-%4ld sec %5ld KBytes %5ld Kbits/sec\r\n", sock, (u32TimeDelta/1000), u32KBytes, u32AvrData);
			break;
	}
}

//------------------------------------------------------------------------------
NMI_API sint8 IperfSocketClose(SOCKET sock)
{
	int i;

	if (sock >= MAX_SOCKET)
		return M2M_ERR_FAIL;

	if(gastrIperfSocketInfo[sock].bIsActive == false)
		return M2M_ERR_FAIL;

	if((gastrIperfSocketInfo[sock].u8State == TEST_STATE_TCP_RX) && (gastrIperfSocketInfo[sock].sockParent == -1))
	{
		for(i=0;i<TCP_SOCK_MAX;i++)
		{
			if(gastrIperfSocketInfo[i].bIsActive == false)
				continue;

			if((gastrIperfSocketInfo[i].u8State == TEST_STATE_TCP_RX) && (gastrIperfSocketInfo[i].sockParent == sock))
			{
				IperfPrintStats(i);
				IperfSocketClose(i);
			}
		}
	}
	else if(gastrIperfSocketInfo[sock].u8State != TEST_STATE_UDP_RX)
		IperfDecrementTimerUse();

	if((gastrIperfSocketInfo[sock].u8State == TEST_STATE_UDP_RX) && (gastrIperfSocketInfo[sock].bIsActive == true))
	{
		for(i=0; i<IPERF_MAX_UDP_STATE; i++)
		{
			if((gastrIperfUdpState[i].bIsActive == true) && (gastrIperfUdpState[i].sockParent == sock))
			{
				m2m_memset((uint8*)&gastrIperfUdpState[i], 0, sizeof(tstrIperfUdpState));
				IperfDecrementTimerUse();
			}
		}
	}

	m2m_memset((uint8*)&gastrIperfSocketInfo[sock], 0, sizeof(tstrIperfSocketInfo));

	printf("Closing socket %d\r\n",sock);

	return close(sock);
}

//Take an array of specified size (sz), and write into it the numbers from
//offs up to offs+sz randomly shuffled.
void init_and_shuffle(uint8 * arr, uint8 sz, uint8 offs)
{
	int i,j;
	
	//Init the array
	for (i=0; i<sz; i++)
		arr[i]=i+offs;
	
	//Fisher-Yates shuffle
	for (i=0; i<sz-1; i++)
	{
		j = (rand() % (sz - i)) + i;
		uint8 tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}
}

//------------------------------------------------------------------------------
NMI_API void IperfUpdate(void)
{
	int i, j;
	uint32 u32TimeDelta;
	uint32 u32AvrData;
	uint32 u32T1;
	tstrIperfSocketInfo *pstrSock;
	tstrIperfUdpState   *pstrUdpState;
    uint8 random_order[MAX_SOCKET];
	
	//Randomise the order we go through the sockets each time
	//to ensure multiple socket streams are fairly treated.
	init_and_shuffle(random_order, UDP_SOCK_MAX, TCP_SOCK_MAX);

	for(i=0; i<UDP_SOCK_MAX; i++)
	{
		pstrSock = &gastrIperfSocketInfo[random_order[i]];

		if(pstrSock->bIsActive && !pstrSock->bIsPaused && (pstrSock->u8State == TEST_STATE_UDP_TX))
		{
			if(pstrSock->s32PktCount == 0)
			{
				if(pstrSock->u8PktsOutstanding > 0)
				{
					if(IperfUDP_WriteFIN(random_order[i]) == M2M_SUCCESS)
					{
						recvfrom(random_order[i], &gstrRxMsgBuffer, IPERF_RX_BUFFER_SIZE, 0);
						//Last packet has been sent and reply received
						pstrSock->u8PktsOutstanding=0;
					}
					//else if send failed, go round and try again in the next tick
				}
				else
				{
					IperfPrintStats(random_order[i]);
					IperfSocketClose(random_order[i]);
				}
			}
			else
			{
				u32TimeDelta = u32msTicks - pstrSock->u32LastTransmission;

				if(!pstrSock->u16MsPeriod || (u32TimeDelta > pstrSock->u16MsPeriod))
				{
					IperfUDP_SendTestPacket(random_order[i], NULL);
					pstrSock->u32LastTransmission += pstrSock->u16MsPeriod;
				}
			}
		}
	}

	//Randomise again
	init_and_shuffle(random_order, MAX_SOCKET, 0);
	
	u32TimeDelta = u32msTicks - u32LastStatsTime;
	if(u32TimeDelta >= 1000)
	{
		u32LastStatsTime += 1000;
		
		uint32 u32SumBytesLastPeriod = 0;
		uint32 u32SumT1 = 0;
		uint32 u32SumTimeDelta = 0;
		uint8 u8StreamCount = 0;

		for(i=0; i<MAX_SOCKET; i++)
		{
			pstrSock = &gastrIperfSocketInfo[random_order[i]];

			if(pstrSock->bIsActive && !pstrSock->bIsPaused)
			{
				if((pstrSock->u8State == TEST_STATE_UDP_TX) || (pstrSock->u8State == TEST_STATE_TCP_TX) || ((pstrSock->u8State == TEST_STATE_TCP_RX) && (pstrSock->sockParent != -1)))
				{
					u32AvrData = (pstrSock->u32BytesLastPeriod*8) / u32TimeDelta;
					u32T1 = (u32LastStatsTime - pstrSock->u32TimeStart) / 1000;

					if(u8StreamCount == 0)
					{
						u32SumTimeDelta = u32msTicks - pstrSock->u32TimeStart;
						u32SumT1 = u32T1;
					}
					
					u8StreamCount++;
					
					u32SumBytesLastPeriod += pstrSock->u32BytesLastPeriod;

					printf("[%d.%d] %4lu-%4ld sec %5ld KBytes %5ld Kbits/sec\r\n", pstrSock->sockParent, random_order[i], u32T1, (u32TimeDelta/1000), pstrSock->u32BytesLastPeriod / 1024, u32AvrData);

					pstrSock->u32BytesLastPeriod = 0;
				}
				else if(pstrSock->u8State == TEST_STATE_UDP_RX)
				{
					for(j=0; j<IPERF_MAX_UDP_STATE; j++)
					{
						pstrUdpState = &gastrIperfUdpState[j];
	
						if((pstrUdpState->bIsActive == true) && (pstrUdpState->sockParent == random_order[i]))
						{
							u32AvrData = (pstrUdpState->u32BytesLastPeriod * 8) / u32TimeDelta;
							u32T1 = (u32LastStatsTime - pstrUdpState->u32TimeStart) / 1000;

							if(u8StreamCount == 0)
							{
								u32SumTimeDelta = u32msTicks - pstrUdpState->u32TimeStart;
								u32SumT1 = u32T1; // Use the time from the first stream for the the SUM
							}
								
							u8StreamCount++;

							u32SumBytesLastPeriod += pstrUdpState->u32BytesLastPeriod;

							printf("[%d.%d] %4lu-%4d sec %5ld KBytes %5ld Kbits/sec %5d/%5d\r\n", random_order[i], j, u32T1, (uint16)((u32msTicks - pstrUdpState->u32TimeStart)/1000), pstrUdpState->u32BytesLastPeriod / 1024, u32AvrData, pstrUdpState->u16RxMissingPktsLastPeriod, pstrUdpState->u16RxPktsLastPeriod);
							pstrUdpState->u32BytesLastPeriod = 0;
							pstrUdpState->u16RxMissingPktsLastPeriod = 0;
							pstrUdpState->u16RxPktsLastPeriod = 0;
						}
					}
				}
			}
		}
		
		if(u8StreamCount > 1)
		{
			u32AvrData = (u32SumBytesLastPeriod*8) / u32TimeDelta;
			printf("[SUM] %4lu-%4d sec %5ld KBytes %5ld Kbits/sec\n", u32SumT1, (uint16)(u32SumTimeDelta/1000), u32SumBytesLastPeriod / 1024, u32AvrData);
		}
	}
}

//------------------------------------------------------------------------------
NMI_API sint8 IperfCreate(tstrIperfInit* pstrIperfInit, bool bIsPaused)
{
	sint8 ret = M2M_ERR_FAIL;

	registerSocketCallback(IperfSocketEventHandler, NULL);

	switch(pstrIperfInit->operating_mode)
	{
		case MODE_TCP_SERVER:
			ret = IperfTCP_ServerStart(pstrIperfInit);
			break;

		case MODE_UDP_SERVER:
			ret = IperfUDP_ServerStart(pstrIperfInit);
			break;

		case MODE_UDP_CLIENT:
			ret = IperfUDP_ClientStart(pstrIperfInit, bIsPaused);
			break;

		case MODE_TCP_CLIENT:
			ret = IperfTCP_ClientStart(pstrIperfInit, bIsPaused);
			break;

		case MODE_TLS_CLIENT:
			ret = IperfTLS_ClientStart(pstrIperfInit, bIsPaused);
			break;
	}

	return ret;
}

//------------------------------------------------------------------------------
NMI_API sint8 IperfStart(void)
{
	int i;
	tstrIperfSocketInfo *pstrSock;

	for(i=0; i<MAX_SOCKET; i++)
	{
		pstrSock = &gastrIperfSocketInfo[i];

		if(pstrSock->bIsActive && pstrSock->bIsPaused)
		{
			if(pstrSock->u8State == TEST_STATE_TCP_TX)
			{
				pstrSock->bIsPaused = false;
				IperfTCP_SendTestPacket(i);
			}
			else if(pstrSock->u8State == TEST_STATE_UDP_TX)
			{
				struct sockaddr_in addr;

				pstrSock->bIsPaused = false;

				addr.sin_family			= AF_INET;
				addr.sin_port			= _htons(pstrSock->u16Port);
				addr.sin_addr.s_addr	= pstrSock->u32IPAddress;

				pstrSock->u32TimeStart			= u32msTicks;
				pstrSock->u32LastTransmission	= pstrSock->u32TimeStart;
				pstrSock->u32LastStatsTime		= pstrSock->u32TimeStart;

				IperfUDP_SendTestPacket(i, &addr);
			}
		}
	}

	return M2M_SUCCESS;
}

//------------------------------------------------------------------------------
NMI_API sint8 IperfStop(SOCKET sock)
{
	int i;

	if (sock >= MAX_SOCKET)
		return M2M_ERR_FAIL;

	if(sock == -1)
	{
		for(i=0; i<MAX_SOCKET; i++)
			IperfSocketStop(i);
	}
	else
	{
		return IperfSocketStop(sock);
	}

	return M2M_SUCCESS;
}
