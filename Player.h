#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextMarshallable.h"
#include "EntityAnimatableSet.h"
#include "SpriteLoader.h"
#include "AudioPlayer.h"

class PlayerPowerTier : public TextMarshallable {
public:
	inline PlayerPowerTier() {}
	inline PlayerPowerTier(EntityAnimatableSet animatableSet, int attackPatternID, float attackPatternLoopDelay, int focusedAttackPatternID, float focusedAttackPatternLoopDelay, int bombAttackPatternID, float bombCooldown) :
		animatableSet(animatableSet), attackPatternID(attackPatternID), attackPatternLoopDelay(attackPatternLoopDelay), focusedAttackPatternID(focusedAttackPatternID), focusedAttackPatternLoopDelay(focusedAttackPatternLoopDelay), bombAttackPatternID(bombAttackPatternID), bombCooldown(bombCooldown) {}

	std::string format() const override;
	void load(std::string formattedString) override;

	inline const EntityAnimatableSet& getAnimatableSet() const { return animatableSet; }
	inline int getAttackPatternID() const { return attackPatternID; }
	inline float getAttackPatternLoopDelay() const { return attackPatternLoopDelay; }
	inline int getFocusedAttackPatternID() const { return focusedAttackPatternID; }
	inline float getFocusedAttackPatternLoopDelay() const { return focusedAttackPatternLoopDelay; }
	inline int getBombAttackPatternID() const { return bombAttackPatternID; }
	inline float getBombCooldown() const { return bombCooldown; }

	inline void setAttackPatternID(int id) { attackPatternID = id; }
	inline void setAttackPatternLoopDelay(float attackPatternLoopDelay) { this->attackPatternLoopDelay = attackPatternLoopDelay; }
	inline void setFocusedAttackPatternID(int id) { focusedAttackPatternID = id; }
	inline void setFocusedAttackPatternLoopDelay(float focusedAttackPatternLoopDelay) { this->focusedAttackPatternLoopDelay = focusedAttackPatternLoopDelay; }
	inline void setBombAttackPatternID(int id) { bombAttackPatternID = id; }
	inline void setBombCooldown(float bombCooldown) { this->bombCooldown = bombCooldown; }

private:
	EntityAnimatableSet animatableSet;

	int attackPatternID;
	// Time after attack pattern ends until it starts looping again
	float attackPatternLoopDelay;
	int focusedAttackPatternID;
	float focusedAttackPatternLoopDelay;

	// Attack pattern ID of the attack pattern that plays when a bomb is used
	int bombAttackPatternID;
	// Time after a bomb is used that the player can use another bomb. Should be greater than the time to go through every attack in the bomb attack pattern.
	float bombCooldown;
};

class EditorPlayer : public TextMarshallable {
public:
	inline EditorPlayer() {}

	std::string format() const override;
	void load(std::string formattedString) override;

	bool legal(SpriteLoader& spriteLoader, std::string& message);

	inline int getInitialHealth() const { return initialHealth; }
	inline int getMaxHealth() const { return maxHealth; }
	inline float getSpeed() const { return speed; }
	inline float getFocusedSpeed() const { return focusedSpeed; }
	inline const std::vector<PlayerPowerTier>& getPowerTiers() const { return powerTiers; }
	inline float getHitboxRadius() const { return hitboxRadius; }
	inline float getHitboxPosX() const { return hitboxPosX; }
	inline float getHitboxPosY() const { return hitboxPosY; }
	inline float getInvulnerabilityTime() const { return invulnerabilityTime; }
	inline SoundSettings getHurtSound() const { return hurtSound; }
	inline SoundSettings getDeathSound() const { return deathSound; }
	inline bool getSmoothPlayerHPBar() const { return smoothPlayerHPBar; }
	inline sf::Color getPlayerHPBarColor() const { return playerHPBarColor; }
	inline Animatable getDiscretePlayerHPSprite() const { return discretePlayerHPSprite; }
	inline int getInitialBombs() const { return initialBombs; }
	inline int getMaxBombs() const { return maxBombs; }
	inline Animatable getBombSprite() const { return bombSprite; }
	inline SoundSettings getBombReadySound() const { return bombReadySound; }
	inline float getBombInvincibilityTime() const { return bombInvincibilityTime; }
	inline bool usesAttackPattern(int attackPatternID) const { return attackPatternIDCount.count(attackPatternID) > 0 && attackPatternIDCount.at(attackPatternID) > 0; }

	/*
	Returns a reference to the power tier.
	*/
	inline PlayerPowerTier& getPowerTier(int index) { return powerTiers[index]; }

	inline void setHurtSound(SoundSettings hurtSound) { this->hurtSound = hurtSound; }
	inline void setDeathSound(SoundSettings deathSound) { this->deathSound = deathSound; }
	inline void setBombReadySound(SoundSettings bombReadySound) { this->bombReadySound = bombReadySound; }
	inline void setInitialHealth(int initialHealth) { this->initialHealth = initialHealth; }
	inline void setMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
	inline void setSpeed(float speed) { this->speed = speed; }
	inline void setFocusedSpeed(float focusedSpeed) { this->focusedSpeed = focusedSpeed; }
	inline void setHitboxRadius(float hitboxRadius) { this->hitboxRadius = hitboxRadius; }
	inline void setHitboxPosX(float hitboxPosX) { this->hitboxPosX = hitboxPosX; }
	inline void setHitboxPosY(float hitboxPosY) { this->hitboxPosY = hitboxPosY; }
	inline void setInvulnerabilityTime(float invulnerabilityTime) { this->invulnerabilityTime = invulnerabilityTime; }
	inline void insertPowerTier(int index, PlayerPowerTier powerTier) { 
		powerTiers.insert(powerTiers.begin() + index, powerTier);

		int attackPatternID = powerTier.getAttackPatternID();
		if (attackPatternIDCount.count(attackPatternID) == 0) {
			attackPatternIDCount[attackPatternID] = 1;
		} else {
			attackPatternIDCount[attackPatternID]++;
		}
	}
	inline void removePowerTier(int index) {
		int attackPatternID = powerTiers[index].getAttackPatternID();
		powerTiers.erase(powerTiers.begin() + index);
		attackPatternIDCount[attackPatternID]--;
	}
	inline void setSmoothPlayerHPBar(bool smoothPlayerHPBar) { this->smoothPlayerHPBar = smoothPlayerHPBar; }
	inline void setPlayerHPBarColor(sf::Color playerHPBarColor) { this->playerHPBarColor = playerHPBarColor; }
	inline void setDiscretePlayerHPSprite(Animatable discretePlayerHPSprite) { this->discretePlayerHPSprite = discretePlayerHPSprite; }
	inline void getInitialBombs(int initialBombs) { this->initialBombs = initialBombs; }
	inline void getMaxBombs(int initialBombs) { this->initialBombs = maxBombs; }
	inline void setBombSprite(Animatable bombSprite) { this->bombSprite = bombSprite; }
	inline void setBombInvincibilityTime(float bombInvincibilityTime) {	this->bombInvincibilityTime = bombInvincibilityTime; }

private:
	int initialHealth = 3;
	int maxHealth = 5;
	// Default player speed
	float speed = 120;
	// Player speed when holding focus key
	float focusedSpeed = 40;

	// Radius of the hitbox associated with this enemy
	float hitboxRadius = 1;
	// Local position of hitbox
	float hitboxPosX = 0, hitboxPosY = 0;

	// Time player is invulnerable for when hit by an enemy bullet
	float invulnerabilityTime = 2.0f;
	
	/*
	The player's power tier increase every POWER_PER_POWER_TIER power, which come from power packs dropped by enemies.
	*/
	std::vector<PlayerPowerTier> powerTiers;

	SoundSettings hurtSound;
	SoundSettings deathSound;

	// If this is true, the HP bar will be a progress bar. If false, there will be a discretePlayerHPSprite displayed for each health the player has. 
	bool smoothPlayerHPBar = false;
	sf::Color playerHPBarColor = sf::Color::Red;
	// Must be a sprite. Only used if smoothPlayerHPBar is false.
	Animatable discretePlayerHPSprite;

	// The sprite shown on the GUI to denote a bomb. Must be a sprite.
	Animatable bombSprite;

	int initialBombs = 2;
	int maxBombs = 6;

	// Sound played when bomb is off cooldown
	SoundSettings bombReadySound;

	// Amount of time player is invincible for after activating a bomb
	float bombInvincibilityTime = 5.0f;

	// Maps an EditorAttackPattern's ID to the number of times it appears in powerTiers.
	// This map isn't saved in format() but is reconstructed in load().
	std::map<int, int> attackPatternIDCount;
};