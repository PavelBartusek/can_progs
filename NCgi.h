/*
 *
 *  Copyright (C) 2014 Jürg Müller, CH-5524
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/ .
 */

#if !defined(NCgi_H)

  #define NCgi_H

  class KEnv
  {
    private:
      static const char * null_str;
      const char * GetEnv(const char * name) const;

    public:
      const char * content_length;
      const char * content_type;
      const char * query_string;

      KEnv();
      ~KEnv();
      void Init();
      const char * get_query(const char * name) const;

  };

#endif

