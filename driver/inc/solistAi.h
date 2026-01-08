/*****************************************************************************
 solistAi.h

 Copyright (C) 2024 ROHM Co., Ltd.
 All rights reserved.

 This software is provided "as is" and any expressed or implied
 warranties, including, but not limited to, the implied warranties of
 merchantability and fitness for a particular purpose are disclaimed.
 ROHM shall not be liable for any direct, indirect, consequential or
 incidental damages arising from using or modifying this software.
 You (customer) can modify and use this software in whole or part on
 your own responsibility, only for the purpose of developing the software
 for use with microcontroller manufactured by ROHM.

 History
    2024.07.31 Ver 1.0.0

******************************************************************************/
/**
 * @file    solistAi.h
 *
 * This module is 'Solist-AI' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Solist-AI
 *  @{
 *
 *  @brief Solist-AI Library and Solist-AI Scope
 *
 *  @file
 */

#ifndef SOLIST_AI_H__
#define SOLIST_AI_H__

#include <stdint.h>
#include "timer0_1.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
#define ODL_MAX_INST_NUM      ( 2 )                       /**< Number of AI models           */
#define ODL_MAX_INPUTS        ( 256 )                     /**< Number of input data          */
#define ODL_MAX_UNITS         ( (ODL_MAX_INPUTS) / 4 )    /**< Number of intermediate nodes  */
#define ODL_SEED_ALPHA        ( 1 )                       /**< Seed of weight Alpha          */

/*=== API parameters value ===*/
/* Activation functions */
#define	ODL_ACTV_LINEAR       ( 0 )                       /**< Linear f(x) = x               */
#define	ODL_ACTV_SIGMOID      ( 1 )                       /**< Sigmoid function              */
#define	ODL_ACTV_RELU         ( 2 )                       /**< ReLU (Ramp) function          */

/* Loss functions */
#define	ODL_LOSS_MAE          ( 0 )                       /**< Mean absolute error           */
#define	ODL_LOSS_MSE          ( 1 )                       /**< Mean squared error            */

/* typedef */
typedef int16_t fft_t;                                   /**< FFT Data Type                  */
typedef int16_t bfloat16;                                /**< bfloat16 Type                  */

/* FFT window function */
typedef enum FftWindow_t
{
	FFT_WINDOW_NONE,                                      /**< No window functitions         */
	FFT_WINDOW_HANN                                       /**< Hanning Window Function       */
}
FftWindow;

/**
 * @brief Parameters for On-Device Learning
 *
 */
typedef struct ODL_Parameters_t
{
    uint16_t inputSize;                                   /**< Number of input nodes         */
    uint16_t hiddenSize;                                  /**< Number of hidden nodes        */
    uint16_t outputSize;                                  /**< Number of output nodes        */

    bfloat16 forgettingFactor;                            /**< Forgetting factor (bfloat16)  */
    uint8_t activationFunction;                           /**< Activation function           */
    uint8_t lossFunction;                                 /**< Loss function                 */
    uint16_t seed;                                        /**< Seed of weight Alpha to generate random number */
}
ODL_Parameters;

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * @name Solist-AI Library related
 *
 * @{
 */

/* FFT functions prototypes */

/**
 * Build sequence for FFT.
 *
 * @param[in]   size             Input size. size must be less than or equal to 1024.
 * @param[in]   window           Window function.
 * @return      None
 */
void fft_Init(uint16_t size, FftWindow window);

/**
 * Start fast fourier fransform.
 *
 * @param[in]   x                Input buffer
 * @param[in]   size             FFT size
 * @return      None
 */
void fft_Start(fft_t x[], uint16_t size);

/**
 * Is FFT busy?
 *
 * @param       - 
 * @retval      0                Not busy
 * @retval      "other than 0"   Busy
 */
uint32_t fft_IsBusy(void);

/**
 * Get FFT result.
 *
 * @param[out]  yout             Output buffer
 * @param[in]   size             FFT size
 * @return      None
 */
void fft_GetResult(fft_t yout[], uint16_t size);


/* ODL functions prototypes */ 

/**
 * Initialize AI model
 *
 * @param[in]   instance         Index of AI model
 * @param[in]   parameters       Parameters
 * @return      None
 */
void ODL_Initialize(uint8_t instance, const ODL_Parameters* parameters);

/**
 *  Reset training
 *
 * @param[in]   instance         Index of AI model
 * @return      None
 */
void ODL_Reset(uint8_t instance);

/**
 * Convert fixed-point to bfloat16
 *
 * @param[out]  y                Output buffer (bfloat16)
 * @param[in]   x                Input buffer (Fixed point)
 * @param[in]   qFormat          Specify fixed-point format of x. For example, qFormat = 12 means Q12 or Q3.12. (5.5 is 0b101.1000 0000 0000 in Q12)
 * @param[in]   size             Size of buffer
 * @return      None
*/
void ODL_ToBfloat16(bfloat16* y, const int16_t* x, uint8_t qFormat, uint16_t size);

/**
 * Generate random numbers
 *
 * @param[out]  y                Output buffer (bfloat16)
 * @param[in]   size             Number of random number generated
 * @param[in]   seed             Seed of random number generator
 * @return      None
*/
void ODL_GenerateRandomNumber(bfloat16* y, uint16_t size, uint16_t seed);

/**
 * Start sequential training
 *
 * @param[in]   instance         Index of AI model
 * @param[in]   x                Buffer of input data
 * @param[in]   t                Buffer of expected data
 * @return      None
*/
void ODL_StartTrain(uint8_t instance, const bfloat16 *x, const bfloat16 *t);

/**
 * Start prediction
 *
 * @param[in]   instance         Index of AI model
 * @param[in]   x                Buffer of input data
 * @param[in]   t                Buffer of expected data. Specify x for unsupervised anomaly detection. It is optional for supervised learning to compute loss.
 * @return      None
*/
void ODL_StartPredict(uint8_t instance, const bfloat16 *x, const bfloat16 *t);

/**
 * Is ODL busy?
 *
 * @param       - 
 * @retval      0                Not busy
 * @retval      "other than 0"   Busy
 */
uint32_t ODL_IsBusy(void);

/**
 * Get ODL result.
 *
 * @param[in]   instance         Index of AI model
 * @param[out]  y                Output buffer
 * @return      None
 */
void ODL_GetResult(uint8_t instance, bfloat16 *y);

/**
 * Get loss.                                          <br>
 *        Learning error during learning.             <br>
 *        Inference error during inference.           <br>
 *
 * @param[in]   instance         Index of AI model
 *  @return     Value of loss
 */
bfloat16 ODL_GetLoss(uint8_t instance);

/**
 * Set weight data (Beta) after training.
 *
 * @param[in]   beta           Buffer of weight data to write
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Write size
 * @return      None
 */
void ODL_SetWeightBeta(const void* beta, uint8_t instance, uint32_t offset, uint16_t size);

/**
 * Set weight data (P) after training.
 *
 * @param[in]   p              Buffer of weight data to write
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Write size
 * @return      None
 */
void ODL_SetWeightP(const void* p, uint8_t instance, uint32_t offset, uint16_t size);

/**
 * Get weight data (Beta) after training.
 *
 * @param[out]  beta           Buffer of weight data to read
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Read size
 * @return      None
 */
void ODL_GetWeightBeta(void* beta, uint8_t instance, uint32_t offset, uint16_t size);

/**
 * Get weight data (P) after training.
 *
 * @param[out]  beta           Buffer of weight data to read
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Read size
 * @return      None
 */
void ODL_GetWeightP(void* p, uint8_t instance, uint32_t offset, uint16_t size);


/**
 * Get size of weight Beta.
 *
 * @param       - 
 * @return      Size
 */
uint32_t ODL_GetWeightBetaSize(void);

/**
 * Get size of weight P.
 *
 * @param       - 
 * @return      Size
 */
uint32_t ODL_GetWeightPSize(void);

/* OSL-UAD function prototypes */

/**
 *  Reset training
 *
 * @param[in]   instance         Index of AI model
 * @return      None
 */
void OSUAD_Reset(void);

/**
 * Initialize AI model
 *
 * @param[in]   parameters       Parameters
 * @param[in]   numberOfInstance Index of AI model
 * @return      None
 */
void OSUAD_Initialize(const ODL_Parameters* parameters, uint8_t numberOfInstance);

/**
 * Get loss.                                          <br>
 *        Learning error during learning.             <br>
 *        Inference error during inference.           <br>
 *
 * @param       -
 *  @return     Value of loss
 */
bfloat16 OSUAD_GetLoss(void);

/**
 * Start prediction
 *
 * @param[in]   instance         Index of AI model
 * @param[in]   x                Buffer of input data
 * @return      None
*/
#define OSUAD_StartPredict(instance, x) ODL_StartPredict(instance, x, x)

/**
 * Start sequential training
 *
 * @param[in]   instance         Index of AI model
 * @param[in]   x                Buffer of input data
 * @return      None
*/
#define OSUAD_StartTrain(instance, x) ODL_StartTrain(instance, x, x)

/**
 * Is OSUAD busy?
 *
 * @param       - 
 * @retval      0                Not busy
 * @retval      "other than 0"   Busy
 */
#define OSUAD_IsBusy() ODL_IsBusy()

/**
 * Set weight data (Beta) after training.
 *
 * @param[in]   beta           Buffer of weight data to write
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Write size
 * @return      None
 */
#define OSUAD_SetWeightBeta(beta, instance, offset, size) ODL_SetWeightBeta(beta, instance, offset, size)

/**
 * Set weight data (P) after training.
 *
 * @param[in]   p              Buffer of weight data to write
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Write size
 * @return      None
 */
#define OSUAD_SetWeightP(p, instance, offset, size) ODL_SetWeightP(p, instance, offset, size)

/**
 * Get weight data (Beta) after training.
 *
 * @param[out]  beta           Buffer of weight data to read
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Read size
 * @return      None
 */
#define OSUAD_GetWeightBeta(beta, instance, offset, size) ODL_GetWeightBeta(beta, instance, offset, size)

/**
 * Get weight data (P) after training.
 *
 * @param[out]  beta           Buffer of weight data to read
 * @param[in]   instance       Index of AI model
 * @param[in]   offset         Offset from the start address of the instance
 * @param[in]   size           Read size
 * @return      None
 */
#define OSUAD_GetWeightP(p, instance, offset, size) ODL_GetWeightP(p, instance, offset, size)

/** @}*/

/**
 * @name Solist-AI Scole related
 *
 * @{
 */

/**
 * Initialize Scope
 *
 * @param       -
 * @return      None
 */
void Scope_Initialize(void);

/**
 * Idle task to process request from Solist-Scope.
 *
 * @param       -
 * @return      None
 */
void Scope_Idle(void);

/**
 * Interrupt handler for SSIOF
 *
 * @param       -
 * @return      None
 */
void Scope_InterruptHandler(void);

/** @}*/

/**
 * @name Solist-AI Common
 *
 * @{
 */

/**
 * Clear AI-RAM
 *
 * @param       -
 * @return      None
 */
void ML_ACC_ClearRAM(void);

/**
 * Enable interrupt
 *
 * @param       -
 * @return      None
 */
void ML_ACC_EnableInterrupt(void);

/**
 * Disable interrupt
 *
 * @param       -
 * @return      None
 */
void ML_ACC_DisableInterrupt(void);

/**
 * Clear interrupt
 *
 * @param       -
 * @return      None
 */
void ML_ACC_ClearInterrupt(void);

/** @}*/
#endif /* SOLIST_AI_H__ */

/** @} */
/** @} */
