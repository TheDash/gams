/**
 * Copyright (c) 2014-2018 Carnegie Mellon University. All Rights Reserved.
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

/**
 * @file Agent.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the definition of the agent-prefixed MADARA variables
 **/

#ifndef   _GAMS_VARIABLES_DEVICES_H_
#define   _GAMS_VARIABLES_DEVICES_H_

#include <vector>
#include <map>
#include <string>

#include "gams/GamsExport.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/containers/Double.h"
#include "madara/knowledge/containers/String.h"
#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "madara/knowledge/containers/Vector.h"
#include "madara/knowledge/containers/Map.h"
#include "madara/knowledge/KnowledgeBase.h"
#include "AccentStatus.h"
#include "gams/groups/GroupBase.h"

namespace gams
{
  namespace variables
  {
    /**
    * A container for agent information
    **/
    class GAMS_EXPORT Agent
    {
    public:
      /**
       * Constructor
       **/
      Agent ();

      /**
       * Destructor
       **/
      ~Agent ();

      /**
       * Assignment operator
       * @param  agent   agent to copy
       **/
      void operator= (const Agent & agent);

      /**
      * Initializes variable containers
      * @param   knowledge  the variable context
      * @param   prefix     the prefix of the agent in the knowledge base
      **/
      void init_vars (madara::knowledge::KnowledgeBase & knowledge,
        const std::string & prefix);

      /**
       * Initializes variable containers
       * @param   knowledge  the variable context
       * @param   id         node identifier
       **/
      void init_vars (madara::knowledge::KnowledgeBase & knowledge,
        const madara::knowledge::KnowledgeRecord::Integer& id);
      
      /**
       * Initializes variable containers
       * @param   knowledge  the variable context
       * @param   id         node identifier
       **/
      void init_vars (madara::knowledge::Variables & knowledge,
        const madara::knowledge::KnowledgeRecord::Integer& id);

      /**
       * Checks if the prefix points to an agent in the knowledge base. This
       * is a simple check that looks for prefix.location being set.
       **/
      static bool is_agent (madara::knowledge::KnowledgeBase & knowledge,
        const std::string prefix);

      /// the acceleration vector of the platform
      madara::knowledge::containers::NativeDoubleArray acceleration;
      
      /// the battery indicator for this agent
      madara::knowledge::containers::Integer battery_remaining;

      /// indicator for whether or not the agent is busy with a mission
      madara::knowledge::containers::Integer bridge_id;

      /// agent specific command 
      madara::knowledge::containers::String algorithm;

      /// the algorithm id (useful if algorithm requests may need to be resent)
      madara::knowledge::containers::Integer algorithm_id;

      /// number of algorithm rejections (bad name or id)
      madara::knowledge::containers::Integer algorithm_rejects;

      /// number of algorithm accepts / changes
      madara::knowledge::containers::Integer algorithm_accepts;

      /// agent specific command has changed
      madara::knowledge::containers::Integer algorithm_changed;

      /// arguments for algorithm
      madara::knowledge::containers::Map algorithm_args;

      /// Last command
      madara::knowledge::containers::String last_algorithm;

      /// Last command
      madara::knowledge::containers::Integer last_algorithm_id;

      /// Last command args
      madara::knowledge::containers::Map last_algorithm_args;

      /// agent specific command
      madara::knowledge::containers::String coverage_type;

      /// desired altitude in meters
      madara::knowledge::containers::Double desired_altitude;

      /// the destination location
      madara::knowledge::containers::NativeDoubleArray dest;

      /// the destination orientation
      madara::knowledge::containers::NativeDoubleArray dest_orientation;
      
      /// the home location
      madara::knowledge::containers::NativeDoubleArray home;

      /// the mobility indicator for this agent (true if mobile)
      madara::knowledge::containers::Integer is_mobile;

      /// the location, usually encoded in GPS, for this agent
      madara::knowledge::containers::NativeDoubleArray location;

      /// the angle for this device (roll, pitch, yaw)
      madara::knowledge::containers::NativeDoubleArray orientation;
      
      /// the minimum altitude for this agent
      madara::knowledge::containers::Double min_alt;
      
      /// indicator for next type of area coverage requested (queue like)
      madara::knowledge::containers::String next_coverage_type;

      /// indicator for next assigned search area id
      madara::knowledge::containers::Integer search_area_id;

      /// the source location
      madara::knowledge::containers::NativeDoubleArray source;

      /// the source angle
      madara::knowledge::containers::NativeDoubleArray source_orientation;
      
      /// indicator for temperature
      madara::knowledge::containers::Double temperature;

      /// the velocity vector of the platform
      madara::knowledge::containers::NativeDoubleArray velocity;
      
      /// container for accents
      AccentStatuses accents;

      /// the MADARA debug level
      madara::knowledge::containers::Integer madara_debug_level;

      /// the GAMS debug level
      madara::knowledge::containers::Integer gams_debug_level;

      /// the rate to send messages
      madara::knowledge::containers::Double send_hz;

      /// the rate to process the algorithm and platform MAPE loop
      madara::knowledge::containers::Double loop_hz;

      /// the prefix for this agent
      std::string prefix;

    protected:
      /**
       * Create agent/local agent name
       * @param id  id of agent as string
       * @return agent variable name
       */
      static std::string make_variable_name (
        const madara::knowledge::KnowledgeRecord::Integer& id);

      /**
       * Set variable settings
       */
      void init_variable_settings ();
    };

    /**
     * An array of agent knowledge
     **/
    typedef std::vector <Agent>   Agents;

    /**
    * A map of agent prefixes to agent knowledge
    **/
    typedef std::map <std::string, Agent>   AgentMap;

    /**
      * Initializes agent containers
      * @param   variables  the variables to initialize
      * @param   knowledge  the knowledge base that houses the variables
      * @param   processes  the number of processes in the agent swarm
      **/
    GAMS_EXPORT void init_vars (Agents & variables,
      madara::knowledge::KnowledgeBase & knowledge,
      const madara::knowledge::KnowledgeRecord::Integer& processes);

    /**
    * Initializes agent containers
    * @param   variables  the variables to initialize
    * @param   knowledge  the knowledge base that houses the variables
    * @param   group      a group containing all agents of interest
    **/
    GAMS_EXPORT void init_vars (Agents & variables,
      madara::knowledge::KnowledgeBase & knowledge,
      const groups::GroupBase & group);
  }
}

#endif // _GAMS_VARIABLES_DEVICES_H_
