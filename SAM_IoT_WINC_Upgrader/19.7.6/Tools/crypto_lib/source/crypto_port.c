#include "m2m_types.h"
#include <Windows.h>
#include <wincrypt.h>

void memcpy_dbg(void *dst, void *src, int count)
{
	int i;
	uint8	*ssrc, *ddst;
	ssrc = (uint8*)src;
	ddst = (uint8*)dst;
	for(i = 0; i < count; i ++)
	{
		ddst[i] = ssrc[i];
	}
}

#define SCRATCH_MEM_MAX_SIZE			(5 * 1024)

uint8	g_scratch_mem[SCRATCH_MEM_MAX_SIZE + 3] = {0};
uint8  *g_scratch_mem_ptr = g_scratch_mem;
uint32	g_scratch_mem_idx  = SCRATCH_MEM_MAX_SIZE;


void *scratch_mem_alloc(uint32 size)
{
	void *buffer = NULL;

	size = WORD_ALIGN(size);
	if(g_scratch_mem_idx >= size)
	{
		g_scratch_mem_idx -= size;
		buffer = &g_scratch_mem_ptr[g_scratch_mem_idx];
	}
	if(buffer == NULL)
		printf("FAILED SCRATCH\n");
	return buffer;
}

uint32 get_scratch_mem_idx(void)
{
	return g_scratch_mem_idx;
}

void restore_scratch_mem_idx(uint32 mem_idx)
{
	if(mem_idx > SCRATCH_MEM_MAX_SIZE)
	{
		mem_idx = SCRATCH_MEM_MAX_SIZE;
	}
	
	if(mem_idx > g_scratch_mem_idx)
		g_scratch_mem_idx = mem_idx;
}

static HCRYPTPROV			ghProv;	/* Crypto context for random bytes */

void getRand(uint8 *pu8buf, uint32 u32n)
{
	static int isInit = 0;

	if(!isInit)
	{
		CryptAcquireContext(&ghProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	}
	CryptGenRandom(ghProv, u32n, pu8buf);
}
