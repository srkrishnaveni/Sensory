/*
 * Sensory Confidential
 *
 * Copyright (C)2000-2014 Sensory Inc.
 *
 *---------------------------------------------------------------------------
 * Sensory Fluent-Speech SDK: fluent.h
 * Recognition API definitions
 *---------------------------------------------------------------------------
 */

#ifndef SFS_FLUENT_H
#define SFS_FLUENT_H
#if defined(__cplusplus)
extern "C" {
#endif

#include <time.h>

#define SFS_VERSION      "3.15.1-269+gb306844"
#define SFS_VERSION_MAJOR 3
#define SFS_VERSION_MINOR 15
#define SFS_VERSION_PATCH 1
#define SFS_VERSION_PRE  "269+gb306844"
#define SFS_VERSION_ID    42

#define _QUOTE(s) #s
#define QUOTE(s) _QUOTE(s)

/* sfsRecogPipe() status definitions */
enum {RECOG_QUIET, RECOG_SILENCE, RECOG_SOUND,
      RECOG_DONE, RECOG_MAXREC, RECOG_IGNORE, RECOG_NODATA};

/* sfsRecogPipe() mode definitions */
enum {SDET_ONLY=0x01, RECOG_ONLY, SDET_RECOG};

/* sfsRecogCreateFromFile() mode definitions */
enum {NO_SDET, SDET};

/* sfsSearchCreate... definitions */
enum {NO_PHRASESPOTTING, PHRASESPOTTING};

/* sfsRecogInit() keep definitions */
enum {RECOG_KEEP_NONE, RECOG_KEEP_WAVE, RECOG_KEEP_WAVE_WORD,
      RECOG_KEEP_WAVE_WORD_PHONEME, RECOG_KEEP_WORD_PHONEME};

/* sfsTuneCreate() mode definitions */
enum {NO_SILENCE, REQUIRE_SILENCE};

/* sfsRecogConfigure() mask definitions */
#define RECOG_SET_LSILENCE     (1<<1)
#define RECOG_SET_TSILENCE     (1<<2)
#define RECOG_SET_MAXREC       (1<<3)
#define RECOG_SET_MINDUR       (1<<4)
#define RECOG_SET_ESILENCET    (1<<5)
#define RECOG_SET_ESPEECHT     (1<<6)
#define RECOG_SET_SHORTTERMMS  (1<<7)
#define RECOG_SET_LONGTERMMS   (1<<8)

/* sfsRecogConfigSet(), sfsRecogConfigGet() definitions */
typedef enum {
  REC_LSILENCE, REC_TSILENCE, REC_MAXREC, REC_MINDUR, REC_ESILENCET,
  REC_ESPEECHT, REC_SHORTTERMMS, REC_LONGTERMMS,
  REC_BACKOFF, REC_THI, REC_SDET_EDIFF, REC_SDET_INITIAL_DELAY,
  REC_SDET_STEM, REC_SDET_LTEM,
  REC_KEEP_SDET_HISTORY, REC_KEEP_FEATURE_HISTORY, REC_KEEP_FEATURES,
  REC_EPQ_ENABLE, REC_EPQ_MIN, REC_EPQ_SCORE, REC_EPQ_NOISE,
  REC_SV_ADJUST_METHOD,
  REC_SV_ADJUST1, REC_SV_ADJUST2, REC_SV_ADJUST3, REC_SV_ADJUST4,
  REC_PARAMA_ADJUST_METHOD, REC_PARAMA_ADJUST1, REC_PARAMA_ADJUST2,
  REC_PARAMA_ADJUST3, REC_PARAMA_ADJUST4,
  REC_PHONEMEREC_SEARCH_TYPE,
  REC_USE_FEAT,
  REC_CHECK_ENERGY_MIN, REC_CHECK_ENERGY_STD_DEV, REC_CHECK_SIL_BEG_MSEC,
  REC_CHECK_SIL_END_MSEC, REC_CHECK_SNR, REC_CHECK_RECORDING_VARIANCE,
  REC_CHECK_CLIPPING_PERCENT, REC_CHECK_CLIPPING_MSEC, REC_CHECK_POOR_RECORDINGS,
  REC_CHECK_SPOT, REC_CHECK_VOWEL_DUR,

  REC_EARLYSTOP_NBEST
} sfsRecogConfig_t;

/* sfsRecogConfigGet() EPQ defaults */
#define EPQ_DEFAULT_MIN -5.0

#if !defined(FLUENTDEV) || !defined(SDK_LVCSR)
/* sfsSearchConfigSet(), sfsSearchConfigGet() definitions */
enum sfsSearchConfigEnum {
  SCH_PRUNING, SCH_BEAM, SCH_GARBAGE,
  SCH_ANY, SCH_NOTA, SCH_LONGPEN
};
#endif

/* sfsSearchConfigure() mask definitions */
#define SEARCH_SET_PRUNING     (1)
#define SEARCH_SET_BEAM        (1<<1)
#define SEARCH_SET_GARBAGE     (1<<2)
#define SEARCH_SET_ANY         (1<<3)
#define SEARCH_SET_NOTA        (1<<4)
#define SEARCH_SET_LONGPEN     (1<<5)

/* sfsPhrasespotConfigSet(), sfsPhrasespotConfigGet() definitions */
typedef enum {
  PS_PARAMA_OFFSET, PS_BEAM, PS_ABSBEAM, PS_DELAY, PS_LONGPEN,
  PS_SEQ_BUFFER, PS_SEQ_SPOT_OPTIONAL, PS_SEQ_SPOT_INCLUDED,
  PS_SEQ_SAMPLES
} sfsPhrasespotConfig_t;

/* sfsPhrasespotConfigure() mask definitions */
#define PHRASESPOT_SET_PARAMA_OFFSET (1)
#define PHRASESPOT_SET_BEAM          (1<<1)
#define PHRASESPOT_SET_ABSBEAM       (1<<2)
#define PHRASESPOT_SET_DELAY         (1<<3)
#define PHRASESPOT_SET_LONGPEN       (1<<4)

/* sfsPhrasespotConfigure() special DELAY flags */
#define PHRASESPOT_DELAY_ASAP   0xFFFE   /* as soon as probable */

/* sfsPhrasespotCreateFromList definitions */
#define PHRASESPOT_LISTEN_SHORT  10      /* Listening window in seconds */
#define PHRASESPOT_LISTEN_MEDIUM 30 
#define PHRASESPOT_LISTEN_LONG   60
#define PHRASESPOT_LISTEN_CONTINUOUS   3600

/* sfsSpeaker definitions */
#define SPEAKER_IGNORE_PAUSE         "IGNORE_PAUSE"
#define SPEAKER_IGNORE_SV_DEFAULT    "IGNORE_SV_DEFAULT"
#define SPEAKER_IGNORE_RTA_DEFAULT   "IGNORE_RTA_DEFAULT"

#define SPEAKER_SET_WANT_NUM_RECORDINGS (1<<1)

typedef enum {
  SPEAKER_WANT_NUM_RECORDINGS, SPEAKER_HAVE_NUM_RECORDINGS,
  SPEAKER_DONE_ADAPTATION, SPEAKER_RECOG_WITH_ADAPTED, SPEAKER_SPEED_ACCURACY,
  SPEAKER_SAMP_PER_CAT, SPEAKER_TRIGGER_SAMP_PER_CAT, 
  SPEAKER_SAMP_PER_CAT_WITHIN, SPEAKER_TARGET_SNR, SPEAKER_DUR_MIN_FACTOR, 
  SPEAKER_DUR_MAX_FACTOR, 
  SPEAKER_USE_FEAT, SPEAKER_TRAIN_ITERATIONS, SPEAKER_TRAIN_ITERATIONS_WITHIN,
  SPEAKER_LEARN_RATE, SPEAKER_LEARN_RATE_WITHIN, SPEAKER_DROPOUT_WITHIN,
  SPEAKER_ADAPT_TYPE, SPEAKER_DSP_SV_THRESH} sfsSpeakerConfig_t;

// changed SPEAKER_DEFAULT_TRIGGER_SAMP_PER_CAT from 60 to 256 on 11-Dec-2013;
// increase is primarily due to the introduction of SPEAKER_TARGET_SNR
#define SPEAKER_DEFAULT_TRIGGER_SAMP_PER_CAT (256) 
#define SPEAKER_DEFAULT_SAMP_PER_CAT_WITHIN  (0)
#define SPEAKER_DEFAULT_TARGET_SNR           (20.0f)
#define SPEAKER_DEFAULT_DUR_MIN_FACTOR       (0.5f)
#define SPEAKER_DEFAULT_DUR_MAX_FACTOR       (1.25f)
#define SPEAKER_DEFAULT_USE_FEAT             (0)
#define SPEAKER_DEFAULT_TRAIN_ITERATIONS     (1)
#define SPEAKER_DEFAULT_TRAIN_ITERATIONS_WITHIN (1)
#define SPEAKER_DEFAULT_LEARN_RATE           (0.1640625f)
#define SPEAKER_DEFAULT_LEARN_RATE_WITHIN    (0.1640625f)
#define SPEAKER_DEFAULT_DROPOUT_WITHIN       (0.0f)
#define SPEAKER_DEFAULT_ADAPT_TYPE           (0)

#define SPEAKER_SAVE_MODEL              (1<<1)
#define SPEAKER_SAVE_DATA               (1<<2)
#define SPEAKER_SAVE_UDT                (1<<3)
#define SPEAKER_SAVE_BOTH     (SPEAKER_SAVE_MODEL | SPEAKER_SAVE_DATA)
#define SPEAKER_SAVE_UDTDATA  (SPEAKER_SAVE_DATA  | SPEAKER_SAVE_UDT)
#define SPEAKER_SAVE_UDTMODEL (SPEAKER_SAVE_MODEL | SPEAKER_SAVE_UDT)
#define SPEAKER_SAVE_UDTBOTH  (SPEAKER_SAVE_MODEL | SPEAKER_SAVE_DATA | SPEAKER_SAVE_UDT)

#define SPEAKER_READ_MODEL              (1<<1)
#define SPEAKER_READ_DATA               (1<<2)
#define SPEAKER_READ_UDT                (1<<3)
#define SPEAKER_READ_BOTH     (SPEAKER_READ_MODEL | SPEAKER_READ_DATA)
#define SPEAKER_READ_UDTDATA  (SPEAKER_READ_DATA  | SPEAKER_READ_UDT)
#define SPEAKER_READ_UDTMODEL (SPEAKER_READ_MODEL | SPEAKER_READ_UDT)
#define SPEAKER_READ_UDTBOTH  (SPEAKER_READ_MODEL | SPEAKER_READ_DATA | SPEAKER_READ_UDT)

/* sfsUdtConfigSet(), sfsUdtConfigGet() definitions */
typedef enum {
  UDT_SAMP_PER_CAT, UDT_PARAM_A_START, UDT_PARAM_A_NUM, UDT_PARAM_A_STEP,
  UDT_PARAM_B_START, UDT_PARAM_B_NUM, UDT_PARAM_B_STEP, UDT_MAX_PARAM_D,
  UDT_NUM_CHUNKS, UDT_MAX_MEMORY, UDT_ABSBEAM, UDT_REQUIRE_SILENCE,
  UDT_SIMILARITY_THRESH, UDT_PARAM_E, UDT_PARAM_F, UDT_PARAM_G, 
  UDT_SAMP_PER_CAT_WITHIN, UDT_IGNORE_TYPE, UDT_SIL_PERCENT, UDT_NOISE_PERCENT,
  UDT_PHONEMEREC_SEARCH_TYPE, UDT_SANITIZE, UDT_DUR_MIN_FACTOR, 
  UDT_DUR_MAX_FACTOR, UDT_POP_THRESH, UDT_DBA_FILTER, UDT_TARGET_SNR, 
  UDT_USE_FEAT, 

  UDT_CHECK_ENERGY_MIN, UDT_CHECK_ENERGY_STD_DEV, UDT_CHECK_SIL_BEG_MSEC,
  UDT_CHECK_SIL_END_MSEC, UDT_CHECK_SNR, UDT_CHECK_RECORDING_VARIANCE,
  UDT_CHECK_CLIPPING_PERCENT, UDT_CHECK_CLIPPING_MSEC, 
  UDT_CHECK_POOR_RECORDINGS, UDT_CHECK_VOWEL_DUR,

  UDT_TRAIN_ITERATIONS, UDT_TRAIN_ITERATIONS_WITHIN, UDT_LEARN_RATE,
  UDT_LEARN_RATE_WITHIN, UDT_DROPOUT_WITHIN, UDT_ADAPT_TYPE,

  UDT_DEBUG_LEVEL
} sfsUdtConfig_t;

#define UDT_DEFAULT_SAMP_PER_CAT        (512)
#define UDT_DEFAULT_PARAM_A_START       (-1100)
#define UDT_DEFAULT_PARAM_A_NUM         (1)
#define UDT_DEFAULT_PARAM_A_STEP        (20)
#define UDT_DEFAULT_PARAM_B_START       (850)
#define UDT_DEFAULT_PARAM_B_NUM         (1)
#define UDT_DEFAULT_PARAM_B_STEP        (100)
#define UDT_DEFAULT_MAX_PARAM_D         (200)
#define UDT_DEFAULT_NUM_CHUNKS          (20)
#define UDT_DEFAULT_MAX_MEMORY          (20000000)
#define UDT_DEFAULT_ABSBEAM             (1000.0f)
#define UDT_DEFAULT_REQUIRE_SILENCE     (0)
#define UDT_DEFAULT_SIMILARITY_THRESH   (350.0f)
#define UDT_DEFAULT_PARAM_E             (800.0f)
#define UDT_DEFAULT_PARAM_F             (500.0f)
#define UDT_DEFAULT_PARAM_G             (0.05f)
#define UDT_DEFAULT_SAMP_PER_CAT_WITHIN (16)
#define UDT_DEFAULT_IGNORE_TYPE         (1)
#define UDT_DEFAULT_SIL_PERCENT         (10)
#define UDT_DEFAULT_NOISE_PERCENT       (40)
#define UDT_DEFAULT_PHONEMEREC_SEARCH_TYPE (1)
#define UDT_DEFAULT_SANITIZE            (5)
/* To turn off duration modification, set min > max */
#define UDT_DEFAULT_DUR_MIN_FACTOR      (0.50f)
#define UDT_DEFAULT_DUR_MAX_FACTOR      (1.25f)
#define UDT_DEFAULT_POP_THRESH          (10.0f)
#define UDT_DEFAULT_DBA_FILTER          (1)
#define UDT_DEFAULT_TARGET_SNR          (20.0f)
#define UDT_DEFAULT_TRAIN_ITERATIONS        (1)
#define UDT_DEFAULT_TRAIN_ITERATIONS_WITHIN (1)
#define UDT_DEFAULT_LEARN_RATE              (0.1640625f)
#define UDT_DEFAULT_LEARN_RATE_WITHIN       (0.1640625f)
#define UDT_DEFAULT_DROPOUT_WITHIN          (0.0f)
#define UDT_DEFAULT_ADAPT_TYPE              (0)

// bit fields that indicate a particular problem in a recording
#define CHECKRECORDING_BITFIELD_ENERGY_MIN         (1 << 0)
#define CHECKRECORDING_BITFIELD_ENERGY_STD_DEV     (1 << 1)
#define CHECKRECORDING_BITFIELD_SIL_BEG_MSEC       (1 << 2)
#define CHECKRECORDING_BITFIELD_SIL_END_MSEC       (1 << 3)
#define CHECKRECORDING_BITFIELD_SNR                (1 << 4)
// CHECKRECORDING_BITFIELD_SCORE_DIFF should be retired; use RECORDING_VARIANCE
// instead!
#define CHECKRECORDING_BITFIELD_SCORE_DIFF         (1 << 5)
#define CHECKRECORDING_BITFIELD_RECORDING_VARIANCE (1 << 5)
#define CHECKRECORDING_BITFIELD_CLIPPING_PERCENT   (1 << 6)
#define CHECKRECORDING_BITFIELD_CLIPPING_MSEC      (1 << 7)
#define CHECKRECORDING_BITFIELD_POOR_RECORDINGS    (1 << 8)
#define CHECKRECORDING_BITFIELD_SPOT               (1 << 9)
#define CHECKRECORDING_BITFIELD_VOWEL_DUR          (1 << 10)

/* sfsTtsConfigSet(), sfsTtsConfigGet() definitions */
typedef enum {
  PROSODY_DOMAIN, PROSODY_PITCHRANGE, PROSODY_PHRASEBREAK_RESTART,
  TTS_SAMPLERATE, TTS_VOCODER
} sfsTtsConfig_t;
/* PROSODY_DOMAIN  values*/
enum {GENERIC, NAMES};
/* PROSODY_PITCHRANGE values */
enum {FULL, HALF};
/* PROSODY_PHRASEBREAK_RESTART values */
#if !defined(NO) && !defined(YES)
enum {NO, YES};
#endif
/* TTS_SAMPLERATE values */
enum {SR_16K, SR_8K};
/* TTS_VOCODER values */
enum {VOC_SENSORY, VOC_MELP};

#define VOCODER_DEFAULT                 (0)
#define VOCODER_SENSORY                 (1)
#define VOCODER_MARY                    (1<<1)

/* sfsAudioCB messages */
enum {
  AUDIO_ERROR_NOT_SUPPORTED,
  AUDIO_ERROR_INTERNAL,
  AUDIO_ERROR_RECORD_STARVATION,
  AUDIO_ERROR_UNSUPPORTED_FORMAT,
  AUDIO_ERROR_BUSY,
  AUDIO_STATE_RECORDING,
  AUDIO_STATE_PLAYBACK,
  AUDIO_STATE_SILENCE,
  AUDIO_STATE_SPEECH,
  AUDIO_STATE_RESULT,
  AUDIO_STATE_IDLE,
  AUDIO_BUFFER_MAX,
  AUDIO_INFO,
};


/* Internal data structures hidden from the user */
#ifndef COMPILINGFLUENTSDK
# if defined(_WIN32) || defined(_WIN32_WCE)
typedef void (__cdecl sfsAudioCB_t)(char msg, void *arg, void *clientData);
# else
typedef void (sfsAudioCB_t)(char msg, void *arg, void *clientData);
# endif
typedef struct sfs_s sfs_t;
typedef struct searchs_t_ searchs_t;
typedef struct recog_t_ recog_t;
typedef struct pronuns_t_ pronuns_t;
typedef struct tts_t_ tts_t;
typedef struct udt_t_ udt_t;
typedef enum sfsSearchConfigEnum sfsSearchConfig_t;
typedef struct tune_t_ tune_t;
typedef struct phsearch_t_ phsearch_t;
//typedef struct pgraph_t_ pgraph_t;
#endif

/* static wave structure [output] */
typedef struct swave_s {
  unsigned short len;            /* wave buffer length */
  short *buf;                    /* wave buffer */
} swave_t;

/* static data structure [input] */
typedef struct sdata_s {
  unsigned short I;               /* Number of data objects */
  const void **d;                 /* Array of data objects */
} sdata_t;

/* UDT callback data and procedure definitions */
typedef struct UDT_CBdata_s UDT_CBdata_t;
typedef short int (UDT_CBfunction_t)(struct UDT_CBdata_s *cd);
struct UDT_CBdata_s {
  UDT_CBfunction_t *cbFunction;
  void             *cbData;
  unsigned long     where;
  unsigned long     whereMax;
  recog_t          *tempRecog;
  searchs_t        *tempSearch;
};

/* structure for one enrollment recording for UDT or speaker verification */
typedef struct sfsEnrollment_s {
  short *audio;
  unsigned long audioLen;
  unsigned long audioSignature;
  char *filename;
  char *reserved1; // set to NULL
  char *pronun; // set to NULL or optional pipelined UDT pronunciation
} sfsEnrollment_t;
typedef sfsEnrollment_t udtEnrollment_t;

/* structure for one user for UDT or speaker verification, containing a name and one or more enrollments */
typedef struct sfsUser_s {
  char *userName;
  char *tag;
  unsigned short numEnroll;
  sfsEnrollment_t *enroll;
  char *reserved1; // set to NULL
  char *reserved2; // set to NULL
} sfsUser_t;
typedef sfsUser_t udtUser_t; // we want to move away from udtUser, but allow backwards compatibility

/* read-from-stream function type */
typedef enum {
  SFS_STREAM_OK, SFS_STREAM_EOF, SFS_STREAM_ERROR
} sfsReadFuncError_t;
typedef size_t (*sfsReadFunc_t)(void *buffer, size_t size, size_t nitems,
                                void *privateData, sfsReadFuncError_t *err);

#if defined(_WIN32) && defined(EXPRT) && defined(FLUENTSDK)
# define SFS_PROTOTYPE(a,b,c) __declspec(dllexport) a b c
#elif defined(__SYMBIAN32__)
# define SFS_PROTOTYPE(a,b,c) EXPORT_C a b c
#elif defined(_WIN32) || defined(_WIN32_WCE)
# define SFS_PROTOTYPE(a,b,c) a __cdecl b c
#elif defined(__SH_3)
# define SFS_PROTOTYPE(a,b,c) extern a b c
#else
# define SFS_PROTOTYPE(a,b,c) a b c
#endif

/* Functions marked with SFS_EXPERIMENTAL() are in a state of flux -
 * names, arguments etc. will likely change in future releases.
 */
#define SFS_EXPERIMENTAL(a,b,c) SFS_PROTOTYPE(a,b,c)

#ifdef NO_DEPRECATED_API_WARNINGS
# undef SFS_DEPRECATED
# define SFS_DEPRECATED(msg,a,b,c) SFS_PROTOTYPE(a,b,c)
#endif

#ifndef SFS_DEPRECATED
# if defined(__GNUC__) && \
  ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#  if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5))
#   define SFS_DEPRECATED(msg,a,b,c) \
  SFS_PROTOTYPE(a,b,c) __attribute__ ((__deprecated__ (msg)))
#  else
#   define SFS_DEPRECATED(msg,a,b,c) \
  SFS_PROTOTYPE(a,b,c) __attribute__ ((__deprecated__))
#  endif
# elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define SFS_DEPRECATED(msg,a,b,c) \
  __declspec(deprecated(msg)) SFS_PROTOTYPE(a,b,c)
# else
#  define SFS_DEPRECATED(msg,a,b,c) SFS_PROTOTYPE(a,b,c)
# endif
#endif
#define REC_EPQ_MINSNR REC_EPQ_MIN

#ifdef FLUENTDEV
# include "fluent-dev.h"
#endif

#if defined(__brew__)
#define sfsFILE void
  SFS_PROTOTYPE(sfs_t *,sfsSessionCreateBrew,(void *app, void *filemgr));
  extern sfsFILE *brew_fopen(void *fileMgr,const char *filename, const char *mode);
  extern size_t brew_fread(void *buf, size_t size, size_t nitems, sfsFILE *fp);
  extern size_t brew_fwrite(const void *buf,size_t size,size_t count,sfsFILE *fp);
  extern int brew_fseek(sfsFILE *fp, long offset, int origin);
  extern long brew_ftell(sfsFILE *fp);
  extern int brew_fclose(sfsFILE *fp);
  extern void brew_DispStr(void *a, char *b);
#endif

/* function prototypes */
/* Core funcs */
SFS_PROTOTYPE(sfs_t *,sfsSessionCreate,(void));
SFS_PROTOTYPE(void ,sfsSessionDestroy,(sfs_t *sfs));
SFS_PROTOTYPE(void ,sfsFree,(void *r));
SFS_PROTOTYPE(void *,sfsMalloc,(size_t size));
SFS_PROTOTYPE(const char *,sfsGetLastError,(sfs_t *sfs));
SFS_PROTOTYPE(void,sfsResetLastError,(sfs_t *sfs));
SFS_PROTOTYPE(const char *,sfsVersion,(void));
SFS_PROTOTYPE(time_t,sfsGetLicenseExpiration,(void));
SFS_PROTOTYPE(const char *,sfsGetLicenseExpirationString,(void));
SFS_PROTOTYPE(const char *,sfsGetLicenseFeatures,(void));

#ifdef _WIN32
/* Miscellaneous compatibility functions, for convenience only.
 * These functions are not documented in the SDK API and are not
 * supported in any way.
 */
SFS_PROTOTYPE(int,getopt,(int argc, char *const argv[], const char *optstr));
#endif

/* Text normalization */
SFS_PROTOTYPE(const char *,sfsTextNormalize,(sfs_t *sfs, pronuns_t *p, const char *word, unsigned short conditionCode));

/* Pronunciation funcs */
SFS_PROTOTYPE(const char *,sfsPronunCompute,(sfs_t *sfs, pronuns_t *p, const char *word, unsigned short nbest, const char *suffix, const char *pos));
SFS_PROTOTYPE(pronuns_t *,sfsPronunCreateFromFile,(sfs_t *sfs, const char *pronunfile, unsigned short dialectIndex));
SFS_PROTOTYPE(pronuns_t *,sfsPronunCreateFromStatic,(sfs_t *sfs, sdata_t *global, unsigned short idx, unsigned short dialectIndex));
SFS_PROTOTYPE(void ,sfsPronunDestroy,(pronuns_t *p));

/* Search funcs */
SFS_PROTOTYPE(searchs_t *,sfsSearchCreateFromStatic,(sfs_t *sfs, recog_t *r, sdata_t *global,unsigned short idx, unsigned short nbest));
SFS_PROTOTYPE(void ,sfsSearchDestroy,(searchs_t *s));
SFS_PROTOTYPE(int,sfsSearchConfigGet,(sfs_t *sfs, recog_t *r, searchs_t *s,
				      sfsSearchConfig_t key, float *value));
SFS_PROTOTYPE(int,sfsSearchConfigSet,(sfs_t *sfs, recog_t *r, searchs_t *s,
				      sfsSearchConfig_t key, float value));
SFS_PROTOTYPE(int,sfsPhrasespotConfigGet,(sfs_t *sfs, recog_t *r, searchs_t *s,
					  sfsPhrasespotConfig_t key,
					  float *value));
SFS_PROTOTYPE(int,sfsPhrasespotConfigSet,(sfs_t *sfs, recog_t *r, searchs_t *s,
					  sfsPhrasespotConfig_t key,
					  float value));
SFS_PROTOTYPE(searchs_t *,sfsPhrasespotCreateFromList,(sfs_t *sfs, recog_t *r, pronuns_t *p, const char **phrases, unsigned short phraseCount, const char **word, const char **pronun, unsigned short wordCount, unsigned short listenWindow));

SFS_PROTOTYPE(searchs_t *,sfsSearchCreateFromFile,(sfs_t *sfs, recog_t *r, const char *searchfile, unsigned short nbest));
SFS_PROTOTYPE(searchs_t *,sfsSearchCreateFromFunc,(sfs_t *sfs, recog_t *r, sfsReadFunc_t readFunc, void *privateData, unsigned short nbest));
SFS_PROTOTYPE(searchs_t *,sfsSearchCreateFromList,(sfs_t *sfs, recog_t *r, pronuns_t *p, const char **word, const char **pronun, unsigned short count, unsigned short nbest));
SFS_PROTOTYPE(searchs_t *,sfsSearchCreateIncremental,(sfs_t *sfs, recog_t *r, searchs_t *s, pronuns_t *p, const char **removeGrams, unsigned short numRemove, const char **addGrams, unsigned short numAdd, const char **word, const char **pronun, unsigned short numWords, unsigned short nbest));
SFS_PROTOTYPE(searchs_t *,sfsSearchCreateFromGrammar,(sfs_t *sfs, recog_t *r, pronuns_t *p, const char *grammar, const char **word, const char **pronun, unsigned short count, unsigned short nbest, char usePhrasespotting));
SFS_PROTOTYPE(int, sfsSearchPhrases,(sfs_t *sfs, searchs_t *s,
                                     char ***map, int *count));
SFS_PROTOTYPE(int ,sfsSearchSaveToFile,(sfs_t *sfs, searchs_t *s, const char *filename));
SFS_PROTOTYPE(int ,sfsSaveEmbedded,(sfs_t *sfs, recog_t *r, searchs_t *s,
                                    const char *target, char *rfilename,
                                    char *sfilename, int reserved,
                                    int trigger));
SFS_PROTOTYPE(int, sfsEmbeddedTargetsGet,
	      (sfs_t *sfs, char ***targets, int *count));

/* Recognizer funcs */
SFS_PROTOTYPE(recog_t *,sfsRecogCreateStub,(sfs_t *sfs, unsigned long samplerate));
SFS_PROTOTYPE(recog_t *,sfsRecogCreateFromStatic,(sfs_t *sfs, sdata_t *global, unsigned short idx, unsigned long maxbuf, unsigned short backoff, char useSdet));
SFS_PROTOTYPE(void ,sfsRecogDestroy,(recog_t *r));
SFS_PROTOTYPE(int ,sfsRecogInit,(sfs_t *sfs, recog_t *r, searchs_t *s, unsigned char keep));
SFS_PROTOTYPE(int ,sfsRecogReset,(sfs_t *sfs, recog_t *r));
SFS_PROTOTYPE(int ,sfsRecogPipe,(sfs_t *sfs, recog_t *r, unsigned long ilen, short *ibuf, char mode, unsigned short *stateResult));
SFS_PROTOTYPE(int ,sfsRecogPipeFlush,(sfs_t *sfs, recog_t *r, char mode, unsigned short *stateResult));
SFS_PROTOTYPE(float,sfsRecogSVscore,(sfs_t *sfs, recog_t *r));
SFS_PROTOTYPE(int ,sfsRecogResult,(sfs_t *sfs, recog_t *r, float *score, const char **res, const char **wordAlign, const char **phoneAlign, const short **inSpeech, unsigned long *inSpeechSamples, const short **sdetSpeech, unsigned long *sdetSpeechSamples));
SFS_PROTOTYPE(int ,sfsRecogSampleConvert,(sfs_t *sfs, recog_t *r, unsigned long inrate, short *inbuf, unsigned long inlen, short **outbuf, unsigned long *outlen));
SFS_PROTOTYPE(int,sfsRecogSearchSaveToFile,(sfs_t *sfs, searchs_t *search, recog_t *r, char *searchFilename, char *recogFilename));
SFS_PROTOTYPE(int ,sfsRecogGetSpeechRange,(sfs_t *sfs, recog_t *r, float *from,float *to));
SFS_PROTOTYPE(int ,sfsRecogGetClipping,(sfs_t *sfs, recog_t *r,float *clipped));
SFS_PROTOTYPE(unsigned long ,sfsRecogGetSampleRate,(sfs_t *sfs, recog_t *r));
SFS_PROTOTYPE(int,sfsRecogCombine,(sfs_t *sfs, int prune, recog_t *r1, recog_t *r2, searchs_t *s1, searchs_t *s2, const char *prefix1, const char *prefix2, recog_t **outputRecog, searchs_t **outputSearch, float *compressionRatio));
SFS_PROTOTYPE(int ,sfsRecogConfigGet,(sfs_t *sfs, recog_t *r,
				      sfsRecogConfig_t key, float *value));
SFS_PROTOTYPE(int ,sfsRecogConfigSet,(sfs_t *sfs, recog_t *r,
				      sfsRecogConfig_t key, float value));
SFS_PROTOTYPE(recog_t *,sfsRecogCreateFromFile,(sfs_t *sfs, const char *netfile, unsigned long maxbuf, unsigned short backoff, char useSdet));
SFS_PROTOTYPE(recog_t *,sfsRecogCreateFromFunc,(sfs_t *sfs, sfsReadFunc_t readFunc, void *privateData, unsigned long maxbuf, unsigned short backoff, char useSdet));
SFS_PROTOTYPE(int,sfsRecogCreateHybrid,(sfs_t *sfs, recog_t **r, pronuns_t **p, unsigned short numObjects, const char ***word, const char ***pronun, unsigned short *numWords, recog_t **hybridRecog, pronuns_t **hybridPronun));
SFS_PROTOTYPE(int,sfsRecogSdetForceDone,(sfs_t *sfs, recog_t *r));
SFS_PROTOTYPE(int,sfsRecogPrep,(sfs_t *sfs, recog_t *r, unsigned long ilen, short *ibuf));
SFS_PROTOTYPE(int,sfsRecogPrepSeq,(sfs_t *sfs, recog_t *dst, recog_t *src));
SFS_EXPERIMENTAL(int,sfsRecogClone,(sfs_t *sfs, recog_t **out, const recog_t *src));
SFS_EXPERIMENTAL(int,sfsRecogAdd,(sfs_t *sfs, recog_t **out,const recog_t *source1, const char *source1tag,const recog_t *source2, const char *source2tag));
SFS_EXPERIMENTAL(int,sfsRecogRemove,(sfs_t *sfs, recog_t **out, const recog_t *source, const char *tag));
SFS_EXPERIMENTAL(int,sfsSearchClone,(sfs_t *sfs, searchs_t **out, const searchs_t *src));
SFS_EXPERIMENTAL(int,sfsSearchAdd,(sfs_t *sfs, searchs_t **out,const searchs_t *source1, const char *source1tag,const searchs_t *source2, const char *source2tag));
SFS_EXPERIMENTAL(int,sfsSearchRemove,(sfs_t *sfs, searchs_t **out, const searchs_t *source, const char *tag));
typedef enum {
  RTA_ACTIVE, RTA_ADAPT, RTA_BESTOF, RTA_WANT_NUM_RECS
} sfsRtaConfig_t;
SFS_EXPERIMENTAL(int,sfsRtaConfigSet,(sfs_t *sfs, recog_t *initializedRecognizer, const char *tag, sfsRtaConfig_t key, int value));
SFS_EXPERIMENTAL(int,sfsRtaConfigGet,(sfs_t *sfs, recog_t *initializedRecognizer, const char *tag, sfsRtaConfig_t key, int *value));
SFS_EXPERIMENTAL(int,sfsRecogTags,(sfs_t *sfs, const recog_t *recog, char ***tags, int *count));
SFS_EXPERIMENTAL(int,sfsSearchTags,(sfs_t *sfs, const searchs_t *search, char ***tags, int *count));


/* Speaker Verification functions */
SFS_PROTOTYPE(int,sfsSpeakerCreateFromFile,(sfs_t *sfs, recog_t *r,
        unsigned short numEnroll, char *filename));
SFS_PROTOTYPE(int,sfsSpeakerInit,(sfs_t *sfs, recog_t *r,
        unsigned short sID, unsigned short wantNumTrainFiles));
SFS_PROTOTYPE(int,sfsSpeakerAdapt,(sfs_t *sfs, recog_t *r,
        unsigned short sID, unsigned short trainMethod,
        char *ignorePhnSpec, unsigned short trainSampWant));
SFS_EXPERIMENTAL(int,sfsSpeakerReadyToAdapt,(sfs_t *sfs, recog_t *r));
SFS_EXPERIMENTAL(int,sfsSpeakerApplyAdaptation,(sfs_t *sfs, recog_t *r));
SFS_PROTOTYPE(int,sfsSpeakerCheckEnrollments,(sfs_t *sfs, recog_t *recog, 
        sfsUser_t *user, unsigned short whichUser,
        unsigned long *globalFeedback, 
        unsigned long *feedbackArray, float *phraseQuality));
SFS_PROTOTYPE(int, sfsSpeakerCheckValueGet, (sfs_t *sfs, recog_t *r, sfsRecogConfig_t param, float *val, unsigned short numEnroll));

SFS_PROTOTYPE(int,sfsSpeakerEnroll,(sfs_t *sfs, recog_t *r,
	char *phrase, char *ignorePhnSpec, unsigned short numUsers, 
        sfsUser_t *user, searchs_t **outputSearch, recog_t **outputRecog));

SFS_PROTOTYPE(int,sfsSpeakerSaveRecogToFile,(sfs_t *sfs, searchs_t *search, 
        recog_t *recog, char *searchFile, char *recogFile));
SFS_PROTOTYPE(int,sfsSpeakerVerify,(sfs_t *sfs, recog_t *r,
        unsigned short sID, float *spkrRawScore, short *recordingOK));
SFS_PROTOTYPE(int,sfsSpeakerGender,(sfs_t *sfs, recog_t *r,
        float *genderProb));
SFS_PROTOTYPE(int,sfsSpeakerReadGenderModel,(sfs_t *sfs, recog_t *r,
        char *filename));
SFS_PROTOTYPE(int,sfsSpeakerReadAntiSpeakerData,(sfs_t *sfs, recog_t *r,
        char *filename));
SFS_PROTOTYPE(int,sfsSpeakerAddAntiData,(sfs_t *sfs, recog_t *r,
        unsigned short sID, short wantExamples,
        unsigned short useMonocat));
SFS_PROTOTYPE(int,sfsSpeakerStoreLastRecording,(sfs_t *sfs, recog_t *r,
        unsigned short sID, short score, short force));
SFS_PROTOTYPE(int,sfsSpeakerAdd,(sfs_t *sfs, recog_t *r,
        unsigned short sID));
SFS_PROTOTYPE(int,sfsSpeakerList,(sfs_t *sfs, recog_t *r,
        unsigned short sID, unsigned short **sIDlist, unsigned short *listLen));
SFS_PROTOTYPE(int,sfsSpeakerRemove,(sfs_t *sfs, recog_t *r,
        unsigned short sID));
SFS_PROTOTYPE(int,sfsSpeakerReset,(sfs_t *sfs, recog_t *r,
        unsigned short sID));
SFS_PROTOTYPE(int,sfsSpeakerConfigure,(sfs_t *sfs, recog_t *r,
        unsigned short sID, unsigned short set,
        unsigned short *wantNumRecordings,
        unsigned short *haveNumRecordings, unsigned short *doneAdaptation));
SFS_PROTOTYPE(int,sfsSpeakerConfigSet,(sfs_t *sfs, recog_t *r,
	unsigned short sID, sfsSpeakerConfig_t key, float value));
SFS_PROTOTYPE(int,sfsSpeakerConfigGet,(sfs_t *sfs, recog_t *r,
        unsigned short sID, sfsSpeakerConfig_t key, float *value));
SFS_PROTOTYPE(int,sfsSpeakerSaveToFile,(sfs_t *sfs, recog_t *r,
        unsigned short sID, char *filename, unsigned short set));
SFS_PROTOTYPE(int,sfsSpeakerReadFromFile,(sfs_t *sfs, recog_t *r,
        unsigned short SID, char *filename, unsigned short flag,
        unsigned short force));
SFS_PROTOTYPE(void,sfsFreeFeats,(short **feats, unsigned long numFrames));
SFS_PROTOTYPE(int,sfsSpeakerCombine,(sfs_t *sfs, recog_t *r, searchs_t *s1, searchs_t*s2, unsigned short sid, searchs_t **outSearch, recog_t **outRecog));
SFS_PROTOTYPE(searchs_t *,sfsSpeakerCombineAndSaveToFile,(sfs_t *sfs,
       recog_t *r, searchs_t *s1, searchs_t *s2, unsigned short sID,
       char *searchFile, char *recogFile));
SFS_PROTOTYPE(int,sfsCheckRecording,(sfs_t *sfs, recog_t *r, 
        short *wave, unsigned long waveLen, unsigned long *feedback));
SFS_PROTOTYPE(int,sfsSpeakerStoreFeatsFromWaveform,(sfs_t *sfs, recog_t *r, searchs_t *search, unsigned short sID, short searchType, short *extras, short *wav, unsigned long wavLen, short force));

/* Tuning and UDT functions */
SFS_PROTOTYPE(udt_t *,sfsUdtCreate,(sfs_t *sfs,
        char *recogFile, char *phnSearchFile, char *udtDataFile,
        unsigned short maxNumEnroll, unsigned long samplerate));
SFS_PROTOTYPE(int,sfsUdtConfigSet,(sfs_t *sfs, udt_t *udt, 
        sfsUdtConfig_t key, float value));
SFS_PROTOTYPE(int,sfsUdtConfigGet,(sfs_t *sfs, udt_t *udt, 
        sfsUdtConfig_t key, float *value));
SFS_PROTOTYPE(int,sfsUdtCheckEnrollments,(sfs_t *sfs, udt_t *udt, 
        sfsUser_t *user, unsigned short whichUser,
        unsigned long *globalFeedback, 
        unsigned long *feedbackArray, float *phraseQuality));
SFS_PROTOTYPE(int, sfsUdtCheckValueGet, (sfs_t *sfs, udt_t *udt, sfsUdtConfig_t param, float *val, unsigned short numEnroll));
SFS_PROTOTYPE(int,sfsUdtEnroll,(sfs_t *sfs, udt_t *udt,
        unsigned short numUsers,
        sfsUser_t *user, searchs_t **outputSearch, recog_t **outputRecog,
        void *cbData, UDT_CBfunction_t *cbProc));
SFS_PROTOTYPE(int,sfsUdtSaveToFile,(sfs_t *sfs, searchs_t *search,
        recog_t *r, char *searchFilename, char *recogFilename));
SFS_PROTOTYPE(int,sfsUdtCacheSaveToFile,(sfs_t *sfs, udt_t *udt, const char *cacheFilename));
SFS_PROTOTYPE(int,sfsUdtCacheLoadFromFile,(sfs_t *sfs, sfsUser_t *user, unsigned short numUsers, udt_t *udt, const char *cacheFilename));
SFS_PROTOTYPE(void,sfsUdtDestroy,(udt_t *udt));
SFS_EXPERIMENTAL(int,sfsUdtEnrollEstimateParams,(sfs_t *sfs, udt_t *udt,
        sfsUser_t *user, unsigned short numUsers,  unsigned short whichUser, 
        float learnPS, float learnSV, float *paramAoffset, float *svThreshold));

/* phoneme recognition functions */
SFS_PROTOTYPE(phsearch_t *, sfsPhonemeSearchCreateFromFile,(sfs_t *sfs,
        recog_t *r, const char *filename, unsigned short nbest));
SFS_PROTOTYPE(int,sfsPhonemeRecogInit,(sfs_t *sfs, recog_t *r, phsearch_t *s,
        char keep));
SFS_PROTOTYPE(void,sfsPhonemeSearchDestroy,(phsearch_t *phsearch));

/* Miscellaneous speech functions */
SFS_PROTOTYPE(int ,sfsWaveFromFile,(sfs_t *sfs, const char *filename, short **speech, unsigned long *samples, unsigned long *sampleRate));
SFS_PROTOTYPE(int ,sfsWaveSaveToFile,(sfs_t *sfs, const char *filename, short *speech, unsigned long samples, unsigned long sampleRate));

/* Tts functions */
SFS_PROTOTYPE(tts_t *,sfsTtsCreateFromFile,(sfs_t *sfs, const char *voicefile));
SFS_PROTOTYPE(short *,sfsTtsSynthesize,(sfs_t *sfs, tts_t *tts, const char *text, float pitch, float duration, float speechrate, unsigned long *speechLen));
SFS_PROTOTYPE(unsigned long,sfsTtsPrep,(sfs_t *sfs, tts_t *tts, const char *text, float pitch, float duration, float speechrate, unsigned long *frameSz));
SFS_PROTOTYPE(short *,sfsTtsGetFrame,(sfs_t *sfs, tts_t *tts, unsigned long *speechLen));
SFS_PROTOTYPE(void,sfsTtsReset,(tts_t *tts));
SFS_PROTOTYPE(void,sfsTtsDestroy,(tts_t *tts));
SFS_PROTOTYPE(tts_t *,sfsTtsCreateFromStatic,(sfs_t *sfs, sdata_t *global, unsigned short idx));
SFS_PROTOTYPE(int,sfsTtsConfigGet,(sfs_t *sfs, tts_t *tts, sfsTtsConfig_t key, int *value));
SFS_PROTOTYPE(int,sfsTtsConfigSet,(sfs_t *sfs, tts_t *tts, sfsTtsConfig_t key, int value));


/* These functions are deprecated and will disappear in a future SDK release.
 * Do not use these in new code.
 */
SFS_DEPRECATED("use sfsSearchConfigGet() and sfsSearchConfigSet() instead",
int, sfsSearchConfigure,(sfs_t *sfs, recog_t *r, searchs_t *s, unsigned short set, float *pruning, float *beam, float *garbage, float *any, float *nota, float *longpen));
SFS_DEPRECATED("use sfsPhrasespotConfigGet() and sfsPhrasespotConfigSet() instead", int ,sfsPhrasespotConfigure,(sfs_t *sfs, recog_t *r, searchs_t *s, unsigned short set, float *paramAOffset, float *beam, float *absBeam, float *delay, float *longpen));
SFS_DEPRECATED("use sfsRecogConfigGet() and sfsRecogConfigSet() instead",
 int ,sfsRecogConfigure,(sfs_t *sfs, recog_t *r, unsigned short set, float *lsil, float *tsil, float *maxRec, float *minD, float *eSilenceT, float *eSpeechT, float *shortTermMs, float *longTermMs));
SFS_DEPRECATED("use standard OS audio interface instead",sfs_t *,sfsSessionCreateWithAudio,(char *hasMixer, sfsAudioCB_t *cb, void *cbData));
SFS_DEPRECATED("use standard OS audio interface instead",int ,sfsWavePlay,(sfs_t *sfs, short *buf, unsigned long len, unsigned long sampleRate, char block));
SFS_DEPRECATED("use standard OS audio interface instead",int ,sfsWavePlayStop,(sfs_t *sfs));
SFS_DEPRECATED("use standard OS audio interface instead",int ,sfsRecogLive,(sfs_t *sfs, recog_t *r, float *score, const char **res, unsigned short *stateResult, const char **wordAlign, const char **phoneAlign, const short **inSpeech, unsigned long *inSpeechSamples, const short **sdetSpeech, unsigned long *sdetSpeechSamples, char *haltSemaphore));
SFS_DEPRECATED("use standard OS audio interface instead",int ,sfsAudioGain,(sfs_t *sfs, recog_t *r, char set, float *gain));
SFS_DEPRECATED("use standard OS audio interface instead",unsigned long ,sfsAudioDiscardInitialSamples,(sfs_t *sfs,unsigned long len));

SFS_DEPRECATED("use sfsRecogCombine() instead", int,sfsSpeakerCombineSearchRecog,(sfs_t *sfs, searchs_t *s1, searchs_t *s2, recog_t *r1, recog_t *r2, searchs_t **outputSearch, recog_t **outRecog));

SFS_DEPRECATED("use the sfsUdt family of functions instead",
        tune_t *, sfsTuneCreateUDT,(sfs_t *sfs, recog_t *r,
        char **pro, short **audio, unsigned long *audioLen,
        unsigned short numEnroll, short trainSampWant, unsigned short userID,
        char *wordIDstring, char *phraseIDstring, unsigned short numParamA,
        short *paramA, unsigned short numParamB, unsigned short *paramB,
        unsigned short delay, float absbeam, unsigned long maxMemory,
        char requireSilence, unsigned long *globalFeedback,
        unsigned long *feedbackArray));
SFS_DEPRECATED("use the sfsUdt family of functions instead",
        int, sfsTuneAddDataFromDB,(sfs_t *sfs, tune_t *tune,
        recog_t *recogTuning, recog_t *recogUDT, unsigned long chunkIdx,
        unsigned long numChunks, char *field, unsigned short numEnroll,
        unsigned short paramD, unsigned short maxParamD,
        char **pro, char *phraseIDstring, long *t1, long *t2));
SFS_DEPRECATED("use the sfsUdt family of functions instead",
        searchs_t *, sfsTuneCreateSearch,(sfs_t *sfs, tune_t *tune,
        recog_t *r, pronuns_t *p, unsigned char hint, unsigned char SV));
SFS_DEPRECATED("use the sfsUdt family of functions instead",
        void, sfsTuneDestroy,(tune_t *tune));
SFS_DEPRECATED("use the sfsUdt family of functions instead",
        int, sfsTuneSaveToFile,(sfs_t *sfs, tune_t *t, const char *filename));
SFS_DEPRECATED("use the sfsUdt family of functions instead",
        tune_t *, sfsTuneCreateFromFile,
        (sfs_t *sfs, recog_t *r, pronuns_t *p, const char *filename));

#if defined(__cplusplus)
}
#endif
#endif /* SFS_FLUENT_H */
