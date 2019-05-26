/*******************************************************************************
* File Name: S7D_ControlReg_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "S7D_ControlReg.h"

/* Check for removal by optimization */
#if !defined(S7D_ControlReg_Sync_ctrl_reg__REMOVED)

static S7D_ControlReg_BACKUP_STRUCT  S7D_ControlReg_backup = {0u};

    
/*******************************************************************************
* Function Name: S7D_ControlReg_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void S7D_ControlReg_SaveConfig(void) 
{
    S7D_ControlReg_backup.controlState = S7D_ControlReg_Control;
}


/*******************************************************************************
* Function Name: S7D_ControlReg_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void S7D_ControlReg_RestoreConfig(void) 
{
     S7D_ControlReg_Control = S7D_ControlReg_backup.controlState;
}


/*******************************************************************************
* Function Name: S7D_ControlReg_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void S7D_ControlReg_Sleep(void) 
{
    S7D_ControlReg_SaveConfig();
}


/*******************************************************************************
* Function Name: S7D_ControlReg_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void S7D_ControlReg_Wakeup(void)  
{
    S7D_ControlReg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
