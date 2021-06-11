// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocolLogin.proto

#include "protocolLogin.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace protocol_msg {
constexpr LoginMsg::LoginMsg(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : user_id_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , password_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , msg_type_(0)
  , timestamp_(0)
  , authority_(0){}
struct LoginMsgDefaultTypeInternal {
  constexpr LoginMsgDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~LoginMsgDefaultTypeInternal() {}
  union {
    LoginMsg _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT LoginMsgDefaultTypeInternal _LoginMsg_default_instance_;
}  // namespace protocol_msg
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_protocolLogin_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_protocolLogin_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_protocolLogin_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_protocolLogin_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::protocol_msg::LoginMsg, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::protocol_msg::LoginMsg, msg_type_),
  PROTOBUF_FIELD_OFFSET(::protocol_msg::LoginMsg, timestamp_),
  PROTOBUF_FIELD_OFFSET(::protocol_msg::LoginMsg, user_id_),
  PROTOBUF_FIELD_OFFSET(::protocol_msg::LoginMsg, password_),
  PROTOBUF_FIELD_OFFSET(::protocol_msg::LoginMsg, authority_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::protocol_msg::LoginMsg)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::protocol_msg::_LoginMsg_default_instance_),
};

const char descriptor_table_protodef_protocolLogin_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\023protocolLogin.proto\022\014protocol_msg\"e\n\010L"
  "oginMsg\022\020\n\010msg_type\030\001 \001(\005\022\021\n\ttimestamp\030\002"
  " \001(\005\022\017\n\007user_id\030\003 \001(\t\022\020\n\010password\030\004 \001(\t\022"
  "\021\n\tauthority\030\005 \001(\005b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_protocolLogin_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_protocolLogin_2eproto = {
  false, false, 146, descriptor_table_protodef_protocolLogin_2eproto, "protocolLogin.proto", 
  &descriptor_table_protocolLogin_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_protocolLogin_2eproto::offsets,
  file_level_metadata_protocolLogin_2eproto, file_level_enum_descriptors_protocolLogin_2eproto, file_level_service_descriptors_protocolLogin_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_protocolLogin_2eproto_getter() {
  return &descriptor_table_protocolLogin_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_protocolLogin_2eproto(&descriptor_table_protocolLogin_2eproto);
namespace protocol_msg {

// ===================================================================

class LoginMsg::_Internal {
 public:
};

LoginMsg::LoginMsg(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:protocol_msg.LoginMsg)
}
LoginMsg::LoginMsg(const LoginMsg& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  user_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_user_id().empty()) {
    user_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_user_id(), 
      GetArenaForAllocation());
  }
  password_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_password().empty()) {
    password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_password(), 
      GetArenaForAllocation());
  }
  ::memcpy(&msg_type_, &from.msg_type_,
    static_cast<size_t>(reinterpret_cast<char*>(&authority_) -
    reinterpret_cast<char*>(&msg_type_)) + sizeof(authority_));
  // @@protoc_insertion_point(copy_constructor:protocol_msg.LoginMsg)
}

inline void LoginMsg::SharedCtor() {
user_id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
password_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&msg_type_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&authority_) -
    reinterpret_cast<char*>(&msg_type_)) + sizeof(authority_));
}

LoginMsg::~LoginMsg() {
  // @@protoc_insertion_point(destructor:protocol_msg.LoginMsg)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void LoginMsg::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  user_id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  password_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void LoginMsg::ArenaDtor(void* object) {
  LoginMsg* _this = reinterpret_cast< LoginMsg* >(object);
  (void)_this;
}
void LoginMsg::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void LoginMsg::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void LoginMsg::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol_msg.LoginMsg)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  user_id_.ClearToEmpty();
  password_.ClearToEmpty();
  ::memset(&msg_type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&authority_) -
      reinterpret_cast<char*>(&msg_type_)) + sizeof(authority_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* LoginMsg::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 msg_type = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          msg_type_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 timestamp = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          timestamp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string user_id = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_user_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "protocol_msg.LoginMsg.user_id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string password = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_password();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "protocol_msg.LoginMsg.password"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 authority = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          authority_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* LoginMsg::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol_msg.LoginMsg)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 msg_type = 1;
  if (this->_internal_msg_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_msg_type(), target);
  }

  // int32 timestamp = 2;
  if (this->_internal_timestamp() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_timestamp(), target);
  }

  // string user_id = 3;
  if (!this->_internal_user_id().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_user_id().data(), static_cast<int>(this->_internal_user_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "protocol_msg.LoginMsg.user_id");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_user_id(), target);
  }

  // string password = 4;
  if (!this->_internal_password().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_password().data(), static_cast<int>(this->_internal_password().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "protocol_msg.LoginMsg.password");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_password(), target);
  }

  // int32 authority = 5;
  if (this->_internal_authority() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(5, this->_internal_authority(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol_msg.LoginMsg)
  return target;
}

size_t LoginMsg::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol_msg.LoginMsg)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string user_id = 3;
  if (!this->_internal_user_id().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_user_id());
  }

  // string password = 4;
  if (!this->_internal_password().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_password());
  }

  // int32 msg_type = 1;
  if (this->_internal_msg_type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_msg_type());
  }

  // int32 timestamp = 2;
  if (this->_internal_timestamp() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_timestamp());
  }

  // int32 authority = 5;
  if (this->_internal_authority() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_authority());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData LoginMsg::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    LoginMsg::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*LoginMsg::GetClassData() const { return &_class_data_; }

void LoginMsg::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<LoginMsg *>(to)->MergeFrom(
      static_cast<const LoginMsg &>(from));
}


void LoginMsg::MergeFrom(const LoginMsg& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:protocol_msg.LoginMsg)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_user_id().empty()) {
    _internal_set_user_id(from._internal_user_id());
  }
  if (!from._internal_password().empty()) {
    _internal_set_password(from._internal_password());
  }
  if (from._internal_msg_type() != 0) {
    _internal_set_msg_type(from._internal_msg_type());
  }
  if (from._internal_timestamp() != 0) {
    _internal_set_timestamp(from._internal_timestamp());
  }
  if (from._internal_authority() != 0) {
    _internal_set_authority(from._internal_authority());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void LoginMsg::CopyFrom(const LoginMsg& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol_msg.LoginMsg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LoginMsg::IsInitialized() const {
  return true;
}

void LoginMsg::InternalSwap(LoginMsg* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &user_id_, GetArenaForAllocation(),
      &other->user_id_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &password_, GetArenaForAllocation(),
      &other->password_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(LoginMsg, authority_)
      + sizeof(LoginMsg::authority_)
      - PROTOBUF_FIELD_OFFSET(LoginMsg, msg_type_)>(
          reinterpret_cast<char*>(&msg_type_),
          reinterpret_cast<char*>(&other->msg_type_));
}

::PROTOBUF_NAMESPACE_ID::Metadata LoginMsg::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_protocolLogin_2eproto_getter, &descriptor_table_protocolLogin_2eproto_once,
      file_level_metadata_protocolLogin_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace protocol_msg
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::protocol_msg::LoginMsg* Arena::CreateMaybeMessage< ::protocol_msg::LoginMsg >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protocol_msg::LoginMsg >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>