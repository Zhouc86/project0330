/*
 * Copyright (c) 2008, 2018, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0, as
 * published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms,
 * as designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an
 * additional permission to link the program and your derivative works
 * with the separately licensed software that they have included with
 * MySQL.
 *
 * Without limiting anything contained in the foregoing, this file,
 * which is part of MySQL Connector/C++, is also subject to the
 * Universal FOSS Exception, version 1.0, a copy of which can be found at
 * http://oss.oracle.com/licenses/universal-foss-exception.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
 */



#ifndef _SQL_BUILD_CONFIG_H_
#define _SQL_BUILD_CONFIG_H_

#ifndef CPPCONN_PUBLIC_FUNC

#if defined(_WIN32)

 // mysqlcppconn_EXPORTS is added by cmake and defined for dynamic lib build only
  #ifdef mysqlcppconn_EXPORTS
    #define CPPCONN_PUBLIC_FUNC __declspec(dllexport)
  #else
    // this is for static build
    #ifdef CPPCONN_LIB_BUILD
      #define CPPCONN_PUBLIC_FUNC
    #else
      // this is for clients using dynamic lib
      #define CPPCONN_PUBLIC_FUNC __declspec(dllimport)
    #endif
  #endif

  /*
    Warning 4251 is about non dll-interface classes being used by ones exported
    from our DLL (for example std lib classes or Boost ones). Following
    the crowd, we ignore this issue for now.
  */

  __pragma(warning (disable:4251))

#else
  #define CPPCONN_PUBLIC_FUNC

  /*
    These are triggered by, e.g., std::auto_ptr<> which is used by Boost.
  */

  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#endif    //#ifndef CPPCONN_PUBLIC_FUNC

#endif    //#ifndef _SQL_BUILD_CONFIG_H_
