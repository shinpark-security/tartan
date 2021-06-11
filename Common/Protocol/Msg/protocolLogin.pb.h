// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocolLogin.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_protocolLogin_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_protocolLogin_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017002 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_protocolLogin_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_protocolLogin_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_protocolLogin_2eproto;
namespace protocol_msg {
class LoginMsg;
struct LoginMsgDefaultTypeInternal;
extern LoginMsgDefaultTypeInternal _LoginMsg_default_instance_;
}  // namespace protocol_msg
PROTOBUF_NAMESPACE_OPEN
template<> ::protocol_msg::LoginMsg* Arena::CreateMaybeMessage<::protocol_msg::LoginMsg>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace protocol_msg {

// ===================================================================

class LoginMsg final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol_msg.LoginMsg) */ {
 public:
  inline LoginMsg() : LoginMsg(nullptr) {}
  ~LoginMsg() override;
  explicit constexpr LoginMsg(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LoginMsg(const LoginMsg& from);
  LoginMsg(LoginMsg&& from) noexcept
    : LoginMsg() {
    *this = ::std::move(from);
  }

  inline LoginMsg& operator=(const LoginMsg& from) {
    CopyFrom(from);
    return *this;
  }
  inline LoginMsg& operator=(LoginMsg&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LoginMsg& default_instance() {
    return *internal_default_instance();
  }
  static inline const LoginMsg* internal_default_instance() {
    return reinterpret_cast<const LoginMsg*>(
               &_LoginMsg_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(LoginMsg& a, LoginMsg& b) {
    a.Swap(&b);
  }
  inline void Swap(LoginMsg* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LoginMsg* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LoginMsg* New() const final {
    return new LoginMsg();
  }

  LoginMsg* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LoginMsg>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const LoginMsg& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const LoginMsg& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LoginMsg* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol_msg.LoginMsg";
  }
  protected:
  explicit LoginMsg(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUserIdFieldNumber = 3,
    kPasswordFieldNumber = 4,
    kMsgTypeFieldNumber = 1,
    kTimestampFieldNumber = 2,
    kAuthorityFieldNumber = 5,
  };
  // string user_id = 3;
  void clear_user_id();
  const std::string& user_id() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_user_id(ArgT0&& arg0, ArgT... args);
  std::string* mutable_user_id();
  PROTOBUF_MUST_USE_RESULT std::string* release_user_id();
  void set_allocated_user_id(std::string* user_id);
  private:
  const std::string& _internal_user_id() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_user_id(const std::string& value);
  std::string* _internal_mutable_user_id();
  public:

  // string password = 4;
  void clear_password();
  const std::string& password() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_password(ArgT0&& arg0, ArgT... args);
  std::string* mutable_password();
  PROTOBUF_MUST_USE_RESULT std::string* release_password();
  void set_allocated_password(std::string* password);
  private:
  const std::string& _internal_password() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_password(const std::string& value);
  std::string* _internal_mutable_password();
  public:

  // int32 msg_type = 1;
  void clear_msg_type();
  ::PROTOBUF_NAMESPACE_ID::int32 msg_type() const;
  void set_msg_type(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_msg_type() const;
  void _internal_set_msg_type(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 timestamp = 2;
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::int32 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 authority = 5;
  void clear_authority();
  ::PROTOBUF_NAMESPACE_ID::int32 authority() const;
  void set_authority(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_authority() const;
  void _internal_set_authority(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:protocol_msg.LoginMsg)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr user_id_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr password_;
  ::PROTOBUF_NAMESPACE_ID::int32 msg_type_;
  ::PROTOBUF_NAMESPACE_ID::int32 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::int32 authority_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_protocolLogin_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LoginMsg

// int32 msg_type = 1;
inline void LoginMsg::clear_msg_type() {
  msg_type_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LoginMsg::_internal_msg_type() const {
  return msg_type_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LoginMsg::msg_type() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.msg_type)
  return _internal_msg_type();
}
inline void LoginMsg::_internal_set_msg_type(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  msg_type_ = value;
}
inline void LoginMsg::set_msg_type(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_msg_type(value);
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.msg_type)
}

// int32 timestamp = 2;
inline void LoginMsg::clear_timestamp() {
  timestamp_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LoginMsg::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LoginMsg::timestamp() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.timestamp)
  return _internal_timestamp();
}
inline void LoginMsg::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  timestamp_ = value;
}
inline void LoginMsg::set_timestamp(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.timestamp)
}

// string user_id = 3;
inline void LoginMsg::clear_user_id() {
  user_id_.ClearToEmpty();
}
inline const std::string& LoginMsg::user_id() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.user_id)
  return _internal_user_id();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginMsg::set_user_id(ArgT0&& arg0, ArgT... args) {
 
 user_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.user_id)
}
inline std::string* LoginMsg::mutable_user_id() {
  std::string* _s = _internal_mutable_user_id();
  // @@protoc_insertion_point(field_mutable:protocol_msg.LoginMsg.user_id)
  return _s;
}
inline const std::string& LoginMsg::_internal_user_id() const {
  return user_id_.Get();
}
inline void LoginMsg::_internal_set_user_id(const std::string& value) {
  
  user_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* LoginMsg::_internal_mutable_user_id() {
  
  return user_id_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* LoginMsg::release_user_id() {
  // @@protoc_insertion_point(field_release:protocol_msg.LoginMsg.user_id)
  return user_id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void LoginMsg::set_allocated_user_id(std::string* user_id) {
  if (user_id != nullptr) {
    
  } else {
    
  }
  user_id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), user_id,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:protocol_msg.LoginMsg.user_id)
}

// string password = 4;
inline void LoginMsg::clear_password() {
  password_.ClearToEmpty();
}
inline const std::string& LoginMsg::password() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.password)
  return _internal_password();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginMsg::set_password(ArgT0&& arg0, ArgT... args) {
 
 password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.password)
}
inline std::string* LoginMsg::mutable_password() {
  std::string* _s = _internal_mutable_password();
  // @@protoc_insertion_point(field_mutable:protocol_msg.LoginMsg.password)
  return _s;
}
inline const std::string& LoginMsg::_internal_password() const {
  return password_.Get();
}
inline void LoginMsg::_internal_set_password(const std::string& value) {
  
  password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* LoginMsg::_internal_mutable_password() {
  
  return password_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* LoginMsg::release_password() {
  // @@protoc_insertion_point(field_release:protocol_msg.LoginMsg.password)
  return password_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void LoginMsg::set_allocated_password(std::string* password) {
  if (password != nullptr) {
    
  } else {
    
  }
  password_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), password,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:protocol_msg.LoginMsg.password)
}

// int32 authority = 5;
inline void LoginMsg::clear_authority() {
  authority_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LoginMsg::_internal_authority() const {
  return authority_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LoginMsg::authority() const {
  // @@protoc_insertion_point(field_get:protocol_msg.LoginMsg.authority)
  return _internal_authority();
}
inline void LoginMsg::_internal_set_authority(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  authority_ = value;
}
inline void LoginMsg::set_authority(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_authority(value);
  // @@protoc_insertion_point(field_set:protocol_msg.LoginMsg.authority)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol_msg

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_protocolLogin_2eproto
