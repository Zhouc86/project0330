#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_HTML_PARSER_PARSER_SYNCHRONIZATION_POLICY_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_HTML_PARSER_PARSER_SYNCHRONIZATION_POLICY_H_

namespace blink {

enum ParserSynchronizationPolicy {
  kAllowAsynchronousParsing,
  kAllowDeferredParsing,
  kForceSynchronousParsing,
};
}

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_HTML_PARSER_PARSER_SYNCHRONIZATION_POLICY_H_
