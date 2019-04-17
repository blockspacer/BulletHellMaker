#pragma once
#include <memory>
#include <vector>
#include <string>
#include "TextMarshallable.h"
#include "EntityAnimatableSet.h"

class PlayerPowerTier : public TextMarshallable {
public:
	inline PlayerPowerTier() {}
	inline PlayerPowerTier(EntityAnimatableSet animatableSet, int attackPatternID, float attackPatternLoopDelay, int focusedAttackPatternID, float focusedAttackPatternLoopDelay) :
		animatableSet(animatableSet), attackPatternID(attackPatternID), attackPatternLoopDelay(attackPatternLoopDelay), focusedAttackPatternID(focusedAttackPatternID), focusedAttackPatternLoopDelay(focusedAttackPatternLoopDelay) {}

	std::string format() override;
	void load(std::string formattedString) override;

	inline const EntityAnimatableSet& getAnimatableSet() const { return animatableSet; }
	inline int getAttackPatternID() const { return attackPatternID; }
	inline float getAttackPatternLoopDelay() const { return attackPatternLoopDelay; }
	inline int getFocusedAttackPatternID() const { return focusedAttackPatternID; }
	inline float getFocusedAttackPatternLoopDelay() const { return focusedAttackPatternLoopDelay; }

	inline void setAttackPatternID(int id) { attackPatternID = id; }
	inline void setAttackPatternLoopDelay(float attackPatternLoopDelay) { this->attackPatternLoopDelay = attackPatternLoopDelay; }
	inline void setFocusedAttackPatternID(int id) { focusedAttackPatternID = id; }
	inline void setFocusedAttackPatternLoopDelay(float focusedAttackPatternLoopDelay) { this->focusedAttackPatternLoopDelay = focusedAttackPatternLoopDelay; }

private:
	EntityAnimatableSet animatableSet;

	int attackPatternID;
	// Time after attack pattern ends until it starts looping again
	float attackPatternLoopDelay;
	int focusedAttackPatternID;
	float focusedAttackPatternLoopDelay;
};

class EditorPlayer : public TextMarshallable {
public:
	inline EditorPlayer() {}
	inline EditorPlayer(float initialHealth, float maxHealth, float speed, float focusedSpeed, float hitboxRadius, float hitboxPosX, float hitboxPosY, std::vector<PlayerPowerTier> powerTiers) : 
		initialHealth(initialHealth), maxHealth(maxHealth), speed(speed), focusedSpeed(focusedSpeed), hitboxRadius(hitboxRadius), hitboxPosX(hitboxPosX), hitboxPosY(hitboxPosY), powerTiers(powerTiers) {}

	std::string format() override;
	void load(std::string formattedString) override;

	inline int getInitialHealth() { return initialHealth; }
	inline int getMaxHealth() { return maxHealth; }
	inline float getSpeed() { return speed; }
	inline float getFocusedSpeed() { return focusedSpeed; }
	inline const std::vector<PlayerPowerTier> getPowerTiers() { return powerTiers; }
	inline float getHitboxRadius() { return hitboxRadius; }
	inline float getHitboxPosX() { return hitboxPosX; }
	inline float getHitboxPosY() { return hitboxPosY; }

	/*
	Returns a reference to the power tier.
	*/
	inline PlayerPowerTier& getPowerTier(int index) { return powerTiers[index]; }

	inline void setInitialHealth(int initialHealth) { this->initialHealth = initialHealth; }
	inline void setMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
	inline void setSpeed(float speed) { this->speed = speed; }
	inline void setFocusedSpeed(float focusedSpeed) { this->focusedSpeed = focusedSpeed; }
	inline void setHitboxRadius(float hitboxRadius) { this->hitboxRadius = hitboxRadius; }
	inline void setHitboxPosX(float hitboxPosX) { this->hitboxPosX = hitboxPosX; }
	inline void setHitboxPosY(float hitboxPosY) { this->hitboxPosY = hitboxPosY; }
	inline void insertPowerTier(int index, PlayerPowerTier powerTier) { powerTiers.insert(powerTiers.begin() + index, powerTier); }
	inline void removePowerTier(int index) { powerTiers.erase(powerTiers.begin() + index); }

private:
	int initialHealth;
	int maxHealth;
	// Default player speed
	float speed;
	// Player speed when holding focus key
	float focusedSpeed;

	// Radius of the hitbox associated with this enemy
	float hitboxRadius;
	// Local position of hitbox
	float hitboxPosX, hitboxPosY;
	
	/*
	The player's power tier increase every POWER_PER_POWER_TIER power, which come from power packs dropped by enemies.
	*/
	std::vector<PlayerPowerTier> powerTiers;
};