#include "DebugRenderSystem.h"
#include "Components.h"
#include "Constants.h"

DebugRenderSystem::DebugRenderSystem(entt::DefaultRegistry& registry, sf::RenderWindow& window, SpriteLoader& spriteLoader, float resolutionMultiplier) : RenderSystem(registry, window, spriteLoader, resolutionMultiplier, false) {
	circleFormat.setFillColor(sf::Color(sf::Color::Transparent));
	circleFormat.setOutlineThickness(3);
}

void DebugRenderSystem::update(float deltaTime) {
	for (int i = 0; i < layers.size(); i++) {
		layers[i].second.clear();
	}

	auto view = registry.view<PositionComponent, SpriteComponent>(entt::persistent_t{});
	view.each([&](auto entity, auto& position, auto& sprite) {
		if (sprite.getSprite()) {
			sprite.getSprite()->setPosition(position.getX() * resolutionMultiplier, (MAP_HEIGHT - position.getY()) * resolutionMultiplier);
			layers[sprite.getRenderLayer()].second.push_back(std::ref(sprite));
		}
	});

	for (std::pair<int, std::vector<std::reference_wrapper<SpriteComponent>>>& pair : layers) {
		std::sort(pair.second.begin(), pair.second.end(), SubLayerComparator());
	}

	// Move background
	backgroundX = std::fmod((backgroundX + backgroundScrollSpeedX * deltaTime), backgroundTextureSizeX);
	backgroundY = std::fmod((backgroundY + backgroundScrollSpeedY * deltaTime), backgroundTextureSizeY);
	backgroundSprite.setTextureRect(sf::IntRect(backgroundX, backgroundY, backgroundTextureWidth, backgroundTextureHeight));

	// Draw background by drawing onto the temp layer first to limit the visible part of the background to the play area
	tempLayerTexture.clear(sf::Color::Transparent);
	backgroundSprite.setPosition(0, -MAP_HEIGHT * resolutionMultiplier);
	tempLayerTexture.draw(backgroundSprite);
	tempLayerTexture.display();
	sf::Sprite backgroundAsSprite(tempLayerTexture.getTexture());
	sf::RenderStates backgroundStates;
	backgroundStates.blendMode = DEFAULT_BLEND_MODE;
	window.draw(backgroundAsSprite, backgroundStates);

	// Draw the layers onto the window directly
	for (int i = 0; i < layers.size(); i++) {
		for (SpriteComponent& sprite : layers[i].second) {
			std::shared_ptr<sf::Sprite> spritePtr = sprite.getSprite();

			window.draw(*spritePtr);
		}
	}

	// Draw the hitboxes
	auto view2 = registry.view<PositionComponent, HitboxComponent>(entt::persistent_t{});
	view2.each([&](auto entity, auto position, auto hitbox) {
		// Radius takes into account outline thickness expanding outwards,
		// making the circle appear larger than it actually is
		circleFormat.setRadius(hitbox.getRadius() - circleFormat.getOutlineThickness());
		if (registry.attachee<PlayerTag>() == entity) {
			circleFormat.setOutlineColor(sf::Color(sf::Color::Blue));
		} else {
			circleFormat.setOutlineColor(sf::Color(sf::Color::Magenta));
		}
		circleFormat.setPosition((position.getX() + hitbox.getX() - circleFormat.getRadius()) * resolutionMultiplier, (MAP_HEIGHT - (position.getY() + hitbox.getY() + circleFormat.getRadius())) * resolutionMultiplier);
		window.draw(circleFormat);
	});
}

void DebugRenderSystem::setResolution(SpriteLoader& spriteLoader, float resolutionMultiplier) {
	spriteLoader.setGlobalSpriteScale(resolutionMultiplier);

	int newPlayAreaWidth = (int)std::round(MAP_WIDTH * resolutionMultiplier);
	int newPlayAreaHeight = (int)std::round(MAP_HEIGHT * resolutionMultiplier);

	sf::View view(sf::FloatRect(0, -newPlayAreaHeight, newPlayAreaWidth, newPlayAreaHeight));
	tempLayerTexture.create(newPlayAreaWidth, newPlayAreaHeight);
	tempLayerTexture.setView(view);

	spriteHorizontalScale = view.getSize().x / tempLayerTexture.getSize().x;
	spriteVerticalScale = view.getSize().y / tempLayerTexture.getSize().y;

	backgroundSprite.setScale(MAP_WIDTH / backgroundTextureWidth * resolutionMultiplier, MAP_HEIGHT / backgroundTextureHeight * resolutionMultiplier);

	if (onResolutionChange) {
		onResolutionChange->publish();
	}
}
