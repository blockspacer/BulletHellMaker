#include "AttackPattern.h"
#include "Components.h"
#include "EditorMovablePointSpawnType.h"

std::string EditorAttackPattern::format() const {
	std::string res = tos(id) + formatString(name) + tos(attackIDs.size());
	for (auto p : attackIDs) {
		res += tos(p.first) + tos(p.second);
	}

	res += tos(actions.size());
	for (auto p : actions) {
		res += formatTMObject(*p);
	}

	res += tos(shadowTrailInterval);
	res += tos(shadowTrailLifespan);

	return res;
}

void EditorAttackPattern::load(std::string formattedString) {
	auto items = split(formattedString, DELIMITER);
	id = std::stoi(items[0]);
	name = items[1];

	int i = 3;
	for (int a = 0; a < std::stoi(items[2]); a++) {
		attackIDs.push_back(std::make_pair(std::stof(items[i]), std::stoi(items[i + 1])));
		i += 2;
	}

	int actionsSize = std::stoi(items[i]);
	int last = i + 1;
	for (i = last; i < actionsSize + last; i++) {
		actions.push_back(EMPActionFactory::create(items[i]));
	}

	shadowTrailInterval = std::stof(items[i++]);
	shadowTrailLifespan = std::stof(items[i++]);

	actionsTotalTime = 0;
	for (auto action : actions) {
		actionsTotalTime += action->getTime();
	}
}

bool EditorAttackPattern::legal(std::string & message) const {
	bool good = true;
	if (contains(name, '(') || contains(name, ')')) {
		message += "Attack pattern \"" + name + "\" cannot have the character '(' or ')' in its name\n";
		good = false;
	}
	if (actions.size() == 0) {
		message += "Attack pattern \"" + name + "\" cannot have an empty list of actions\n";
		good = false;
	}
	return good;
}

void EditorAttackPattern::changeEntityPathToAttackPatternActions(EntityCreationQueue& queue, entt::DefaultRegistry & registry, uint32_t entity, float timeLag) {
	auto& pos = registry.get<PositionComponent>(entity);
	//registry.get<MovementPathComponent>(entity).setActions(queue, registry, entity, actions);
	registry.replace<MovementPathComponent>(entity, queue, entity, registry, entity, std::make_shared<SpecificGlobalEMPSpawn>(0, pos.getX(), pos.getY()), actions, timeLag);
}

void EditorAttackPattern::addAttackID(float time, int id) {
	auto item = std::make_pair(time, id);
	attackIDs.insert(std::upper_bound(attackIDs.begin(), attackIDs.end(), item), item);
}

void EditorAttackPattern::insertAction(int index, std::shared_ptr<EMPAction> action) {
	actions.insert(actions.begin() + index, action);

	// Recalculate completely just in case of floating point imprecision
	actionsTotalTime = 0;
	for (auto action : actions) {
		actionsTotalTime += action->getTime();
	}
}

void EditorAttackPattern::removeAction(int index) {
	actions.erase(actions.begin() + index);

	// Recalculate completely just in case of floating point imprecision
	actionsTotalTime = 0;
	for (auto action : actions) {
		actionsTotalTime += action->getTime();
	}
}