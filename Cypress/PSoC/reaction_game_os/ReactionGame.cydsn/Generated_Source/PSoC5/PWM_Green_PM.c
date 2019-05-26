/*******************************************************************************
* File Name: PWM_Green_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_Green.h"

static PWM_Green_backupStruct PWM_Green_backup;


/*******************************************************************************
* Function Name: PWM_Green_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Green_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Green_SaveConfig(void) 
{

    #if(!PWM_Green_UsingFixedFunction)
        #if(!PWM_Green_PWMModeIsCenterAligned)
            PWM_Green_backup.PWMPeriod = PWM_Green_ReadPeriod();
        #endif /* (!PWM_Green_PWMModeIsCenterAligned) */
        PWM_Green_backup.PWMUdb = PWM_Green_ReadCounter();
        #if (PWM_Green_UseStatus)
            PWM_Green_backup.InterruptMaskValue = PWM_Green_STATUS_MASK;
        #endif /* (PWM_Green_UseStatus) */

        #if(PWM_Green_DeadBandMode == PWM_Green__B_PWM__DBM_256_CLOCKS || \
            PWM_Green_DeadBandMode == PWM_Green__B_PWM__DBM_2_4_CLOCKS)
            PWM_Green_backup.PWMdeadBandValue = PWM_Green_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Green_KillModeMinTime)
             PWM_Green_backup.PWMKillCounterPeriod = PWM_Green_ReadKillTime();
        #endif /* (PWM_Green_KillModeMinTime) */

        #if(PWM_Green_UseControl)
            PWM_Green_backup.PWMControlRegister = PWM_Green_ReadControlRegister();
        #endif /* (PWM_Green_UseControl) */
    #endif  /* (!PWM_Green_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Green_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Green_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Green_RestoreConfig(void) 
{
        #if(!PWM_Green_UsingFixedFunction)
            #if(!PWM_Green_PWMModeIsCenterAligned)
                PWM_Green_WritePeriod(PWM_Green_backup.PWMPeriod);
            #endif /* (!PWM_Green_PWMModeIsCenterAligned) */

            PWM_Green_WriteCounter(PWM_Green_backup.PWMUdb);

            #if (PWM_Green_UseStatus)
                PWM_Green_STATUS_MASK = PWM_Green_backup.InterruptMaskValue;
            #endif /* (PWM_Green_UseStatus) */

            #if(PWM_Green_DeadBandMode == PWM_Green__B_PWM__DBM_256_CLOCKS || \
                PWM_Green_DeadBandMode == PWM_Green__B_PWM__DBM_2_4_CLOCKS)
                PWM_Green_WriteDeadTime(PWM_Green_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Green_KillModeMinTime)
                PWM_Green_WriteKillTime(PWM_Green_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Green_KillModeMinTime) */

            #if(PWM_Green_UseControl)
                PWM_Green_WriteControlRegister(PWM_Green_backup.PWMControlRegister);
            #endif /* (PWM_Green_UseControl) */
        #endif  /* (!PWM_Green_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Green_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Green_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Green_Sleep(void) 
{
    #if(PWM_Green_UseControl)
        if(PWM_Green_CTRL_ENABLE == (PWM_Green_CONTROL & PWM_Green_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Green_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Green_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Green_UseControl) */

    /* Stop component */
    PWM_Green_Stop();

    /* Save registers configuration */
    PWM_Green_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Green_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Green_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Green_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Green_RestoreConfig();

    if(PWM_Green_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Green_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
