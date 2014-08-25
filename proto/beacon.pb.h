// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: beacon.proto

#ifndef PROTOBUF_beacon_2eproto__INCLUDED
#define PROTOBUF_beacon_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "event_bus.pb.h"
// @@protoc_insertion_point(includes)

namespace rubicon {
namespace data_pipeline {
namespace aggregation_framework {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_beacon_2eproto();
void protobuf_AssignDesc_beacon_2eproto();
void protobuf_ShutdownFile_beacon_2eproto();

class Beacon_PB;

// ===================================================================

class Beacon_PB : public ::google::protobuf::Message {
 public:
  Beacon_PB();
  virtual ~Beacon_PB();
  
  Beacon_PB(const Beacon_PB& from);
  
  inline Beacon_PB& operator=(const Beacon_PB& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Beacon_PB& default_instance();
  
  void Swap(Beacon_PB* other);
  
  // implements Message ----------------------------------------------
  
  Beacon_PB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Beacon_PB& from);
  void MergeFrom(const Beacon_PB& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 server_id = 1;
  inline bool has_server_id() const;
  inline void clear_server_id();
  static const int kServerIdFieldNumber = 1;
  inline ::google::protobuf::uint32 server_id() const;
  inline void set_server_id(::google::protobuf::uint32 value);
  
  // required uint32 server_port = 2;
  inline bool has_server_port() const;
  inline void clear_server_port();
  static const int kServerPortFieldNumber = 2;
  inline ::google::protobuf::uint32 server_port() const;
  inline void set_server_port(::google::protobuf::uint32 value);
  
  // required uint32 service_id = 3;
  inline bool has_service_id() const;
  inline void clear_service_id();
  static const int kServiceIdFieldNumber = 3;
  inline ::google::protobuf::uint32 service_id() const;
  inline void set_service_id(::google::protobuf::uint32 value);
  
  // optional string service_name = 4;
  inline bool has_service_name() const;
  inline void clear_service_name();
  static const int kServiceNameFieldNumber = 4;
  inline const ::std::string& service_name() const;
  inline void set_service_name(const ::std::string& value);
  inline void set_service_name(const char* value);
  inline void set_service_name(const char* value, size_t size);
  inline ::std::string* mutable_service_name();
  inline ::std::string* release_service_name();
  
  // repeated .fim.eventbus.EndPoint channel = 5;
  inline int channel_size() const;
  inline void clear_channel();
  static const int kChannelFieldNumber = 5;
  inline const ::fim::eventbus::EndPoint& channel(int index) const;
  inline ::fim::eventbus::EndPoint* mutable_channel(int index);
  inline ::fim::eventbus::EndPoint* add_channel();
  inline const ::google::protobuf::RepeatedPtrField< ::fim::eventbus::EndPoint >&
      channel() const;
  inline ::google::protobuf::RepeatedPtrField< ::fim::eventbus::EndPoint >*
      mutable_channel();
  
  // @@protoc_insertion_point(class_scope:rubicon.data_pipeline.aggregation_framework.Beacon_PB)
 private:
  inline void set_has_server_id();
  inline void clear_has_server_id();
  inline void set_has_server_port();
  inline void clear_has_server_port();
  inline void set_has_service_id();
  inline void clear_has_service_id();
  inline void set_has_service_name();
  inline void clear_has_service_name();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 server_id_;
  ::google::protobuf::uint32 server_port_;
  ::std::string* service_name_;
  ::google::protobuf::RepeatedPtrField< ::fim::eventbus::EndPoint > channel_;
  ::google::protobuf::uint32 service_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  friend void  protobuf_AddDesc_beacon_2eproto();
  friend void protobuf_AssignDesc_beacon_2eproto();
  friend void protobuf_ShutdownFile_beacon_2eproto();
  
  void InitAsDefaultInstance();
  static Beacon_PB* default_instance_;
};
// ===================================================================


// ===================================================================

// Beacon_PB

// required uint32 server_id = 1;
inline bool Beacon_PB::has_server_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Beacon_PB::set_has_server_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Beacon_PB::clear_has_server_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Beacon_PB::clear_server_id() {
  server_id_ = 0u;
  clear_has_server_id();
}
inline ::google::protobuf::uint32 Beacon_PB::server_id() const {
  return server_id_;
}
inline void Beacon_PB::set_server_id(::google::protobuf::uint32 value) {
  set_has_server_id();
  server_id_ = value;
}

// required uint32 server_port = 2;
inline bool Beacon_PB::has_server_port() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Beacon_PB::set_has_server_port() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Beacon_PB::clear_has_server_port() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Beacon_PB::clear_server_port() {
  server_port_ = 0u;
  clear_has_server_port();
}
inline ::google::protobuf::uint32 Beacon_PB::server_port() const {
  return server_port_;
}
inline void Beacon_PB::set_server_port(::google::protobuf::uint32 value) {
  set_has_server_port();
  server_port_ = value;
}

// required uint32 service_id = 3;
inline bool Beacon_PB::has_service_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Beacon_PB::set_has_service_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Beacon_PB::clear_has_service_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Beacon_PB::clear_service_id() {
  service_id_ = 0u;
  clear_has_service_id();
}
inline ::google::protobuf::uint32 Beacon_PB::service_id() const {
  return service_id_;
}
inline void Beacon_PB::set_service_id(::google::protobuf::uint32 value) {
  set_has_service_id();
  service_id_ = value;
}

// optional string service_name = 4;
inline bool Beacon_PB::has_service_name() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Beacon_PB::set_has_service_name() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Beacon_PB::clear_has_service_name() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Beacon_PB::clear_service_name() {
  if (service_name_ != &::google::protobuf::internal::kEmptyString) {
    service_name_->clear();
  }
  clear_has_service_name();
}
inline const ::std::string& Beacon_PB::service_name() const {
  return *service_name_;
}
inline void Beacon_PB::set_service_name(const ::std::string& value) {
  set_has_service_name();
  if (service_name_ == &::google::protobuf::internal::kEmptyString) {
    service_name_ = new ::std::string;
  }
  service_name_->assign(value);
}
inline void Beacon_PB::set_service_name(const char* value) {
  set_has_service_name();
  if (service_name_ == &::google::protobuf::internal::kEmptyString) {
    service_name_ = new ::std::string;
  }
  service_name_->assign(value);
}
inline void Beacon_PB::set_service_name(const char* value, size_t size) {
  set_has_service_name();
  if (service_name_ == &::google::protobuf::internal::kEmptyString) {
    service_name_ = new ::std::string;
  }
  service_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Beacon_PB::mutable_service_name() {
  set_has_service_name();
  if (service_name_ == &::google::protobuf::internal::kEmptyString) {
    service_name_ = new ::std::string;
  }
  return service_name_;
}
inline ::std::string* Beacon_PB::release_service_name() {
  clear_has_service_name();
  if (service_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = service_name_;
    service_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated .fim.eventbus.EndPoint channel = 5;
inline int Beacon_PB::channel_size() const {
  return channel_.size();
}
inline void Beacon_PB::clear_channel() {
  channel_.Clear();
}
inline const ::fim::eventbus::EndPoint& Beacon_PB::channel(int index) const {
  return channel_.Get(index);
}
inline ::fim::eventbus::EndPoint* Beacon_PB::mutable_channel(int index) {
  return channel_.Mutable(index);
}
inline ::fim::eventbus::EndPoint* Beacon_PB::add_channel() {
  return channel_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::fim::eventbus::EndPoint >&
Beacon_PB::channel() const {
  return channel_;
}
inline ::google::protobuf::RepeatedPtrField< ::fim::eventbus::EndPoint >*
Beacon_PB::mutable_channel() {
  return &channel_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace aggregation_framework
}  // namespace data_pipeline
}  // namespace rubicon

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_beacon_2eproto__INCLUDED
