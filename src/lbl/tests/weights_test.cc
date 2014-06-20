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
  EXPECT_TRUE(weights.checkGradient(corpus, indices, gradient));
}

TEST_F(TestWeights, TestGradientCheckDiagonal) {
  config.diagonal_contexts = true;

  Weights weights(config, metadata, corpus);
  vector<int> indices = {0, 1, 2, 3};
  Real objective;
  boost::shared_ptr<Weights> gradient =
      weights.getGradient(corpus, indices, objective);

  EXPECT_NEAR(6.012124939, objective, EPS);
  EXPECT_TRUE(weights.checkGradient(corpus, indices, gradient));
}

TEST_F(TestWeights, TestGetObjective) {
  Weights weights(config, metadata, corpus);
  vector<int> indices = {0, 1, 2, 3};
  EXPECT_NEAR(6.0826482, weights.getObjective(corpus, indices), EPS);
}

TEST_F(TestWeights, TestClear) {
  Weights weights(config, metadata, corpus);
  weights.clear();
}

} // namespace oxlm
