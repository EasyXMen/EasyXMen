#ifndef XCPONCAN_CBK_H
#define XCPONCAN_CBK_H

/*=======[I N C L U D E S]====================================================*/
#include "Xcp_GenericTypes.h"
#include "Xcp_Cfg.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/******************************************************************************/
/*
 * @brief               <Xcp callback Function>
 *
 * <The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PD.>
 * . Service ID   :       <(0x40)> Sync/Async   :       <Synchronous> Reentrancy           <Reentrant for different
 * PduIds. Non reentrant for the same PduId.>
 * @param[in]           <TxPduId>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
extern FUNC(void, XCP_CANIFTXINDICATION_CODE) Xcp_CanIfTxConfirmation(PduIdType TxPduId);

/******************************************************************************/
/*
 * @brief               <Xcp callback Function>
 *
 * <Indication of a received PDU from a lower layer communication interface module.> .
 * Service ID   :       <(0x42)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant for different PduIds. Non reentrant for the same PduId.>
 * @param[in]           <RxPduId>/<PduInfoPtr>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
extern FUNC(void, XCP_CANIFRXINDICATION_CODE)
    Xcp_CanIfRxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr);

#endif
