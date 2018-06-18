/**
 * Copyright (c) 2014 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following acknowledgments and disclaimers.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The names "Carnegie Mellon University," "SEI" and/or "Software
 *    Engineering Institute" shall not be used to endorse or promote products
 *    derived from this software without prior written permission. For written
 *    permission, please contact permission@sei.cmu.edu.
 *
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *    appear in their names without prior written permission of
 *    permission@sei.cmu.edu.
 *
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *
 *      This material is based upon work funded and supported by the Department
 *      of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *      University for the operation of the Software Engineering Institute, a
 *      federally funded research and development center. Any opinions,
 *      findings and conclusions or recommendations expressed in this material
 *      are those of the author(s) and do not necessarily reflect the views of
 *      the United States Department of Defense.
 *
 *      NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *      INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 *      UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *      IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *      FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *      OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *      NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *      TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 *
 *      This material has been approved for public release and unlimited
 *      distribution.
 **/

#include "Multicontroller.h"

#include <iostream>
#include <sstream>

#include "madara/utility/Utility.h"
#include "gams/algorithms/AlgorithmFactoryRepository.h"
#include "gams/platforms/PlatformFactoryRepository.h"
#include "gams/loggers/GlobalLogger.h"
#include "madara/utility/EpochEnforcer.h"

// Java-specific header includes
#ifdef _GAMS_JAVA_
#include "gams/algorithms/java/JavaAlgorithm.h"
#include "gams/platforms/java/JavaPlatform.h"
#include "gams/utility/java/Acquire_VM.h"
#endif

typedef  madara::knowledge::KnowledgeRecord::Integer  Integer;

typedef  madara::utility::EpochEnforcer<std::chrono::steady_clock> EpochEnforcer;

gams::controllers::Multicontroller::Multicontroller (
  madara::knowledge::KnowledgeBase & knowledge)
  : algorithm_ (0), knowledge_ (knowledge), platform_ (0)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::constructor:" \
    " default constructor called.\n");
}

gams::controllers::Multicontroller::~Multicontroller ()
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::destructor:" \
    " deleting algorithm.\n");
  delete algorithm_;

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::destructor:" \
    " deleting platform.\n");
  delete platform_;

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::destructor:" \
    " deleting accents.\n");
  for (algorithms::Algorithms::iterator i = accents_.begin ();
    i != accents_.end (); ++i)
  {
    delete *i;
  }
}

void gams::controllers::Multicontroller::add_platform_factory (
  const std::vector <std::string> & aliases,
  platforms::PlatformFactory * factory)
{
  gams::platforms::global_platform_factory()->add (aliases, factory);
}

void gams::controllers::Multicontroller::add_algorithm_factory (
  const std::vector <std::string> & aliases,
  algorithms::AlgorithmFactory * factory)
{
  gams::algorithms::global_algorithm_factory()->add (aliases, factory);
}

int
gams::controllers::Multicontroller::monitor (void)
{
  int result (0);

  if (platform_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::monitor:" \
      " calling platform_->sense ()\n");

    result = platform_->sense ();
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_WARNING,
      "gams::controllers::Multicontroller::monitor:" \
      " Platform undefined. Unable to call platform_->sense ()\n");
  }

  return result;
}

int
gams::controllers::Multicontroller::system_analyze (void)
{
  int return_value (0);
  //bool error (false);

  /**
   * Note that certain agent variables like command are kept local only.
   * @see gams::variables::Agent::init_vars
   * @see gams::variables::Swarm::init_vars
   **/

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::system_analyze:" \
    " checking agent and swarm commands\n");

  if (self_.agent.algorithm != "")
  {
    std::string prefix(self_.agent.algorithm.get_name() + ".");
    madara::knowledge::KnowledgeMap args(knowledge_.to_map_stripped(prefix));

    self_.agent.algorithm_args.sync_keys();

    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::system_analyze:" \
      " Processing agent command: %s\n", (*self_.agent.algorithm).c_str());

    init_algorithm (self_.agent.algorithm.to_string (), args);

    self_.agent.last_algorithm = self_.agent.algorithm.to_string ();

    // reset the command
    self_.agent.algorithm = "";
    self_.agent.last_algorithm_args.clear(true);
    self_.agent.algorithm_args.exchange(self_.agent.last_algorithm_args,
                                      true, true);
  }
  else if (swarm_.algorithm != "")
  {
    std::string prefix(swarm_.algorithm.get_name() + ".");
    madara::knowledge::KnowledgeMap args(knowledge_.to_map_stripped(prefix));

    swarm_.algorithm_args.sync_keys();

    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::system_analyze:" \
      " Processing swarm command: %s\n", (*swarm_.algorithm).c_str());

    init_algorithm (swarm_.algorithm.to_string (), args);

    self_.agent.last_algorithm = swarm_.algorithm.to_string ();

    // reset the command
    swarm_.algorithm = "";
    self_.agent.last_algorithm_args.clear(true);
    swarm_.algorithm_args.exchange(self_.agent.last_algorithm_args,
                                 true, true);
  }

  if (self_.agent.madara_debug_level !=
    (Integer)madara::logger::global_logger->get_level ())
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::system_analyze:" \
      " Settings MADARA debug level to %d\n", (int)*self_.agent.madara_debug_level);

    madara::logger::global_logger->set_level ((int)*self_.agent.madara_debug_level);
  }

  if (self_.agent.gams_debug_level !=
    (Integer)gams::loggers::global_logger->get_level ())
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::system_analyze:" \
      " Settings GAMS debug level to %d\n", (int)*self_.agent.gams_debug_level);

    gams::loggers::global_logger->set_level ((int)*self_.agent.gams_debug_level);
  }

  return return_value;
}

int
gams::controllers::Multicontroller::analyze (void)
{
  int return_value (0);

  if (platform_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::analyze:" \
      " calling platform_->analyze ()\n");

    return_value |= platform_->analyze ();
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::analyze:" \
      " Platform undefined. Unable to call platform_->analyze ()\n");
  }

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::analyze:" \
    " calling system_analyze ()\n");
  return_value |= system_analyze ();

  if (algorithm_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::analyze:" \
      " calling algorithm_->analyze ()\n");

    return_value |= algorithm_->analyze ();
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::analyze:" \
      " Algorithm undefined. Unable to call algorithm_->analyze ()\n");
  }


  if (accents_.size () > 0)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::analyze:" \
      " calling analyze on accents\n");
    for (algorithms::Algorithms::iterator i = accents_.begin ();
      i != accents_.end (); ++i)
    {
      (*i)->analyze ();
    }
  }

  return return_value;
}

int
gams::controllers::Multicontroller::plan (void)
{
  int return_value (0);

  if (algorithm_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::plan:" \
      " calling algorithm_->plan ()\n");

    return_value |= algorithm_->plan ();
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::plan:" \
      " Algorithm undefined. Unable to call algorithm_->plan ()\n");
  }

  if (accents_.size () > 0)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::plan:" \
      " calling plan on accents\n");

    for (algorithms::Algorithms::iterator i = accents_.begin ();
      i != accents_.end (); ++i)
    {
      (*i)->plan ();
    }
  }

  return return_value;
}

int
gams::controllers::Multicontroller::execute (void)
{
  int return_value (0);

  if (algorithm_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::execute:" \
      " calling algorithm_->execute ()\n");

    return_value |= algorithm_->execute ();
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_WARNING,
      "gams::controllers::Multicontroller::execute:" \
      " Algorithm undefined. Unable to call algorithm_->execute ()\n");
  }

  if (accents_.size () > 0)
  {
    for (algorithms::Algorithms::iterator i = accents_.begin ();
      i != accents_.end (); ++i)
    {
      (*i)->execute ();
    }
  }

  return return_value;
}

int
gams::controllers::Multicontroller::run_once_ (void)
{
  // return value
  int return_value (0);

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " calling monitor ()\n");

  // lock the context from any external updates
  madara::knowledge::ContextGuard guard (knowledge_);

  return_value |= monitor ();

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " after monitor (), %d modifications to send\n",
    (int)knowledge_.get_context ().get_modifieds ().size ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_DETAILED,
    "%s\n",
    knowledge_.debug_modifieds ().c_str ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " calling analyze ()\n");

  return_value |= analyze ();

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " after analyze (), %d modifications to send\n",
    (int)knowledge_.get_context ().get_modifieds ().size ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_DETAILED,
    "%s\n",
    knowledge_.debug_modifieds ().c_str ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " calling plan ()\n");

  return_value |= plan ();

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " after plan (), %d modifications to send\n",
    (int)knowledge_.get_context ().get_modifieds ().size ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_DETAILED,
    "%s\n",
    knowledge_.debug_modifieds ().c_str ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " calling execute ()\n");

  return_value |= execute ();

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " after execute (), %d modifications to send\n",
    (int)knowledge_.get_context ().get_modifieds ().size ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_DETAILED,
    "%s\n",
    knowledge_.debug_modifieds ().c_str ());

  return return_value;
}

int
gams::controllers::Multicontroller::run_once (void)
{
  // return value
  int return_value (run_once_ ());

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::run:" \
    " sending updates\n");

  // send modified values through network
  knowledge_.send_modifieds ();

  return return_value;
}

int
gams::controllers::Multicontroller::run (double ,
  double , double )
{
  return 0;
}

void
gams::controllers::Multicontroller::init_accent (const std::string & algorithm,
const madara::knowledge::KnowledgeMap & args)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_accent:" \
    " initializing accent %s\n", algorithm.c_str ());

  if (algorithm == "")
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_ERROR,
      "gams::controllers::Multicontroller::init_accent:" \
      " ERROR: accent name is null\n");
  }
  else
  {
    // create new accent pointer and algorithm factory
    algorithms::BaseAlgorithm * new_accent (0);
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_accent:" \
      " factory is creating accent %s\n", algorithm.c_str ());

    new_accent = algorithms::global_algorithm_factory()->create (algorithm, args);

    if (new_accent)
    {
      accents_.push_back (new_accent);
    }
    else
    {
      madara_logger_ptr_log (gams::loggers::global_logger.get (),
        gams::loggers::LOG_ERROR,
        "gams::controllers::Multicontroller::init_accent:" \
        " ERROR: created accent is null.\n");
    }
  }
}

void gams::controllers::Multicontroller::clear_accents (void)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::clear_accents:" \
    " deleting and clearing all accents\n");

  for (unsigned int i = 0; i < accents_.size (); ++i)
  {
    delete accents_[i];
  }

  accents_.clear ();
}
void
gams::controllers::Multicontroller::init_algorithm (
const std::string & algorithm, const madara::knowledge::KnowledgeMap & args)
{
  // initialize the algorithm

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_algorithm:" \
    " initializing algorithm %s\n", algorithm.c_str ());

  if (algorithm == "")
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "Algorithm is empty.\n\n" \
      "SUPPORTED ALGORITHMS:\n" \
      "  bridge | bridging\n" \
      "  random area coverage\n" \
      "  priotized area coverage\n"
      );
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_algorithm:" \
      " deleting old algorithm\n");

    delete algorithm_;

    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_algorithm:" \
      " factory is creating algorithm %s\n", algorithm.c_str ());

    algorithm_ = algorithms::global_algorithm_factory()->create (
      algorithm, args);

    if (algorithm_ == 0)
    {
      // the user is going to expect this kind of error to be printed immediately
      madara_logger_ptr_log (gams::loggers::global_logger.get (),
        gams::loggers::LOG_MAJOR,
        "gams::controllers::Multicontroller::init_algorithm:" \
        " failed to create algorithm\n");
    }
    else
    {
#ifdef _GAMS_JAVA_
      algorithms::JavaAlgorithm * jalg =
        dynamic_cast <algorithms::JavaAlgorithm *> (algorithm_);

      if (jalg)
      {
        // Acquire the Java virtual machine
        gams::utility::java::Acquire_VM jvm;

        jclass controller_class = gams::utility::java::find_class (
          jvm.env, "ai/gams/controllers/Multicontroller");
        jobject alg = jalg->get_java_instance ();
        jclass alg_class = jvm.env->GetObjectClass (alg);
        
        jmethodID init_call = jvm.env->GetMethodID (alg_class,
          "init", "(Lai/gams/controllers/Multicontroller;)V");
        jmethodID controllerFromPointerCall = jvm.env->GetStaticMethodID (
          controller_class,
          "fromPointer", "(JZ)Lai/gams/controllers/Multicontroller;");

        if (init_call && controllerFromPointerCall)
        {
          madara_logger_ptr_log (gams::loggers::global_logger.get (),
            gams::loggers::LOG_MAJOR,
            "gams::controllers::Multicontroller::init_algorithm:" \
            " Calling BaseAlgorithm init method.\n");
          jobject controller = jvm.env->CallStaticObjectMethod (controller_class,
            controllerFromPointerCall, (jlong)this, (jboolean)false); 

          jvm.env->CallVoidMethod (
            alg, init_call, controller);

          jvm.env->DeleteLocalRef (controller);
        }
        else
        {
          madara_logger_ptr_log (gams::loggers::global_logger.get (),
            gams::loggers::LOG_ERROR,
            "gams::controllers::Multicontroller::init_algorithm:" \
            " ERROR. Could not locate init and fromPointer calls in "
            "Multicontroller. Unable to initialize algorithm.\n");
        }

        jvm.env->DeleteLocalRef (alg_class);
        jvm.env->DeleteLocalRef (alg);
        jvm.env->DeleteWeakGlobalRef (controller_class);
      }
      else
      {
        init_vars (*algorithm_);
      }
#else
      init_vars (*algorithm_);
#endif
    }
  }
}

void
gams::controllers::Multicontroller::init_platform (
  const std::string & platform,
  const madara::knowledge::KnowledgeMap & args)
{
  // initialize the platform

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_platform:" \
    " initializing platform %s\n", platform.c_str ());

  if (platform == "")
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_ERROR,
      "Platform is empty.\n\n" \
      "SUPPORTED PLATFORMS:\n" \
      "  drone-rk\n" \
      "  vrep\n" \
      );
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_platform:" \
      " deleting old platform\n");

    delete platform_;
    platforms::PlatformFactoryRepository factory (&knowledge_, &sensors_, &platforms_, &self_);

    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_platform:" \
      " factory is creating platform %s\n", platform.c_str ());

    platform_ = factory.create (platform, args);

    init_vars (*platform_);

    if (algorithm_)
    {
      madara_logger_ptr_log (gams::loggers::global_logger.get (),
        gams::loggers::LOG_MAJOR,
        "gams::controllers::Multicontroller::init_platform:" \
        " algorithm is already initialized. Updating to new platform\n");

      algorithm_->set_platform (platform_);
    }
  }
}

void gams::controllers::Multicontroller::init_algorithm (algorithms::BaseAlgorithm * algorithm)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_algorithm:" \
    " deleting old algorithm\n");

  delete algorithm_;
  algorithm_ = algorithm;

  if (algorithm_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_algorithm:" \
      " initializing vars in algorithm\n");

    init_vars (*algorithm_);
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_algorithm:" \
      " algorithm was reset to none\n");
  }
}


void gams::controllers::Multicontroller::init_platform (platforms::BasePlatform * platform)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_platform:" \
    " deleting old platform\n");

  delete platform_;
  platform_ = platform;

  if (platform_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_platform:" \
      " initializing vars in platform\n");

    init_vars (*platform_);

    if (algorithm_)
    {
      madara_logger_ptr_log (gams::loggers::global_logger.get (),
        gams::loggers::LOG_MAJOR,
        "gams::controllers::Multicontroller::init_platform:" \
        " algorithm is already initialized. Updating to new platform\n");

      algorithm_->set_platform (platform_);
    }
  }
  else
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_platform:" \
      " platform was reset to none\n");
  }
}

#ifdef _GAMS_JAVA_

void gams::controllers::Multicontroller::init_algorithm (jobject algorithm)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_algorithm (java):" \
    " deleting old algorithm\n");

  delete algorithm_;

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_algorithm (java):" \
    " creating new Java algorithm\n");

  algorithm_ = new gams::algorithms::JavaAlgorithm (algorithm);

  if (algorithm_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_algorithm (java):" \
      " initializing vars for algorithm\n");

    init_vars (*algorithm_);
  }
}


void gams::controllers::Multicontroller::init_platform (jobject platform)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_platform (java):" \
    " deleting old platform\n");

  delete platform_;

  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_platform (java):" \
    " creating new Java platform\n");

  platform_ = new gams::platforms::JavaPlatform (platform);

  if (platform_)
  {
    madara_logger_ptr_log (gams::loggers::global_logger.get (),
      gams::loggers::LOG_MAJOR,
      "gams::controllers::Multicontroller::init_platform (java):" \
      " initializing vars for platform\n");

    init_vars (*platform_);

    if (algorithm_)
    {
      madara_logger_ptr_log (gams::loggers::global_logger.get (),
        gams::loggers::LOG_MAJOR,
        "gams::controllers::Multicontroller::init_platform (java):" \
        " Algorithm exists. Updating its platform.\n");

      algorithm_->set_platform (platform_);
    }
  }
}

#endif

void
gams::controllers::Multicontroller::init_vars (
const Integer & id,
const Integer & processes)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_vars:" \
    " %" PRId64 " id, %" PRId64 " processes\n", id, processes);

  // initialize the agents, swarm, and self variables
  variables::init_vars (agents_, knowledge_, processes);
  swarm_.init_vars (knowledge_, processes);
  self_.init_vars (knowledge_, id);
}

void
gams::controllers::Multicontroller::init_vars (platforms::BasePlatform & platform)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_vars:" \
    " initializing platform's vars\n");

  platform.knowledge_ = &knowledge_;
  platform.self_ = &self_;
  platform.sensors_ = &sensors_;
}


void
gams::controllers::Multicontroller::init_vars (algorithms::BaseAlgorithm & algorithm)
{
  madara_logger_ptr_log (gams::loggers::global_logger.get (),
    gams::loggers::LOG_MAJOR,
    "gams::controllers::Multicontroller::init_vars:" \
    " initializing algorithm's vars\n");

  algorithm.agents_ = &agents_;
  algorithm.knowledge_ = &knowledge_;
  algorithm.platform_ = platform_;
  algorithm.self_ = &self_;
  algorithm.sensors_ = &sensors_;
}

gams::algorithms::BaseAlgorithm *
gams::controllers::Multicontroller::get_algorithm (void)
{
  return algorithm_;
}

gams::platforms::BasePlatform *
gams::controllers::Multicontroller::get_platform (void)
{
  return platform_;
}
