/*!
@file
mdns.c

*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "mdns/include/mdns.h"
#include "socket/include/socket.h"
#include "common/include/nm_common.h"
#include "m2m_test_config.h"
#include "string.h"
#include "socket/source/socket_internal.h"
#include "growl/include/growl.h"
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

//#define MDNS_LOGS

#ifdef MDNS_LOGS
#define MDNS_INFO(...)			do{M2M_INFO("(DNS)"); M2M_PRINT(__VA_ARGS__);}while(0)
#else
#define MDNS_INFO(...)
#endif

#define TYPE_PTR 0x000C
#define CLASS_IN 0x0001
#define MDNS_TYPE TYPE_PTR
#define MDNS_CLASS CLASS_IN
#define MAX_QUERY_SIZE				64
#define MSG_BUFF_SIZE 				512
#define DNS_RSP_FLAG_1		          	0x84
#define DNS_RSP_FLAG_2		         		0x00
#define TIME_TO_LIVE 				0x000000FF
#define IPV4_DATA_LENGTH  			0x04
#define SERVICE_NAME_LENGTH                  0x14
#define MDNS_BUF_SIZE     1500
#define DNS_SERVER_CACHE_SIZE			3
#define HTONS(n) 		((unsigned short )((((unsigned short ) (n)) << 8) | (((unsigned short ) (n)) >> 8)))
#define uip_ipaddr1(addr) (HTONS(((unsigned short  *)(addr))[0]) >> 8)
#define uip_ipaddr2(addr) (HTONS(((unsigned short  *)(addr))[0]) & 0xff)
#define uip_ipaddr3(addr) (HTONS(((unsigned short  *)(addr))[1]) >> 8)
#define uip_ipaddr4(addr) (HTONS(((unsigned short  *)(addr))[1]) & 0xff)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
typedef struct  {
	char 	query [MAX_QUERY_SIZE]  ;
	char 	queryForChecking [MAX_QUERY_SIZE]  ;
	unsigned short 	u16type ;
	unsigned short 	u16class ;
	unsigned short 	u16size  ;
}strdnsquery;

typedef struct  {
	unsigned short 	id;
	unsigned char 	flags1, flags2;
	unsigned short 	numquestions;
	unsigned short 	numanswers;
	unsigned short 	numauthrr;
	unsigned short 	numextrarr;
}tstrDnsHdr;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void convertServiceName (const char * name , int size,char * buf );
int MDNS_ParseQuery(unsigned char * pu8RxBuffer, tstrDnsHdr *pstrDnsHdr, strdnsquery *pstrDnsQuery  );
void MDNS_SendResp (signed char sock,unsigned char * pu8IPAddr, unsigned short u16Port,tstrDnsHdr *pstrDnsHdr,strdnsquery *pstrDnsQuery);
void MDNS_RecvfromCB(signed char  sock,unsigned char *pu8RxBuffer,signed short s16DataSize,unsigned char *pu8IPAddr,unsigned short	 u16Port,void *pvArg);
void UDP_SocketEventHandler(signed char  sock, unsigned char u8Msg, void * pvMsg);
extern void GROWL_APPSocketEventHandler(SOCKET sock, uint8 u8Msg, void * pvMsg);
extern void AppServerCb(uint8* pu8HostName, uint32 u32ServerIP);
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

static char		gpacDnsServerCache[DNS_SERVER_CACHE_SIZE][MDNS_HOSTNAME_SIZE] =
{
	"_services._dns-sd._udp.local","_workstation._tcp.local","_http._tcp.local"
};
unsigned char   gPu8Buf [MDNS_BUF_SIZE];
unsigned char	gu8pos ;
signed 	 char 	dns_server_sock ;
#define serviceName "_ATMELWIFI._tcp"


void convertServiceName ( const char * name , int size,char * buf )
{
	int i = 0;
	int count=0;
	while (i<size)
	{
		if(name[i]=='.')
		{
			buf[i-count]=count;
			count=0;
		}
		else
		{
			count++;
			buf[i+1]=name[i];
		}
		i++;
	}
	buf[i-count]=count-1;

}
int MDNS_ParseQuery(unsigned char * pu8RxBuffer, tstrDnsHdr *pstrDnsHdr, strdnsquery *pstrDnsQuery  )
{
	unsigned char  dot_size,temp=0;
	unsigned short n=0,i=0,u16index=0;
	int	bDNSmatch = 0;
	/*	00	-----------2 BYTES------------------- 16  17 18  19  20	  21  22	 23  24 25 26  27 28	29 30 31*/
	/*  ----Identification--------------------------|QR|	Opcode       |AA|TC|RD|RA|Z|AD|CD|Rcode   | */
	/*	----Total Questions------------------------|-----------------Total Answer RRs-------------*/
	/*	----Total Authority RRs	--------------------|----------------Total Additional RRs-----------*/
	/*	---------------------------------      Questions	  ------------------------------------- */
	/*	------------------------------------ Answer RRs	  ---------------------------------*/
	/*	----------------------------------- Authority RRs	  --------------------------------*/
	/*	-----------------------------------Additional RRs	  -------------------------------*/
	MDNS_INFO("Parsing DNS Packet\n");
	pstrDnsHdr->id = (( pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	MDNS_INFO ("id =  %.4x \n",pstrDnsHdr->id);
	u16index+=2;
	pstrDnsHdr->flags1= pu8RxBuffer[u16index++];
	pstrDnsHdr->flags2= pu8RxBuffer[u16index++];
	MDNS_INFO ("flags =  %.2x %.2x \n",pstrDnsHdr->flags1,pstrDnsHdr->flags2);
	pstrDnsHdr->numquestions = ((pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	MDNS_INFO ("numquestions =  %.4x \n",pstrDnsHdr->numquestions);
	u16index+=2;
	pstrDnsHdr->numanswers = ((pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	MDNS_INFO ("numanswers =  %.4x \n",pstrDnsHdr->numanswers);
	u16index+=2;
	pstrDnsHdr->numauthrr = ((pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	MDNS_INFO ("numauthrr =  %.4x \n",pstrDnsHdr->numauthrr);
	u16index+=2;
	pstrDnsHdr->numextrarr = ((pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	MDNS_INFO ("numextrarr =  %.4x \n",pstrDnsHdr->numextrarr);
	u16index+=2;
	dot_size =pstrDnsQuery->query[n++]= pu8RxBuffer[u16index++];
	pstrDnsQuery->u16size=1;
	while (dot_size--!=0) //(pu8RxBuffer[++u16index] != 0)
	{
		pstrDnsQuery->query[n++]=pstrDnsQuery->queryForChecking[i++]=pu8RxBuffer[u16index++] ;
		pstrDnsQuery->u16size++;
		gu8pos=temp;
		if (dot_size == 0 )
		{
			pstrDnsQuery->queryForChecking[i++]= '.' ;
			temp=u16index;
			dot_size =pstrDnsQuery->query[n++]= pu8RxBuffer[u16index++];
			pstrDnsQuery->u16size++;
		}
	}
	pstrDnsQuery->queryForChecking[--i] = 0;

	MDNS_INFO("parsed query <%s>\n",pstrDnsQuery->queryForChecking);

	/* Search for any match in the local DNS table.
	*/
	for(n = 0; n < DNS_SERVER_CACHE_SIZE; n++)
	{
		MDNS_INFO("Saved URL <%s>\n",gpacDnsServerCache[n]);
		if(strcmp(gpacDnsServerCache[n], pstrDnsQuery->queryForChecking) ==0)
		{
			bDNSmatch= 1;
			MDNS_INFO("MATCH \n");
		}
		else
		{
			MDNS_INFO("Mismatch\n");
		}
	}
	pstrDnsQuery->u16class = ((pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	u16index+=2;
	pstrDnsQuery->u16type= ((pu8RxBuffer[u16index]<<8)| (pu8RxBuffer[u16index+1]));
	return bDNSmatch;
}
void MDNS_SendResp (signed char sock,unsigned char * pu8IPAddr, unsigned short u16Port,tstrDnsHdr *pstrDnsHdr,strdnsquery *pstrDnsQuery)
{
	unsigned short u16index=0;
	tstrSockAddr strclientAddr ;
	unsigned char * pu8sendBuf;
	char * serviceName2 = (char*)malloc(sizeof(serviceName)+1);
	unsigned int MULTICAST_IP =  0xFB0000E0;
	pu8sendBuf= gPu8Buf;
	memcpy(&strclientAddr.u32IPAddr,&MULTICAST_IP,IPV4_DATA_LENGTH);
	strclientAddr.u16Port=u16Port;
	MDNS_INFO("%s \n",pstrDnsQuery->query);
	MDNS_INFO("Query Size = %d \n",pstrDnsQuery->u16size);
	MDNS_INFO("class = %.4x \n",pstrDnsQuery->u16class);
	MDNS_INFO("type  = %.4x \n",pstrDnsQuery->u16type);
	MDNS_INFO("PREPARING DNS ANSWER BEFORE SENDING\n");

	/*----------------------------ID 2 Bytes -----------------------------*/
	pu8sendBuf [u16index++] =0;  //( pstrDnsHdr->id>>8);
	pu8sendBuf [u16index++] =  0;//( pstrDnsHdr->id)&(0xFF);
	MDNS_INFO ("(ResPonse) id = %.2x %.2x  \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*----------------------------Flags 2 Bytes----------------------------*/
	pu8sendBuf [u16index++] =  DNS_RSP_FLAG_1;
	pu8sendBuf [u16index++] =  DNS_RSP_FLAG_2;
	MDNS_INFO ("(ResPonse) Flags = %.2x %.2x  \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*----------------------------No of Questions--------------------------*/
	pu8sendBuf [u16index++] =0x00;
	pu8sendBuf [u16index++] =0x01;
	MDNS_INFO ("(ResPonse) Questions  = %.2x %.2x  \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*---------------------------No of Answers----------------------------*/
	pu8sendBuf [u16index++] =0x00;
	pu8sendBuf [u16index++] =0x01;
	MDNS_INFO ("(ResPonse) Answers = %.2x %.2x  \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*---------------------------No of Authority RRs------------------------*/
	pu8sendBuf [u16index++] =0x00;
	pu8sendBuf [u16index++] =0x00;
	MDNS_INFO ("(ResPonse) Authority RRs = %.2x %.2x  \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*----------------------------No of Additional RRs----------------------*/
	pu8sendBuf [u16index++] =0x00;
	pu8sendBuf [u16index++] =0x00;
	MDNS_INFO ("(ResPonse) Additional RRs = %.2x %.2x  \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*--------------------------------Query-----------------------------*/
	memcpy(&pu8sendBuf[u16index],pstrDnsQuery->query,pstrDnsQuery->u16size);
	MDNS_INFO("\nsize = %d \n",pstrDnsQuery->u16size);
	u16index+=pstrDnsQuery->u16size;
	/*-------------------------------Query Type----------------------------*/
	pu8sendBuf [u16index++] = ( pstrDnsQuery->u16type>>8);//MDNS_TYPE>>8;
	pu8sendBuf [u16index++] = ( pstrDnsQuery->u16type)&(0xFF);//(MDNS_TYPE&0xFF);
	MDNS_INFO ("Query Type =  %.2x %.2x \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*------------------------------Query Class-----------------------------------*/
	pu8sendBuf [u16index++] =MDNS_CLASS>>8;//(( pstrDnsQuery->u16class>>8)|0x80);
	pu8sendBuf [u16index++] = (MDNS_CLASS & 0xFF);//( pstrDnsQuery->u16class)&(0xFF);
	MDNS_INFO ("Query Class =  %.2x %.2x \n", pu8sendBuf[u16index-2],pu8sendBuf[u16index-1]);
	/*########################Answers#########################*/

	/*------------------------------Name---------------------------------*/
	pu8sendBuf [u16index++]=  0xC0 ; //pointer to query name location
	pu8sendBuf [u16index++]= 0x0C ; // instead of writing the whole query name again
	/*-----------------------------Type----------------------------------*/
	pu8sendBuf [u16index++] =MDNS_TYPE>>8;  //Type 12 PTR (domain name Pointer).
	pu8sendBuf [u16index++] =(MDNS_TYPE&0xFF);
	/*------------------------------Class-----------------------------------*/
	pu8sendBuf [u16index++] =0x00;//MDNS_CLASS;  //Class IN, Internet.
	pu8sendBuf [u16index++] =0x01;// (MDNS_CLASS & 0xFF);
	/*-----------------------------TTL----------------------------------*/
	pu8sendBuf [u16index++] =(TIME_TO_LIVE >>24);
	pu8sendBuf [u16index++] =(TIME_TO_LIVE >>16);
	pu8sendBuf [u16index++] =(TIME_TO_LIVE >>8);
	pu8sendBuf [u16index++] =(TIME_TO_LIVE );
	/*---------------------------Date Length----------------------------------*/
	pu8sendBuf [u16index++] =(sizeof(serviceName)+2)>>8;//added 2 bytes for the pointer
	pu8sendBuf [u16index++] =(sizeof(serviceName)+2);
	/*-----------------------------DATA--------------------------------*/
	convertServiceName(serviceName,sizeof(serviceName),serviceName2);
	memcpy(&pu8sendBuf[u16index],serviceName2,sizeof(serviceName)+1);
	u16index+=sizeof(serviceName);
	pu8sendBuf [u16index++] =0xC0;//Pointer to .local (from name)
	pu8sendBuf [u16index++] =gu8pos;//23
	/*###########################################################*/
	strclientAddr.u16Port=HTONS(MDNS_SERVER_PORT);
	sendto( sock, pu8sendBuf,(uint16)u16index,0,(struct sockaddr*)&strclientAddr,sizeof(strclientAddr)); /* MultiCast RESPONSE*/
	strclientAddr.u16Port=u16Port;
	memcpy(&strclientAddr.u32IPAddr,pu8IPAddr,IPV4_DATA_LENGTH);
	sendto( sock, pu8sendBuf,(uint16)u16index,0,(struct sockaddr*)&strclientAddr,sizeof(strclientAddr)); /* UniCast RESPONSE*/

}

void MDNS_RecvfromCB(signed char  sock,unsigned char *pu8RxBuffer,signed short s16DataSize,unsigned char *pu8IPAddr,unsigned short	 u16Port,void *pvArg)
{
	MDNS_INFO("DnsServer_RecvfromCB \n");
	if((pu8RxBuffer != 0) && (s16DataSize > 0))
	{
		tstrDnsHdr strDnsHdr  ;
		strdnsquery strDnsQuery ;
		MDNS_INFO("DNS Packet Received  \n");

		if(MDNS_ParseQuery(&pu8RxBuffer[0], &strDnsHdr,&strDnsQuery))
		MDNS_SendResp (sock,pu8IPAddr, u16Port,&strDnsHdr,&strDnsQuery );
	}
	else
	{
		MDNS_INFO("DnsServer_RecvfromCB Error !\n");
	}
}
/*********************************************************************/

void UDP_SocketEventHandler(signed char  sock, unsigned char u8Msg, void * pvMsg)
{
	if (sock == dns_server_sock)
	{
		if(u8Msg == SOCKET_MSG_BIND)
		{
			tstrSocketBindMsg	*pstrBindStatus = (tstrSocketBindMsg*)pvMsg;
			if(pstrBindStatus->status != 0)
			{
				M2M_ERR("MDNS Bind Failed With ERR <%d>\n",pstrBindStatus->status);
				close(dns_server_sock);
				MDNS_ServerStart();
				return;
			}
		}
		else if(u8Msg == SOCKET_MSG_RECVFROM)
		{
			tstrSocketRecvMsg	*pstrRx = (tstrSocketRecvMsg*)pvMsg;
			if(pstrRx->pu8Buffer != 0)
			{
				if(pstrRx->s16BufferSize)
				MDNS_RecvfromCB(sock,pstrRx->pu8Buffer,pstrRx->s16BufferSize,(unsigned char *)&pstrRx->strRemoteAddr.sin_addr,pstrRx->strRemoteAddr.sin_port,0);
				
			}
		}
		recv(sock, gPu8Buf, MDNS_BUF_SIZE, 0);
	}
	else
	{
		GROWL_APPSocketEventHandler(sock, u8Msg,pvMsg);
	}
}
/*********************************************************************/
void MDNS_ServerInit(void)
{
	dns_server_sock = -1;
	socketInit();
	registerSocketCallback(UDP_SocketEventHandler,AppServerCb);
}
/*********************************************************************/
void MDNS_ServerStart(void)
{
	tstrSockAddr	strAddr ;
	char  multicast_address[] = "224.0.0.251";
	uint32 MULTICAST_IP =  nmi_inet_addr(multicast_address);
	
	if(dns_server_sock >= 0)
	{
		close(dns_server_sock);
	}
	dns_server_sock = socket( AF_INET, SOCK_DGRAM,0);
	M2M_INFO("DNS_server_init %d\n",dns_server_sock);
	setsockopt(dns_server_sock,1,IP_ADD_MEMBERSHIP,&MULTICAST_IP,sizeof(MULTICAST_IP));
	strAddr.u16Port	=HTONS(MDNS_SERVER_PORT);//ask here
	bind(dns_server_sock,(struct sockaddr*)&strAddr,sizeof(strAddr));
}

