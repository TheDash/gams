
#ifndef _GAMS_JAVA_ACQUIRE_VM_H_
#define _GAMS_JAVA_ACQUIRE_VM_H_

#include "gams_jni.h"

/**
 * @file Acquire_VM.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains helper classes for attaching and detaching to a VM.
 **/

namespace gams
{
  namespace utility
  {
    namespace java
    {
      /**
       * @class Acquire_VM
       * @brief This class encapsulates attaching and detaching to a VM
       **/
      class Acquire_VM
      {
      public:
        Acquire_VM()
        {
          needs_detach = !gams_jni_is_attached();
          env = gams_jni_get_env();
        }

        ~Acquire_VM()
        {
          if(needs_detach)
            jni_detach();
        }

        JNIEnv * env;
        bool needs_detach;
      };
    }
  }
}

#endif // not defined _GAMS_JAVA_ACQUIRE_VM_H_
