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

#ifndef __MECHAEMU_H__
#define __MECHAEMU_H__

#include <memory.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * @brief reset the crypto context
 */
void meResetCryptoContext(void);
void meCardCalcUniqueKey(uint8_t *CardIV, uint8_t *CardMaterial);
/**
 * @brief generate challenge for memory card.
 * @param MechaChallenge1 output parameter: Challenge 1
 * @param MechaChallenge2 output parameter: Challenge 2
 * @param MechaChallenge3 output parameter: Challenge 3
 */
void meCardGenerateChallenge(uint8_t *CardIV,
                             uint8_t *CardNonce,
                             uint8_t *MechaNonce,
                             uint8_t *MechaChallenge1,
                             uint8_t *MechaChallenge2,
                             uint8_t *MechaChallenge3);

/**
 * @brief final verify response from memory card.
 */
int meCardVerifyResponse(uint8_t *CardResponse1, uint8_t *CardResponse2, uint8_t *CardResponse3);

/**
 * @brief calculate ContentKey (kbit and kc) offset in a KELF
 */
int meGetContentKeyOffset(uint8_t *KelfHeader);

/**
 * @brief Decrypt the encrypted ContentKey for disk/rom
 * from a given Kelf header
 */
void meDecryptDiskContentKey(uint8_t *KelfHeader);

/**
 * @brief Encrypt the ContentKey for disk/rom with decrypted
 * ContentKey recovered from card
 */
void meEncryptDiskContentKey(uint8_t *KelfHeader, uint8_t *ContentKey);
void meCpEncryptedContentKey(uint8_t *KelfHeader, uint8_t *ContentKey);

/**
 * @brief encrypt the decrypted ContentKey for the Memory Card with the Session Key
 */
void meEncryptWithSessionKey(uint8_t *ContentKey);

/**
 * @brief decrypt the encrypted ContentKey from the Memory Card with the Session Key
 */
void meDecryptWithSessionKey(uint8_t *ContentKey);

#endif

