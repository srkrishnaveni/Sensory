// Copyright (C)2000-2015 Sensory Inc.
// Sensory Confidential


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include "thf.h"
#include <android/log.h>
#include <time.h>

#define SAVELOG 0 // Write results to SD Card
#define SSDEBUG 0

#if SAVELOG
#define PHRASESPOT_DELAY   (300) // NOTE: does not use sfsResetRecog; 0 will cause multiple triggers
#else
#define PHRASESPOT_DELAY   (PHRASESPOT_DELAY_ASAP)  // NOTE: this needs to become 0xFFFF for decoy/smart delay triggers
#endif

#define PHRASESPOT_LONGPEN   (0.02f)
#define STRSZ   (512)
#define NBEST   (1)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "SensoryTrueHandsFree JNI",__VA_ARGS__)
#define printf(a) LOG(a)
#define THROW(a) { ewhere=(a); goto error; }
#define THROW2(a,b) {ewhere=(a); ewhat=(b); goto error; }
#define DEBUGFILE "/sdcard/myaudio4.raw"

static char * formatExpirationDate(time_t expiration)
{
    static char expdate[33];
    if (!expiration) return "never";
    strftime(expdate, 32, "%a, %b %d, %Y", gmtime(&expiration));
    return expdate;
}

static void createRecog(sfs_t *sfs, recog_t **recog_r, searchs_t **recog_s, const char *netfile, const char *searchfile, long recogMode)
{
    const char *ewhere, *ewhat = NULL;
	float seqBuffer = 0.0f;
	float delay = 0.0f;

    if(*recog_r) sfsRecogDestroy(*recog_r);  *recog_r = NULL;
    if(*recog_s) sfsSearchDestroy(*recog_s); *recog_s = NULL;

    // TODO: only need to do this one times
    //LOG("createRecog: Loading recognizer...");
    if(!(*recog_r = sfsRecogCreateFromFile(sfs, netfile,0xFFFF,-1,SDET)))
        THROW("Error: sfsRecogCreateFromFile");

    /* Create search */
    //LOG("createRecog: Load search... %s\n", searchfile);
    if(!(*recog_s = sfsSearchCreateFromFile(sfs,*recog_r,searchfile,NBEST)))
        THROW("Error: sfsSearchCreateFromFile");

    /* Configure trig search */
	//LOG("createRecog: Get phrasespot parameters...");
    if (!sfsPhrasespotConfigGet(sfs,*recog_r,*recog_s,PS_SEQ_BUFFER,&seqBuffer))
        THROW("Error: sfsPhrasespotConfigGet PS_SEQ_BUFFER");

    if (!sfsPhrasespotConfigGet(sfs,*recog_r,*recog_s,PS_DELAY,&delay))
        THROW("Error: sfsPhrasespotConfigGet PS_SEQ_BUFFER");
		
    //LOG("createRecog: delay = %f | sqBuffer = %f",delay,seqBuffer);

	if (recogMode == 1)
	{
		//LOG("createRecog: Setting phrasespot parameters for PS_BEAM");
		if (!sfsPhrasespotConfigSet(sfs,*recog_r,*recog_s,PS_BEAM, 100))
			THROW("Error: sfsPhrasespotConfigSet PS_BEAM");

		//LOG("createRecog: Setting phrasespot parameters for PS_ABSBEAM");
		if (!sfsPhrasespotConfigSet(sfs,*recog_r,*recog_s,PS_ABSBEAM, 100))
			THROW("Error: sfsPhrasespotConfigSet PS_ABSBEAM");
		
		//LOG("createRecog: Setting phrasespot parameters for PS_DELAY");
		if (!sfsPhrasespotConfigSet(sfs,*recog_r,*recog_s,PS_DELAY, 500))
			THROW("Error: sfsPhrasespotConfigSet PS_DELAY");
		
		//LOG("createRecog: Configuring trig phrasespot for sequential buffering...");
		if (!sfsPhrasespotConfigSet(sfs,*recog_r,*recog_s,PS_SEQ_BUFFER, 500))
			THROW("Error: sfsPhrasespotConfigSet");
	} 
	else
	{
		//LOG("createRecog: Setting phrasespot parameters for commands");
		if (!sfsPhrasespotConfigSet(sfs,*recog_r,*recog_s,PS_DELAY, 1000))
			THROW("Error: sfsPhrasespotConfigSet PS_DELAY");
	}
	return;
error:
  if(!ewhat && sfs) ewhat = sfsGetLastError(sfs);
  if(*recog_s)     sfsSearchDestroy(*recog_s);
  if(*recog_r)     sfsRecogDestroy(*recog_r);
  LOG("Panic - %s: %s\n\n",ewhere,ewhat);
  LOG("[ hit Enter to exit ]");
  getchar();
  if(sfs)   sfsSessionDestroy(sfs);
  exit(1);
  return;
}

// Get the SDK expiration date
JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_getExpirationDate( JNIEnv* env, jobject job, jlong arg)
{
    jstring res = NULL;
    res = (*env)->NewStringUTF(env, formatExpirationDate(sfsGetLicenseExpiration()));
    return res;
}

JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_getLicenseExpirationDate( JNIEnv* env,jobject job, jlong arg)
{
    jstring res = NULL;
    res = (*env)->NewStringUTF(env, formatExpirationDate(sfsGetLicenseExpiration()));
    return res;
}

JNIEXPORT jlong JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_createSensorySession( JNIEnv* env,jobject job)
{
    trig_t *t = memset(malloc(sizeof(trig_t)),0,sizeof(trig_t));
//    jlong x;
//    const char *ewhere, *ewhat = NULL;
    long tnow, texp;

    tnow = time(NULL);
    texp = sfsGetLicenseExpiration();
    LOG("Sensory: License Expiration time = %lx",texp);
    LOG("Sensory: Current system time     = %lx",tnow);

    if((texp > 0) && (texp < time(NULL)))
        return (-1);

    t->sfs = sfsSessionCreate();
    LOG("SFS SDK VERSION = %s",sfsVersion());
    LOG("t->sfs = %lx",(long)t->sfs);

    return (jlong)(long)t;
}

JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_createRecogFromFile(JNIEnv* env,jobject job, jlong arg, jstring jnetfile, jstring jsearchfile, jlong listeningState, jlong recogMode)
{
    trig_t *t = (trig_t*)(long)arg;
    const char *ewhere, *ewhat=NULL;
    const char *netfile = (*env)->GetStringUTFChars(env, jnetfile, 0);
    //LOG("netfile = %s", netfile);
    const char *searchfile = (*env)->GetStringUTFChars(env, jsearchfile, 0);
    //LOG("searchfile = %s", searchfile);

#if SSDEBUG
    FILE *fp = fopen(DEBUGFILE,"wb"); fclose(fp);
#endif

    if(!t || !t->sfs) THROW("Must call createSensorySession first");

	//LOG("Sensory listening state: %ld", (long)listeningState);
	long pkgType = (long)listeningState;
    switch (pkgType)
    {
        case SENSORY_GROUP_TRIGGER:
             createRecog (t->sfs, &t->trig_r, &t->trig_s, netfile, searchfile, recogMode);
             break;

        case SENSORY_GROUP_FUNCTION:
             createRecog (t->sfs, &t->comm_func_r, &t->comm_func_s, netfile, searchfile, recogMode);
             break;

        case SENSORY_GROUP_RESPONSE:
             createRecog (t->sfs, &t->comm_response_r, &t->comm_response_s, netfile, searchfile, recogMode);
             break;

        case SENSORY_GROUP_CONFIRMATION:
             createRecog (t->sfs, &t->comm_dictation_r, &t->comm_dictation_s, netfile, searchfile, recogMode);
             break;

        case SENSORY_GROUP_MUSIC:
             createRecog (t->sfs, &t->comm_music_r, &t->comm_music_s, netfile, searchfile, recogMode);
             break;

        case SENSORY_GROUP_MUSIC_SOURCE:
            createRecog (t->sfs, &t->comm_music_option_r, &t->comm_music_option_s, netfile, searchfile, recogMode);
            break;

        case SENSORY_GROUP_FINDGAS_OPTIONS:
            createRecog (t->sfs, &t->comm_findgas_option_r, &t->comm_findgas_option_s, netfile, searchfile, recogMode);
            break;

        default:
            LOG("Sensory listening state is not correct: %ld", (long)listeningState);
  }

  return;

 error:
    if(!ewhat && t->sfs) ewhat = sfsGetLastError(t->sfs);
    if(t->trig_s) sfsSearchDestroy(t->trig_s);
    if(t->trig_r) sfsRecogDestroy(t->trig_r);
    if(t->comm_func_r) sfsRecogDestroy(t->comm_func_r);
    if(t->comm_func_s) sfsSearchDestroy(t->comm_func_s);
    if(t->comm_response_r) sfsRecogDestroy(t->comm_response_r);
    if(t->comm_response_s) sfsSearchDestroy(t->comm_response_s);
    if(t->comm_dictation_r) sfsRecogDestroy(t->comm_dictation_r);
    if(t->comm_dictation_s) sfsSearchDestroy(t->comm_dictation_s);
    if(t->comm_music_r) sfsRecogDestroy(t->comm_music_r);
    if(t->comm_music_s) sfsSearchDestroy(t->comm_music_s);
    if(t->comm_music_option_r) sfsRecogDestroy(t->comm_music_option_r);
    if(t->comm_music_option_s) sfsSearchDestroy(t->comm_music_option_s);
    if(t->comm_findgas_option_r) sfsRecogDestroy(t->comm_findgas_option_r);
    if(t->comm_findgas_option_s) sfsSearchDestroy(t->comm_findgas_option_s);

    LOG("Panic - %s: %s\n\n",ewhere,ewhat);
    printf("[ hit Enter to exit ]");
    getchar();
    if(t->sfs)   sfsSessionDestroy(t->sfs);
    exit(1);
    return;
}

JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_initRecogForCurrentUIState( JNIEnv* env,jobject job, jlong arg, jlong listeningState)
{
    trig_t *t=(trig_t*)(long)arg;
    const char *ewhere, *ewhat=NULL;
	
	long pkgType = (long) listeningState;

	//LOG("initRecogForCurrentScreen pkgtype: %ld", pkgType);
	
   switch (pkgType)
   {
       case SENSORY_GROUP_TRIGGER:
            if (!sfsRecogInit(t->sfs,t->trig_r,t->trig_s,RECOG_KEEP_NONE)) // former: RECOG_KEEP_WAVE_WORD
                THROW("Error: sfsRecogInit for trigger");
            break;

       case SENSORY_GROUP_FUNCTION:
            if (!sfsRecogInit(t->sfs,t->comm_func_r,t->comm_func_s,RECOG_KEEP_NONE))
                THROW("Error: sfsRecogInit for function");
            break;

       case SENSORY_GROUP_RESPONSE:
            if (!sfsRecogInit(t->sfs,t->comm_response_r,t->comm_response_s,RECOG_KEEP_NONE))
                THROW("Error: sfsRecogInit for response");
            break;

       case SENSORY_GROUP_CONFIRMATION:
            if (!sfsRecogInit(t->sfs,t->comm_dictation_r,t->comm_dictation_s,RECOG_KEEP_NONE))
                THROW("Error: sfsRecogInit for dictation");
            break;

       case SENSORY_GROUP_MUSIC:
            if (!sfsRecogInit(t->sfs,t->comm_music_r,t->comm_music_s,RECOG_KEEP_NONE))
                THROW("Error: sfsRecogInit for music");
            break;

       case SENSORY_GROUP_MUSIC_SOURCE:
            if (!sfsRecogInit(t->sfs,t->comm_music_option_r,t->comm_music_option_s,RECOG_KEEP_NONE))
                THROW("Error: sfsRecogInit for music options");
            break;

       case SENSORY_GROUP_FINDGAS_OPTIONS:
            if (!sfsRecogInit(t->sfs,t->comm_findgas_option_r,t->comm_findgas_option_s,RECOG_KEEP_NONE))
                THROW("Error: sfsRecogInit for findgas options");
            break;

       default:
           LOG("RecogInit: Sensory listening state is not correct: %ld", (long) listeningState);
    }
	return;
	
error:
    if(!ewhat && t->sfs) ewhat = sfsGetLastError(t->sfs);
    if(t->trig_s) sfsSearchDestroy(t->trig_s);
    if(t->trig_r) sfsRecogDestroy(t->trig_r);
    if(t->comm_func_r) sfsRecogDestroy(t->comm_func_r);
    if(t->comm_func_s) sfsSearchDestroy(t->comm_func_s);
    if(t->comm_response_r) sfsRecogDestroy(t->comm_response_r);
    if(t->comm_response_s) sfsSearchDestroy(t->comm_response_s);
    if(t->comm_dictation_r) sfsRecogDestroy(t->comm_dictation_r);
    if(t->comm_dictation_s) sfsSearchDestroy(t->comm_dictation_s);
    if(t->comm_music_r) sfsRecogDestroy(t->comm_music_r);
    if(t->comm_music_s) sfsSearchDestroy(t->comm_music_s);
    if(t->comm_music_option_r) sfsRecogDestroy(t->comm_music_option_r);
    if(t->comm_music_option_s) sfsSearchDestroy(t->comm_music_option_s);
    if(t->comm_findgas_option_r) sfsRecogDestroy(t->comm_findgas_option_r);
    if(t->comm_findgas_option_s) sfsSearchDestroy(t->comm_findgas_option_s);

    LOG("Panic - %s: %s\n\n",ewhere,ewhat);
    printf("[ hit Enter to exit ]");
    getchar();
    if(t->sfs)   sfsSessionDestroy(t->sfs);
    exit(1);
    return;
}

JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_recogPipe(JNIEnv* env,jobject job,jlong arg,jobject buffer, jlong listeningState)
{
   trig_t *t = (trig_t*)(long)arg;

   short *sdata = (short*)(*env)->GetDirectBufferAddress(env, buffer);
   unsigned long sz = (unsigned long)(jlong)(*env)->GetDirectBufferCapacity(env, buffer)/2;
   
   unsigned short status;
   const char *ewhere, *ewhat=NULL;
   const char *rres;
   float score;
   jstring res = NULL;
	
	long pkgType = (long) listeningState;
	//LOG("Inside recogPipe: listening: %ld", pkgType);
	switch (pkgType)
	{
        case SENSORY_GROUP_TRIGGER:
			if(!sfsRecogPipe(t->sfs,t->trig_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->trig_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       case SENSORY_GROUP_FUNCTION:
			if(!sfsRecogPipe(t->sfs,t->comm_func_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->comm_func_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       case SENSORY_GROUP_RESPONSE:
			if(!sfsRecogPipe(t->sfs,t->comm_response_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->comm_response_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       case SENSORY_GROUP_CONFIRMATION:
			if(!sfsRecogPipe(t->sfs,t->comm_dictation_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->comm_dictation_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       case SENSORY_GROUP_MUSIC:
			if(!sfsRecogPipe(t->sfs,t->comm_music_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->comm_music_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       case SENSORY_GROUP_MUSIC_SOURCE:
			if(!sfsRecogPipe(t->sfs,t->comm_music_option_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->comm_music_option_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       case SENSORY_GROUP_FINDGAS_OPTIONS:
			if(!sfsRecogPipe(t->sfs,t->comm_findgas_option_r,sz,sdata,RECOG_ONLY,&status)) THROW("recogPipe");
			if(status == RECOG_DONE)
			{
				const char *walign, *palign;
				if (!sfsRecogResult(t->sfs,t->comm_findgas_option_r,&score,&rres,&walign,SAVELOG?&palign:NULL,NULL,NULL,NULL,NULL)) THROW("sfsRecogResult");

				LOG("***RESULT = %s score = %f",rres,score);
				if (rres)
				{
					t->result = realloc(t->result,strlen(rres)+1);
					strcpy(t->result,rres);
					res = (*env)->NewStringUTF(env,t->result);
				}
				if(walign) LOG("***WORDALIGN = %s",walign);
			} 
            break;

       default:
           LOG("RecogInit: Sensory listening state is not correct: %ld", (long)listeningState);
    }

    return res;

error:
  if(!ewhat && t->sfs) ewhat = sfsGetLastError(t->sfs);
  if(t->trig_r) sfsRecogDestroy(t->trig_r);
  if(t->comm_func_r) sfsRecogDestroy(t->comm_func_r);
  if(t->comm_response_r) sfsRecogDestroy(t->comm_response_r);
  if(t->comm_dictation_r) sfsRecogDestroy(t->comm_dictation_r);
  if(t->comm_music_r) sfsRecogDestroy(t->comm_music_r);
  if(t->comm_music_option_r) sfsRecogDestroy(t->comm_music_option_r);
  if(t->comm_findgas_option_r) sfsRecogDestroy(t->comm_findgas_option_r);
 
  LOG("Panic - %s: %s\n\n",ewhere,ewhat);
  LOG("[ hit Enter to exit ]");
  getchar();
  if(t->sfs)   sfsSessionDestroy(t->sfs);
  exit(1);
  return NULL;
}

JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_getSDKVersion( JNIEnv* env, jobject job, jlong arg)
{
    jstring res = NULL;
    static char sdkVersion[8];
    sprintf(sdkVersion, "%d.%d.%d", SFS_VERSION_MAJOR, SFS_VERSION_MINOR, SFS_VERSION_PATCH);
    res = (*env)->NewStringUTF(env, sdkVersion);
    return res;
}

JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_closeRecog( JNIEnv* env,jobject thiz, jlong arg )
{
    trig_t *t = (trig_t*)(long)arg;
    sfs_t *sfs = t->sfs;

    recog_t *trigr = t->trig_r;
    sfsRecogDestroy(trigr); trigr = NULL;
    searchs_t *trigs = t->trig_s;
    sfsSearchDestroy(trigs); trigs = NULL;

    recog_t *commr = t->trig_r;
    sfsRecogDestroy(commr); commr = NULL;
    searchs_t *comms = t->trig_s;
    sfsSearchDestroy(comms); comms = NULL;

    recog_t *comm_func_r = t->comm_func_r;
    sfsRecogDestroy(comm_func_r); comm_func_r = NULL;
    searchs_t *comm_func_s = t->comm_func_s;
    sfsSearchDestroy(comm_func_s); comm_func_s = NULL;

    recog_t *comm_response_r = t->comm_response_r;
    sfsRecogDestroy(comm_response_r); comm_response_r = NULL;
    searchs_t *comm_response_s = t->comm_response_s;
    sfsSearchDestroy(comm_response_s); comm_response_s = NULL;

    recog_t *comm_dictation_r = t->comm_dictation_r;
    sfsRecogDestroy(comm_dictation_r); comm_dictation_r = NULL;
    searchs_t *comm_dictation_s = t->comm_dictation_s;
    sfsSearchDestroy(comm_dictation_s); comm_dictation_s = NULL;

    recog_t *comm_music_r = t->comm_music_r;
    sfsRecogDestroy(comm_music_r); comm_music_r = NULL;
    searchs_t *comm_music_s = t->comm_music_s;
    sfsSearchDestroy(comm_music_s); comm_music_s = NULL;

    recog_t *comm_music_option_r = t->comm_music_option_r;
    sfsRecogDestroy(comm_music_option_r); comm_music_option_r = NULL;
    searchs_t *comm_music_option_s = t->comm_music_option_s;
    sfsSearchDestroy(comm_music_option_s); comm_music_option_s = NULL;

    recog_t *comm_findgas_option_r = t->comm_findgas_option_r;
    sfsRecogDestroy(comm_findgas_option_r); comm_findgas_option_r = NULL;
    searchs_t *comm_findgas_option_s = t->comm_findgas_option_s;
    sfsSearchDestroy(comm_findgas_option_s); comm_findgas_option_s = NULL;

    sfsSessionDestroy(sfs); sfs = NULL;
    if(t->result) free(t->result);
    free(t); t = NULL;
    return;
}
