/*******************************************************************************
* File Name: S7D_SegmentG.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_S7D_SegmentG_H) /* Pins S7D_SegmentG_H */
#define CY_PINS_S7D_SegmentG_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "S7D_SegmentG_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 S7D_SegmentG__PORT == 15 && ((S7D_SegmentG__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    S7D_SegmentG_Write(uint8 value);
void    S7D_SegmentG_SetDriveMode(uint8 mode);
uint8   S7D_SegmentG_ReadDataReg(void);
uint8   S7D_SegmentG_Read(void);
void    S7D_SegmentG_SetInterruptMode(uint16 position, uint16 mode);
uint8   S7D_SegmentG_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the S7D_SegmentG_SetDriveMode() function.
     *  @{
     */
        #define S7D_SegmentG_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define S7D_SegmentG_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define S7D_SegmentG_DM_RES_UP          PIN_DM_RES_UP
        #define S7D_SegmentG_DM_RES_DWN         PIN_DM_RES_DWN
        #define S7D_SegmentG_DM_OD_LO           PIN_DM_OD_LO
        #define S7D_SegmentG_DM_OD_HI           PIN_DM_OD_HI
        #define S7D_SegmentG_DM_STRONG          PIN_DM_STRONG
        #define S7D_SegmentG_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define S7D_SegmentG_MASK               S7D_SegmentG__MASK
#define S7D_SegmentG_SHIFT              S7D_SegmentG__SHIFT
#define S7D_SegmentG_WIDTH              1u

/* Interrupt constants */
#if defined(S7D_SegmentG__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in S7D_SegmentG_SetInterruptMode() function.
     *  @{
     */
        #define S7D_SegmentG_INTR_NONE      (uint16)(0x0000u)
        #define S7D_SegmentG_INTR_RISING    (uint16)(0x0001u)
        #define S7D_SegmentG_INTR_FALLING   (uint16)(0x0002u)
        #define S7D_SegmentG_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define S7D_SegmentG_INTR_MASK      (0x01u) 
#endif /* (S7D_SegmentG__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define S7D_SegmentG_PS                     (* (reg8 *) S7D_SegmentG__PS)
/* Data Register */
#define S7D_SegmentG_DR                     (* (reg8 *) S7D_SegmentG__DR)
/* Port Number */
#define S7D_SegmentG_PRT_NUM                (* (reg8 *) S7D_SegmentG__PRT) 
/* Connect to Analog Globals */                                                  
#define S7D_SegmentG_AG                     (* (reg8 *) S7D_SegmentG__AG)                       
/* Analog MUX bux enable */
#define S7D_SegmentG_AMUX                   (* (reg8 *) S7D_SegmentG__AMUX) 
/* Bidirectional Enable */                                                        
#define S7D_SegmentG_BIE                    (* (reg8 *) S7D_SegmentG__BIE)
/* Bit-mask for Aliased Register Access */
#define S7D_SegmentG_BIT_MASK               (* (reg8 *) S7D_SegmentG__BIT_MASK)
/* Bypass Enable */
#define S7D_SegmentG_BYP                    (* (reg8 *) S7D_SegmentG__BYP)
/* Port wide control signals */                                                   
#define S7D_SegmentG_CTL                    (* (reg8 *) S7D_SegmentG__CTL)
/* Drive Modes */
#define S7D_SegmentG_DM0                    (* (reg8 *) S7D_SegmentG__DM0) 
#define S7D_SegmentG_DM1                    (* (reg8 *) S7D_SegmentG__DM1)
#define S7D_SegmentG_DM2                    (* (reg8 *) S7D_SegmentG__DM2) 
/* Input Buffer Disable Override */
#define S7D_SegmentG_INP_DIS                (* (reg8 *) S7D_SegmentG__INP_DIS)
/* LCD Common or Segment Drive */
#define S7D_SegmentG_LCD_COM_SEG            (* (reg8 *) S7D_SegmentG__LCD_COM_SEG)
/* Enable Segment LCD */
#define S7D_SegmentG_LCD_EN                 (* (reg8 *) S7D_SegmentG__LCD_EN)
/* Slew Rate Control */
#define S7D_SegmentG_SLW                    (* (reg8 *) S7D_SegmentG__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define S7D_SegmentG_PRTDSI__CAPS_SEL       (* (reg8 *) S7D_SegmentG__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define S7D_SegmentG_PRTDSI__DBL_SYNC_IN    (* (reg8 *) S7D_SegmentG__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define S7D_SegmentG_PRTDSI__OE_SEL0        (* (reg8 *) S7D_SegmentG__PRTDSI__OE_SEL0) 
#define S7D_SegmentG_PRTDSI__OE_SEL1        (* (reg8 *) S7D_SegmentG__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define S7D_SegmentG_PRTDSI__OUT_SEL0       (* (reg8 *) S7D_SegmentG__PRTDSI__OUT_SEL0) 
#define S7D_SegmentG_PRTDSI__OUT_SEL1       (* (reg8 *) S7D_SegmentG__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define S7D_SegmentG_PRTDSI__SYNC_OUT       (* (reg8 *) S7D_SegmentG__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(S7D_SegmentG__SIO_CFG)
    #define S7D_SegmentG_SIO_HYST_EN        (* (reg8 *) S7D_SegmentG__SIO_HYST_EN)
    #define S7D_SegmentG_SIO_REG_HIFREQ     (* (reg8 *) S7D_SegmentG__SIO_REG_HIFREQ)
    #define S7D_SegmentG_SIO_CFG            (* (reg8 *) S7D_SegmentG__SIO_CFG)
    #define S7D_SegmentG_SIO_DIFF           (* (reg8 *) S7D_SegmentG__SIO_DIFF)
#endif /* (S7D_SegmentG__SIO_CFG) */

/* Interrupt Registers */
#if defined(S7D_SegmentG__INTSTAT)
    #define S7D_SegmentG_INTSTAT            (* (reg8 *) S7D_SegmentG__INTSTAT)
    #define S7D_SegmentG_SNAP               (* (reg8 *) S7D_SegmentG__SNAP)
    
	#define S7D_SegmentG_0_INTTYPE_REG 		(* (reg8 *) S7D_SegmentG__0__INTTYPE)
#endif /* (S7D_SegmentG__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_S7D_SegmentG_H */


/* [] END OF FILE */
