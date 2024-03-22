/******************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TSIN  -  TS INjector
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/dev/sources/chal/core/tdal/trunk/inc/tdal_tsin.h $
 *            $Rev: 441 $
 *            $Date: 2011-09-28 13:56:52 +0200 (Wed, 28 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION - declaration of the API of TDAL_TSIN
 *
 *****************************************************************************/

#ifndef _TDAL_TSIN_H_
#define _TDAL_TSIN_H_

/*******************************************************************************
 *   Includes
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 *   Defines
 ******************************************************************************/

/******************************************************************************
 *   Macros
 ******************************************************************************/

/******************************************************************************
 *   Typedefs
 ******************************************************************************/

/* TDAL_TSIN error codes */
typedef enum {
    eTDAL_TSIN_NO_ERROR = 0,        /* no error. */
    eTDAL_TSIN_ERROR_BAD_ARG,       /* wrong argument value. */
    eTDAL_TSIN_ERROR_NO_MEMORY,     /* not enough memory. */
    eTDAL_TSIN_ERROR_IN_DRIVER,     /* error in the driver */
    eTDAL_TSIN_ERROR_CONFLICT,	    /* error in access to a TS source or a
                                       demux */
    eTDAL_TSIN_WARNING,             /* an error occured but it could be OK */
    eTDAL_TSIN_WARNING_MEMORY,      /* there is not enough memory but operation
                                       may be partially performed  */
    eTDAL_TSIN_ERROR_CAPABILITIES,	/* not handled in this TDAL TSIN
                                       implementation */
    eTDAL_TSIN_ERROR_NOT_DONE       /* command not done. */
} tTDAL_TSIN_Error;


typedef    void *    tTDAL_TSIN_Handle;



/* callback function called when a buffer is full */
typedef void (* tTDAL_TSIN_BufferInjectedCallbackFct )(
    tTDAL_TSIN_Handle       handle,
    uint8_t *               p_address,
    uint32_t                length
);


/* enumeration of different sources for TS data */
typedef enum TDAL_TSIN_Source_e {
    eTDAL_TSIN_SOURCE_TUNER1,       /* Tuner front-end 1 */
    eTDAL_TSIN_SOURCE_TUNER2,       /* Tuner front-end 2 */
    eTDAL_TSIN_SOURCE_IEEE1394,     /* A IEEE1394 source */
    eTDAL_TSIN_SOURCE_MEMORY        /* TS Data come from memory */
} tTDAL_TSIN_Source;


/* enumeration of different demux destination for TS data */
typedef enum TDAL_TSIN_Demux_e {
    eTDAL_TSIN_DEMUX1,    /* Demux #1 */
    eTDAL_TSIN_DEMUX2,    /* Demux #2 */
    eTDAL_TSIN_DEMUX3     /* Demux #3 */
} tTDAL_TSIN_Demux;

/* structure containing parameters for opening a new injection */
typedef struct TDAL_TSIN_OpenParam_s {
    tTDAL_TSIN_Source	                source ;
    tTDAL_TSIN_Demux	                demux ;
    tTDAL_TSIN_BufferInjectedCallbackFct  	buffer_injected_callback ;
} tTDAL_TSIN_OpenParam;



/*******************************************************************************
 *   Variables Declarations (IMPORT)
 ******************************************************************************/
/* none */

/*******************************************************************************
 *   Functions Declarations (GLOBAL)
 ******************************************************************************/
/*   see tdal_tsin.c for detailled comments for these functions   */

            /* Initializes the TDAL_TSIN driver */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Init( void );

            /* Terminates the TDAL_TSIN driver */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Term( void );


            /* Initializes an injection */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Open(
                            tTDAL_TSIN_Handle *      p_handle,
                            tTDAL_TSIN_OpenParam *   p_oprm
                          );

            /* Closes an injection */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Close(
                            tTDAL_TSIN_Handle   handle
                          );

            /* Allocate buffers */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_AllocateBuffers(
                            uint8_t *           p_number,
                            uint32_t            buffer_length,
                            uint8_t *           p_addresses[]
                          );

            /* Deallocate buffers */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_DeallocateBuffers(
                            uint8_t             p_number,
                            uint8_t *           p_addresses[]
                          );

            /* Start injecting data */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Start(
                            tTDAL_TSIN_Handle   handle
                          );

            /* Stop the injection of data */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Stop(
                            tTDAL_TSIN_Handle   handle
                          );

            /* Flush data */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_Flush(
                            tTDAL_TSIN_Handle   handle
                          );

            /* Notify the TSIN driver that buffer
               content should be injected */
GLOBAL  tTDAL_TSIN_Error  TDAL_TSIN_InjectData(
                            tTDAL_TSIN_Handle   handle,
                            uint8_t *           address,
                            uint32_t            length
                          );


#ifdef __cplusplus
}
#endif

#endif /* _TDAL_TSIN_H_ */
