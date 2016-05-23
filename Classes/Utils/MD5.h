/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)
 
 based on:
 
 md5.h and md5.c
 reference implementation of RFC 1321
 
 Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 rights reserved.
 
 License to copy and use this software is granted provided that it
 is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 Algorithm" in all material mentioning or referencing this software
 or this function.
 
 License is also granted to make and use derivative works provided
 that such works are identified as "derived from the RSA Data
 Security, Inc. MD5 Message-Digest Algorithm" in all material
 mentioning or referencing the derived work.
 
 RSA Data Security, Inc. makes no representations concerning either
 the merchantability of this software or the suitability of this
 software for any particular purpose. It is provided "as is"
 without express or implied warranty of any kind.
 
 These notices must be retained in any copies of any part of this
 documentation and/or software.
 
 */

#ifndef _UTILS_MD5_
#define _UTILS_MD5_

#include "HashAlgorithm.h"
#include <string>
#include <vector>
#include <iostream>

namespace utils
{
    class MD5 : public HashAlgorithm
    {
    private:
        unsigned char _buffer[64];
        unsigned int _count[2];
        unsigned int _currentDigest[4];
        unsigned char _digest[16];
        
    public:
        MD5();
        
    protected:
        virtual void _hashCore(const char* input, int length) override;
        virtual char* _hashFinal() override;
        
    private:
        void _transform(const unsigned char block[64]);
        static void _decode(unsigned int output[], const unsigned char input[], int len);
        static void _encode(unsigned char output[], const unsigned int input[], int len);
        
        static inline unsigned int F(unsigned int x, unsigned int y, unsigned int z);
        static inline unsigned int G(unsigned int x, unsigned int y, unsigned int z);
        static inline unsigned int H(unsigned int x, unsigned int y, unsigned int z);
        static inline unsigned int I(unsigned int x, unsigned int y, unsigned int z);
        static inline unsigned int rotateLeft(unsigned int x, int n);
        static inline void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
        static inline void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
        static inline void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
        static inline void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
    };
}

#endif
