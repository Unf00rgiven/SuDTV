/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tkel/trunk/inc/tkel.h $
 *            $Rev: 7054 $
 *            $Date: 2013-02-12 14:16:21 +0100 (Tue, 12 Feb 2013) $
 *
 * PUBLIC
 *
 * DESCRIPTION: definition of the API of Thin Kernel Encapsulation Layer
 *
 *    LIPPA - December 2010 
 *              To implemente the mapping SmarDTV DAL OS -> TKEL, add 3 
 *              management task functions: TKEL_CreateTask_UnlimitStack,
 *              TKEL_ExitTask and TKEL_KillTask.
 *
 *****************************************************************************/

#ifndef _TKEL_H_
#define _TKEL_H_

#if defined(__cplusplus)
extern "C" {
#endif

/* TKEL error codes */
typedef enum
{
    TKEL_NO_ERR,    /* No error. */
    TKEL_TIMEOUT,   /* A timeout occurred. */
    TKEL_BAD_ARG,   /* Wrong argument value(s). */
    TKEL_NOT_DONE   /* Command not done. */
} TKEL_err;


TKEL_err TKEL_Init(void);

#if defined(__cplusplus)
}
#endif

#endif /* _TKEL_H_ */
