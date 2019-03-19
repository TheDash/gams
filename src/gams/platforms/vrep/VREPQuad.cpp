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

#ifdef _GAMS_VREP_ // only compile this if we are simulating in VREP

#include "VREPQuad.h"


#include <iostream>
#include <cmath>

#include "madara/knowledge/containers/DoubleVector.h"

#include "gams/variables/Sensor.h"

using std::endl;
using std::cout;
using std::string;
using madara::knowledge::containers::NativeDoubleVector;
using madara::knowledge::containers::Double;

const string gams::platforms::VREPQuad::DEFAULT_MODEL_FILENAME(
  "Quadricopter_NoCamera.ttm");

const string gams::platforms::VREPQuad::DEFAULT_MODEL(
 (getenv("GAMS_ROOT") == 0) ? 
  DEFAULT_MODEL_FILENAME : // if GAMS_ROOT is not defined, assume in working dir
 (string(getenv("GAMS_ROOT")) + "/resources/vrep/" + DEFAULT_MODEL_FILENAME)
  );

std::string
gams::platforms::VREPQuadFactory::get_default_model()
{
  return VREPQuad::DEFAULT_MODEL;
}

std::string
gams::platforms::VREPQuadFactory::get_default_model(std::string directory)
{
  return directory + "/" + VREPQuad::DEFAULT_MODEL_FILENAME;
}

gams::platforms::BasePlatform *
gams::platforms::VREPQuadFactory::create(
  const madara::knowledge::KnowledgeMap & args,
  madara::knowledge::KnowledgeBase * knowledge,
  variables::Sensors * sensors,
  variables::Platforms * platforms,
  variables::Self * self)
{
  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_MINOR,
    "entering gams::platforms::VREPQuadFactory::create\n");

  BasePlatform * result(0);

  if (knowledge && sensors && platforms && self)
  {
    if (knowledge->get_num_transports() == 0)
    {
      madara::transport::QoSTransportSettings settings;

      settings.type = madara::transport::MULTICAST;
      settings.hosts.push_back("239.255.0.1:4150");

      knowledge_->attach_transport("", settings);
      knowledge_->activate_transport();

      madara_logger_ptr_log(gams::loggers::global_logger.get(),
        gams::loggers::LOG_MINOR,
         "gams::platforms::VREPQuadFactory::create:" \
        " no transports found, attaching multicast\n");
    }

    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_MAJOR,
       "gams::platforms::VREPQuadFactory::create:" \
      " creating VREPQuad object\n");

    // specify the model file
    string model_file;
    simxUChar is_client_side; // file is on server

    madara::knowledge::KnowledgeMap::const_iterator client_side_found =
      args.find("client_side");
    madara::knowledge::KnowledgeMap::const_iterator model_file_found =
      args.find("model_file");
    madara::knowledge::KnowledgeMap::const_iterator resource_dir_found =
      args.find("resource_dir");

    if (client_side_found != args.end() &&
        client_side_found->second.to_integer() == 1)
    {
      is_client_side = 1;
    }
    else
    {
      is_client_side = 0;
    }

    if (model_file_found != args.end())
    {
      model_file = model_file_found->second.to_string();
    }
    else if (resource_dir_found != args.end())
    {
      model_file = get_default_model(resource_dir_found->second.to_string());
    }
    else
    {
      model_file = get_default_model();
    }

    result = create_quad(model_file, is_client_side, knowledge, sensors, 
      platforms, self);
  }
  else
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ERROR,
       "gams::platforms::VREPQuadFactory::create:" \
      " invalid knowledge, sensors, platforms, or self\n");
  }

  if (result == 0)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_MAJOR,
       "gams::platforms::VREPQuadFactory::create:" \
      " error creating VREPQuad object\n");
  }

  return result;
}

gams::platforms::VREPQuad *
gams::platforms::VREPQuadFactory::create_quad(
  std::string model_file, 
  simxUChar is_client_side, 
  madara::knowledge::KnowledgeBase * knowledge,
  variables::Sensors * sensors,
  variables::Platforms * platforms,
  variables::Self * self)
{
  return new VREPQuad(model_file, is_client_side, knowledge, sensors, 
    platforms, self);
}

gams::platforms::VREPQuad::VREPQuad(
  std::string model_file, 
  simxUChar is_client_side, 
  madara::knowledge::KnowledgeBase * knowledge,
  variables::Sensors * sensors,
  variables::Platforms * platforms,
  variables::Self * self) :
  VREPBase(model_file, is_client_side, knowledge, sensors, self)
{
  if (knowledge && sensors && platforms && self)
  {
   (*platforms)[get_id()].init_vars(*knowledge, get_id());
    status_ =(*platforms)[get_id()];

    self->agent.desired_altitude = self->id.to_integer() + 1;
  }
}


void
gams::platforms::VREPQuad::add_model_to_environment(const string &model_file, 
  const simxUChar is_client_side)
{
  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_MAJOR,
    "gams::platforms::VREPQuad::add_model_to_environment(" \
    "%s, %d)\n", model_file.c_str(),(int)is_client_side);

  if (simxLoadModel(client_id_, model_file.c_str(), is_client_side, &node_id_,
    simx_opmode_oneshot_wait) != simx_error_noerror)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ERROR,
       "gams::platforms::VREPQuad::add_model_to_environment:" \
      " error loading model in vrep\n");
    exit(-1);
  }

  if (node_id_ < 0)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ERROR,
       "gams::platforms::VREPQuad::add_model_to_environment:" \
      " invalid handle id\n");
    exit(-1);
  }
}

std::string gams::platforms::VREPQuad::get_id() const
{
  return "vrep_quad";
}

std::string gams::platforms::VREPQuad::get_name() const
{
  return "VREP Quadcopter";
}

void
gams::platforms::VREPQuad::get_target_handle()
{
  //find the dummy base sub-object
  simxInt handlesCount = 0,*handles = NULL;
  simxInt parentsCount = 0,*parents = NULL;
  simxGetObjectGroupData(client_id_, sim_object_dummy_type, 2, &handlesCount,
    &handles, &parentsCount, &parents, NULL, NULL, NULL, NULL,
    simx_opmode_oneshot_wait);

  // find node base
  simxInt nodeBase = -1;
  for (simxInt i = 0; i < handlesCount; ++i)
  {
    if (parents[i] == node_id_)
    {
      nodeBase = handles[i];
      break;
    }
  }

  // find the target sub-object of the base sub-object
  node_target_ = -1;
  simxGetObjectChild(client_id_, nodeBase, 0, &node_target_,
    simx_opmode_oneshot_wait);

  if (node_target_ < 0)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_ERROR,
       "gams::platforms::VREPQuad::get_target_handle:" \
      " invalid target handle id\n");
  }
}

#endif // _GAMS_VREP_
