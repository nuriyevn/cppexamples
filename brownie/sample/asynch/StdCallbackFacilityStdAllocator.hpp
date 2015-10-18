//
// StdCallbackFacilityStdAllocator.hpp
//    default memory allocator for the StdCallbackFacility template class
//
// Copyright (c) 2010, Mathias Czichi
// this file is part of the brownie Libraries and distributed under the license
// described at the end of the file

#ifndef brownie_asynch_StdCallbackFacilityStdAllocator_hpp
#define brownie_asynch_StdCallbackFacilityStdAllocator_hpp

#include "logging/StdCallbackFacilityStdAllocator.hpp"

namespace brownie {
namespace asynch {

template <typename LoggingFacility_ = brownie::asynch::logging::StdCallbackFacilityStdAllocator>
struct StdCallbackFacilityStdAllocator {
   StdCallbackFacilityStdAllocator()
   : mFacilities() {
      mFacilities.logCtor(thisPtr());
   }
   
   ~StdCallbackFacilityStdAllocator() {
      mFacilities.logDtor(thisPtr());
   }
   
   template <typename Call_>
   bool allocateCall(Call_*& pCall) {
      const bool success = allocateObject(reinterpret_cast<void*&>(pCall), sizeof(Call_));
         mFacilities.logAllocateCall(this, success, pCall);
         return success;
   }
   
   template <typename Call_>
   void freeCall(Call_* pCall) {
      mFacilities.logFreeCall(this, pCall, true);
      freeObject(pCall);
   }
   
   template <typename Notification_>
   bool allocateNotification(Notification_*& pNotification) {
      const bool success = allocateObject(reinterpret_cast<void*&>(pNotification), sizeof(Notification_));
         mFacilities.logAllocateNotification(this, success, pNotification);
         return success;
   }
   
   template <typename Notification_>
   void freeNotification(Notification_* pNotification) {
      mFacilities.logFreeNotification(this, pNotification, true);
      freeObject(pNotification);
   }
   
   const bool hasAllocations() const {
      return (mFacilities.numAllocations() > 0);
   }
   
   bool allocateObject(void*& pObject,
                       const int size);

   void freeObject(void* pObject);

   void increaseAllocations() {
      mFacilities.increaseAllocations();
      mFacilities.logIncreaseAllocations(this, mFacilities.numAllocations());
   }
   
   void decreaseAllocations() {
      mFacilities.decreaseAllocations();
      mFacilities.logDecreaseAllocations(this, mFacilities.numAllocations());
   }
   
   StdCallbackFacilityStdAllocator* thisPtr() { return this; }
   
   struct Facilities
   : public LoggingFacility_ {
      Facilities()
      : LoggingFacility_()
      , mAllocations(0) { }
      
      void increaseAllocations() { mAllocations += 1; }
      void decreaseAllocations() { mAllocations -= 1; }
      const int numAllocations() const { return mAllocations; }
      int mAllocations;
   }; // struct Facilities
   
   Facilities mFacilities;
}; // struct StdCallbackFacilityStdAllocator

// implementation
template <typename LoggingFacility_>
bool StdCallbackFacilityStdAllocator<LoggingFacility_>::allocateObject(void*& pObject,
                                                                       const int size) {
      pObject = new char[size];
   const bool success = (pObject != 0);
      if (success) {
         increaseAllocations();
      }
      return success;
}

template <typename LoggingFacility_>
void StdCallbackFacilityStdAllocator<LoggingFacility_>::freeObject(void* pObject) {
   delete[] reinterpret_cast<char*>(pObject);
   decreaseAllocations();
}

} // namespace asynch
} // namespace brownie
#endif // brownie_asynch_StdCallbackFacilityStdAllocator_hpp

// LICENSE
//
// brownie Libraries
// 
// Copyright (c) 2010, Mathias Czichi
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//    1) Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//       
//    2) Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//       
//    3) Neither the name of Mathias Czichi nor the names of its contributors may
//       be used to endorse or promote products derived from this software without
//       specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.