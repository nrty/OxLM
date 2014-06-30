#include "gtest/gtest.h"

#include <boost/make_shared.hpp>

#include "lbl/weights.h"
#include "utils/constants.h"
#include "utils/testing.h"

namespace oxlm {

class TestWeights : public testing::Test {
 protected:
  void SetUp() {
    config.word_representation_size = 3;
    config.vocab_size = 5;
    config.ngram_order = 3;

    vector<int> data = {2, 3, 4, 1};
    corpus = boost::make_shared<Corpus>(data);
  }

  ModelData config;
  boost::shared_ptr<Metadata> metadata;
  boost::shared_ptr<Corpus> corpus;
};

TEST_F(TestWeights, TestGradientCheck) {
  Weights weights(config, metadata, corpus);
  vector<int> indices = {0, 1, 2, 3};
  Real objective;
  boost::shared_ptr<Weights> gradient =
      weights.getGradient(corpus, indices, objective);

  EXPECT_NEAR(6.0826482, objective, EPS);
  // In truth, using float for model parameters instead of double seriously
  // degrades the gradient computation, but has no negative effect on the
  // performance of the model and gives a 2x speed up and reduces memory by 2x.
  //
  // If you suspect there might be something off with the gradient, change
  // typedef Real to double and set a lower accepted error (e.g. 1e-5) when
  // checking the gradient.
  EXPECT_TRUE(weights.checkGradient(corpus, indices, gradient, 1e-3));
}

TEST_F(TestWeights, TestGradientCheckDiagonal) {
  config.diagonal_contexts = true;

  Weights weights(config, metadata, corpus);
  vector<int> indices = {0, 1, 2, 3};
  Real objective;
  boost::shared_ptr<Weights> gradient =
      weights.getGradient(corpus, indices, objective);

  EXPECT_NEAR(6.084939479, objective, EPS);
  // See the comment above if you suspect the gradient is not computed
  // correctly.
  EXPECT_TRUE(weights.checkGradient(corpus, indices, gradient, 1e-3));
}

} // namespace oxlm