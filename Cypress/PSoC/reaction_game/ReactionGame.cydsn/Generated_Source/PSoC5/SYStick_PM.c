/*******************************************************************************
* File Name: SYStick_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "SYStick.h"

static SYStick_backupStruct SYStick_backup;


/*******************************************************************************
* Function Name: SYStick_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  SYStick_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void SYStick_SaveConfig(void) 
{
    #if (!SYStick_UsingFixedFunction)
        SYStick_backup.TimerUdb = SYStick_ReadCounter();
        SYStick_backup.InterruptMaskValue = SYStick_STATUS_MASK;
        #if (SYStick_UsingHWCaptureCounter)
            SYStick_backup.TimerCaptureCounter = SYStick_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!SYStick_UDB_CONTROL_REG_REMOVED)
            SYStick_backup.TimerControlRegister = SYStick_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: SYStick_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  SYStick_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SYStick_RestoreConfig(void) 
{   
    #if (!SYStick_UsingFixedFunction)

        SYStick_WriteCounter(SYStick_backup.TimerUdb);
        SYStick_STATUS_MASK =SYStick_backup.InterruptMaskValue;
        #if (SYStick_UsingHWCaptureCounter)
            SYStick_SetCaptureCount(SYStick_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!SYStick_UDB_CONTROL_REG_REMOVED)
            SYStick_WriteControlRegister(SYStick_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: SYStick_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  SYStick_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void SYStick_Sleep(void) 
{
    #if(!SYStick_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(SYStick_CTRL_ENABLE == (SYStick_CONTROL & SYStick_CTRL_ENABLE))
        {
            /* Timer is enabled */
            SYStick_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            SYStick_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    SYStick_Stop();
    SYStick_SaveConfig();
}


/*******************************************************************************
* Function Name: SYStick_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  SYStick_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SYStick_Wakeup(void) 
{
    SYStick_RestoreConfig();
    #if(!SYStick_UDB_CONTROL_REG_REMOVED)
        if(SYStick_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                SYStick_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
