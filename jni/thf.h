// Copyright (C)2000-2015 Sensory Inc.
// Sensory Confidential

#include "fluent.h"
typedef struct trig_s {
  sfs_t     *sfs;
  recog_t   *trig_r;
  searchs_t *trig_s;
  recog_t   *comm_func_r;
  searchs_t *comm_func_s;
  recog_t   *comm_response_r;
  searchs_t *comm_response_s;
  recog_t   *comm_dictation_r;
  searchs_t *comm_dictation_s;
  recog_t   *comm_music_r;
  searchs_t *comm_music_s;
  recog_t   *comm_music_option_r;
  searchs_t *comm_music_option_s;
  recog_t   *comm_findgas_option_r;
  searchs_t *comm_findgas_option_s;
  char      *result;

  const char *logfile;
} trig_t;

#define SENSORY_GROUP_TRIGGER 1
#define SENSORY_GROUP_FUNCTION        2
#define SENSORY_GROUP_RESPONSE        3
#define SENSORY_GROUP_CONFIRMATION    4
#define SENSORY_GROUP_MUSIC_SOURCE    5
#define SENSORY_GROUP_MUSIC			  6
#define SENSORY_GROUP_FINDGAS_OPTIONS 7

#ifdef __cplusplus
extern "C" {
#endif

//JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_getExpirationDate( JNIEnv* env, jobject job, jlong arg);
//JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_getSDKVersion( JNIEnv* env, jobject job, jlong arg);
//JNIEXPORT jlong JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_initSDK( JNIEnv* env,jobject job, jstring jlogfile);
//JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_setupRecog( JNIEnv* env,jobject job, jlong arg, jstring jtrignetfile, jstring jtrigsearchfile, jstring jcommnetfile, jstring jcommsearchfile);
//JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_initRecog( JNIEnv* env,jobject job, jlong arg, jlong mode);
//JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_pipeRecog( JNIEnv* env,jobject job,jlong arg,jobject buffer, jlong mode);

JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_getLicenseExpirationDate( JNIEnv* env,jobject job, jlong arg);
JNIEXPORT jlong JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_createSensorySession( JNIEnv* env,jobject job);
JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_createRecogFromFile(JNIEnv* env,jobject job, jlong arg, jstring jnetfile, jstring jsearchfile, jlong listeningState, jlong recogMode);
JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_initRecogForCurrentUIState( JNIEnv* env,jobject job, jlong arg, jlong listeningState);
JNIEXPORT jstring JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_recogPipe( JNIEnv* env,jobject job,jlong arg,jobject buffer, jlong listeningState);
JNIEXPORT void JNICALL Java_com_sensedriver_speechrecognizer_sensory_SensoryRecognizer_closeRecog( JNIEnv* env,jobject thiz, jlong arg );


#ifdef __cplusplus
}
#endif