/*
 * ps3mca-tool - PlayStation 3 Memory Card Adaptor Software
 * Copyright (C) 2011 - "someone who wants to stay anonymous"
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mecha_emu.h"
#include "mecha_keys.h"
#include "util.h"
#include "cipher.h"
#include "config.h"

#include <stdio.h>

#define _CrtDbgBreak() __asm__ ("\tint $0x3\n")

struct CryptoContext {
    uint8_t UniqueKey[16];
    uint8_t FileKey[16];
    uint8_t CardIV[8];
    uint8_t CardMaterial[8];
    uint8_t CardNonce[8];
    uint8_t MechaNonce[8];
    uint8_t SessionKey[8];
    uint8_t DecryptedKbit[16];
    uint8_t DecryptedKc[16];
    uint8_t EncryptedKbit[16];
    uint8_t EncryptedKc[16];
} __attribute__((packed));

struct Kelf_Header {
    uint32_t unknown1;
    uint32_t unknown2;
    uint16_t unknown3_half;
    uint16_t version;
    uint32_t unknown4;
    uint32_t ELF_size;		/* Size of data blocks = Decrypted elf size */
    uint16_t Kelf_header_size;	/* Kelf header size                         */
    uint16_t unknown5;
    uint16_t flags;			/* ? for header purpose                     */
    uint16_t BIT_count;		/* used to set/get kbit and kc offset       */
    uint32_t mg_zones;
} __attribute__((packed));

static struct CryptoContext context;

void meResetCryptoContext(void)
{
    memset((void *)&context, 0, sizeof(struct CryptoContext));
}

/*
 * meCardCalcUniqueKey: calculate unique key for memory card authentication.
 */
void meCardCalcUniqueKey(uint8_t *CardIV, uint8_t *CardMaterial)
{
    uint8_t Input[8];

    memxor(CardIV, CardMaterial, Input, 8);

    cipherCbcEncrypt(&context.UniqueKey[0], Input, 8, MC_CARDKEY_HASHKEY_1, 2, MC_CARDKEY_MATERIAL_1);
    cipherCbcEncrypt(&context.UniqueKey[8], Input, 8, MC_CARDKEY_HASHKEY_2, 2, MC_CARDKEY_MATERIAL_2);

    memcpy(context.CardIV, CardIV, 8);
    memcpy(context.CardMaterial, CardMaterial, 8);
    if (VERBOSE > 1) PRINTBUF(context.UniqueKey)
}

void meCardGenerateChallenge(uint8_t *CardIV, uint8_t *CardNonce, uint8_t *MechaNonce,
                             uint8_t *MechaChallenge1, uint8_t *MechaChallenge2, uint8_t *MechaChallenge3)
{
    cipherCbcEncrypt(MechaChallenge1, MechaNonce, 8, context.UniqueKey, 2, MC_CHALLENGE_MATERIAL);
    cipherCbcEncrypt(MechaChallenge2, CardNonce,  8, context.UniqueKey, 2, MechaChallenge1);
    cipherCbcEncrypt(MechaChallenge3, CardIV,     8, context.UniqueKey, 2, MechaChallenge2);

    memcpy(context.CardNonce, CardNonce, 8);
    memcpy(context.MechaNonce, MechaNonce, 8);
}

int meCardVerifyResponse(uint8_t *CardResponse1, uint8_t *CardResponse2, uint8_t *CardResponse3)
{
    uint8_t DecryptedResponse1[8], DecryptedResponse2[8], DecryptedResponse3[8];
    int r = -1;

    cipherCbcDecrypt(DecryptedResponse1, CardResponse1, 8, context.UniqueKey, 2, MC_CHALLENGE_MATERIAL);
    cipherCbcDecrypt(DecryptedResponse2, CardResponse2, 8, context.UniqueKey, 2, CardResponse1);
    cipherCbcDecrypt(DecryptedResponse3, CardResponse3, 8, context.UniqueKey, 2, CardResponse2);

    if (!memcmp(DecryptedResponse1, context.CardNonce, 8) && !memcmp(DecryptedResponse2, context.MechaNonce, 8)) {
        memcpy(context.SessionKey, DecryptedResponse3, 8);
        r = 0;
        if (VERBOSE > 2) PRINTBUF(context.SessionKey);
    }

    return  r;
}

int meGetContentKeyOffset(uint8_t *KelfHeader)
{
    int i, offset;
    struct Kelf_Header *hdr = (struct Kelf_Header *)KelfHeader;

    offset = 32;

    for (i=0; i<read_le_uint16((uint8_t *)&hdr->BIT_count); i++)
        offset += 16;

    if ((read_le_uint32((uint8_t *)&hdr->flags) & 1) != 0)
        offset += KelfHeader[offset] + 1;

    if ((read_le_uint32((uint8_t *)&hdr->flags) & 0xf000) == 0)
        offset += 8;

    return offset;
}

void meDecryptDiskContentKey(uint8_t *KelfHeader)
{
    int CK_offset;
    uint8_t HeaderData[8];

    CK_offset = meGetContentKeyOffset(KelfHeader);

    /* calculate file keys necessary to decrypt Kbit and Kc from MG header and populate cryptographic context */
    memxor(KelfHeader, &KelfHeader[8], HeaderData, 8);
    cipherCbcEncrypt(&context.FileKey[0], HeaderData, 8, MG_KBIT_MASTER_KEY, 2, MG_KBIT_MATERIAL);
    cipherCbcEncrypt(&context.FileKey[8], HeaderData, 8, MG_KC_MASTER_KEY,   2, MG_KC_MATERIAL);
    memcpy(context.EncryptedKbit, &KelfHeader[CK_offset], 16);
    memcpy(context.EncryptedKc, &KelfHeader[CK_offset+16], 16);

    /* finally Decrypt the ContentKey */
    cipherCbcDecrypt(&context.DecryptedKbit[0], &context.EncryptedKbit[0], 8, context.FileKey, 2, MG_IV_NULL);
    cipherCbcDecrypt(&context.DecryptedKbit[8], &context.EncryptedKbit[8], 8, context.FileKey, 2, MG_IV_NULL);
    cipherCbcDecrypt(&context.DecryptedKc[0],   &context.EncryptedKc[0],   8, context.FileKey, 2, MG_IV_NULL);
    cipherCbcDecrypt(&context.DecryptedKc[8],   &context.EncryptedKc[8],   8, context.FileKey, 2, MG_IV_NULL);
}

void meEncryptDiskContentKey(uint8_t *KelfHeader, uint8_t *ContentKey)
{
    int CK_offset;
    uint8_t HeaderData[8];

    CK_offset = meGetContentKeyOffset(KelfHeader);

    /* calculate file keys necessary to decrypt Kbit and Kc from MG header and populate cryptographic context */
    memxor(KelfHeader, &KelfHeader[8], HeaderData, 8);
    cipherCbcEncrypt(&context.FileKey[0], HeaderData, 8, MG_KBIT_MASTER_KEY, 2, MG_KBIT_MATERIAL);
    cipherCbcEncrypt(&context.FileKey[8], HeaderData, 8, MG_KC_MASTER_KEY,   2, MG_KC_MATERIAL);
    memcpy(context.EncryptedKbit, &KelfHeader[CK_offset], 16);
    memcpy(context.EncryptedKc, &KelfHeader[CK_offset+16], 16);

    /* finally Decrypt the ContentKey */
    cipherCbcEncrypt(&context.EncryptedKbit[0], &context.DecryptedKbit[0], 8, context.FileKey, 2, MG_IV_NULL);
    cipherCbcEncrypt(&context.EncryptedKbit[8], &context.DecryptedKbit[8], 8, context.FileKey, 2, MG_IV_NULL);
    cipherCbcEncrypt(&context.EncryptedKc[0],   &context.DecryptedKc[0],   8, context.FileKey, 2, MG_IV_NULL);
    cipherCbcEncrypt(&context.EncryptedKc[8],   &context.DecryptedKc[8],   8, context.FileKey, 2, MG_IV_NULL);

    memcpy(&ContentKey[0], &context.EncryptedKbit[0], 16);
    memcpy(&ContentKey[16], &context.EncryptedKc[0], 16);

}

void meCpEncryptedContentKey(uint8_t *KelfHeader, uint8_t *ContentKey)
{
    int CK_offset;
    uint8_t HeaderData[8];

    CK_offset = meGetContentKeyOffset(KelfHeader);

    /* calculate file keys necessary to decrypt Kbit and Kc from MG header */
    memxor(KelfHeader, &KelfHeader[8], HeaderData, 8);
    cipherCbcEncrypt(&context.FileKey[0], HeaderData, 8, MG_KBIT_MASTER_KEY, 2, MG_KBIT_MATERIAL);
    cipherCbcEncrypt(&context.FileKey[8], HeaderData, 8, MG_KC_MASTER_KEY,   2, MG_KC_MATERIAL);

    memcpy(&ContentKey[0], &KelfHeader[CK_offset], 16);
    memcpy(&ContentKey[16], &KelfHeader[CK_offset+16], 16);

}

void meEncryptWithSessionKey(uint8_t *ContentKey)
{
    cipherCbcEncrypt(&context.EncryptedKbit[0], &context.DecryptedKbit[0], 8, context.SessionKey, 1, MG_IV_NULL);
    cipherCbcEncrypt(&context.EncryptedKbit[8], &context.DecryptedKbit[8], 8, context.SessionKey, 1, MG_IV_NULL);
    cipherCbcEncrypt(&context.EncryptedKc[0],   &context.DecryptedKc[0],   8, context.SessionKey, 1, MG_IV_NULL);
    cipherCbcEncrypt(&context.EncryptedKc[8],   &context.DecryptedKc[8],   8, context.SessionKey, 1, MG_IV_NULL);

    memcpy(&ContentKey[0], context.EncryptedKbit, 16);
    memcpy(&ContentKey[16], context.EncryptedKc, 16);
}

void meDecryptWithSessionKey(uint8_t *ContentKey)
{
    memcpy(context.EncryptedKbit, &ContentKey[0], 16);
    memcpy(context.EncryptedKc, &ContentKey[16], 16);

    cipherCbcDecrypt(&context.DecryptedKbit[0], &context.EncryptedKbit[0], 8, context.SessionKey, 1, MG_IV_NULL);
    cipherCbcDecrypt(&context.DecryptedKbit[8], &context.EncryptedKbit[8], 8, context.SessionKey, 1, MG_IV_NULL);
    cipherCbcDecrypt(&context.DecryptedKc[0], 	&context.EncryptedKc[0]  , 8, context.SessionKey, 1, MG_IV_NULL);
    cipherCbcDecrypt(&context.DecryptedKc[8], 	&context.EncryptedKc[8]  , 8, context.SessionKey, 1, MG_IV_NULL);

    memcpy(&ContentKey[0], context.DecryptedKbit, 16);
    memcpy(&ContentKey[16], context.DecryptedKc, 16);
}
