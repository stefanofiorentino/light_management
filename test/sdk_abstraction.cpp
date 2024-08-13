#include <gmock/gmock.h>

#include <string>

struct DirectiveSequencerInterface
{
  virtual ~DirectiveSequencerInterface() = default;
  virtual void onDirective(const std::string& directive) const = 0;
};

struct DirectiveSequencer final : public DirectiveSequencerInterface
{                                          // check 2 classi final
  virtual ~DirectiveSequencer() = default; // check 1 distruttore virtuale
  void onDirective(const std::string& /*directive*/) const override
  {
    std::puts(__PRETTY_FUNCTION__);
  }
};

// best case scenario
// TODO......... DirectiveSequencer::onDirective(...)

// worst case scenario
struct DirectiveSequencerWrapper : public DirectiveSequencerInterface
{
  explicit DirectiveSequencerWrapper(
    DirectiveSequencerInterface& directiveSequencer)
    : m_directiveSequencer(directiveSequencer)
  {
  }
  void onDirective(const std::string& directive) const override
  {
    // precondition
    m_directiveSequencer.onDirective(directive);
    // postcondition
  }

private:
  DirectiveSequencerInterface& m_directiveSequencer;
};
//===================================================
struct SampleApp
{
  explicit SampleApp(DirectiveSequencerInterface& directiveSequencer)
    : m_directiveSequencer(directiveSequencer)
  {
    m_directiveSequencer.onDirective("");
  }

private:
  DirectiveSequencerInterface& m_directiveSequencer;
};

TEST(sdk_abstraction, test)
{
  DirectiveSequencer directiveSequencer;
  DirectiveSequencerWrapper directiveSequencerWrapper(directiveSequencer);
  [[maybe_unused]]
  SampleApp sampleApp(directiveSequencerWrapper);
}
