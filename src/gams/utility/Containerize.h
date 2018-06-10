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
 * @file Containerize.h
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * This file declares the to_container and from_container methods
 **/

#ifndef  _GAMS_UTILITY_CONTAINERIZE_H_
#define  _GAMS_UTILITY_CONTAINERIZE_H_

#include "gams/GamsExport.h"

#include <string>

#include "madara/knowledge/KnowledgeBase.h"

namespace gams
{
  namespace utility
  {
    /**
    * A class meant to assist with retrieving information
    * into GAMS containers
    **/
    class GAMS_EXPORT Containerize
    {
    public:
      /**
       * Constructor
       **/
      Containerize (const std::string& n = "");

      /**
       * Destructor
       **/
      virtual ~Containerize ();

      /**
       * Get name of the object
       * @return name of the object
       **/
      std::string get_name () const;

      /**
       * Set name of the object
       * @param n   new name of the object
       **/
      void set_name (const std::string& n);

      /**
       * Set knowledge base to use
       **/
      void set_knowledge_base (madara::knowledge::KnowledgeBase* kb);

      /**
       * Resend the information in the container using same knowledge base as
       * previous to_container
       **/
      void modify();

      /**
       * Store object in knowledge base used previously
       * @param name      location of object in Knowlege Base
       **/
      void to_container (const std::string& name = "");

      /**
       * Store object in knowledge base
       * @param kb        Knowledge Base to store object in
       * @param name      location of object in Knowlege Base
       **/
      void to_container (madara::knowledge::KnowledgeBase& kb, 
        const std::string& name = "");

      /**
       * Load object from knowledge base
       * @param name      location of object in Knowlege Base
       * @return true if object successfully loaded from knowledge base
       **/
      bool from_container (const std::string& name = "");

      /**
       * Load object from knowledge base
       * @param kb        Knowledge Base with object
       * @param name      location of object in Knowlege Base
       * @return true if object successfully loaded from knowledge base
       **/
      bool from_container (
        madara::knowledge::KnowledgeBase& kb, 
        const std::string& name = "");

    protected:
      /// object type suffix
      const static std::string object_type_suffix_;

      /// name of this object
      std::string name_;

      /// previous knowledge base used for sending objects
      madara::knowledge::KnowledgeBase* prev_kb_;

      /**
       * These are used to check on the type of the object in from_container.
       * When inheriting from this class, add your new class to this list to 
       * check for your unique ID. Assign this value in the knowledge base to 
       * "prefix + object_type_suffix"
       **/
      enum Class_ID
      {
        REGION_TYPE_ID                  = 0x00000001,
        PRIORITIZED_REGION_TYPE_ID      = 0x00000002,
        SEARCH_AREA_TYPE_ID             = 0x00000004,
        INVALID                         = 0x80000000,
      };

      /**
       * Check if the type is correct
       * @param kb        Knowledge Base with object
       * @param name      Prefix of object in the KB
       * @param expected  Expected value
       * @return true if correct type, false otherwise
       **/
      bool is_valid_type (madara::knowledge::KnowledgeBase& kb,
        const std::string& name, const Class_ID& expected) const;

      /**
       * Gets type of object in database
       * @param kb        Knowledge Base with object
       * @param name      Prefix of object in the KB
       * @return Class_ID of object in kb
       **/
      static Class_ID get_type (madara::knowledge::KnowledgeBase& kb,
        const std::string& name);

    private:
      /**
       * Check if object is of correct type
       * @param kb        Knowledge Base with object
       * @param name      Prefix of object in the KB
       * @return true if name is a valid object type in kb
       **/
      virtual bool check_valid_type (
        madara::knowledge::KnowledgeBase& kb,
        const std::string& name) const = 0;

      /**
       * Store object in knowledge base
       * @param kb        Knowledge Base to store object in
       * @param name      location of object in Knowlege Base
       **/
      virtual void to_container_impl (
        madara::knowledge::KnowledgeBase& kb, 
        const std::string& name) = 0;

      /**
       * Load object from knowledge base
       * @param kb        Knowledge Base with object
       * @param name      location of object in Knowlege Base
       **/
      virtual bool from_container_impl (
        madara::knowledge::KnowledgeBase& kb, 
        const std::string& name) = 0;
    };
  }
}

#endif // _GAMS_UTILITY_CONTAINERIZE_H_
