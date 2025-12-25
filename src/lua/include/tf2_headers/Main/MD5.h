#if !defined(MainMD5_Included)
#define MainMD5_Included

#define MD5_DIGEST_LENGTH 16
#define MD5_BIT_LENGTH (MD5_DIGEST_LENGTH * sizeof(unsigned char))

typedef struct MD5Value_t
{
	unsigned char bits[MD5_DIGEST_LENGTH];
} MD5Value_t;

typedef struct MD5Context_t
{
	unsigned int buf[4];
	unsigned int bits[2];
	unsigned char in[64];
} MD5Context_t;

#endif