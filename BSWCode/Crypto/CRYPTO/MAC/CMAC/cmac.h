#ifndef CMAC_H
#define CMAC_H
#if (CRYPTO_ALGORITHM_CMAC == STD_ON)
#include "Crypto_Types.h"

FUNC(Std_ReturnType, CMAC_CODE)
Crypto_GernerateMAC(uint8 MAC[16], uint8 Key[16], uint8 IVKey[16], uint8* add, uint8 inputlength);
FUNC(Std_ReturnType, CMAC_CODE)
Crypto_VerifyMAC(uint8* Key, uint8* IVKey, uint8* add, uint8 inputlength, uint8 MAC_Result[16], uint32 Verifylength);

#endif /* CRYPTO_ALGORITHM_CMAC == STD_ON */
#endif /* CMAC_H */
