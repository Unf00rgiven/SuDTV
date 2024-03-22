#ifndef _TDAL_ANDROID_DRM_H_
#define _TDAL_ANDROID_DRM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------*/
/*           Types definition        */
/*------------------------------------------*/

typedef enum {
    eTDAL_ANDROID_DRM_NO_ERROR = 0, eTDAL_ANDROID_DRM_ERROR
} tTDAL_ANDROID_DRM_Error;

tTDAL_ANDROID_DRM_Error TDAL_ANDROID_DRM_SetLicenseServerUrl(
        const char* licenseServerUrl);

#ifdef __cplusplus
}
#endif
#endif /* _TDAL_ANDROID_DRM_H_ */
