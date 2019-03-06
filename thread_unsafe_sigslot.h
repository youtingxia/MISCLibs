// created By MarvinZhai

#if __cplusplus < 201103L
#error "Need C++11 or Higher stdandard Impl"
#endif

#pragma once

#include <set>
#include <type_traits>

#undef DELETE_DEFAULT_CTOR
#undef DELETE_ASSIGN
#undef DELETE_COPY_AND_ASSIGN
#undef DELETE_MOVE_COPY
#undef DELETE_MOVE_COPY_AND_ASSIGN

#define DELETE_DEFAULT_CTOR(TypeName) \
  TypeName() = delete

#define DELETE_ASSIGN(TypeName) \
  TypeName &operator=(const TypeName &) = delete
#define DELETE_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName &) = delete;   \
  DELETE_ASSIGN(TypeName)

#define DELETE_MOVE_COPY(TypeName) \
  TypeName(const TypeName &&) = delete
#define DELETE_MOVE_COPY_AND_ASSIGN(TypeName)      \
  TypeName &operator=(const TypeName &&) = delete; \
  DELETE_MOVE_COPY(TypeName)

namespace thread_unsafe_sigslot {

class _signal_base_interface;
class has_slots {
  DELETE_COPY_AND_ASSIGN(has_slots);
  DELETE_MOVE_COPY_AND_ASSIGN(has_slots);
  friend class _signal_base_interface;
public:
  has_slots() = default;
  virtual ~has_slots(); // disconnect all
private:
  std::set<_signal_base_interface *> m_senders;
};

class _signal_base_interface {
public:
  _signal_base_interface() : m_phasslot(nullptr) {}
  virtual ~_signal_base_interface(){
    onDisconnect();
  }
  virtual void disconnect() = 0;
protected:
  has_slots *m_phasslot;
  void onConnect(has_slots *slotptr){
    m_phasslot = slotptr;
    m_phasslot->m_senders.insert(this);
  }
  void onDisconnect(){
    if (m_phasslot)
      m_phasslot->m_senders.erase(this);
    m_phasslot = nullptr;
  }
};

has_slots::~has_slots()
{
  for (auto s : m_senders)
    s->disconnect();
  m_senders.clear();
}


template <typename... args_type>
class _connection_base {
public:
  virtual ~_connection_base() = default;
  virtual void emit(args_type... args) = 0;
};
template <typename dest_type, typename... arg_types>
class _connectionN final : public _connection_base<arg_types...> {
  DELETE_DEFAULT_CTOR(_connectionN);
  DELETE_COPY_AND_ASSIGN(_connectionN);
  DELETE_MOVE_COPY_AND_ASSIGN(_connectionN);
public:
  _connectionN(dest_type *pobject, void (dest_type::*pmemfun)(arg_types...))
      : m_pobject(pobject), m_pmemfun(pmemfun) {}
  ~_connectionN() = default;
  void emit(arg_types... args) override {
    (m_pobject->*m_pmemfun)(args...);
  }
private:
  dest_type *m_pobject;
  void (dest_type::*m_pmemfun)(arg_types...);
};

template <typename... args_type>
class _signalN final : public _signal_base_interface {
  DELETE_COPY_AND_ASSIGN(_signalN);
  DELETE_MOVE_COPY_AND_ASSIGN(_signalN);
public:
  _signalN() : _signal_base_interface(), m_pconn(nullptr) {}
  ~_signalN() { disconnect(); }
  template <typename desttype>
  void connect(desttype *pclass, void (desttype::*pmemfun)(args_type...)){
    if (m_pconn)
      delete m_pconn;
    m_pconn = new _connectionN<desttype, args_type...>(pclass, pmemfun);
    static_assert(std::is_base_of<has_slots, desttype>::value, "slot must drived from has_slots");
    onConnect(static_cast<has_slots *>(pclass));
  }
  void disconnect() override {
    if (m_pconn)
      delete m_pconn;
    m_pconn = nullptr;
    onDisconnect();
  }
  void operator()(args_type... args){
    if (m_pconn)
      m_pconn->emit(args...);
  }

private:
  _connection_base<args_type...> *m_pconn;
};

// alias _signalN : signal0,signal1,signal2 ... ...
template <template <typename ...> class T, typename ...args>
struct _restrictZeroArgs {  
  static_assert(sizeof...(args) == 0, "incorrect number of arguments!");
};

template <template <typename ...> class T>
struct _restrictZeroArgs<T> {
  using type = T<>;
};

template <typename ...no_args>
using signal0 = typename _restrictZeroArgs<_signalN, no_args...>::type;

template <typename arg1_type>
using signal1 = _signalN<arg1_type>;

template <typename arg1_type, typename arg2_type>
using signal2 = _signalN<arg1_type, arg2_type>;

template <typename arg1_type, typename arg2_type, typename arg3_type>
using signal3 = _signalN<arg1_type, arg2_type, arg3_type>;

template <typename arg1_type, typename arg2_type, typename arg3_type, typename arg4_type>
using signal4 = _signalN<arg1_type, arg2_type, arg3_type, arg4_type>;

template <typename a1, typename a2, typename a3, typename a4, typename a5>
using signal5 = _signalN<a1, a2, a3, a4, a5>;

template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
using signal6 = _signalN<a1, a2, a3, a4, a5, a6>;

template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
using signal7 = _signalN<a1, a2, a3, a4, a5, a6, a7>;

template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8>
using signal8 = _signalN<a1, a2, a3, a4, a5, a6, a7, a8>;

} // namespace threadUnSafe_sigslot
