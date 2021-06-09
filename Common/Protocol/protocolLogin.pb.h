// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocolLogin.proto

#ifndef PROTOBUF_protocolLogin_2eproto__INCLUDED
#define PROTOBUF_protocolLogin_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protocol_msg {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_protocolLogin_2eproto();
void protobuf_AssignDesc_protocolLogin_2eproto();
void protobuf_ShutdownFile_protocolLogin_2eproto();

class LoginMsg;

// ===================================================================

class LoginMsg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:protocol_msg.LoginMsg) */ {
 public:
  LoginMsg();
  virtual ~LoginMsg();

  LoginMsg(const LoginMsg& from);

  inline LoginMsg& operator=(const LoginMsg& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LoginMsg& default_instance();

  void Swap(LoginMsg* other);

  // implements Message ----------------------------------------------

  inline LoginMsg* New() const { return New(NULL); }

  LoginMsg* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LoginMsg& from);
  void MergeFrom(const LoginMsg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(LoginMsg* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 msg_type = 1;
  void clear_msg_type();
  static const int kMsgTypeFieldNumber = 1;
  ::google::protobuf::int32 msg_type() const;
  void set_msg_type(::google::protobuf::int32 value);

  // optional int32 timestamp = 2;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  ::google::protobuf::int32 timestamp() const;
  void set_timestamp(::google::protobuf::int32 value);

  // optional string user_id = 3;
  void clear_user_id();
  static const int kUserIdFieldNumber = 3;
  const ::std::string& user_id() const;
  void set_user_id(const ::std::string& value);
  void set_user_id(const char* value);
  void set_user_id(const char* value, size_t size);
  ::std::string* mutable_user_id();
  ::std::string* release_user_id();
  void set_allocated_user_id(::std::string* user_id);

  // optional string password = 4;
  void clear_password();
  static const int kPasswordFieldNumber = 4;
  const ::std::string& password() const;
  void set_password(const ::std::string& value);
  void set_password(const char* value);
  void set_password(const char* value, size_t size);
  ::std::string* mutable_password();
  ::std::string* release_password();
  void set_allocated_password(::std::string* password);

  // optional int32 authority = 5;
  void clear_authority();
  static const int kAuthorityFieldNumber = 5;
  ::google::protobuf::int32 authority() const;
  void set_authority(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protocol_msg.LoginMsg)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int32 msg_type_;
  ::google::protobuf::int32 timestamp_;
  ::google::protobuf::internal::ArenaStringPtr user_id_;
  ::google::protobuf::internal::ArenaStringPtr password_;
  ::google::protobuf::int32 authority_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_protocolLogin_2eproto();
  friend void protobuf_AssignDesc_protocolLogin_2eproto();
  friend void protobuf_ShutdownFile_protocolLogin_2eproto();

  void InitAsDefaultInstance();
  static LoginMsg* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// LoginMsg

// optional int32 msg_type = 1;
inline void LoginMsg::clear_msg_type() {
  msg_type_ = 0;
}
inline ::google::protobuf::int32 LoginMsg::msg_type() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.msg_type)
  return msg_type_;
}
inline void LoginMsg::set_msg_type(::google::protobuf::int32 value) {
  
  msg_type_ = value;
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.msg_type)
}

// optional int32 timestamp = 2;
inline void LoginMsg::clear_timestamp() {
  timestamp_ = 0;
}
inline ::google::protobuf::int32 LoginMsg::timestamp() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.timestamp)
  return timestamp_;
}
inline void LoginMsg::set_timestamp(::google::protobuf::int32 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.timestamp)
}

// optional string user_id = 3;
inline void LoginMsg::clear_user_id() {
  user_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& LoginMsg::user_id() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.user_id)
  return user_id_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginMsg::set_user_id(const ::std::string& value) {
  
  user_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.user_id)
}
inline void LoginMsg::set_user_id(const char* value) {
  
  user_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:protocol_msg.LoginMsg.user_id)
}
inline void LoginMsg::set_user_id(const char* value, size_t size) {
  
  user_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:protocol_msg.LoginMsg.user_id)
}
inline ::std::string* LoginMsg::mutable_user_id() {
  
  // @@protoc_insertion_point(field_mutable:protocol_msg.LoginMsg.user_id)
  return user_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* LoginMsg::release_user_id() {
  // @@protoc_insertion_point(field_release:protocol_msg.LoginMsg.user_id)
  
  return user_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginMsg::set_allocated_user_id(::std::string* user_id) {
  if (user_id != NULL) {
    
  } else {
    
  }
  user_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), user_id);
  // @@protoc_insertion_point(field_set_allocated:protocol_msg.LoginMsg.user_id)
}

// optional string password = 4;
inline void LoginMsg::clear_password() {
  password_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& LoginMsg::password() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.password)
  return password_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginMsg::set_password(const ::std::string& value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.password)
}
inline void LoginMsg::set_password(const char* value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:protocol_msg.LoginMsg.password)
}
inline void LoginMsg::set_password(const char* value, size_t size) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:protocol_msg.LoginMsg.password)
}
inline ::std::string* LoginMsg::mutable_password() {
  
  // @@protoc_insertion_point(field_mutable:protocol_msg.LoginMsg.password)
  return password_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* LoginMsg::release_password() {
  // @@protoc_insertion_point(field_release:protocol_msg.LoginMsg.password)
  
  return password_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginMsg::set_allocated_password(::std::string* password) {
  if (password != NULL) {
    
  } else {
    
  }
  password_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), password);
  // @@protoc_insertion_point(field_set_allocated:protocol_msg.LoginMsg.password)
}

// optional int32 authority = 5;
inline void LoginMsg::clear_authority() {
  authority_ = 0;
}
inline ::google::protobuf::int32 LoginMsg::authority() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.authority)
  return authority_;
}
inline void LoginMsg::set_authority(::google::protobuf::int32 value) {
  
  authority_ = value;
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.authority)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol_msg

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_protocolLogin_2eproto__INCLUDED
