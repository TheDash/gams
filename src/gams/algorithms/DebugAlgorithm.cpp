/**
 * Copyright(c) 2014 Carnegie Mellon University. All Rights Reserved.
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

#include "DebugAlgorithm.h"
#include "gams/loggers/GlobalLogger.h"

#include <iostream>

gams::algorithms::BaseAlgorithm *
gams::algorithms::DebugAlgorithmFactory::create(
  const madara::knowledge::KnowledgeMap & args,
  madara::knowledge::KnowledgeBase * knowledge,
  platforms::BasePlatform * platform,
  variables::Sensors * sensors,
  variables::Self * self,
  variables::Agents * /*agents*/)
{
  std::string executions(".executions");

  if (args.size() != 0)
  {
    madara::knowledge::KnowledgeMap::const_iterator i = args.find("0");
    if (i != args.end())
      executions = i->second.to_string();
  }

  return new DebugAlgorithm(knowledge, platform, sensors, self, executions);
}

gams::algorithms::DebugAlgorithm::DebugAlgorithm(
  madara::knowledge::KnowledgeBase * knowledge,
  platforms::BasePlatform * platform,
  variables::Sensors * sensors,
  variables::Self * self,
  const std::string & executions_location)
  : BaseAlgorithm(knowledge, platform, sensors, self)
{
  if (knowledge)
  {
    status_.init_vars(*knowledge, "debug", self->agent.prefix);
    status_.init_variable_values();
    k_executions_.set_name(executions_location, *knowledge);
  }
}

gams::algorithms::DebugAlgorithm::~DebugAlgorithm()
{
}

void
gams::algorithms::DebugAlgorithm::operator=(const DebugAlgorithm & rhs)
{
  if (this != &rhs)
  {
    this->platform_ = rhs.platform_;
    this->sensors_ = rhs.sensors_;
    this->self_ = rhs.self_;
    this->status_ = rhs.status_;
    this->k_executions_ = rhs.k_executions_;
  }
}

int
gams::algorithms::DebugAlgorithm::analyze(void)
{
  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_ALWAYS,
    "%" PRId64 ":%" PRId64 ": algorithm.analyze()\n",
    *self_->id, *k_executions_);
  
  if (platform_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.platform_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.platform_ is not set\n",
      *self_->id, *k_executions_);
  }

  if (sensors_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.sensors_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.sensors_ is not set\n",
      *self_->id, *k_executions_);
  }

  if (self_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.self_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.self_ is not set\n",
      *self_->id, *k_executions_);
  }

  status_.waiting = 1;
  status_.ok = 1;
  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_ALWAYS,
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.ok == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.paused == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.waiting == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.deadlocked == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.failed == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.unknown == %" PRId64 "\n"
    ,
    *self_->id, *k_executions_, *status_.ok,
    *self_->id, *k_executions_, *status_.paused,
    *self_->id, *k_executions_, *status_.waiting,
    *self_->id, *k_executions_, *status_.deadlocked,
    *self_->id, *k_executions_, *status_.failed,
    *self_->id, *k_executions_, *status_.unknown);

  return OK;
}

int
gams::algorithms::DebugAlgorithm::execute(void)
{
  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_ALWAYS,
    "%" PRId64 ":%" PRId64 ": algorithm.execute()\n",
    *self_->id, *k_executions_);
  
  if (platform_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.platform_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.platform_ is not set\n",
      *self_->id, *k_executions_);
  }

  if (sensors_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.sensors_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.sensors_ is not set\n",
      *self_->id, *k_executions_);
  }

  if (self_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.self_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.self_ is not set\n",
      *self_->id, *k_executions_);
  }

  status_.waiting = 0;

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_ALWAYS,
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.ok == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.paused == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.waiting == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.deadlocked == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.failed == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.unknown == %" PRId64 "\n",
    *self_->id, *k_executions_, *status_.ok,
    *self_->id, *k_executions_, *status_.paused,
    *self_->id, *k_executions_, *status_.waiting,
    *self_->id, *k_executions_, *status_.deadlocked,
    *self_->id, *k_executions_, *status_.failed,
    *self_->id, *k_executions_, *status_.unknown);

  if (platform_)
  {
    pose::Position next(platform_->get_frame(), 1, 2, 3);
    platform_->move(next);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ERROR,
      "%" PRId64 ":%" PRId64 ":  ERROR: platform_ is null. Cannot call move().\n",
      *self_->id, *k_executions_);
  }

  ++k_executions_;

  return 0;
}


int
gams::algorithms::DebugAlgorithm::plan(void)
{
  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_ALWAYS,
    "%" PRId64 ":%" PRId64 ": algorithm.plan()\n",
    *self_->id, *k_executions_);
  
  if (platform_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.platform_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.platform_ is not set\n",
      *self_->id, *k_executions_);
  }

  if (sensors_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.sensors_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.sensors_ is not set\n",
      *self_->id, *k_executions_);
  }

  if (self_)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   algorithm.self_ is set\n",
      *self_->id, *k_executions_);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ALWAYS,
      "%" PRId64 ":%" PRId64 ":   ERROR: algorithm.self_ is not set\n",
      *self_->id, *k_executions_);
  }

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_ALWAYS,
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.ok == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.paused == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.waiting == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.deadlocked == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.failed == %" PRId64 "\n"
    "%" PRId64 ":%" PRId64 ":     algorithm.status_.unknown == %" PRId64 "\n",
    *self_->id, *k_executions_, *status_.ok,
    *self_->id, *k_executions_, *status_.paused,
    *self_->id, *k_executions_, *status_.waiting,
    *self_->id, *k_executions_, *status_.deadlocked,
    *self_->id, *k_executions_, *status_.failed,
    *self_->id, *k_executions_, *status_.unknown);

  return 0;
}
