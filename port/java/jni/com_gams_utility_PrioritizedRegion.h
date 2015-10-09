/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "gams/GAMSExport.h"
/* Header for class com_gams_utility_PrioritizedRegion */

#ifndef _Included_com_gams_utility_PrioritizedRegion
#define _Included_com_gams_utility_PrioritizedRegion
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_PrioritizedRegion
 * Signature: ()J
 */
GAMSExport jlong JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1PrioritizedRegion
  (JNIEnv *, jobject);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
GAMSExport jstring JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1toString
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_fromContainer
 * Signature: (JJLjava/lang/String;)V
 */
GAMSExport void JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1fromContainer
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_toContainer
 * Signature: (JJLjava/lang/String;)V
 */
GAMSExport void JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1toContainer
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_modify
 * Signature: (J)V
 */
GAMSExport void JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1modify
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_freePrioritizedRegion
 * Signature: (J)V
 */
GAMSExport void JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1freePrioritizedRegion
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_getPriority
 * Signature: (J)J
 */
GAMSExport jlong JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1getPriority
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_PrioritizedRegion
 * Method:    jni_setPriority
 * Signature: (JJ)V
 */
GAMSExport void JNICALL Java_com_gams_utility_PrioritizedRegion_jni_1setPriority
  (JNIEnv *, jobject, jlong, jlong);

#ifdef __cplusplus
}
#endif
#endif
