/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */
/*                                                                        */
/*    tx_port.h                                          ARMv8-A-SMP      */
/*                                                           6.1.10       */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains data type definitions that make the ThreadX      */
/*    real-time kernel function identically on a variety of different     */
/*    processor architectures.  For example, the size or number of bits   */
/*    in an "int" data type vary between microprocessor architectures and */
/*    even C compilers for the same microprocessor.  ThreadX does not     */
/*    directly use native C data types.  Instead, ThreadX creates its     */
/*    own special types that can be mapped to actual data types by this   */
/*    file to guarantee consistency in the interface and functionality.   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     William E. Lamie         Initial Version 6.1           */
/*  01-31-2022     Bhupendra Naphade        Modified comment(s),updated   */
/*                                            macro definition,           */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/

#ifndef TX_PORT_H
#define TX_PORT_H


/************* Define ThreadX SMP constants.  *************/

/* Define the ThreadX SMP maximum number of cores.  */

#ifndef TX_THREAD_SMP_MAX_CORES
#define TX_THREAD_SMP_MAX_CORES                 4
#endif


/* Define the ThreadX SMP core mask. */

#ifndef TX_THREAD_SMP_CORE_MASK
#define TX_THREAD_SMP_CORE_MASK                 0xF            /* Where bit 0 represents Core 0, bit 1 represents Core 1, etc.  */
#endif


/* Define INLINE_DECLARE to whitespace for ARM compiler.  */

#define INLINE_DECLARE


/* Define ThreadX SMP initialization macro.  */

#define TX_PORT_SPECIFIC_PRE_INITIALIZATION


/* Define ThreadX SMP pre-scheduler initialization.  */

#define TX_PORT_SPECIFIC_PRE_SCHEDULER_INITIALIZATION


/* Enable the inter-core interrupt logic.  */

#define TX_THREAD_SMP_INTER_CORE_INTERRUPT


/* Determine if there is customer-specific wakeup logic needed.  */

#ifdef TX_THREAD_SMP_WAKEUP_LOGIC

/* Include customer-specific wakeup code.  */

#include "tx_thread_smp_core_wakeup.h"
#else

#ifdef TX_THREAD_SMP_DEFAULT_WAKEUP_LOGIC

/* Default wakeup code.  */
#define TX_THREAD_SMP_WAKEUP_LOGIC
#define TX_THREAD_SMP_WAKEUP(i)                _tx_thread_smp_core_preempt(i)
#endif
#endif


/* Ensure that the in-line resume/suspend define is not allowed.  */

#ifdef TX_INLINE_THREAD_RESUME_SUSPEND
#undef TX_INLINE_THREAD_RESUME_SUSPEND
#endif


/************* End ThreadX SMP constants.  *************/


/* Determine if the optional ThreadX user define file should be used.  */

#ifdef TX_INCLUDE_USER_DEFINE_FILE


/* Yes, include the user defines in tx_user.h. The defines in this file may
   alternately be defined on the command line.  */

#include "tx_user.h"
#endif


/* Define compiler library include files.  */

#include <stdlib.h>
#include <string.h>


/* Define ThreadX basic types for this port.  */

#define VOID                                    void
typedef char                                    CHAR;
typedef unsigned char                           UCHAR;
typedef int                                     INT;
typedef unsigned int                            UINT;
typedef int                                     LONG;
typedef unsigned int                            ULONG;
typedef unsigned long long                      ULONG64;
typedef short                                   SHORT;
typedef unsigned short                          USHORT;
#define ULONG64_DEFINED

/* Override the alignment type to use 64-bit alignment and storage for pointers.  */

#define ALIGN_TYPE_DEFINED
typedef unsigned long long                      ALIGN_TYPE;


/* Override the free block marker for byte pools to be a 64-bit constant.   */

#define TX_BYTE_BLOCK_FREE                      ((ALIGN_TYPE) 0xFFFFEEEEFFFFEEEE)


/* Define the priority levels for ThreadX.  Legal values range
   from 32 to 1024 and MUST be evenly divisible by 32.  */

#ifndef TX_MAX_PRIORITIES
#define TX_MAX_PRIORITIES                       32
#endif


/* Define the minimum stack for a ThreadX thread on this processor. If the size supplied during
   thread creation is less than this value, the thread create call will return an error.  */

#ifndef TX_MINIMUM_STACK
#define TX_MINIMUM_STACK                        200         /* Minimum stack size for this port  */
#endif


/* Define the system timer thread's default stack size and priority.  These are only applicable
   if TX_TIMER_PROCESS_IN_ISR is not defined.  */

#ifndef TX_TIMER_THREAD_STACK_SIZE
#define TX_TIMER_THREAD_STACK_SIZE              4096        /* Default timer thread stack size  */
#endif

#ifndef TX_TIMER_THREAD_PRIORITY
#define TX_TIMER_THREAD_PRIORITY                0           /* Default timer thread priority    */
#endif


/* Define various constants for the ThreadX ARM port.  */

#define TX_INT_DISABLE                          0xC0        /* Disable IRQ & FIQ interrupts     */
#define TX_INT_ENABLE                           0x00        /* Enable IRQ & FIQ interrupts            */


/* Define the clock source for trace event entry time stamp. The following two item are port specific.
   For example, if the time source is at the address 0x0a800024 and is 16-bits in size, the clock
   source constants would be:

#define TX_TRACE_TIME_SOURCE                    *((ULONG *) 0x0a800024)
#define TX_TRACE_TIME_MASK                      0x0000FFFFUL

*/

#ifndef TX_MISRA_ENABLE
#ifndef TX_TRACE_TIME_SOURCE
#define TX_TRACE_TIME_SOURCE                    _tx_thread_smp_time_get()
#endif
#else
#ifndef TX_TRACE_TIME_SOURCE
ULONG   _tx_misra_time_stamp_get(VOID);
#define TX_TRACE_TIME_SOURCE                    _tx_misra_time_stamp_get()
#endif
#endif
#ifndef TX_TRACE_TIME_MASK
#define TX_TRACE_TIME_MASK                      0xFFFFFFFFUL
#endif


/* Define the port specific options for the _tx_build_options variable. This variable indicates
   how the ThreadX library was built.  */

#ifdef TX_ENABLE_FIQ_SUPPORT
#define TX_FIQ_ENABLED                          1
#else
#define TX_FIQ_ENABLED                          0
#endif

#ifdef TX_ENABLE_IRQ_NESTING
#define TX_IRQ_NESTING_ENABLED                  2
#else
#define TX_IRQ_NESTING_ENABLED                  0
#endif

#ifdef TX_ENABLE_FIQ_NESTING
#define TX_FIQ_NESTING_ENABLED                  4
#else
#define TX_FIQ_NESTING_ENABLED                  0
#endif

#define TX_PORT_SPECIFIC_BUILD_OPTIONS          (TX_FIQ_ENABLED | TX_IRQ_NESTING_ENABLED | TX_FIQ_NESTING_ENABLED)


/* Define the in-line initialization constant so that modules with in-line
   initialization capabilities can prevent their initialization from being
   a function call.  */

#ifdef TX_MISRA_ENABLE
#define TX_DISABLE_INLINE
#else
#define TX_INLINE_INITIALIZATION
#endif


/* Determine whether or not stack checking is enabled. By default, ThreadX stack checking is
   disabled. When the following is defined, ThreadX thread stack checking is enabled.  If stack
   checking is enabled (TX_ENABLE_STACK_CHECKING is defined), the TX_DISABLE_STACK_FILLING
   define is negated, thereby forcing the stack fill which is necessary for the stack checking
   logic.  */

#ifndef TX_MISRA_ENABLE
#ifdef TX_ENABLE_STACK_CHECKING
#undef TX_DISABLE_STACK_FILLING
#endif
#endif


/* Define the TX_THREAD control block extensions for this port. The main reason
   for the multiple macros is so that backward compatibility can be maintained with
   existing ThreadX kernel awareness modules.  */

#define TX_THREAD_EXTENSION_0
#define TX_THREAD_EXTENSION_1
#define TX_THREAD_EXTENSION_2                  ULONG        tx_thread_fp_enable;
#define TX_THREAD_EXTENSION_3                  VOID         *tx_thread_extension_ptr;


/* Define the port extensions of the remaining ThreadX objects.  */

#define TX_BLOCK_POOL_EXTENSION
#define TX_BYTE_POOL_EXTENSION
#define TX_EVENT_FLAGS_GROUP_EXTENSION
#define TX_MUTEX_EXTENSION
#define TX_QUEUE_EXTENSION
#define TX_SEMAPHORE_EXTENSION
#define TX_TIMER_EXTENSION


/* Define the user extension field of the thread control block.  Nothing
   additional is needed for this port so it is defined as white space.  */

#ifndef TX_THREAD_USER_EXTENSION
#define TX_THREAD_USER_EXTENSION
#endif


/* Define the macros for processing extensions in tx_thread_create, tx_thread_delete,
   tx_thread_shell_entry, and tx_thread_terminate.  */


#define TX_THREAD_CREATE_EXTENSION(thread_ptr)
#define TX_THREAD_DELETE_EXTENSION(thread_ptr)
#define TX_THREAD_COMPLETED_EXTENSION(thread_ptr)
#define TX_THREAD_TERMINATED_EXTENSION(thread_ptr)


/* Define the ThreadX object creation extensions for the remaining objects.  */

#define TX_BLOCK_POOL_CREATE_EXTENSION(pool_ptr)
#define TX_BYTE_POOL_CREATE_EXTENSION(pool_ptr)
#define TX_EVENT_FLAGS_GROUP_CREATE_EXTENSION(group_ptr)
#define TX_MUTEX_CREATE_EXTENSION(mutex_ptr)
#define TX_QUEUE_CREATE_EXTENSION(queue_ptr)
#define TX_SEMAPHORE_CREATE_EXTENSION(semaphore_ptr)
#define TX_TIMER_CREATE_EXTENSION(timer_ptr)


/* Define the ThreadX object deletion extensions for the remaining objects.  */

#define TX_BLOCK_POOL_DELETE_EXTENSION(pool_ptr)
#define TX_BYTE_POOL_DELETE_EXTENSION(pool_ptr)
#define TX_EVENT_FLAGS_GROUP_DELETE_EXTENSION(group_ptr)
#define TX_MUTEX_DELETE_EXTENSION(mutex_ptr)
#define TX_QUEUE_DELETE_EXTENSION(queue_ptr)
#define TX_SEMAPHORE_DELETE_EXTENSION(semaphore_ptr)
#define TX_TIMER_DELETE_EXTENSION(timer_ptr)


/* Determine if the ARM architecture has the CLZ instruction. This is available on
   architectures v5 and above. If available, redefine the macro for calculating the
   lowest bit set.  */

#ifndef TX_DISABLE_INLINE

#define TX_LOWEST_SET_BIT_CALCULATE(m, b)       b =  (UINT) __builtin_ctz((unsigned int) m);

#endif


/* Define the internal timer extension to also hold the thread pointer such that _tx_thread_timeout
   can figure out what thread timeout to process.  */

#define TX_TIMER_INTERNAL_EXTENSION             VOID    *tx_timer_internal_extension_ptr;


/* Define the thread timeout setup logic in _tx_thread_create.  */

#define TX_THREAD_CREATE_TIMEOUT_SETUP(t)    (t) -> tx_thread_timer.tx_timer_internal_timeout_function =    &(_tx_thread_timeout);            \
                                             (t) -> tx_thread_timer.tx_timer_internal_timeout_param =       0;                                \
                                             (t) -> tx_thread_timer.tx_timer_internal_extension_ptr =       (VOID *) (t);


/* Define the thread timeout pointer setup in _tx_thread_timeout.  */

#define TX_THREAD_TIMEOUT_POINTER_SETUP(t)   (t) =  (TX_THREAD *) _tx_timer_expired_timer_ptr -> tx_timer_internal_extension_ptr;


/************* Define ThreadX SMP data types and function prototypes.  *************/

struct TX_THREAD_STRUCT;


/* Define the ThreadX SMP protection structure.   */

typedef struct TX_THREAD_SMP_PROTECT_STRUCT
{
    ULONG           tx_thread_smp_protect_in_force;
    ULONG           tx_thread_smp_protect_core;
    ULONG           tx_thread_smp_protect_count;
    ULONG           tx_thread_smp_protect_pad_0;
    ULONG           tx_thread_smp_protect_pad_1;
    ULONG           tx_thread_smp_protect_pad_2;
    ULONG           tx_thread_smp_protect_pad_3;
} TX_THREAD_SMP_PROTECT;


/* Define ThreadX interrupt lockout and restore macros for protection on
   access of critical kernel information.  The restore interrupt macro must
   restore the interrupt posture of the running thread prior to the value
   present prior to the disable macro.  In most cases, the save area macro
   is used to define a local function save area for the disable and restore
   macros.  */

#define TX_INTERRUPT_SAVE_AREA                  UINT interrupt_save;

#define TX_DISABLE                              interrupt_save =  _tx_thread_smp_protect();
#define TX_RESTORE                              _tx_thread_smp_unprotect(interrupt_save);

/************* End ThreadX SMP data type and function prototype definitions.  *************/


/* Define the interrupt lockout macros for each ThreadX object.  */

#define TX_BLOCK_POOL_DISABLE                   TX_DISABLE
#define TX_BYTE_POOL_DISABLE                    TX_DISABLE
#define TX_EVENT_FLAGS_GROUP_DISABLE            TX_DISABLE
#define TX_MUTEX_DISABLE                        TX_DISABLE
#define TX_QUEUE_DISABLE                        TX_DISABLE
#define TX_SEMAPHORE_DISABLE                    TX_DISABLE


/* Define FP extension for ARMv8.  Each is assumed to be called in the context of the executing thread.  */

#ifndef TX_SOURCE_CODE
#define tx_thread_fp_enable                     _tx_thread_fp_enable
#define tx_thread_fp_disable                    _tx_thread_fp_disable
#endif

VOID    tx_thread_fp_enable(VOID);
VOID    tx_thread_fp_disable(VOID);


/* Define the version ID of ThreadX.  This may be utilized by the application.  */

#ifdef TX_THREAD_INIT
CHAR                            _tx_version_id[] =
                                    "Copyright (c) 2024 Microsoft Corporation.  *  ThreadX ARMv8-A-SMP Version 6.4.2 *";
#else
extern  CHAR                    _tx_version_id[];
#endif


#endif
