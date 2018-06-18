
#include "ai_gams_variables_Swarm.h"
#include "gams/variables/Swarm.h"

namespace containers = madara::knowledge::containers;
namespace engine = madara::knowledge;
namespace variables = gams::variables;

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_Swarm
 * Signature: ()J
 */
jlong JNICALL Java_ai_gams_variables_Swarm_jni_1Swarm__
  (JNIEnv * , jobject)
{
  return (jlong) new variables::Swarm ();
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_Swarm
 * Signature: (J)J
 */
jlong JNICALL Java_ai_gams_variables_Swarm_jni_1Swarm__J
  (JNIEnv * , jobject, jlong cptr)
{
  return (jlong) new variables::Swarm (*(variables::Swarm *)cptr);
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_freeSwarm
 * Signature: (J)V
 */
void JNICALL Java_ai_gams_variables_Swarm_jni_1freeSwarm
  (JNIEnv * , jclass, jlong cptr)
{
  delete (variables::Swarm *) cptr;
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_init
 * Signature: (JJJLjava/lang/String;)V
 */
void JNICALL Java_ai_gams_variables_Swarm_jni_1init
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  variables::Swarm * current = (variables::Swarm *) cptr;

  if (current)
  {
    const char * str_name = env->GetStringUTFChars(name, 0);

    if (type == 0)
    {
      engine::KnowledgeBase * kb = (engine::KnowledgeBase *) context;
      current->init_vars (*kb);
    }

    env->ReleaseStringUTFChars(name, str_name);
  }
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_ai_gams_variables_Swarm_jni_1toString
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::Swarm * current = (variables::Swarm *) cptr;
  if (current)
  {
    result = env->NewStringUTF("Swarm");
  }
  else
  {
    result = env->NewStringUTF ("");
  }

  return result;
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_getCommand
 * Signature: (J)J
 */
jlong JNICALL Java_ai_gams_variables_Swarm_jni_1getCommand
  (JNIEnv * , jobject, jlong cptr)
{
  variables::Swarm * current = (variables::Swarm *) cptr;

  return (jlong) &current->algorithm;
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_getArgs
 * Signature: (J)J
 */
jlong JNICALL Java_ai_gams_variables_Swarm_jni_1getArgs
  (JNIEnv * , jobject, jlong cptr)
{
  variables::Swarm * current = (variables::Swarm *) cptr;

  return (jlong) &current->algorithm_args;
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_getMinAlt
 * Signature: (J)J
 */
jlong JNICALL Java_ai_gams_variables_Swarm_jni_1getMinAlt
  (JNIEnv * , jobject, jlong cptr)
{
  variables::Swarm * current = (variables::Swarm *) cptr;

  return (jlong) &current->min_alt;
}

/*
 * Class:     ai_gams_variables_Swarm
 * Method:    jni_getSize
 * Signature: (J)J
 */
jlong JNICALL Java_ai_gams_variables_Swarm_jni_1getSize
  (JNIEnv * , jobject, jlong cptr)
{
  variables::Swarm * current = (variables::Swarm *) cptr;

  return (jlong) &current->size;
}
