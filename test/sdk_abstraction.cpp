#include <gmock/gmock.h>

#include <string>

struct DirectiveSequencerInterface {
  virtual ~DirectiveSequencerInterface() = default;
  virtual void onDirective(const std::string &directive) const = 0;
};

struct DirectiveSequencer final
    : public DirectiveSequencerInterface { // check 2 classi final
  virtual ~DirectiveSequencer() = default; // check 1 distruttore virtuale
  void onDirective(const std::string &directive) const override {
    // precondition
    std::puts(__PRETTY_FUNCTION__);
    // NOT HERE!!!
    std::puts(__PRETTY_FUNCTION__);
    // postcondition
  }
};

// best case scenario
// TODO......... DirectiveSequencer::onDirective(...)

// worst case scenario
struct DirectiveSequencerWrapper : public DirectiveSequencerInterface {
  DirectiveSequencerWrapper(DirectiveSequencer &directiveSequencer)
      : m_directiveSequencer(directiveSequencer) {}
  void onDirective(const std::string &directive) const override {
    // precondition
    m_directiveSequencer.onDirective(directive);
    // postcondition
  }

private:
  DirectiveSequencer &m_directiveSequencer;
};
//===================================================
struct SampleApp {
  SampleApp(DirectiveSequencer &directiveSequencer)
      : m_directiveSequencer(directiveSequencer) {}

private:
  DirectiveSequencer &m_directiveSequencer;
};

TEST(sdk_abstraction, test) {
  DirectiveSequencer directiveSequencer;
  DirectiveSequencerWrapper directiveSequencerWrapper(directiveSequencer);
  SampleApp sampleApp(directiveSequencer);
  directiveSequencer.onDirective("");
}
