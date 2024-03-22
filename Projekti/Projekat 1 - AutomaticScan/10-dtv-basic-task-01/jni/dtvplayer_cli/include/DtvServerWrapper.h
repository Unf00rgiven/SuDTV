#ifndef DTV_SERVER_WRAPPER_H
#define DTV_SERVER_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*tDtvPlayer_Binder_WindowCb)(void*);
	
void DtvPlayer_Binder_Init(tDtvPlayer_Binder_WindowCb cb);

#ifdef __cplusplus
}
#endif

#endif // DTV_SERVER_WRAPPER_H
