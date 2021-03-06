#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <SFML/Audio.hpp>
#include "MovablePoint.h"
#include "SpriteLoader.h"
#include "TextMarshallable.h"
#include "Components.h"
#include "AudioPlayer.h"
#include <entt/entt.hpp>

class EditorAttack;
class EditorAttackPattern;
class EditorMovablePoint;
class EditorEnemy;
class EditorEnemyPhase;
class EditorPlayer;
class BulletModel;

class LevelPackMetadata : public TextMarshallable {
public:
	std::string format() const override;
	void load(std::string formattedString) override;

	std::shared_ptr<EditorPlayer> getPlayer();
	const std::vector<std::pair<std::string, std::string>>& getSpriteSheets() { return spriteSheets; }

	void setPlayer(std::shared_ptr<EditorPlayer> player);

	void addSpriteSheet(std::string spriteSheetMetadataFileName, std::string spriteSheetImageFileName) {
		spriteSheets.push_back(std::make_pair(spriteSheetMetadataFileName, spriteSheetImageFileName));
	}
	void removeSpriteSheet(int index) {
		spriteSheets.erase(spriteSheets.begin() + index);
	}

private:
	std::shared_ptr<EditorPlayer> player;
	// Pairs of sheet metadata file name and sprite sheet image file name, in that order
	std::vector<std::pair<std::string, std::string>> spriteSheets;
};

class LevelPack {
public:
	LevelPack(AudioPlayer& audioPlayer, std::string name);

	/*
	Load the LevelPack from its folder.
	*/
	void load();
	/*
	Save the LevelPack into its folder.
	*/
	void save();

	/*
	Creates the sprite loader that contains info for all animatables that are used in this level pack.
	*/
	std::unique_ptr<SpriteLoader> createSpriteLoader();

	/*
	Insert a Level into this LevelPack at the specified index.
	*/
	void insertLevel(int index, std::shared_ptr<Level> level);

	/*
	Create an EditorAttack and add it to this LevelPack.
	*/
	std::shared_ptr<EditorAttack> createAttack();
	/*
	Create an EditorAttackPattern and add it to this LevelPack.
	*/
	std::shared_ptr<EditorAttackPattern> createAttackPattern();
	/*
	Create an EditorEnemy and add it to this LevelPack.
	*/
	std::shared_ptr<EditorEnemy> createEnemy();
	/*
	Create an EditorEnemyPhase and add it to this LevelPack.
	*/
	std::shared_ptr<EditorEnemyPhase> createEnemyPhase();
	/*
	Create an BulletModel and add it to this LevelPack.
	*/
	std::shared_ptr<BulletModel> createBulletModel();

	/*
	Updates an attack.
	If the attack ID is already in the LevelPack, overwrite the attack.
	If the attack ID is not in the LevelPack, add in the attack.
	*/
	void updateAttack(std::shared_ptr<EditorAttack> attack);
	/*
	Updates an attack pattern.
	If the attack pattern ID is already in the LevelPack, overwrite the attack pattern.
	If the attack pattern ID is not in the LevelPack, add in the attack pattern.
	*/
	void updateAttackPattern(std::shared_ptr<EditorAttackPattern> attackPattern);
	/*
	Updates an enemy.
	If the enemy ID is already in the LevelPack, overwrite the enemy.
	If the enemy ID is not in the LevelPack, add in the enemy.
	*/
	void updateEnemy(std::shared_ptr<EditorEnemy> enemy);
	/*
	Updates an enemy phase.
	If the enemy phase ID is already in the LevelPack, overwrite the enemy phase.
	If the enemy phase ID is not in the LevelPack, add in the enemy phase.
	*/
	void updateEnemyPhase(std::shared_ptr<EditorEnemyPhase> enemyPhase);
	/*
	Updates a bullet model.
	If the bullet model ID is already in the LevelPack, overwrite the bullet model.
	If the bullet model ID is not in the LevelPack, add in the bullet model.
	*/
	void updateBulletModel(std::shared_ptr<BulletModel> bulletModel);

	void deleteLevel(int levelIndex);
	void deleteAttack(int id);
	void deleteAttackPattern(int id);
	void deleteEnemy(int id);
	void deleteEnemyPhase(int id);
	void deleteBulletModel(int id);

	/*
	Returns a list of indices of Levels that use the EditorEnemy
	with ID enemyID.
	*/
	std::vector<int> getEnemyUsers(int enemyID);
	/*
	Returns a list of IDs of EditorEnemies that use the EditorEnemyPhase
	with ID editorEnemyPhaseID.
	*/
	std::vector<int> getEditorEnemyUsers(int editorEnemyPhaseID);
	/*
	Returns whether the LevelPack's EditorPlayer uses the EditorAttackPattern
	with ID attackPatternID.
	*/
	bool attackPatternIsUsedByPlayer(int attackPatternID);
	/*
	Returns a list of IDs of EditorEnemyPhases that use the EditorAttackPattern
	with ID attackPatternID.
	*/
	std::vector<int> getAttackPatternEnemyUsers(int attackPatternID);
	/*
	Returns a list of IDs of EditorAttackPatterns that use the EditorAttack
	with ID attackID.
	*/
	std::vector<int> getAttackUsers(int attackID);
	/*
	Returns a list of IDs of EditorAttacks that use the BulletModel
	with ID bulletModelID.
	*/
	std::vector<int> getBulletModelUsers(int bulletModelID);

	bool hasEnemy(int id);
	bool hasEnemyPhase(int id);
	bool hasAttackPattern(int id);
	bool hasAttack(int id);
	bool hasBulletModel(int id);
	bool hasLevel(int levelIndex);
	bool hasBulletModel(int id) const;

	std::string getName();
	std::shared_ptr<Level> getLevel(int levelIndex) const;
	std::shared_ptr<EditorAttack> getAttack(int id) const;
	std::shared_ptr<EditorAttackPattern> getAttackPattern(int id) const;
	std::shared_ptr<EditorEnemy> getEnemy(int id) const;
	std::shared_ptr<EditorEnemyPhase> getEnemyPhase(int id) const;
	std::shared_ptr<BulletModel> getBulletModel(int id) const;
	std::shared_ptr<EditorPlayer> getPlayer();
	std::string getFontFileName();

	std::map<int, std::shared_ptr<EditorAttack>>::iterator getAttackIteratorBegin();
	std::map<int, std::shared_ptr<EditorAttack>>::iterator getAttackIteratorEnd();
	std::map<int, std::shared_ptr<EditorAttackPattern>>::iterator getAttackPatternIteratorBegin();
	std::map<int, std::shared_ptr<EditorAttackPattern>>::iterator getAttackPatternIteratorEnd();
	std::map<int, std::shared_ptr<EditorEnemy>>::iterator getEnemyIteratorBegin();
	std::map<int, std::shared_ptr<EditorEnemy>>::iterator getEnemyIteratorEnd();
	std::map<int, std::shared_ptr<EditorEnemyPhase>>::iterator getEnemyPhaseIteratorBegin();
	std::map<int, std::shared_ptr<EditorEnemyPhase>>::iterator getEnemyPhaseIteratorEnd();
	std::map<int, std::shared_ptr<BulletModel>>::iterator getBulletModelIteratorBegin();
	std::map<int, std::shared_ptr<BulletModel>>::iterator getBulletModelIteratorEnd();

	int getNextAttackID() const { return nextAttackID; }
	int getNextAttackPatternID() const { return nextAttackPatternID; }
	int getNextEnemyID() const { return nextEnemyID; }
	int getNextEnemyPhaseID() const { return nextEnemyPhaseID; }
	int getNextBulletModelID() const { return nextBulletModelID; }

	std::shared_ptr<entt::SigH<void()>> getOnChange();

	void setPlayer(std::shared_ptr<EditorPlayer> player);
	void setFontFileName(std::string fontFileName) { this->fontFileName = fontFileName; }

	// Returns the radius of the largest bullet in the level pack
	float searchLargestBulletHitbox() const;
	// Returns the radius of the largest item activation radius in the level pack
	float searchLargestItemActivationHitbox() const;
	// Returns the radius of the largest item hitbox radius in the level pack
	float searchLargestItemCollectionHitbox() const;

	void playSound(const SoundSettings& soundSettings) const;
	void playMusic(const MusicSettings& musicSettings) const;

private:
	std::string name;

	AudioPlayer& audioPlayer;

	LevelPackMetadata metadata;

	int nextAttackID = 0;
	int nextAttackPatternID = 0;
	int nextEnemyID = 0;
	int nextEnemyPhaseID = 0;
	int nextBulletModelID = 0;

	// ordered levels
	std::vector<std::shared_ptr<Level>> levels;
	// The IDs of EditorAttack/EditorAttackPattern/EditorEnemy/EditorEnemyPhase are always positive, unless it is a temporary object.
	// Temporary Editor_____ objects are deleted when deleteTemporaryEditorObjects() is called, and they cannot be saved in save().
	// attack id : attack
	std::map<int, std::shared_ptr<EditorAttack>> attacks;
	// attack pattern id : attack pattern
	std::map<int, std::shared_ptr<EditorAttackPattern>> attackPatterns;
	// enemy id : enemy
	std::map<int, std::shared_ptr<EditorEnemy>> enemies;
	// enemy phase id : enemy phase
	std::map<int, std::shared_ptr<EditorEnemyPhase>> enemyPhases;
	// bullet model id : bullet model
	std::map<int, std::shared_ptr<BulletModel>> bulletModels;

	std::string fontFileName = "font.ttf";

	// Called when a change is made to one of the level pack objects, which
	// includes EditorAttack, EditorAttackPattern, EditorEnemy, EditorEnemyPhase,
	// Level, BulletModel, and EditorPlayer.
	//TODO: publish this signal in update______()
	std::shared_ptr<entt::SigH<void()>> onChange;
};