#include "EditorMovablePoint.h"
#include "EditorMovablePointSpawnType.h"

std::string EditorMovablePoint::format() {
	std::string res = "";

	res += "(" + tos(hitboxRadius) + ")" + delim;
	res += "(" + tos(hitboxPosX) + ")" + delim;
	res += "(" + tos(hitboxPosY) + ")" + delim;
	res += "(" + tos(despawnTime) + ")" + delim;

	res += "(" + tos(children.size()) + ")";
	for (auto emp : children) {
		res += delim + "(" + emp->format() + ")";
	}

	res += delim + "(" + tos(actions.size()) + ")";
	for (auto action : actions) {
		res += delim + "(" + action->format() + ")";
	}

	res += delim + "(" + spawnType->format() + ")";

	res += delim + "(" + tos(shadowTrailInterval) + ")";
	res += delim + "(" + tos(shadowTrailLifespan) + ")";
	res += delim + tos((int)rotationType);

	return res;
}

void EditorMovablePoint::load(std::string formattedString) {
	auto items = split(formattedString, DELIMITER);

	hitboxRadius = std::stof(items[0]);
	hitboxPosX = std::stof(items[1]);
	hitboxPosY = std::stof(items[2]);
	despawnTime = std::stof(items[3]);

	int i;
	for (i = 5; i < stoi(items[4]) + 5; i++) {
		std::shared_ptr<EditorMovablePoint> emp = std::make_shared<EditorMovablePoint>(nextID, shared_from_this());
		emp->load(items[i]);
		children.push_back(emp);
	}

	int last = i + 1;
	int actionsSize = stoi(items[i]);
	for (i = last; i < actionsSize + last; i++) {
		actions.push_back(EMPActionFactory::create(items[i]));
	}

	spawnType = EMPSpawnTypeFactory::create(items[i++]);

	shadowTrailInterval = stoi(items[i++]);
	shadowTrailLifespan = stoi(items[i++]);
	rotationType = static_cast<ROTATION_TYPE>(std::stoi(items[i++]));
}

bool EditorMovablePoint::legal(SpriteLoader& spriteLoader, std::string & message) {
	bool good = true;
	if (actions.size() == 0) {
		// Add a tab to show that this EMP is from the parent attack
		message += "\tMovablePoint id " + tos(id) + " must not have an empty list of actions\n";
		good = false;
	}
	if (!spawnType) {
		message += "\tMovablePoint id " + tos(id) + " is missing a spawn type\n";
		good = false;
	}
	if (shadowTrailInterval < 0 && shadowTrailLifespan != 0) {
		message += "\tMovablePoint id " + tos(id) + " has a negative shadow trail interval\n";
		good = false;
	}
	if (hitboxRadius > 0) {
		if (!animatable.isSprite() && !loopAnimation && !baseSprite.isSprite()) {
			message += "\tMovablePoint id " + tos(id) + " is missing a base sprite\n";
			good = false;
		} else {
			// Make sure base sprite can be loaded
			if (!animatable.isSprite() && !loopAnimation) {
				try {
					if (baseSprite.isSprite()) {
						spriteLoader.getSprite(baseSprite.getAnimatableName(), baseSprite.getSpriteSheetName());
					} else {
						spriteLoader.getAnimation(baseSprite.getAnimatableName(), baseSprite.getSpriteSheetName(), false);
					}
				} catch (const char* str) {
					message += "\t" + std::string(str) + "\n";
					good = false;
				}
			}
		}

		// Make sure animatable can be loaded
		try {
			if (animatable.isSprite()) {
				spriteLoader.getSprite(animatable.getAnimatableName(), animatable.getSpriteSheetName());
			} else {
				spriteLoader.getAnimation(animatable.getAnimatableName(), animatable.getSpriteSheetName(), false);
			}
		} catch (const char* str) {
			message += "\t" + std::string(str) + "\n";
			good = false;
		}
	}
	for (auto child : children) {
		if (!child->legal(spriteLoader, message)) {
			good = false;
		}
	}
	return good;
}

void EditorMovablePoint::setSpawnType(std::shared_ptr<EMPSpawnType> spawnType) {
	this->spawnType = spawnType;
}

void EditorMovablePoint::insertAction(int index, std::shared_ptr<EMPAction> action) {
	actions.insert(actions.begin() + index, action);
}

void EditorMovablePoint::removeAction(int index) {
	actions.erase(actions.begin() + index);
}

void EditorMovablePoint::addChild(std::shared_ptr<EditorMovablePoint> child) {
	// Insert while maintaining order
	int indexToInsert = children.size();
	for (int i = 0; i < children.size(); i++) {
		if (child->getSpawnType()->getTime() <= children[i]->getSpawnType()->getTime()) {
			indexToInsert = i;
			break;
		}
	}
	children.insert(children.begin() + indexToInsert, child);
}
