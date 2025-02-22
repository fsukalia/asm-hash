/*
 * sha256.c
 * Author: Fabjan Sukalia(fsukalia@gmail.com)
 * Date: 2014-04-28
 */


#include "sha256.h"

void process_block(block* b, const uint8_t block[], unsigned int n, bool data_bits) {
	(void)data_bits;
#ifdef SHA256_USE_ASM
	sha256_process_blocks_asm(block, (uint32_t*)(b->func_data), n);
#else
	sha256_process_blocks(block, (uint32_t*)(b->func_data), n);
#endif
}

/* SHA-256 */
void sha256_init(sha256_context* ctxt) {
	if (ctxt == NULL)
		return;
	
	sha256_init_hash(ctxt->hash);	
	block_init(&ctxt->b, BLOCK_SIZE_512, ctxt->buffer, process_block, ctxt->hash);
}

void sha256_finalize(sha256_context* ctxt) {
	block_util_finalize(&ctxt->b, BLOCK_LENGTH_64 | BLOCK_BIG_ENDIAN | BLOCK_SIMPLE_PADDING);
}

void sha256_add(sha256_context* ctxt, const uint8_t data[], size_t length) {
	block_add(&ctxt->b, length, data);
}

void sha256_get_digest(sha256_context* ctxt, uint8_t digest[SHA256_DIGEST_SIZE]) {
	sha256_hash_to_digest(ctxt->hash, digest);
}

/* SHA-224 */
void sha224_init(sha256_context* ctxt) {
	if (ctxt == NULL)
		return;
	
	block_init(&ctxt->b, BLOCK_SIZE_512, ctxt->buffer, process_block, ctxt->hash);
	sha224_init_hash(ctxt->hash);
}

void sha224_add(sha224_context* ctxt, const uint8_t data[], uint64_t length) {
	sha256_add(ctxt, data, length);
}

void sha224_finalize(sha224_context* ctxt) {
	sha256_finalize(ctxt);
}

void sha224_get_digest(sha256_context* ctxt, uint8_t digest[SHA224_DIGEST_SIZE]) {
	sha224_hash_to_digest(ctxt->hash, digest);
}

