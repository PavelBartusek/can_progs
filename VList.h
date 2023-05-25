/*
 *
 *  Copyright (C) 2015 Jürg Müller, CH-5524
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

#if !defined(VList_H)

  #define VList_H

  #include <time.h>
  #include "KStream.h"

  class KSortItem
  {
    public:
      const char * Key;
      
      KSortItem() { Key = NULL; }
      virtual ~KSortItem();
      void SetKey(const char * NewKey);
  };

  class KSortItemList
  {
    private:
      bool IsSorted;
      unsigned SortItemCount;
      unsigned MaxItemCount;
      const KSortItem ** SortItemList;

      const KSortItem * GetItem(unsigned Index) const;
      unsigned AppendItem(const KSortItem * Item);

    public:
      KSortItemList() { IsSorted = false; SortItemCount = 0; MaxItemCount = 0; SortItemList = NULL; }
      virtual ~KSortItemList();
    
      void Clear();
      unsigned GetCount() const { return SortItemCount; }
      const char * GetKey(unsigned Index) const;
      bool AppendKey(const char * NewKey);
      int FindItem(const char * FindKey) const;
      virtual int Compare(unsigned Index, const char * CmpKey) const;
      virtual int Compare(unsigned Index1, unsigned Index2) const;
      bool ReadFile(const char * Filename);
    
      bool Sort();
  };

  typedef const char * startlist_type[100];

  class VExploreDir
  {
    private:
  		unsigned MaxPathLength;
    public:
      int Level;
      bool dir_first;
      bool use_hidden_files;
      char * FromPath;
      char * RelPath;

      VExploreDir();
      virtual ~VExploreDir();
      virtual bool ExploreFile(const char * Filename) = 0;
      virtual bool ExploreDir(const char * DirName);
      bool ExploreSortedList(bool dirs);
      bool Start(const char * StartFromDir);
      bool Start(startlist_type StartFromDir);
  };

  class VListFileDir
  {
    private:
      void * dir;
      long PathLen;
#if defined(__VC__) || defined(_MSC_VER)
      void * FindFileData;
      bool FirstRead;
#endif
    public:
      char Name[256];
      char FullPath[2048];
      long mode;
      long size;
      time_t time;

      VListFileDir();
      virtual ~VListFileDir();

      virtual bool Open(const char * Path);
      bool Open(const char * Path, const char * RelPath);
      virtual void Close();
      virtual bool Next();
      bool IsFile() const;
      bool IsDir() const;
  };

#endif

