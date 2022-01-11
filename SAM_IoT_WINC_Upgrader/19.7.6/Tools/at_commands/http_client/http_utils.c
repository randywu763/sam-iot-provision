#include "http_utils.h"


#define TO_LOWER_CASE(ch)			\
	((((ch) >= 65) && ((ch) <= 90)) ? ((ch) + 32) : (ch))

/*********************************************************************
HttpstrCmpIgnoreCase
*/
sint8 HttpstrCmpIgnoreCase(char *pcStr1, char *pcStr2, uint32 u32Size)
{
	sint8		s8Ret = 1;
	if((pcStr1 != NULL) && (pcStr2 != NULL) && (u32Size > 0))
	{
		uint32	u32Idx;
		char		c1, c2;

		s8Ret = 0;
		for(u32Idx = 0; u32Idx < u32Size; u32Idx ++)
		{
			c1 = TO_LOWER_CASE(pcStr1[u32Idx]);
			c2 = TO_LOWER_CASE(pcStr2[u32Idx]);
			if(c1 != c2)
			{
				s8Ret = c1 - c2;
				break;
			}
		}
	}
	return s8Ret;
}
/*********************************************************************
Dec2Ascii
*/
uint32 Ascii2Dec(char *pcBuf)
{
	uint32	u32Val = 0;

	if(pcBuf != NULL)
	{
		while(*pcBuf != 0)
		{
			if(*pcBuf != '0')
			{				
				sint32	s32Idx;
				uint32	u32X = 1;
				uint32	u32Size = (uint32)strlen(pcBuf);
				for(s32Idx = u32Size - 1; s32Idx >= 0; s32Idx --)
				{
					if((pcBuf[s32Idx] >= '0') && (pcBuf[s32Idx] <= '9'))
					{
						u32Val += u32X * (pcBuf[s32Idx] - '0');
						u32X = u32X * 10;
					}
				}

				break;
			}
			pcBuf ++;
		}
	}
	return u32Val;
}
/*********************************************************************
HttpstrTOK
*/
char* HttpstrTOK(char *pcStr, char *pcSep)
{
	char		*pcRetStr = NULL;	
	static char	*pcCurPos = NULL;
	uint8		u8nSep;
	uint8		u8Idx;

	if(pcStr != NULL)
	{
		pcCurPos	= pcStr;
		if(pcSep != NULL)
		{
			u8nSep 	= (uint8)strlen(pcSep);
			while(*pcCurPos)
			{
				for(u8Idx = 0; u8Idx < u8nSep; u8Idx ++)
				{
					if(pcSep[u8Idx] == *pcCurPos)
					{
						pcCurPos ++;
						break;
					}
				}
				if(u8Idx == u8nSep)
					break;
			}
		}
		pcRetStr = pcCurPos;
	}
	else
	{
		pcRetStr = pcCurPos;
		if(pcSep != NULL)
		{
			u8nSep 	= (uint8)strlen(pcSep);
			while(*pcCurPos)
			{
				for(u8Idx = 0; u8Idx < u8nSep; u8Idx ++)
				{
					if(pcSep[u8Idx] == *pcCurPos)
					{
						*pcCurPos ++ = 0;
						break;
					}
				}
				if(u8Idx < u8nSep)
					break;

				pcCurPos ++;
			}
		}
	}
	return pcRetStr;
}
/*********************************************************************
Dec2Ascii
*/
void Dec2Ascii(uint32 u32Digit, char **ppcBuf)
{
	if(ppcBuf != NULL)
	{
		char	*pcBuf = *ppcBuf;
		uint8	u8ReqZeroPad = 4 - (((uint32)pcBuf) % 4);
		uint8	u8nDigits = 7;

		if((pcBuf != NULL) && (u32Digit < 10000000))
		{
			uint8	au8Digits[11];
			uint8	u8Count;
			uint32	u32Val = u32Digit;
			
			au8Digits[0]	= (uint8)(u32Val   / 1000000);
			u32Val			= u32Val % 1000000;
			
			au8Digits[1] 	= (uint8)(u32Val   / 100000);
			u32Val			= u32Val % 100000;

			au8Digits[2] 	= (uint8)(u32Val   / 10000);
			u32Val			= u32Val % 10000;

			au8Digits[3] 	= (uint8)(u32Val   / 1000);
			u32Val			= u32Val % 1000;

			au8Digits[4] 	= (uint8)(u32Val   / 100);
			u32Val			= u32Val % 100;

			au8Digits[5] 	= (uint8)(u32Val   / 10);
			au8Digits[6]	= u32Val % 10;

			for(u8Count = 0; u8Count < 7; u8Count ++)
			{
				if(au8Digits[u8Count] != 0)
				{
					u8nDigits = 7 - u8Count;
					break;
				}
			}

			if(u8nDigits == u8ReqZeroPad)
			{
				u8ReqZeroPad = 0;
			}
			else if(u8nDigits < u8ReqZeroPad)
			{
				u8ReqZeroPad = u8ReqZeroPad - u8nDigits;
			}
			else
			{
				uint8	u8Pad;
				uint8	u8Q, u8R;

				u8Q = (u8nDigits - u8ReqZeroPad) >> 2;
				u8R = (u8nDigits - u8ReqZeroPad) & 0x03;
				u8Pad = u8Q + u8ReqZeroPad;
				u8Pad += (u8R ? 4 : 0);
				u8ReqZeroPad = u8Pad - u8nDigits;
			}
						
			for(u8Count = 7 - u8nDigits ; u8Count < 7; u8Count ++)
			{
				*pcBuf ++ = au8Digits[u8Count] + '0';
			}
			*ppcBuf = pcBuf;

		}
	}
}
/*********************************************************************
Ascii2Hex
*/
uint8 Ascii2Hex(char u8AsciiChar)
{
	uint8	u8Hex = 0xFF;
	if((u8AsciiChar >= 'A') && (u8AsciiChar <= 'F'))
	{
		u8Hex = u8AsciiChar - 'A';
	}
	else if((u8AsciiChar >= 'a') && (u8AsciiChar <= 'f'))
	{
		u8Hex = u8AsciiChar - 'a';
	}
	else if((u8AsciiChar >= '0') && (u8AsciiChar <= '9'))
	{
		u8Hex = u8AsciiChar - '0';
	}
	return u8Hex;
}
/*********************************************************************
AsciiStr2Hex
*/
static uint8 AsciiStr2Hex(uint8* pu8Str)
{
	uint8	u8Hex = 0xFF;
	
	u8Hex = Ascii2Hex((char)pu8Str[1]);
	u8Hex |= (Ascii2Hex((char)pu8Str[0])<<4);
	
	return u8Hex;
}
/*********************************************************************
UrlDecode
*/
void UrlDecode(uint8* temp, int limit)
{
	int i =0;

	if(temp == NULL)
		return;
	
	while((*temp) != 0)
	{
		if((char)(*temp) == '%')
		{
			int sLen =0;

			*temp = AsciiStr2Hex(temp+1);
			sLen = m2m_strlen(temp+3);
			
			if(sLen > (limit * 3))
			{
				M2M_ERR("slen too large!\n");
				break;
			}

			m2m_memcpy((temp+1), (temp+3), sLen+1);
			
			//M2M_INFO("value = %d , %x %x\n",*temp ,*(temp+1),*(temp+2));
		}

		i++;
		temp++;

		if(i > limit)
		{
			M2M_ERR("Stuck in UrlDecode %d %d\n",i,limit);
			break;
		}
	}
}
/*********************************************************************
Dec2Ascii
*/
char * HttpStrStr(char *pcIn, char *pcStr)
{
	char 	firstCh;
	uint32 	u32StrLen;

	firstCh = *pcStr++;
	if (!firstCh)
		return pcIn;

	u32StrLen = strlen(pcStr);
	do
	{
		char inCh;
		do 
		{
			inCh = *pcIn++;
			if (!inCh)
				return NULL;
		} while(inCh != firstCh);
	} while(memcmp(pcIn, pcStr, u32StrLen) != 0);
	return (pcIn - 1);
}
