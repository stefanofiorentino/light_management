#include <gmock/gmock.h>

#include <stdexcept>

struct base
{
  virtual ~base() = default;
  virtual bool foo() const noexcept { return true; }
};

template<typename T>
struct not_null_ptr
{
  explicit not_null_ptr(T* t)
    : pointee(t)
  {
  }
  explicit not_null_ptr(std::nullptr_t) = delete;

  T& operator*() const
  {
    if (!pointee)
      throw std::runtime_error("");
    return *pointee;
  }

  T* operator->() const
  {
    if (!pointee)
      throw std::runtime_error("");
    return pointee;
  }

private:
  T* pointee;
};

TEST(not_null_ptr, whenSimpleThenWorks)
{
  base b;
  not_null_ptr<base> pb(&b);
  ASSERT_TRUE(pb->foo());
}

static base*
getPtr()
{
  return nullptr;
}

TEST(not_null_ptr, whenOwnsANullptrThenThrows)
{
  not_null_ptr<base> pb(getPtr());
  ASSERT_THROW(pb->foo(), std::runtime_error);
}

static bool
invokeFooOnConstRefPb(not_null_ptr<base> const& pb)
{
  return pb->foo();
}

TEST(not_null_ptr, whenPassedByConstRefThenWorks)
{
  base b;
  not_null_ptr<base> pb(&b);
  ASSERT_TRUE(invokeFooOnConstRefPb(pb));
}

static bool
invokeFooOnRefPb(not_null_ptr<base>& pb)
{
  return pb->foo();
}

TEST(not_null_ptr, whenPassedByRefThenWorks)
{
  base b;
  not_null_ptr<base> pb(&b);
  ASSERT_TRUE(invokeFooOnRefPb(pb));
}

static bool
invokeFooOnPbByCopy(not_null_ptr<base> pb)
{
  return pb->foo();
}

TEST(not_null_ptr, whenPassedByCopyThenWorks)
{
  base b;
  not_null_ptr<base> pb(&b);
  ASSERT_TRUE(invokeFooOnPbByCopy(pb));
}

TEST(not_null_ptr, whenDereferenceThenWorks)
{
  base b;
  not_null_ptr<base> pb(&b);
  auto& deref = *pb;
  ASSERT_TRUE(deref.foo());
}

TEST(not_null_ptr, whenDereferenceNullPtrThenThrows)
{
  not_null_ptr<base> pb(getPtr());
  ASSERT_THROW(*pb, std::runtime_error);
}
