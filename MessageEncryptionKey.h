/*
 * @Author       : wilbur
 * @Date         : 2024-03-29 12:42:10
 * @LastEditTime : 2024-03-29 21:12:48
 * @LastEditors  : wilbur
 * @Description  : Please enter file description
 * Copyright (c) 2024, All Rights Reserved.
 */
/*
 *  Copyright (c) 2023 Project CHIP Authors.
 * 
 *  Use of this source code is governed by a BSD-style
 *  license that can be found in the LICENSE file or at
 *  https://opensource.org/license/bsd-3-clause
 * 
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MESSAGEENCRYPTIONKEY_H_
#define MESSAGEENCRYPTIONKEY_H_

class MessageEncryptionKey
{
public:
    uint16_t keyId;
    uint8_t sessionType;
    unsigned char *dataEncKey;
    guint dataEncKeyLen;
    uint64_t srcNodeId;
    MessageEncryptionKey *nextKey;

    bool IsSameKey(const MessageEncryptionKey& otherKey) const;
};

enum {
    kDataEncKeyLength_AES128CCM = 16
};

class MessageEncryptionKeyTable
{
public:
    static void Init();
    static const MessageEncryptionKey *FindKeysById(uint16_t keyId);
    static const MessageEncryptionKey *AddKey(const MessageEncryptionKey& keyData);

private:
    MessageEncryptionKeyTable(); // Not defined
    ~MessageEncryptionKeyTable(); // Not defined

    static wmem_tree_t *sKeys;
};



#endif /* MESSAGEENCRYPTIONKEY_H_ */
