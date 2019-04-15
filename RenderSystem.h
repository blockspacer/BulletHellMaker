#pragma once
#include "Components.h"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <utility>
#include <vector>
#include <functional>
#include <memory>

struct BloomSettings {
	bool useBloom = false;

	float glowStrength; // [1, inf]; determines how much the sprite and its "bloom shadow" glows
	float minBright; // [0, 1]; determines how much to darken only the sprite
	sf::BlendMode blendMode; // blend mode used when drawing the unblurred texture
};

/*
Shaders are implemented here with the assumption that there will never be user-customizable shaders that can
be unique to a sprite.
*/
class RenderSystem {
public:
	/*
	Window's view should already be set and should not be changed.
	*/
	RenderSystem(entt::DefaultRegistry& registry, sf::RenderWindow& window);
	void update(float deltaTime);

private:
	entt::DefaultRegistry& registry;
	sf::RenderWindow& window;

	struct SubLayerComparator {
		int operator()(const SpriteComponent& a, const SpriteComponent& b) {
			return a.getSubLayer() < b.getSubLayer();
		}
	};

	// Pairs of layer and entities in that layer
	// Entites in the same layer are sorted by sublayer
	std::vector<std::pair<int, std::vector<std::reference_wrapper<SpriteComponent>>>> layers;
	// Maps layer to the texture, onto which all sprites in a layer on drawn
	// All textures are the same size
	std::map<int, sf::RenderTexture> layerTextures;
	// Maps layer to the global shaders applied on that layer
	std::map<int, std::vector<std::unique_ptr<sf::Shader>>> globalShaders;

	// Bloom settings for each layer
	std::vector<BloomSettings> bloom;
	// Shaders used to blur layers
	std::vector<std::unique_ptr<sf::Shader>> bloomBlurShaders;
	// Shader used to brighten textures after blurring them
	sf::Shader bloomGlowShader;
	// Shader used to darken textures before blurring them
	sf::Shader bloomDarkShader;

	// Temporary layer texture for using multiple shaders
	sf::RenderTexture tempLayerTexture;
	sf::RenderTexture tempLayerTexture2;

	// Black magic needed to scale texture conversion into sprites correctly since views do not match the texture size
	// I actually don't know why this is needed only when using 2 or more global shaders on a texture
	float spriteHorizontalScale;
	float spriteVerticalScale;

	// Default blend mode
	sf::BlendMode blendMode = sf::BlendMode(sf::BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, sf::BlendMode::Equation::Add, sf::BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, sf::BlendMode::Equation::Add);
};