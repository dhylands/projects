/****************************************************************************/
/**
*
*  @file    SRecMem.h
*
*  @brief   Structure definitions used by the SRec2C program.
*
****************************************************************************/
/**
*  @defgroup   SRecMem   Structures used by the SRec2C program.
*
****************************************************************************/

#if !defined( SRECMEM_H )
#define SRECMEM_H           ///< Include Guard

// ---- Include Files ------------------------------------------------------

/**
 * @addtogroup SRecMem
 * @{
 */

// ---- Constants and Types ------------------------------------------------

/**
 * Describes a single memory block
 */

typedef struct
{
   unsigned    m_loadAddr; ///< Address that data block should be loaded at.
   const void *m_data;     ///< Pointer to data.
   unsigned    m_dataLen;  ///< Number of bytes of data.

} SRecMemBlock;

/**
 * Describes a collection of memory blocks.
 */

typedef struct
{
   unsigned             m_startAddr;   ///< Start address for executable.
   unsigned             m_numBlock;    ///< Number of memory segments.
   const SRecMemBlock  *m_block;       ///< Array of memory segments.

} SRecMem;

// ---- Variable Externs ---------------------------------------------------
// ---- Function Prototypes ------------------------------------------------

/// @}

#endif // SRECMEM_H

