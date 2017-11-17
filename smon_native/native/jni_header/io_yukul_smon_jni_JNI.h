/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_yukul_smon_jni_JNI */

#ifndef _Included_io_yukul_smon_jni_JNI
#define _Included_io_yukul_smon_jni_JNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    setPeriod
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_setPeriod
  (JNIEnv *, jobject, jint);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    setCallbcak
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_setCallbcak
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_start
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_stop
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getCpu
 * Signature: ()Lio/yukul/smon/jni/dto/CpuData;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getCpu
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getMem
 * Signature: ()Lio/yukul/smon/jni/dto/MemData;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getMem
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getSys
 * Signature: ()Lio/yukul/smon/jni/dto/SysData;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getSys
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getDisk
 * Signature: ()Lio/yukul/smon/jni/dto/DiskData;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getDisk
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getNet
 * Signature: ()Lio/yukul/smon/jni/dto/NetData;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getNet
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getProcList
 * Signature: ()Ljava/util/TreeSet;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getProcList
  (JNIEnv *, jobject);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    registProc
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_registProc
  (JNIEnv *, jobject, jstring);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    unregistProc
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_unregistProc
  (JNIEnv *, jobject, jstring);

/*
 * Class:     io_yukul_smon_jni_JNI
 * Method:    getProc
 * Signature: ()Lio/yukul/smon/jni/dto/ProcData;
 */
JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getProc
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
