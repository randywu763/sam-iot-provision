/*!
@file			
	http_utils.h

@brief	HTTP Utility Functions

	Common functions and types for the HTTP Protocol.
			
@author		
	Ahmed Ezzat

@date		
	18 Sep 2014
*/

#ifndef __HTTP_UTILS_H__
#define __HTTP_UTILS_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "common/include/nm_common.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define HTTP_200_OK											"HTTP/1.1 200 OK\r\n"
#define HTTP_303_REDIRECT									"HTTP/1.1 303 See Other\r\n"
#define HTTP_400_BAD_REQ									"HTTP/1.1 400 Bad Request\r\n"
#define HTTP_404_NOT_FOUND									"HTTP/1.1 404 Not Found\r\n"
#define HTTP_405_METHOD_NOT_ALLOWED							"HTTP/1.1 405 Method Not Allowed\r\n"
#define HTTP_500_SERVER_ERROR								"HTTP/1.1 500 Internal Server Error\r\n"
#define HTTP_CACHE_CONTROL_CACHABLE							"Cache-Control: public, max-age=3600\r\n"
#define HTTP_CACHE_CONTROL_NO_CACHE							"Cache-Control: no-cache, no-store, must-revalidate\r\n"



//#define HTTP_DEBUG
//#define HTTP_DUMP_REQ
//#define HTTP_DUMP_RESP

#ifndef __HTTP_DBG_FILE__
#define HTTP_PRINT			M2M_PRINT
#else
extern FILE *fpLogFile2;
#define HTTP_PRINT(...)	\
do	\
{										\
	if(fpLogFile2 == 0)	\
	{\
	fpLogFile2 = fopen("http.log","w");\
	}\
	fprintf(fpLogFile2,__VA_ARGS__);	\
	fflush(fpLogFile2);	\
}while(0)

#endif

#define HTTP_ERR(...)	\
	do{HTTP_PRINT("(HTTP)(ERR)(%d)",__LINE__); HTTP_PRINT(__VA_ARGS__);}while(0)

#define HTTP_INFO(...)			HTTP_PRINT("(HTTP)" __VA_ARGS__)

#ifdef HTTP_DEBUG
#define HTTP_DBG(...)			HTTP_PRINT("(HTTP)" __VA_ARGS__)	
#else
#define HTTP_DBG(...)
#endif
	
	
#ifdef HTTP_DUMP_REQ
#define HTTP_REQ(REQ, SZ)		\
		do{\
			HTTP_PRINT("(HTTP)REQ <%lu>\n",SZ);\
			HTTP_PRINT("*======*======*======*\n"); \
			HTTP_PRINT("%s");\
			HTTP_PRINT("*======*======*======*\n",(REQ));\
		}while(0)
#else
#define HTTP_REQ(REQ, SZ)	
#endif


#ifdef HTTP_DUMP_RESP
#define HTTP_RESP(RESP)		\
		do{\
			HTTP_PRINT("(HTTP)RESP <%d>\n",strlen((RESP)));\
			HTTP_PRINT("*======*======*======*\n"); \
			HTTP_PRINT("%s",(RESP));\
			HTTP_PRINT("*======*======*======*\n");\
		}while(0)
#else
#define HTTP_RESP(RESP)
#endif


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*!
@enum	\
	tenuHTTPStatusCode

@brief
	HTTP Standard response status codes.
*/
typedef enum{
	HTTP_STATUS_OK						= 200,
	/*!<
		Request processing is OK.
	*/
	HTTP_STATUS_REDIRECT					= 303,
	/*!<
		The requested URL does not exist and a new URL should be used instead.
	*/
	HTTP_STATUS_BAD_REQ					= 400,
	/*!<
		The request format is wrong.
	*/
	HTTP_STATUS_NOT_FOUND				= 404,
	/*!<
		The requested file is not found on the server.
	*/
	HTTP_STATUS_METHOD_NOT_ALLOWED	= 405,
	/*!<
		The HTTP Request method is not supported by the server.
	*/
	HTTP_STATUS_SERVER_ERROR			= 500
	/*!<
		Something goes wrong at the server side.
	*/
}tenuHTTPStatusCode;


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION DECLARATIONS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


uint32	Ascii2Dec(char *pcBuf);
void	Dec2Ascii(uint32 u32Digit, char **ppcBuf);
char*	HttpStrStr(char *pcIn, char *pcStr);
char*	HttpstrTOK(char *pcStr, char *pcSep);
sint8	HttpstrCmpIgnoreCase(char *pcStr1, char *pcStr2, uint32 u32Size);
void	UrlDecode(uint8* temp, int limit);
uint8	Ascii2Hex(char u8AsciiChar);

#endif /* __HTTP_UTILS_H__ */
