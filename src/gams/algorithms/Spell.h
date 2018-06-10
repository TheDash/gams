/**
 * Copyright (c) 2017 Carnegie Mellon University. All Rights Reserved.
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
 * 3. The names Carnegie Mellon University, "SEI and/or Software
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
 *      are those of the author (s) and do not necessarily reflect the views of
 *      the United States Department of Defense.
 * 
 *      NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *      INSTITUTE MATERIAL IS FURNISHED ON AN AS-IS BASIS. CARNEGIE MELLON
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
 * @file Spell.h
 * @author David Kyle <dskyle@sei.cmu.edu>
 *
 * Algorithm for spelling words and letters with agents
 **/

#ifndef _GAMS_ALGORITHMS_SPELL_H_
#define _GAMS_ALGORITHMS_SPELL_H_

#include <vector>
#include <string>

#include "gams/variables/Sensor.h"
#include "gams/platforms/BasePlatform.h"
#include "gams/variables/AlgorithmStatus.h"
#include "gams/variables/Self.h"
#include "gams/algorithms/BaseAlgorithm.h"
#include "gams/pose/Position.h"
#include "gams/algorithms/AlgorithmFactory.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/containers/Barrier.h"
#include "gams/groups/GroupFactoryRepository.h"

namespace gams
{
  namespace algorithms
  {
    /**
    *
    **/
    class GAMS_EXPORT Spell : public BaseAlgorithm
    {
    public:

      /**
       * Constructor
       * @param  group        group name of agents to use. Agents drawn from
       *                      list in-order, 3 per character
       * @param  text         text to form
       * @param  origin       location of upper left corner of first letter;
       *                      rotation portion deterimines direction:
       *                         Z-axis points upwards, normal to plane of text
       *                         X-axis is direction letters flow towards
       * @param  height       height of letters (in meters)
       * @param  width        width of letters (in width; all are fixed-width)
       * @param  buffer       distance between letters
       * @param  knowledge    the context containing variables and values
       * @param  platform     the underlying platform the algorithm will use
       * @param  sensors      map of sensor names to sensor information
       * @param  self         self-referencing variables
       **/
      Spell (
        const std::string &group, std::string text,
        pose::Pose origin,
        double height, double width,
        double buffer,
        const std::string & barrier,
        madara::knowledge::KnowledgeBase * knowledge = 0,
        platforms::BasePlatform * platform = 0,
        variables::Sensors * sensors = 0,
        variables::Self * self = 0);

      /**
       * Analyzes environment, platform, or other information
       * @return bitmask status of the platform. @see Status.
       **/
      virtual int analyze (void);
      
      /**
       * Plans the next execution of the algorithm
       * @return bitmask status of the platform. @see Status.
       **/
      virtual int execute (void);

      /**
       * Plans the next execution of the algorithm
       * @return bitmask status of the platform. @see Status.
       **/
      virtual int plan (void);
      
    protected:
      std::string text_;

      /// factory for interacting with user-defined groups
      groups::GroupFactoryRepository group_factory_;

      /// the group that the user wishes the algorithm to use
      groups::GroupBase * group_;

      /// a convenience list of all current group members
      groups::AgentVector group_members_;

      /// the origin of the spelled word/phrase
      pose::Pose origin_;

      /// the height of the letters to spell in meters
      double height_;
      
      /// the width of the letters to spell in meters
      double width_;
      
      /// the expected buffer between letters in meters
      double buffer_;

      /// the index of the agent in the member list
      int index_, count_, node_;

      /// the next position to go to
      pose::Position next_pos_;

      /// the step count in the waypoint list
      size_t step_;

      /// a barrier between all agents before steps can proceed
      madara::knowledge::containers::Barrier barrier_;
    };
    
    /**
     * A factory class for creating Spell algorithms
     **/
    class GAMS_EXPORT SpellFactory : public AlgorithmFactory
    {
    public:

      /**
       * Creates a Spell Algorithm.
       * @param   args      Map of arguments, from name to value
       * @param   knowledge the knowledge base to use
       * @param   platform  the platform. This will be set by the
       *                    controller in init_vars.
       * @param   sensors   the sensor info. This will be set by the
       *                    controller in init_vars.
       * @param   self      self-referencing variables. This will be
       *                    set by the controller in init_vars
       * @param   agents   the list of agents, which is dictated by
       *                    init_vars when a number of processes is set. This
       *                    will be set by the controller in init_vars
       **/
      virtual BaseAlgorithm * create (
        const madara::knowledge::KnowledgeMap & args,
        madara::knowledge::KnowledgeBase * knowledge,
        platforms::BasePlatform * platform,
        variables::Sensors * sensors,
        variables::Self * self,
        variables::Agents * agents);
    };
  }
}

#endif // _GAMS_ALGORITHMS_FORMATION_SYNC_H_
