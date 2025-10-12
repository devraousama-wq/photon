#include <gtest/gtest.h>

#include "photon/scene/json_loader.hpp"

TEST(SceneLoader, ValidatesCornellScene) {
    const auto errors = photon::scene::validate_scene_json("scenes/cornell.json");
    EXPECT_TRUE(errors.empty());
}
