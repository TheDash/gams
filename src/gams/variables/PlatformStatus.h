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

/**
 * @file PlatformStatus.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the definition of the platform-referencing MADARA variables
 **/

#ifndef   _GAMS_VARIABLES_PLATFORM_H_
#define   _GAMS_VARIABLES_PLATFORM_H_

#include <vector>
#include <map>
#include <string>

#include "gams/GamsExport.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/KnowledgeBase.h"
#include "gams/variables/Agent.h"

namespace gams
{
  namespace variables
  {
    /**
    * A container for platform status information
    **/
    class GAMS_EXPORT PlatformStatus
    {
    public:
      /**
       * Constructor
       **/
      PlatformStatus ();

      /**
       * Destructor
       **/
      ~PlatformStatus ();

      /**
       * Assignment operator
       * @param  rhs   value to copy
       **/
      void operator= (const PlatformStatus & rhs);

      /**
       * Initializes variable containers
       * @param   knowledge  the knowledge base that houses the variables
       * @param   new_name   the name of the platform
       **/
      void init_vars (madara::knowledge::KnowledgeBase & knowledge,
        const std::string & new_name = "");
      
      /**
       * Initializes variable containers
       * @param   knowledge  the variable context
       * @param   new_name   the name of the platform
       **/
      void init_vars (madara::knowledge::Variables & knowledge,
        const std::string & new_name = "");

      /// the id of this agent
      std::string name;
      
      /// the prefix of the platform status variables in the knowledge base
      std::string prefix;

      /// the agent-specific variables
      //Agent agent;
      
      /// status flag for number of communication channels available
      madara::knowledge::containers::Integer communication_available;

      /// status flag for deadlocked
      madara::knowledge::containers::Integer deadlocked;
      
      /// status flag for failed
      madara::knowledge::containers::Integer failed;
      
      /// status flag for the detection of active spoofing of GPS
      madara::knowledge::containers::Integer gps_spoofed;

      /// status flag for full movement availability
      madara::knowledge::containers::Integer movement_available;

      /// status flag for moving to a location
      madara::knowledge::containers::Integer moving;

      /// status flag for rotating to an angle
      madara::knowledge::containers::Integer rotating;

      /// status flag for ok
      madara::knowledge::containers::Integer ok;

      /// status flag for paused while moving to a location
      madara::knowledge::containers::Integer paused_moving;

      /// status flag for paused while rotating to anangle
      madara::knowledge::containers::Integer paused_rotating;

      /// status flag for reduced sensing available
      madara::knowledge::containers::Integer reduced_sensing;

      /// status flag for reduced movement available
      madara::knowledge::containers::Integer reduced_movement;

      /// status flag for full sensor availability
      madara::knowledge::containers::Integer sensors_available;

      /// status flag for waiting
      madara::knowledge::containers::Integer waiting;

    protected:
      /**
       * Get variable prefix
       * @return string variable prefix
       */
      std::string make_variable_prefix () const;

      /**
       * Initialize variable values
       */
      void init_variable_values ();
    };
    
    /// deprecated typedef. Please use PlatformStatus instead.
    typedef PlatformStatus Platform;

    /// a map of sensor names to the sensor information
    typedef  std::map <std::string, PlatformStatus>   Platforms;
    
    /// a typedef for convenience and legibility
    typedef  Platforms   PlatformStatuses;

    /// a list of sensor names
    typedef  std::vector <std::string>        PlatformNames;
  }
}

#endif // _GAMS_VARIABLES_PLATFORM_H_
