#include <gmock/gmock.h>

#include <memory_resource>

TEST(pmr, vector_string)
{
  std::pmr::vector<std::pmr::string> pmr_vector_string;
  pmr_vector_string.push_back("first");
  ASSERT_EQ("first", pmr_vector_string.front());
}

TEST(pmr, monotonic_buffer_resource)
{
  // Arrange
  std::pmr::monotonic_buffer_resource pool{ 1024 };
  std::pmr::polymorphic_allocator<char> alloc{ &pool };
  std::pmr::vector<std::pmr::string> vec{ alloc };

  // Act
  vec.emplace_back("Hello, ");
  vec.emplace_back("world!");

  // Assert
  ASSERT_EQ("Hello, world!", vec.front() + vec.back());
}

TEST(pmr, synchronized_pool_resource)
{
  // Arrange
  std::pmr::monotonic_buffer_resource buffer{ 1024 };
  std::pmr::synchronized_pool_resource pool{ &buffer };
  std::pmr::polymorphic_allocator<char> alloc{ &pool };
  std::pmr::vector<std::pmr::string> vec{ alloc };

  // Act
  vec.emplace_back("Hello, ");
  vec.emplace_back("world!");

  // Assert
  ASSERT_EQ("Hello, world!", vec.front() + vec.back());
}
