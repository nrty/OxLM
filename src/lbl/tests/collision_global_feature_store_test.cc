#include "gtest/gtest.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "lbl/collision_global_feature_store.h"
#include "utils/constants.h"
#include "utils/testing.h"

namespace ar = boost::archive;

namespace oxlm {

class CollisionGlobalFeatureStoreTest : public testing::Test {
 protected:
  void SetUp() {
    CollisionMinibatchFeatureStore g_store(3, 10, 3);

    context = {1, 2, 3};
    VectorReal values(3);
    values << 4, 2, 5;
    g_store.update(context, values);

    store = CollisionGlobalFeatureStore(3, 10, 3);
    gradient_store = boost::make_shared<CollisionMinibatchFeatureStore>(
        g_store);
  }

  vector<int> context;
  CollisionGlobalFeatureStore store;
  boost::shared_ptr<MinibatchFeatureStore> gradient_store;
};

TEST_F(CollisionGlobalFeatureStoreTest, TestUpdateSquared) {
  store.updateSquared(gradient_store);

  VectorReal expected_values(3);
  expected_values << 144, 36, 225;
  EXPECT_MATRIX_NEAR(expected_values, store.get(context), EPS);
}

TEST_F(CollisionGlobalFeatureStoreTest, TestUpdateAdaGrad) {
  store.updateSquared(gradient_store);
  boost::shared_ptr<GlobalFeatureStore> adagrad_store =
      boost::make_shared<CollisionGlobalFeatureStore>(store);

  store.updateAdaGrad(gradient_store, adagrad_store, 1);
  VectorReal expected_values(3);
  expected_values << 141, 33, 222;
  EXPECT_MATRIX_NEAR(expected_values, store.get(context), EPS);
}

TEST_F(CollisionGlobalFeatureStoreTest, TestUpdateRegularizer) {
  store.updateSquared(gradient_store);
  store.l2GradientUpdate(gradient_store, 0.5);

  EXPECT_NEAR(3812.25, store.l2Objective(gradient_store, 1), EPS);
  VectorReal expected_values(3);
  expected_values << 72, 18, 112.5;
  EXPECT_MATRIX_NEAR(expected_values, store.get(context), EPS);
}

TEST_F(CollisionGlobalFeatureStoreTest, TestSerialization) {
  store.updateSquared(gradient_store);
  boost::shared_ptr<GlobalFeatureStore> store_ptr =
      boost::make_shared<CollisionGlobalFeatureStore>(store);
  boost::shared_ptr<GlobalFeatureStore> store_copy_ptr;

  stringstream stream(ios_base::binary | ios_base::out | ios_base::in);
  ar::binary_oarchive output_stream(stream);
  output_stream << store_ptr;

  ar::binary_iarchive input_stream(stream);
  input_stream >> store_copy_ptr;

  boost::shared_ptr<CollisionGlobalFeatureStore> expected_ptr =
      CollisionGlobalFeatureStore::cast(store_ptr);
  boost::shared_ptr<CollisionGlobalFeatureStore> actual_ptr =
      CollisionGlobalFeatureStore::cast(store_copy_ptr);

  EXPECT_NE(nullptr, expected_ptr);
  EXPECT_NE(nullptr, actual_ptr);
  EXPECT_EQ(*expected_ptr, *actual_ptr);

  VectorReal expected_values(3);
  expected_values << 144, 36, 225;
  EXPECT_MATRIX_NEAR(expected_values, actual_ptr->get(context), EPS);
}

} // namespace oxlm
