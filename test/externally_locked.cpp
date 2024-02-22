#include <gmock/gmock.h>

#include <mutex>

// https://www.informit.com/articles/article.aspx?p=25298&seqNum=4
class BankAccount
{
  int balance_{};

public:
  void Deposit(int amount) { balance_ += amount; }
  void Withdraw(int amount) { balance_ -= amount; }
  int balance() const { return balance_; }
};

template<class T, class Owner>
class ExternallyLocked
{
  T obj_;

public:
  ExternallyLocked() = default;
  explicit ExternallyLocked(const T& obj)
    : obj_(obj)
  {
  }
  T& Get(std::unique_lock<Owner>&) { return obj_; }
};

class AccountManager
{
  mutable std::mutex mtx_;
  ExternallyLocked<BankAccount, AccountManager> checkingAcct_;
  ExternallyLocked<BankAccount, AccountManager> savingsAcct_;

public:
  void lock() const { mtx_.lock(); }
  void unlock() const { return mtx_.unlock(); }
  void Checking2Savings(int amount)
  {
    std::unique_lock<AccountManager> guard(*this);
    checkingAcct_.Get(guard).Withdraw(amount);
    savingsAcct_.Get(guard).Deposit(amount);
  }
  int CheckingBalance()
  {
    std::unique_lock<AccountManager> guard(*this);
    return checkingAcct_.Get(guard).balance();
  }
};

TEST(ExternallyLocked, simple)
{
  AccountManager am;
  am.Checking2Savings(2);
  ASSERT_EQ(-2, am.CheckingBalance());
}
